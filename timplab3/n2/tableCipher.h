#pragma once
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <locale>
#include <codecvt>

// Класс для исключений шифрования табличной перестановки
class table_cipher_error : public std::exception {
private:
    std::string message;
public:
    explicit table_cipher_error(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class TableCipher {
private:
    int numColumns; // количество столбцов таблицы (ключ)

    // Статические члены для алфавита
    static const std::wstring numAlpha; // Русский алфавит (объявление)
    static std::map <wchar_t, int> alphaNum; // Ассоциативный массив "номер по символу" (объявление)
    static bool isInitialized; // Флаг для однократной инициализации alphaNum (объявление)

    // Вспомогательные методы для преобразования
    std::vector<int> convert(const std::wstring& s); // строка -> вектор индексов
    std::wstring convert(const std::vector<int>& v); // вектор индексов -> строка

    // Проверка ключа
    void validateKey(int key);

    // Вспомогательный метод для инициализации alphaNum
    static void initializeAlphaNum();

public:
    // Конструктор с ключом
    TableCipher(int key);

    // Метод шифрования
    std::wstring encrypt(const std::wstring& text);

    // Метод дешифрования
    std::wstring decrypt(const std::wstring& cipher_text);
};

// --- УДАЛЕНА СТРОКА: extern const std::wstring TableCipher::numAlpha; ---