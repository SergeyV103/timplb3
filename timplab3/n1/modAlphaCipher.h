#pragma once
#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>

// Класс для исключений шифрования
class cipher_error : public std::exception {
private:
    std::string message;
public:
    explicit cipher_error(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class modAlphaCipher
{
private:
    std::wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::map <wchar_t,int> alphaNum;
    std::vector <int> key;
    std::vector<int> convert(const std::wstring& s);
    std::wstring convert(const std::vector<int>& v);
    std::wstring toUpper(const std::wstring& s);
public:
    modAlphaCipher()=delete;
    modAlphaCipher(const std::wstring& skey);
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};