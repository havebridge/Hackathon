#include "utility.h"


namespace Utility
{
	void InitOpenSSL() 
	{
		OpenSSL_add_all_algorithms();
	}

	void CleanupOpenSSL() 
	{
		EVP_cleanup();
	}

	const unsigned char* aes_key = reinterpret_cast<const unsigned char*>("01234567890123456789012345678901");
	const unsigned char* aes_iv = reinterpret_cast<const unsigned char*>("0123456789012345");


    std::vector<unsigned char> Encrypt(const std::string& plaintext)
    {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            throw std::runtime_error("EVP_CIPHER_CTX_new failed");

        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, aes_iv) != 1)
            throw std::runtime_error("EVP_EncryptInit_ex failed");

        std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
        int len = 0, ciphertext_len = 0;

        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, reinterpret_cast<const unsigned char*>(plaintext.data()), plaintext.size()) != 1)
            throw std::runtime_error("EVP_EncryptUpdate failed");

        ciphertext_len = len;

        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1)
            throw std::runtime_error("EVP_EncryptFinal_ex failed");

        ciphertext_len += len;
        ciphertext.resize(ciphertext_len);

        EVP_CIPHER_CTX_free(ctx);
        return ciphertext;
    }

    std::string Decrypt(const std::vector<unsigned char>& ciphertext)
    {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            throw std::runtime_error("EVP_CIPHER_CTX_new failed");

        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, aes_iv) != 1)
            throw std::runtime_error("EVP_DecryptInit_ex failed");

        std::vector<unsigned char> plaintext(ciphertext.size());
        int len = 0, plaintext_len = 0;

        if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1)
            throw std::runtime_error("EVP_DecryptUpdate failed");

        plaintext_len = len;

        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1)
            throw std::runtime_error("EVP_DecryptFinal_ex failed");

        plaintext_len += len;
        plaintext.resize(plaintext_len);

        EVP_CIPHER_CTX_free(ctx);
        return std::string(plaintext.begin(), plaintext.end());
    }

    std::string BinaryToHex(const std::vector<unsigned char>& data)
    {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (unsigned char byte : data) {
            oss << std::setw(2) << static_cast<unsigned>(byte);
        }
        return oss.str();
    }


    std::vector<unsigned char> HexToBinary(const std::string& hex) {
        std::vector<unsigned char> result;
        for (size_t i = 0; i < hex.length(); i += 2) {
            result.push_back(static_cast<unsigned char>(std::stoi(hex.substr(i, 2), nullptr, 16)));
        }
        return result;
    }
}