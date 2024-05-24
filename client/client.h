#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <format>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <format>

#include "nlohmann/json.hpp"
#include <winsock2.h>

using json = nlohmann::json;

#pragma comment(lib, "WS2_32.lib")

enum ButtonType : int8_t
{
	SIGN_UP = 1,
	SIGN_IN
};

class Client
{
private:
	WSAData wsa;

	SOCKET client_socket;

	struct sockaddr_in server_info;
	int server_info_lenght;

	struct sockaddr_in client_info;
	int client_info_lenght;

	std::thread recv_thread;

	bool is_connected = false;
	bool is_admin = false;
	bool is_admin_exist = false;


public:
	explicit Client() noexcept;
	~Client();

	bool GetRights() { return is_admin; }

public:
	bool Connect(const std::string& ip, int port);

	bool SendUserInfoSignUp(const std::string& login, const std::string& password);
	bool SendUserInfoSignIn(const std::string& login, const std::string& password);
	bool SendAdminPassword(std::string password);
	void SendButtonInfo(int button_type);
	void SendCheckFlag(bool check_flag);
	void RecvRightsInfo();


	void Disconnect();
	void Run();

public:
	static Client& GetInstance()
	{
		static Client instance;
		return instance;
	}
};
