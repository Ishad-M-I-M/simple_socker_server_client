#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  // Create a socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  printf("clientSocket: %d\n", clientSocket);
  if (clientSocket <= 0) {
    perror("Error creating socket");
    return -1;
  }

  // Set up the server address structure
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);  // Port number
  serverAddress.sin_addr.s_addr =
      INADDR_LOOPBACK;  // Use 127.0.0.1 for local testing

  // Connect to the server
  puts("Connecting to server...");
  int status = connect(clientSocket, (struct sockaddr*)&serverAddress,
              sizeof(serverAddress));
  printf("status: %d\n", status);
  if (status < 0) {
    perror("Error connecting to server");
    close(clientSocket);
    return -1;
  } else {
    puts("Connected to server");
  }
  

  // Receive data from the server
  char buffer[1024];
  ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
  if (bytesRead == -1) {
    perror("Error receiving data");
    close(clientSocket);
    return -1;
  }

  // Print the received data
  buffer[bytesRead] = '\0';  // Null-terminate the string
  printf("Received from server: %s\n", buffer);

  // Close the socket
  close(clientSocket);

  return 0;
}
