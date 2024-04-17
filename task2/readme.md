# Task 2

## Description
The program controls the UDP server's behavior based on the value of output 3:

- When output 3 reaches or exceeds 3.0:
    - Frequency of output 1 is set to 1Hz.
    - Amplitude of output 1 is set to 8000.
- When output 3 drops below 3.0:
    - Frequency of output 1 is set to 2Hz.
    - Amplitude of output 1 is set to 4000.

The program also retains the functionality of printing values from the server to
the standard output, operating within 20ms time windows instead of 100ms ones.
For results, check `logs/client_multi_20ms.log`