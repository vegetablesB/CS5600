
/*
 * @Author: Cao Ning
 * @Date: 2023-02-08 18:45:19
 * @FilePath: Main.java
 * @Assignment: HW5 Synchronization with Monitors (Java)
 * @Course: CS5600 Spring 2023
 */
import java.util.Random;

public class Main extends Thread {
    private BoundedBuffer buffer;
    private Random random = new Random();
    private int threadId;

    public Main(int threadId, BoundedBuffer buffer) {
        this.threadId = threadId;
        this.buffer = buffer;
    }

    @Override
    public void run() {
        for (int i = 0; i < 30; i++) {
            int randomNumber = 10000 + random.nextInt(90000); // generates a 5 digit random number
            String data = threadId + "-" + randomNumber;
            try {
                buffer.deposit(data);
                System.out.println("Thread " + threadId + " inserted " + data + " into the buffer");
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        BoundedBuffer buffer = new BoundedBuffer(100);
        Main t1 = new Main(1, buffer);
        Main t2 = new Main(2, buffer);
        Main t3 = new Main(3, buffer);
        t1.start();
        t2.start();
        t3.start();
        try {
            t1.join();
            t2.join();
            t3.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            System.out.println("\nAll threads have finished");
            System.out.println("\nBuffer contents: " + buffer);
            System.out.println("\nNumber of elements in the buffer: " + buffer.getCount());
            System.out.println(
                    "\n10 available spaces left due the buffer size is 100, and 3 threads each inserted 30 elements");
        }

    }
}
