#include "database.h"

Database::Database() noexcept
{
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql, "localhost", "root", "password", "hackathon", 3306, NULL, 0);

	if (connection == NULL)
	{
		std::cerr << mysql_error(&mysql) << std::endl;
		mysql_close(&mysql);
	}
}

bool Database::SearchAdmin()
{
	int count = 0;

	std::string admin_login = "admin";
	std::vector<unsigned char> encrypted_admin_password = Utility::Encrypt("admin");
	std::string password_hex = Utility::BinaryToHex(encrypted_admin_password);

	std::string query_string = "SELECT COUNT(*) FROM user WHERE login = ? AND password = ?";

	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);

	if (!stmt)
	{
		std::cerr << "mysql_stmt_init(), out of memory\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		return false;
	}

	if (mysql_stmt_prepare(stmt, query_string.c_str(), query_string.length()) != 0)
	{
		std::cerr << "mysql_stmt_prepare(), SELECT failed for query: " << query_string << '\n';
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	MYSQL_BIND bind[2];
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = const_cast<char*>(admin_login.c_str());
	bind[0].buffer_length = admin_login.length();
	bind[0].is_null = 0;

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = const_cast<char*>(password_hex.c_str());
	bind[1].buffer_length = password_hex.length();
	bind[1].is_null = 0;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		std::cerr << "mysql_stmt_bind_param() failed\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	MYSQL_BIND result_bind;
	memset(&result_bind, 0, sizeof(result_bind));

	result_bind.buffer_type = MYSQL_TYPE_LONG;
	result_bind.buffer = &count;
	result_bind.is_null = 0;

	if (mysql_stmt_bind_result(stmt, &result_bind) != 0)
	{
		std::cerr << "mysql_stmt_bind_result() failed\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return 0;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		std::cerr << "mysql_stmt_execute(), SELECT failed for query: " << query_string << '\n';
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return 0;
	}

	if (mysql_stmt_fetch(stmt) != 0)
	{
		std::cerr << "mysql_stmt_fetch(), SELECT failed for query: " << query_string << '\n';
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return 0;
	}

	mysql_stmt_close(stmt);

	return count != 0;
}

bool Database::AddAdmin(const std::string& login, const std::string& password)
{
	std::vector<unsigned char> encrypted_password = Utility::Encrypt(password);
	std::string password_hex = Utility::BinaryToHex(encrypted_password);

	std::string query_string = "INSERT INTO user (login, password, last_visited_at) VALUES (?, ?, NOW())";

	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);

	if (!stmt)
	{
		std::cerr << "mysql_stmt_init(), out of memory\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		return false;
	}

	if (mysql_stmt_prepare(stmt, query_string.c_str(), query_string.length()) != 0)
	{
		std::cerr << "mysql_stmt_prepare(), INSERT failed for query: " << query_string << '\n';
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	MYSQL_BIND bind[2];
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = const_cast<char*>(login.c_str());
	bind[0].buffer_length = login.length();
	bind[0].is_null = 0;

	bind[1].buffer_type = MYSQL_TYPE_STRING;
	bind[1].buffer = const_cast<char*>(password_hex.c_str());
	bind[1].buffer_length = password_hex.length();
	bind[1].is_null = 0;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		std::cerr << "mysql_stmt_bind_param() failed\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		std::cerr << "mysql_stmt_execute(), INSERT failed\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	mysql_stmt_close(stmt);

	return true;
}

bool Database::UpdateAdminPassword(const std::string& password)
{
	std::vector<unsigned char> encrypted_password = Utility::Encrypt(password);
	std::string password_hex = Utility::BinaryToHex(encrypted_password);

	std::string query_string = "UPDATE user SET password = ? WHERE login = 'admin'";

	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);

	if (!stmt)
	{
		std::cerr << "mysql_stmt_init(), out of memory\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		return false;
	}

	if (mysql_stmt_prepare(stmt, query_string.c_str(), query_string.length()) != 0)
	{
		std::cerr << "mysql_stmt_prepare(), UPDATE failed for query: " << query_string << '\n';
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	MYSQL_BIND bind[1];
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = const_cast<char*>(password_hex.c_str());
	bind[0].buffer_length = password_hex.length();
	bind[0].is_null = 0;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		std::cerr << "mysql_stmt_bind_param() failed\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		std::cerr << "mysql_stmt_execute(), UPDATE failed\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	mysql_stmt_close(stmt);

	return true;
}

bool Database::InsertUser(user_info& uinfo)
{
	std::vector<unsigned char> encrypted_password = Utility::Encrypt(uinfo.password);
	std::string password_hex = Utility::BinaryToHex(encrypted_password);  
	std::string query_string = "INSERT INTO user (login, password, last_visited_at) VALUES (?, ?, NOW())";

	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);

	if (!stmt) {
		std::cerr << "mysql_stmt_init(), out of memory\n";
		return false;
	}

	if (mysql_stmt_prepare(stmt, query_string.c_str(), query_string.length()) != 0) {
		std::cerr << "mysql_stmt_prepare(), INSERT failed for query: " << query_string << '\n';
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';  
		mysql_stmt_close(stmt);
		return false;
	}

	MYSQL_BIND bind[2];
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = const_cast<char*>(uinfo.login.c_str());
	bind[0].buffer_length = uinfo.login.length();
	bind[0].is_null = 0;

	bind[1].buffer_type = MYSQL_TYPE_STRING;  
	bind[1].buffer = const_cast<char*>(password_hex.c_str());
	bind[1].buffer_length = password_hex.length();
	bind[1].is_null = 0;

	if (mysql_stmt_bind_param(stmt, bind) != 0) {
		std::cerr << "mysql_stmt_bind_param() failed\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';  
		mysql_stmt_close(stmt);
		return false;
	}

	if (mysql_stmt_execute(stmt) != 0) {
		std::cerr << "mysql_stmt_execute(), INSERT failed\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';  
		mysql_stmt_close(stmt);
		return false;
	}

	mysql_stmt_close(stmt);

	return true;
}

bool Database::UpdateUserInfo(const std::string& login)
{
	std::string query_string = "UPDATE user SET last_visited_at = NOW() WHERE login = ?";

	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
	if (!stmt)
	{
		std::cerr << "mysql_stmt_init(), out of memory\n";
		return false;
	}

	if (mysql_stmt_prepare(stmt, query_string.c_str(), query_string.length()) != 0)
	{
		std::cerr << "mysql_stmt_prepare(), UPDATE failed for query: " << query_string << '\n';
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	MYSQL_BIND bind[1];
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = const_cast<char*>(login.c_str());
	bind[0].buffer_length = login.length();
	bind[0].is_null = 0;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		std::cerr << "mysql_stmt_bind_param() failed\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		std::cerr << "mysql_stmt_execute(), UPDATE failed\n";
		std::cerr << "Error: " << mysql_stmt_error(stmt) << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	mysql_stmt_close(stmt);
	return true;
}

bool Database::UserExist(const std::string& login)
{
	int count = 0;

	std::string query_string = "SELECT COUNT(*) FROM user WHERE login = ?";

	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);

	if (!stmt)
	{
		std::cerr << "mysql_stmt_init(), out of memory\n";
		return false;
	}

	if (mysql_stmt_prepare(stmt, query_string.c_str(), query_string.length()) != 0)
	{
		std::cerr << "mysql_stmt_prepare(), SELECT failed for query: " << query_string << '\n';
		mysql_stmt_close(stmt);
		return false;
	}

	MYSQL_BIND bind[1];
	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = const_cast<char*>(login.c_str());
	bind[0].buffer_length = login.length();
	bind[0].is_null = 0;

	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		std::cerr << "mysql_stmt_bind_param() failed\n";
		mysql_stmt_close(stmt);
		return 0;
	}

	MYSQL_BIND result_bind;
	memset(&result_bind, 0, sizeof(result_bind));

	result_bind.buffer_type = MYSQL_TYPE_LONG;
	result_bind.buffer = &count;
	result_bind.is_null = 0;

	if (mysql_stmt_bind_result(stmt, &result_bind) != 0)
	{
		std::cerr << "mysql_stmt_bind_result() failed\n";
		mysql_stmt_close(stmt);
		return 0;
	}

	if (mysql_stmt_execute(stmt) != 0)
	{
		std::cerr << "mysql_stmt_exectute(), SELECT failed for query: " << query_string << '\n';
		mysql_stmt_close(stmt);
		return 0;
	}

	if (mysql_stmt_fetch(stmt) != 0)
	{
		std::cerr << "mysql_stmt_fetch(), SELECT failed for query: " << query_string << '\n';
		mysql_stmt_close(stmt);
		return 0;
	}

	mysql_stmt_close(stmt);

	return count != 0;
}

Database::~Database() noexcept
{
	mysql_free_result(res);
	mysql_close(connection);
}