#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_VECTOR_SIZE 100

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == 0)
        printf("Connected to server!\n");
    else
    {
        printf("Failed to connect to server!\n");
        exit(1);
    }

    // Get user input for the vector
    int vectorSize;
    printf("Enter the size of the vector: ");
    scanf("%d", &vectorSize);

    int vector[MAX_VECTOR_SIZE];
    printf("Enter the elements of the vector: ");
    for (int i = 0; i < vectorSize; i++)
    {
        scanf("%d", &vector[i]);
    }

    // Send the size of the vector to the server
    send(clientSocket, &vectorSize, sizeof(vectorSize), 0);

    // Send the vector data to the server
    send(clientSocket, vector, vectorSize * sizeof(int), 0);

    // Receive the sorted vector from the server
    int sortedVector[MAX_VECTOR_SIZE];
    recv(clientSocket, sortedVector, vectorSize * sizeof(int), 0);

    // Print the sorted vector
    printf("Sorted vector: ");
    for (int i = 0; i < vectorSize; i++)
    {
        printf("%d ", sortedVector[i]);
    }
    printf("\n");

    // Close the client socket
    close(clientSocket);

    return 0;
}
