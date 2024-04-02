// server.cpp
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void	free_buffer(char *buffer) {
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = '\0';
	}
}

int main() {
	int serverSocket, clientSocket, maxClients, activity, sd;
	struct sockaddr_in serverAddr;
	fd_set readfds;
	std::vector<int> clients(MAX_CLIENTS, 0);
	char buffer[BUFFER_SIZE];

	// Create socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		std::cerr << "Error creating socket\n";
		exit(EXIT_FAILURE);
	}

	// Prepare the sockaddr_in structure
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(8888);
	// Bind
	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		std::cerr << "Bind failed\n";
		exit(EXIT_FAILURE);
	}

	// Listen
	listen(serverSocket, 3);

	maxClients = serverSocket;

	std::cout << "IRC Server started...\n";

	while (true) {
		FD_ZERO(&readfds);
		FD_SET(serverSocket, &readfds);

		for (auto& client : clients) {
			if (client > 0) {
				FD_SET(client, &readfds);
			}
		}

		activity = select(maxClients + 1, &readfds, nullptr, nullptr, nullptr);
		if ((activity < 0) && (errno != EINTR)) {
			std::cerr << "Error in select\n";
		}

		if (FD_ISSET(serverSocket, &readfds)) {
			clientSocket = accept(serverSocket, nullptr, nullptr);
			if (clientSocket < 0) {
				std::cerr << "Accept failed\n";
				exit(EXIT_FAILURE);
			}

			std::cout << "New connection, socket fd: " << clientSocket << ", ip: "
					<< inet_ntoa(serverAddr.sin_addr) << ", port: " << ntohs(serverAddr.sin_port) << std::endl;

			// Add new client socket to the list of clients
			for (int i = 0; i < MAX_CLIENTS; ++i) {
				if (clients[i] == 0) {
					clients[i] = clientSocket;
					break;
				}
			}

			if (clientSocket > maxClients) {
				maxClients = clientSocket;
			}
		}

		for (int i = 0; i < MAX_CLIENTS; ++i) {
			sd = clients[i];
			if (FD_ISSET(sd, &readfds)) {
				if (recv(sd, buffer, BUFFER_SIZE, 0) <= 0) {
					// Some error occurred, or connection closed
					getpeername(sd, (struct sockaddr *)&serverAddr, (socklen_t*)&serverAddr);
					std::cout << "Host disconnected, ip: " << inet_ntoa(serverAddr.sin_addr)
							<< ", port: " << ntohs(serverAddr.sin_port) << std::endl;

					close(sd);
					clients[i] = 0;
				} else
				{
					// Handle message from client
					// You can implement your IRC server logic here
					std::string receve = static_cast<std::string>(buffer);
					std::cout << "Received: " << receve << std::endl;
					if (receve != "")
					{
						std::vector<int>::iterator begin = clients.begin();
						std::vector<int>::iterator end = clients.end();
						for (; begin != end; begin++) {
							if (receve == "Jan\n")
								send(*begin, "JAn wurde geschreiben\n", strlen(buffer), 0);
							if (*begin != sd)
								send(*begin, buffer, strlen(buffer), 0);
						}
					}
					free_buffer(buffer);
				}
			}
		}
	}

	close(serverSocket);

	return 0;
}
