#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define HOST "127.0.0.1"
#define TIMEOUT 100

int open_tcp_socket(int port)
{
    // Create socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sock_fd < 0)
    {
        perror("Unable to create socket");
        return -1;
    }

    // Set port and IP 
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(HOST);

    // Send connection request to server
    if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Connection failed");
        return -1;
    }

    // Set timeout for receiving data
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1 * 1000; // Set timeout in milliseconds
    if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("ERROR setting socket options");
        return -1;
    }

    return sock_fd;  
}

char *recv_data(int sock_fd) 
{
    char buffer[1024];
    char *save_ptr = NULL;
    char *recent_out = NULL;

    int bytes_recv = recv(sock_fd, buffer, sizeof(buffer), 0);
    if (bytes_recv == 0) 
    {
        strcpy(buffer, "--");
    }
    
    char* out = strtok(buffer, "\n");
    while (out != NULL) 
    {
        recent_out = out; //get the recent out value
        out = strtok(NULL, "\n"); // Get the next out
    }

    if (recent_out == NULL)
    {
        strcpy(recent_out, "--");
    }
    
    // memset(buffer, 0, sizeof(buffer));

    return recent_out;
}

int main(void)
{
    struct timeval now;
    long int start_time_msec, current_time_msec;
    char output_string[1024];

    int sock_desc_4001 = open_tcp_socket(4001);
    int sock_desc_4002 = open_tcp_socket(4002);
    int sock_desc_4003 = open_tcp_socket(4003);

    if (sock_desc_4001 == -1 || sock_desc_4002 == -1 || sock_desc_4003 == -1)
    {
        close(sock_desc_4001);
        close(sock_desc_4002);
        close(sock_desc_4003);
        return -1;
    }

    // Get the start time
    gettimeofday(&now, NULL);
    start_time_msec = ((long int)now.tv_sec * 1000) + ((long int)now.tv_usec / 1000);

    while (1) 
    {
        char* out1 = recv_data(sock_desc_4001);
        char* out2 = recv_data(sock_desc_4002);
        char* out3 = recv_data(sock_desc_4003);

        gettimeofday(&now, NULL);
        current_time_msec = ((long int)now.tv_sec * 1000) + ((long int)now.tv_usec / 1000);

        if ((current_time_msec - start_time_msec) >= TIMEOUT) 
        {
            start_time_msec = current_time_msec;
            sprintf(output_string, "{\"timestamp\": %lu, \"out1\": \"%s\", \"out2\": \"%s\", \"out3\": \"%s\"}\n",
                    current_time_msec, out1, out2, out3);                                   
            printf("%s", output_string);
        }
        fflush(stdout);
    }
    return 0;
}