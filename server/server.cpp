#include "server.h"

#pragma warning(disable: 4996)

Server::Server(std::string ip, int port) noexcept
	:
	wsa{ 0 },
	server_socket(INVALID_SOCKET),
	ip(ip),
	port(port),
	server_info{ 0 },
	server_info_lenght(sizeof(server_info)),
	running(false) {}

bool Server::Init()
{
	if ((WSAStartup(MAKEWORD(2, 2), &wsa)) != 0)
	{
		std::cerr << "WSAStartup() failed\n";
		return false;
	}

	if ((server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		std::cerr << "socket() failed\n";
		return false;
	}

	unsigned long mode = 0;

	if (ioctlsocket(server_socket, FIONBIO, &mode) == SOCKET_ERROR)
	{
		std::cerr << "ioctlsocket() failed\n";
		return false;
	}

	char yes = '1';

	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == SOCKET_ERROR)
	{
		std::cerr << "setsockopt() failed\n";
		return false;
	}

	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(port);
	server_info.sin_addr.s_addr = inet_addr(ip.c_str());

	ZeroMemory(server_info.sin_zero, 8);

	if (bind(server_socket, (const sockaddr*)&server_info, server_info_lenght) == SOCKET_ERROR)
	{
		std::cerr << "bind() failed\n";
		return false;
	}

	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "listen() failed\n";
		return false;
	}

	client_handler_func = std::thread(&Server::ClientHandler, this);

	std::cout << "TCP Server started at " << inet_ntoa(server_info.sin_addr) << ':' << htons(server_info.sin_port) << '\n';

	running = true;

	return running;
}


void Server::GetUserInfo(SOCKET client_socket, user_info& client_info)
{
	std::string recieved_buffer;
	int recieved_buffer_size = 0;

	if (recv(client_socket, (char*)&recieved_buffer_size, sizeof(int), 0) <= 0)
	{
		std::cout << "ClientHandler(): recieved_buffer_size recv\n";
	}

	recieved_buffer.resize(recieved_buffer_size);

	if (recv(client_socket, recieved_buffer.data(), recieved_buffer_size, 0) <= 0)
	{
		std::cout << "ClientHandler(): recieved_buffer_size recv\n";
	}

	json json_data = json::parse(recieved_buffer);

	client_info.login = json_data["login"];
	client_info.password = json_data["password"];
}

void Server::SendInfoMessageToClient(SOCKET client_socket, bool is_exist)
{
	if (send(client_socket, reinterpret_cast<const char*>(&is_exist), sizeof(bool), 0) == -1)
	{
		std::cerr << "SendClientsInfo(): send is_any_client_connected\n";
	}
}


bool Server::HandleSignUp(SOCKET client_socket, user_info& client_info)
{
	bool is_exist = database.UserExist(client_info.login);
	SendInfoMessageToClient(client_socket, is_exist);

	if (is_exist)
	{
		std::cout << "DONT EXIST\n";
		return false;
	}

	database.InsertUser(client_info);
	return true;
}

bool Server::HandleSignIn(SOCKET client_socket, user_info& client_info)
{
	bool is_exist = database.UserExist(client_info.login);
	SendInfoMessageToClient(client_socket, is_exist);
	database.UpdateUserInfo(client_info.login);

	if (!is_exist)
	{
		return false;
	}

	return true;
}

int Server::GetButtonType(SOCKET client_socket)
{
	int button_type = 0;
	unsigned long mode = 1;
	if (ioctlsocket(client_socket, FIONBIO, &mode) != NO_ERROR)
	{
	}

	if (recv(client_socket, (char*)&button_type, sizeof(int), 0) == -1)
	{
		//std::cout << WSAGetLastError() << '\n';
		//perror("GetButtonType(): button_type recv");
	}
	else
	{
		mode = 0;
		if (ioctlsocket(client_socket, FIONBIO, &mode) != NO_ERROR)
		{
		}
	}



	return button_type;
}

void Server::ClientHandler()
{
	while (true)
	{
		std::cout << "CLIENT HANDER FUNC\n";
		bool client_connected = false;

		struct sockaddr_in client_info_sockaddr;
		int client_info_lenght = sizeof(client_info_sockaddr);

		user_info client_info = {};

		SOCKET client_socket = accept(server_socket, (sockaddr*)&client_info_sockaddr, &client_info_lenght);
		std::cout << "Client Socket:" << client_socket << '\n';
		client_count++;

		if (client_count == 1)
		{
			std::cout << "ADMIN\n";

			if (!database.SearchAdmin())
			{
				database.AddAdmin("admin", "admin");
			}

			bool is_admin = true;
			if (send(client_socket, reinterpret_cast<const char*>(&is_admin), sizeof(bool), 0) == -1)
			{
				std::cerr << "ClientHandler(): is_admin send\n";
			}

			std::string new_password;
			int new_password_size = 0;

			bool check_flag = false;
			if (recv(client_socket, (char*)&check_flag, sizeof(bool), 0) == -1)
			{
				std::cerr << "RecvFlag(): check_flag recv\n";
			}

			if (check_flag == false)
			{
				std::cerr << "Login or Password empty!";
			}
			else
			{
				if (recv(client_socket, reinterpret_cast<char*>(&new_password_size), sizeof(int), 0) == -1)
				{
					std::cerr << "ClientHandler(): new_password_size recv\n";
				}
				std::cout << "size: " << new_password_size << "\n\n\n";

				new_password.resize(new_password_size);

				if (recv(client_socket, new_password.data(), new_password_size, 0) <= 0)
				{
					std::cerr << "ClientHandler(): recieved_buffer_size recv\n";
				}

				std::cout << "new password: " << new_password.data() << '\n';

				is_admin_bool = true;
				database.UpdateAdminPassword(new_password);
			}
		}
		else
		{
			std::cout << "NOT ADMIN\n";
			bool is_admin = false;
			if (send(client_socket, reinterpret_cast<const char*>(&is_admin), sizeof(bool), 0) == -1)
			{
				std::cerr << "ClientHandler(): is_admin send\n";
			}
			while (!client_connected)
			{
				ButtonType button_type = static_cast<ButtonType>(GetButtonType(client_socket));

				bool check_flag = false;
				if (recv(client_socket, (char*)&check_flag, sizeof(bool), 0) == -1)
				{
					//std::cerr << "RecvFlag(): check_flag recv\n";
				}
				else
				{
					std::cout << "GOOD\n";
				}

				if (check_flag == false)
				{
					std::cout << "BAD\n";
					continue;
				}


				switch (button_type)
				{
				case ButtonType::SIGN_UP:
				{
					std::cout << "Client Pressed Sign up Button\n";
					GetUserInfo(client_socket, client_info);
					client_connected = HandleSignUp(client_socket, client_info);
				} break;
				case ButtonType::SIGN_IN:
				{
					std::cout << "Client Pressed Sign In Button\n";
					GetUserInfo(client_socket, client_info);
					client_connected = HandleSignIn(client_socket, client_info);
				} break;
				default:
				{
					break;
				}
				}
			}
		}


		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

}

Server::~Server()
{
	if (client_handler_func.joinable())
	{
		client_handler_func.join();
	}

	closesocket(server_socket);
	WSACleanup();
}