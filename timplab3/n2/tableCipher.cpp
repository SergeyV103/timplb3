#include "tableCipher.h"
#include <algorithm>
#include <iostream>
#include <cwctype>

// Конструктор с установкой ключа
TableCipher::TableCipher(int key) {
    validateKey(key); // проверка корректности ключа
    numColumns = key; // установка количества столбцов
}

// Проверка ключа
void TableCipher::validateKey(int key) {
    if (key <= 0) {
        throw table_cipher_error("Ключ должен быть положительным числом");
    }
    if (key > 1000) { // Добавляем разумное ограничение
        throw table_cipher_error("Ключ слишком большой. Максимальное значение: 1000");
    }
}

// Метод шифрования
std::wstring TableCipher::encrypt(const std::wstring& text) {
    // Проверка входного текста
    if (text.empty()) {
        throw table_cipher_error("Пустой текст для шифрования!");
    }
    
    // Проверка символов текста
    for (wchar_t c : text) {
        if (!std::iswalpha(c) && c != L' ') {
            throw table_cipher_error("Текст содержит недопустимые символы! Разрешены только буквы и пробелы.");
        }
    }
    
    // Проверка, что ключ не больше длины текста
    if (numColumns > text.length()) {
        throw table_cipher_error("Ключ не может быть больше длины текста");
    }
    
    // Вычисление количества строк
    int numRows = (text.length() + numColumns - 1) / numColumns;
    
    // Проверка на слишком большую таблицу
    if (numRows > 10000 || numColumns > 10000) {
        throw table_cipher_error("Слишком большая таблица для шифрования");
    }
    
    // Создание таблицы для заполнения
    std::vector<std::vector<wchar_t>> table(numRows, std::vector<wchar_t>(numColumns, L' '));
    
    // ЗАПИСЬ: по горизонтали слева направо, сверху вниз
    int index = 0;
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numColumns; col++) {
            if (index < text.length()) {
                table[row][col] = text[index++];
            }
        }
    }
    
    // ЧТЕНИЕ: сверху вниз, справа налево
    std::wstring result;
    for (int col = numColumns - 1; col >= 0; col--) {
        for (int row = 0; row < numRows; row++) {
            // Берем символ, только если он реально был в исходном тексте
            int original_index = row * numColumns + col;
            if (original_index < text.length()) {
                result += text[original_index];
            }
        }
    }
    
    return result;
}

// Метод дешифрования
std::wstring TableCipher::decrypt(const std::wstring& cipher_text) {
    // Проверка зашифрованного текста
    if (cipher_text.empty()) {
        throw table_cipher_error("Пустой текст для расшифровки!");
    }
    
    // Проверка символов зашифрованного текста
    for (wchar_t c : cipher_text) {
        if (!std::iswalpha(c) && c != L' ') {
            throw table_cipher_error("Зашифрованный текст содержит недопустимые символы!");
        }
    }
    
    // Проверка, что ключ не больше длины текста
    if (numColumns > cipher_text.length()) {
        throw table_cipher_error("Ключ не может быть больше длины зашифрованного текста");
    }
    
    // Вычисление количества строк
    int numRows = (cipher_text.length() + numColumns - 1) / numColumns;
    
    // Вычисление количества символов в последней строке
    int lastRowLength = cipher_text.length() % numColumns;
    if (lastRowLength == 0) lastRowLength = numColumns;
    
    // Создание пустой таблицы
    std::vector<std::vector<wchar_t>> table(numRows, std::vector<wchar_t>(numColumns, L' '));
    
    // ЗАПИСЬ: заполняем таблицу по столбцам справа налево, сверху вниз
    int index = 0;
    for (int col = numColumns - 1; col >= 0; col--) {
        for (int row = 0; row < numRows; row++) {
            // Пропускаем ячейки в неполных столбцах последней строки
            if (row == numRows - 1 && col >= lastRowLength) {
                continue;
            }
            if (index < cipher_text.length()) {
                table[row][col] = cipher_text[index++];
            }
        }
    }
    
    // ЧТЕНИЕ: по строкам слева направо, сверху вниз
    std::wstring result;
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numColumns; col++) {
            if (table[row][col] != L' ') {
                result += table[row][col];
            }
        }
    }
    
    return result;
}