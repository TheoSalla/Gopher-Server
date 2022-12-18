#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345  // port to listen on
#define BUFSIZE 1024  // maximum size of a request or response

int main(int argc, char *argv[]) {
  // create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error creating socket");
    exit(EXIT_FAILURE);
  }

  // bind the socket to a port
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));  // clear the struct
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;  // listen on any interface
  serv_addr.sin_port = htons(PORT);  // listen on the specified port
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("Error binding socket");
    exit(EXIT_FAILURE);
  }

  // listen for connections
  listen(sockfd, 5);  // allow up to 5 queued connections

  // accept incoming connections
  while (1) {
    int clifd = accept(sockfd, NULL, NULL);  // accept the connection
    if (clifd < 0) {
      perror("Error accepting connection");
      continue;  // try again
    }

    // read the request
    char request[BUFSIZE];
    int bytes_read = read(clifd, request, BUFSIZE - 1);  // read the request
    if (bytes_read < 0) {
      perror("Error reading request");
      close(clifd);  // close the connection
      continue;  // try again
    }
    request[bytes_read] = '\0';  // null-terminate the request string

    // construct the response
    char response[BUFSIZE];
    sprintf(response, "Hello, Gopher!\r\n");  // static response for now

    // send the response
    int bytes_written = write(clifd, response, strlen(response));
    if (bytes_written < 0) {
      perror("Error writing response");
      close(clifd);  // close the connection
      continue;  // try again
    }
    // close the connection
    close(clifd);
  }
  return 0;

}
