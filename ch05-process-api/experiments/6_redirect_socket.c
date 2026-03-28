// Experiment 6: stdout to a socket
// - Same trick as file redirect, but fd 1 becomes a socket
// - Starts a local TCP server, forks, child connects and redirects
//
// Run in one terminal:   nc -l 9999
// Run in another:        ./6_redirect_socket
// Watch: "ls" output appears in the nc terminal!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void) {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // CHILD: connect to localhost:9999, redirect stdout there

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            fprintf(stderr, "socket failed\n");
            exit(1);
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(9999);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            fprintf(stderr, "connect failed — run 'nc -l 9999' first!\n");
            exit(1);
        }

        // Redirect: close stdout, duplicate socket to fd 1
        dup2(sock, STDOUT_FILENO);
        close(sock); // original fd no longer needed

        // exec ls — its printf goes over the network
        char *args[] = {"ls", "-la", NULL};
        execvp("ls", args);
        fprintf(stderr, "exec failed\n");
        exit(1);
    } else {
        wait(NULL);
        fprintf(stderr, "parent: child finished\n");
    }

    return 0;
}
