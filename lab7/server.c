#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_VECTOR_SIZE 100

void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // Listen for incoming connections
    if (listen(serverSocket, 10) == 0)
        printf("Server listening on port 12345...\n");
    else
        printf("Server failed to listen!\n");

    addr_size = sizeof(serverStorage);

    while (1)
    {
        // Accept connection from client
        clientSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);

        // Receive the size of the vector
        int vectorSize;
        recv(clientSocket, &vectorSize, sizeof(vectorSize), 0);

        // Receive the vector data
        int vector[MAX_VECTOR_SIZE];
        recv(clientSocket, vector, vectorSize * sizeof(int), 0);

        // Sort the vector
        insertionSort(vector, vectorSize);

        // Send the sorted vector back to the client
        send(clientSocket, vector, vectorSize * sizeof(int), 0);

        // Close the client socket
        close(clientSocket);
    }

    return 0;
}
