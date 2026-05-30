#include <iostream>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib") //link Winsock

int main() {
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0) {
		std::cerr << "WSAStartup failed: " << result << std::endl;
		return 1;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET) {
		std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	std::cout << "Socket created successfully." << std::endl;
	// test if you are server or client
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(1488);

	std::cout << "Are you server or client? (s/c): ";
	char choice;
	std::cin >> choice;

	switch (choice) {
		case 's': {
			server_addr.sin_addr.s_addr = INADDR_ANY;

			int bind_result = bind(sock, (sockaddr*)&server_addr, sizeof(server_addr));

			if (bind_result == SOCKET_ERROR) {
				std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
				closesocket(sock);
				WSACleanup();
				return 1;
			}
			std::cout << "Socket bound successfully" << std::endl;

			int listen_result = listen(sock, SOMAXCONN);
			if (listen_result == SOCKET_ERROR) {
				std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
				closesocket(sock);
				WSACleanup();
				return 1;
			}
			std::cout << "Listening for incoming connections" << std::endl;
			SOCKET client_sock = accept(sock, nullptr, nullptr);
			if (client_sock == INVALID_SOCKET) {
				std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
				closesocket(sock);
				WSACleanup();
				return 1;
			}
			std::cout << "Client connected" << std::endl;

			// getting data from client

			char buffer[1024];
			int bytes_received = 0;

			do {
				memset(buffer, 0, sizeof(buffer));

				bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
				if (bytes_received == SOCKET_ERROR) {
					std::cout << "recv failed: " << WSAGetLastError() << std::endl;
				}
				if (bytes_received > 0) {
					std::cout << "Received data: " << buffer << std::endl;
				}
				if (bytes_received == 0) {
					std::cout << "Connection closed by client." << std::endl;
				}
			} while (bytes_received > 0);
			closesocket(client_sock);
			closesocket(sock);
			WSACleanup();

			std::cout << "Server closed." << std::endl;
			break;
		}
		case 'c':{
			inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
			int connect_result = connect(sock, (sockaddr*)&server_addr, sizeof(server_addr));

			if (connect_result == SOCKET_ERROR) {
				std::cerr << "connect failed: " << WSAGetLastError() << std::endl;
				closesocket(sock);
				WSACleanup();
				return 1;
			}
			else {
				std::string message;

				while (true) {
					std::cout << "Enter a message to send to the server (or 'exit' to quit): ";
					std::getline(std::cin, message);
					if (message == "exit") {
						break;
					}
					int send_result = send(sock, message.c_str(), static_cast<int>(message.size()), 0);
					if (send_result == SOCKET_ERROR) {
						std::cerr << "send failed: " << WSAGetLastError() << std::endl;
						break;
					}
					else {
						std::cout << "Message sent successfully!" << std::endl;
					}
					std::cout << "Connected to server successfully!" << std::endl;
				}
			
			}
			closesocket(sock);
			WSACleanup();
			std::cout << "Client closed." << std::endl;
			return 0;
		}
	}
}
