#pragma once

#include <iostream>
#include <string>
#include <mysql.h>

#include "default.h"
#include "utility.h"


class Database
{
private:
	MYSQL mysql;
	MYSQL* connection = nullptr;
	MYSQL_RES* res = nullptr;
	MYSQL_ROW row;
	int qstate = 0;

public:
	Database() noexcept;
	~Database() noexcept;

public:
	bool SearchUser(const std::string& login, const std::string& password);
	bool UpdateUserInfo(const std::string& login);
	bool UserExist(const std::string& login);
	bool SearchAdmin();
	bool AddAdmin(const std::string& login, const std::string& password);
	bool UpdateAdminPassword(const std::string& password);
	bool InsertUser(user_info& uinfo);
	void DeleteUsersInfo();
	void DeleteUser();
};