/*
 * @Author: Cao Ning
 * @Date: 2023-02-08 18:43:31
 * @FilePath: BoundedBuffer.java
 * @Assignment: HW5 Synchronization with Monitors (Java)
 * @Course: CS5600 Spring 2023
 */
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class BoundedBuffer {
    private final String[] buffer;
    private final int capacity;

    private int front;
    private int rear;
    private int count;

    private final Lock lock = new ReentrantLock();

    private final Condition notFull = lock.newCondition();
    private final Condition notEmpty = lock.newCondition();

    public BoundedBuffer(int capacity) {
        super();

        this.capacity = capacity;

        buffer = new String[capacity];
    }

    public void deposit(String data) throws InterruptedException {
        lock.lock();

        try {
            while (count == capacity) {
                notFull.await();
            }

            buffer[rear] = data;
            rear = (rear + 1) % capacity;
            count++;

            notEmpty.signal();
        } finally {
            lock.unlock();
        }
    }

    public String fetch() throws InterruptedException {
        lock.lock();

        try {
            while (count == 0) {
                notEmpty.await();
            }

            String result = buffer[front];
            front = (front + 1) % capacity;
            count--;

            notFull.signal();

            return result;
        } finally {
            lock.unlock();
        }
    }

    // print the buffer by overriding the toString() method
    @Override
    public String toString() {
        String result = "[";

        for (int i = 0; i < capacity - 1; i++) {
            // print like this [1, 2, 3, 4, 5]
            result += buffer[i] + ", ";
        }
        result += buffer[capacity - 1] + "]";

        return result;
    }

    // get the number of elements in the buffer
    public int getCount() {
        return count;
    }

}