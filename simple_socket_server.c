#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  // Create a socket
  int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    perror("Error creating socket");
    return -1;
  }

  // Set up the server address structure
  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8080);  // Port number
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  int reuse = 1;
  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse,
                 sizeof(int)) != 0) {
    puts("Can't set the reuse option on the socket");
    return EXIT_FAILURE;
  }
  // Bind the socket to the specified address and port
  if (bind(serverSocket, (struct sockaddr*)&serverAddress,
           sizeof(serverAddress)) < 0) {
    perror("Error binding socket");
    close(serverSocket);
    return -1;
  }

  // Listen for incoming connections
  if (listen(serverSocket, 10) < 0) {
    perror("Error listening for connections");
    close(serverSocket);
    return -1;
  }

  printf("Server listening on port 8080...\n");

  // Accept connections
  int clientSocket = accept(serverSocket, NULL, NULL);
  if (clientSocket < 0) {
    perror("Error accepting connection");
    close(serverSocket);
    return -1;
  } else {
    puts("Client connected");
  }

  // Send a message to the client
  const char* message = "Hello, client!";
  send(clientSocket, message, strlen(message), 0);

  // Close the sockets
  close(clientSocket);
  close(serverSocket);

  return 0;
}
