# Question 1

*A disk rotates at 7200 RPM and can transfer 200MB/s of data from its outer track. What is the maximum number of bytes of data that a single outer track can hold? Show your calculations, state any assumptions, and show your work so we can see how you derived your answer.*

**The following answer is not right, -10 points get.**
**I have this reedited answer because the discussion between prof and one of the student in teams.**
**My original answer is at the end of this file.**

Since we don't have information about the bit density and the diameter of the disk, let's make some reasonable assumptions to estimate the maximum number of bytes of data that a single outer track can hold.

Assuming that the disk has a bit density(linear density) of 500,000 bits per inch and a diameter of 3.5 inches, we can calculate the circumference of the outermost track as follows:

$$Circumference = π × diameter = 3.1416 × 3.5\ inches = 10.9956\ inches$$
The total number of bits that can be stored on the outermost track can be calculated by multiplying the track length by the bit density. This gives us:
$$\begin{aligned}Number\ of\ bits &= bit\ density × circumference\\&= 500,000\ bits/inch × 10.9956\ inches \\&= 5,497,800\ bits/revolution\end{aligned}$$
To convert the number of bits to bytes, we divide by 8, which gives us:

$$Number\ of\ bytes = 5,497,800\ bits / 8 = 687,225\ bytes$$

Therefore, the estimated maximum number of bytes of data that a single outer track can hold, based on the assumptions of a bit density of 500,000 bits per inch and a diameter of 3.5 inches, is approximately **687,225 bytes**.

# Question 2

*Given an average seek time of 5ms, a rotational speed of 10,000 RPM, and a transfer rate of 150 MB/s, how long does a 65536-byte random read request take, in milliseconds, on average? What if average seek time is 12ms and rotational speed is 5400 RPM? Show your calculations, state any assumptions, and show your work so we can see how you derived your answer.*

To calculate the average time it takes for a 65536-byte random read request for 10000RPM, we need to consider the following factors:

1. Seek time: The time it takes for the read/write head to move to the correct track on the disk.
$$5ms$$
2. Rotational latency: The time it takes for the disk to rotate to the correct sector. Half rotation.
$$Rotations/Second = 10000\ RPM = \frac{10000}{60}r/s = \frac{500}{3}r/s$$
$$Rotational\ latency = \frac{1}{\frac{500}{3}*2}=3ms$$
3. Transfer time: The time it takes for the data to be transferred from the disk to the computer.
$$Transfer\ time = \frac{65536byte}{150KB/ms}\approx0.44ms$$

Therefore, the total time it takes for the read request is:
$$time = 5ms + 3ms + 0.44ms = 8.44ms$$

To calculate the average time it takes for a 65536-byte random read request for 5400RPM, we need to consider the following factors:

1. Seek time: The time it takes for the read/write head to move to the correct track on the disk.
$$12ms$$
2. Rotational latency: The time it takes for the disk to rotate to the correct sector. Half rotation.
$$Rotations/Second = 5600\ RPM = \frac{5600}{60}r/s = \frac{280}{3}r/s$$
$$Rotational\ latency = \frac{1}{\frac{280}{3}*2}\approx5.55ms$$
3. Transfer time: The time it takes for the data to be transferred from the disk to the computer.
$$Transfer\ time = \frac{65536byte}{150KB/ms}\approx0.44ms$$

Therefore, the total time it takes for the read request is:
$$time = 12ms + 5.55ms + 0.44ms = 17.99ms$$.

# Question 3

*If a system has an estimated mean time between failures of 1,250,000 hours, what is its expected annual reliability expressed as a probability?*

Rather than expressing reliability in terms *MTBF* and *MTTR*, it can also be, like availability, expressed as the likelihood that a system is functioning properly during some time period t, given some measure of *MTBF* – and, as a corollary, in terms of availability and *MTTR*.

From this perspective, an estimate of the reliability can be calculated as follows:

$$MTBF = 1250000$$
$$R=e^{−t/MTBF} = e^{-365*24/1250000} \approx 0.993016 = 99.3016\%$$

# Question 4

*A power control system is required to have an availability of at least 99.999% in accordance with regulations. The systems development team has conducted empirical testing and derived an mean time to restore of 1.25 hours and an mean time between failure of 3.1667 years. Will the system meet the expected availability?*

$$MTTR = 1.25\ hours$$
$$MTBF = 3.1667\ years = 27740.292\ hours$$
$$A = \frac{MTBF}{MTBF + MTTR} = \frac{27740.292}{27741.542} = 0.9999549412 < 99.999\%$$
Then, the system will not meet the expected availability.

# q1

First, we need to convert the RPM (revolutions per minute) to RPS (revolutions per second) by dividing by 60: $$7200\ RPM / 60 = 120\ RPS$$
Next, we can calculate the time it takes for one revolution by taking the reciprocal of the RPS: $$1 / 120\ RPS = 0.00833\ seconds/revolution$$
Since the disk can transfer 200MB/s of data from its outer track, we can calculate the maximum number of bytes transferred in one revolution by multiplying the transfer rate by the time per revolution: $$200MB/s *0.00833 seconds/revolution = 1.666 MB/revolution$$
Finally, we can convert the result from MB to bytes by multiplying by 1,000,000:
$$1.666\ MB/revolution* 1,000,000\ bytes/MB = 1,666,000\ bytes/revolution$$
So, the maximum number of bytes of data that a single outer track can hold is **1,666,000 bytes**. This calculation assumes that the transfer rate is constant and that the disk is always reading from the outer track.
