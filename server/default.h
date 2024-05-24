#pragma once

#include <string>

enum ButtonType : int8_t
{
	SIGN_UP = 1,
	SIGN_IN
};

struct user_info
{
	std::string login;
	std::string password;
};