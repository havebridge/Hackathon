#pragma once

#include <winsock2.h>
#include "nlohmann/json.hpp"

#include "database.h"
#include "thread_pool.h"


using json = nlohmann::json;



class Server
{
private:
	WSAData wsa;

	SOCKET server_socket;
	std::string ip;
	int port;

	struct sockaddr_in server_info;
	int server_info_lenght;

	bool running;
	bool is_admin_bool = false;

	Database database;
	Core::ThreadPool thread_pool;
	std::thread client_handler_func;
	uint8_t client_count = 0;

private:
	void ClientHandler();
	int GetButtonType(SOCKET client_socket);
	void GetUserInfo(SOCKET client_socket, user_info& client_info);
	void SendInfoMessageToClient(SOCKET client_socket, bool is_exist);

	bool HandleSignUp(SOCKET client_socket, user_info& client_info);
	bool HandleSignIn(SOCKET client_socket, user_info& client_info);
public:
	explicit Server(std::string ip, int port) noexcept;
	~Server();

public:
	bool Init();
};
