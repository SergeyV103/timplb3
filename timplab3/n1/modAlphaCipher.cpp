#include "modAlphaCipher.h"
#include <locale>
#include <codecvt>
#include <iostream>
#include <cwctype>

modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
    // Проверка ключа на пустоту
    if (skey.empty()) {
        throw cipher_error("Пустой ключ! Ключ не может быть пустой строкой.");
    }
    
    // Инициализация алфавита (должна быть ДО проверки символов)
    for(unsigned i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }

    // Проверка ключа на допустимые символы (только русские буквы)
    for (wchar_t c : skey) {
        if (alphaNum.find(std::towupper(c)) == alphaNum.end()) {
            throw cipher_error("Недопустимый символ в ключе! Ключ должен содержать только буквы русского алфавита.");
        }
    }
    
    key = convert(toUpper(skey));
    // Проверка результата конвертации ключа
    if (key.empty()) {
        throw cipher_error("Ключ не содержит допустимых символов русского алфавита.");
    }
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    // Проверка входного текста
    if (open_text.empty()) {
        throw cipher_error("Пустой текст для шифрования!");
    }
    
    // Проверка символов текста
    for (wchar_t c : open_text) {
        if (!std::iswalpha(c) && c != L' ') {
            throw cipher_error("Текст содержит недопустимые символы! Разрешены только буквы и пробелы.");
        }
    }
    
    std::wstring text = toUpper(open_text);
    std::vector<int> work = convert(text);
    
    // Проверка результата конвертации
    if (work.empty()) {
        throw cipher_error("Текст не содержит символов русского алфавита после обработки.");
    }
    
    // Шифрование
    for(unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
        // Дополнительная проверка на корректность индекса
        if (work[i] < 0 || work[i] >= static_cast<int>(alphaNum.size())) {
            throw cipher_error("Ошибка при шифровании: некорректный индекс символа.");
        }
    }
    
    return convert(work);
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    // Проверка зашифрованного текста
    if (cipher_text.empty()) {
        throw cipher_error("Пустой текст для расшифровки!");
    }
    
    // Проверка символов зашифрованного текста
    for (wchar_t c : cipher_text) {
        if (!std::iswalpha(c) && c != L' ') {
            throw cipher_error("Зашифрованный текст содержит недопустимые символы!");
        }
    }
    
    std::wstring text = toUpper(cipher_text);
    std::vector<int> work = convert(text);
    
    // Проверка результата конвертации
    if (work.empty()) {
        throw cipher_error("Зашифрованный текст не содержит символов русского алфавита.");
    }
    
    // Расшифрование
    for(unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
        // Дополнительная проверка на корректность индекса
        if (work[i] < 0 || work[i] >= static_cast<int>(alphaNum.size())) {
            throw cipher_error("Ошибка при расшифровке: некорректный индекс символа.");
        }
    }
    
    return convert(work);
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s)
{
    std::vector<int> result;
    for(auto c : s) {
        auto it = alphaNum.find(c);
        if (it != alphaNum.end()) {
            result.push_back(it->second);
        }
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v)
{
    std::wstring result;
    for(auto i : v) {
        if (i < 0 || i >= static_cast<int>(numAlpha.size())) {
            throw cipher_error("Индекс символа выходит за границы алфавита.");
        }
        result.push_back(numAlpha[i]);
    }
    return result;
}

std::wstring modAlphaCipher::toUpper(const std::wstring& s)
{
    std::wstring result;
    std::locale loc("ru_RU.UTF-8");
    
    for(auto c : s) {
        result.push_back(std::toupper(c, loc));
    }
    return result;
}