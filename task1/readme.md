# Task 1

## Description
- The `client_multi_threaded.c` program reads data from three TCP ports (4001, 4002, and 4003) and outputs
  JSON objects to standard output every 100 milliseconds. 
- Each JSON object contains four key-value pairs: timestamp, representing milliseconds since the
  Unix epoch, out1, out2, and out3, which correspond to the strings received from server ports 
  4001, 4002, and 4003 respectively. 
- If no value is received from a port within the 100ms period, the value for that port should be
  "--". If multiple values are received from a port within the same 100ms window, only the most 
  recent value should be printed.

## Server output analysis 

| Output | Frequency(Hz) | Amplitude(mV) |  Shape / Wave |
| :----: | :------:      | :-------:     |     :---:     |
| out1   |   500         |   5000        |     sine      |
| out2   |   250         |   5000        |   triangular  |
| out3   |   1           |   5000        |    square     |
