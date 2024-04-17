#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define HOST "127.0.0.1"
#define TIMEOUT 100
#define NUM_THREADS 3

char *recent_out[NUM_THREADS];
volatile sig_atomic_t keep_running = 1;

int open_tcp_socket(int port)
{
    // Create socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sock_fd < 0) 
    {
        perror("TCP Socket creation failed.");
        return -1;
    }

    // Set port and IP 
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(HOST);

    // Send connection request to server
    if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("TCP Connection failed.");
        return -1;
    }

    // Set timeout for receiving data
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0.1 * 1000; // Set timeout in milliseconds
    if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        perror("Setting Socket Options Failed.");
        return -1;
    }

    return sock_fd;  
}

void *recv_data(void *arg)
{
    int sock_fd = *((int *) arg);
    int thread_id = *((int *) arg + 1);
    char *save_ptr = NULL;

    // Separate buffer for each tcp port
    char buffer[1024]; 
    // Initialize the buffer with --
    strncpy(buffer, "--\n", sizeof(buffer));

    while (1)
    {
        if (keep_running)
        {
            int bytes_recv = recv(sock_fd, buffer, sizeof(buffer), 0);
            if (bytes_recv == 0) 
            {
                strncpy(buffer, "--\n", sizeof(buffer));
            }

            char *out = strtok_r(buffer, "\n", &save_ptr);

            // Get the recent outputs from the server buffer
            while (out != NULL) 
            {
                // Secure thread synchronization to prevent conflicts 
                // since the main thread also updates recent_out.
                if (keep_running)
                {
                    // update the out
                    recent_out[thread_id] = out;   
                }
                out = strtok_r(NULL, "\n", &save_ptr);
            }
        }
    }
    pthread_exit(NULL);
}

int main() 
{
    int ports[NUM_THREADS] = {4001, 4002, 4003};
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS][2];
    struct timeval now;
    long int start_time_msec, current_time_msec;
    char output_string[1024];

    // Creating threads for three tcp ports
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        thread_args[i][0] = open_tcp_socket(ports[i]);
        thread_args[i][1] = i;

        if(thread_args[i][0] < 0)
        {
            close(thread_args[i][0]);
            exit(EXIT_FAILURE);
        }

        if (pthread_create(&threads[i], NULL, recv_data, (void *) thread_args[i]) != 0) 
        {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    // Get the start time
    gettimeofday(&now, NULL);
    start_time_msec = ((long int)now.tv_sec * 1000) + ((long int)now.tv_usec / 1000);

    while (1)
    {
        // Get the current time
        gettimeofday(&now, NULL);
        current_time_msec = ((long int) now.tv_sec * 1000) + ((long int) now.tv_usec / 1000);

        if ((current_time_msec - start_time_msec) >= TIMEOUT) 
        {
            keep_running = 0;
            start_time_msec = current_time_msec;

            sprintf(output_string, "{\"timestamp\": %lu, \"out1\": \"%s\", \"out2\": \"%s\", \"out3\": \"%s\"}\n",
                    current_time_msec, recent_out[0], recent_out[1], recent_out[2]);
            printf("%s", output_string);

            // To replace the existing values with "--" in all server outputs and clear them from memory.
            for (int id = 0; id < NUM_THREADS; id++)
            {
                strcpy(recent_out[id], "--");
            }

            keep_running = 1;
        }

        fflush(stdout);
    }

    return 0;
}
