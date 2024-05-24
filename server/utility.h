#pragma once
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>
#include <iomanip>

namespace Utility
{
	void InitOpenSSL();
	void CleanupOpenSSL();
	std::string BinaryToHex(const std::vector<unsigned char>& data);
	std::vector<unsigned char> HexToBinary(const std::string& hex);
	std::vector<unsigned char> Encrypt(const std::string& plaintext);
	std::string Decrypt(const std::vector<unsigned char>& ciphertext);
}
