# Embedded Linux Homework Task

# Initial Setup

Install Docker and load the Docker image present in the directory:
```
docker load -i fsw-linux-homework.tar.gz
```

Run it:
```
docker run -p 4000:4000/udp -p 4001:4001 -p 4002:4002 -p 4003:4003 fsw-linux-homework
```

The Docker image contains a server application that exposes three TCP ports
(4001, 4002, 4003) and produces data on them. Additionally, the server operates
a "control channel" by listening for messages on UDP port 4000 and reacting to
them.

## Server output analysis 

| Output | Frequency(Hz)  | Amplitude(mV) |  Shape / Wave |
| :----: | :------:       | :-------:     |     :---:     |
| out1   |   1(500)       |   5000        |     sine      |
| out2   |   0.5(250)     |   5000        |   triangular  |
| out3   |   0.25(125)    |   5000        |    square     |