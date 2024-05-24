#include "client.h"

#pragma warning(disable: 4996)



Client::Client() noexcept
	:
	wsa{ 0 },
	client_socket(INVALID_SOCKET),
	client_info{ 0 },
	client_info_lenght(sizeof(client_info)),
	server_info{ 0 },
	server_info_lenght(sizeof(server_info)) {}


bool Client::Connect(const std::string& ip, int port)
{
	if ((WSAStartup(MAKEWORD(2, 2), &wsa)) != 0)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("WSAStartup");
		return false;
	}

	if ((client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("socket");
		return false;
	}

	char mode = '1';

	if ((setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &mode, sizeof(char)) == SOCKET_ERROR))
	{
		std::cout << WSAGetLastError() << '\n';
		perror("setsockport");
		return false;
	}

	server_info.sin_family = PF_INET;
	server_info.sin_port = htons(port);
	server_info.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

	ZeroMemory(server_info.sin_zero, 8);

	if (connect(client_socket, (const sockaddr*)&server_info, server_info_lenght) == SOCKET_ERROR)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("client connect");
		return false;
	}

	if (getsockname(client_socket, (sockaddr*)&client_info, &client_info_lenght) == -1)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("getsockname");
		return false;
	}

	is_connected = true;

	return is_connected;
}


void Client::RecvRightsInfo()
{
	if (recv(client_socket, (char*)&is_admin, sizeof(bool), 0) == -1)
	{
		std::cerr << "RecvFlag(): is_admin recv\n";
	}
}

void Client::SendCheckFlag(bool check_flag)
{
	if (send(client_socket, reinterpret_cast<const char*>(&check_flag), sizeof(bool), 0) == -1)
	{
		std::cerr << "SendCheckFlag(): check_flag send\n";
	}
}

void Client::SendButtonInfo(int button_type)
{
	if (send(client_socket, reinterpret_cast<const char*>(&button_type), sizeof(int), 0) == -1)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("SendButtonInfo(): type button send");
	}
}

bool Client::SendUserInfoSignUp(const std::string& login, const std::string& password)
{
	json jsonData = {
		{"login", login},
		{"password", password},
	};

	std::string serialized_data = jsonData.dump();
	int serialized_data_size = serialized_data.size();

	if (send(client_socket, reinterpret_cast<const char*>(&serialized_data_size), sizeof(int), 0) == -1)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("SendUserInfoSignUp(): serialized_data_size send");
		return false;
	}

	if (send(client_socket, serialized_data.c_str(), serialized_data_size, 0) == -1)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("SendUserInfoSignUp(): serialized_data send");
		return false;
	}

	bool is_exist = false;

	if (recv(client_socket, (char*)&is_exist, sizeof(bool), 0) == -1)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("SendUserInfoSignUp(): is_exist recv");
		return false;
	}

	return (is_exist == true) ? false : true;
}

bool Client::SendUserInfoSignIn(const std::string& login, const std::string& password)
{
	json json_data = {
		{"login", login},
		{"password", password},
	};

	std::string serialized_data = json_data.dump();
	int serialized_data_size = serialized_data.size();

	if (send(client_socket, reinterpret_cast<const char*>(&serialized_data_size), sizeof(int), 0) == -1)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("SendUserInfoSignUp(): serialized_data_size send");
		return false;
	}

	if (send(client_socket, serialized_data.c_str(), serialized_data_size, 0) == -1)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("SendUserInfoSignUp(): serialized_data send");
		return false;
	}


	bool is_found = false;

	if (recv(client_socket, (char*)&is_found, sizeof(bool), 0) == -1)
	{
		std::cout << WSAGetLastError() << '\n';
		perror("SendUserInfoSignUp(): is_found recv");
		return false;
	}

	return is_found;
}


bool Client::SendAdminPassword(std::string password)
{
	int password_data_size = password.size();

	if (send(client_socket, reinterpret_cast<const char*>(&password_data_size), sizeof(int), 0) == -1)
	{
		std::cout << WSAGetLastError() << '\n';
		return false;
	}

	std::cout << "password size: " << password_data_size << '\n';

	if (send(client_socket, password.c_str(), password_data_size, 0) == -1)
	{
		std::cout << WSAGetLastError() << '\n';
		return false;
	}

	std::cout << "password: " << password << '\n';
}

void Client::Disconnect()
{
	std::cout << "Disconnect\n";
	shutdown(client_socket, SD_SEND);
	closesocket(client_socket);
	is_connected = false;
}


void Client::Run()
{
	while (true);

	Disconnect();
}

Client::~Client()
{
	if (recv_thread.joinable())
	{
		recv_thread.join();
	}
	closesocket(client_socket);
	WSACleanup();
}