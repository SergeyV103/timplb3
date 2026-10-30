#include <UnitTest++/UnitTest++.h>
#include <locale>
#include <codecvt>
#include <cwctype>
#include <string>
#include <vector>
#include "modAlphaCipher.h" 

// Установка локали для корректной работы с wstring в тестах.
namespace {
    struct TestSuiteSetup {
        TestSuiteSetup() {
            std::locale::global(std::locale("ru_RU.UTF-8"));
        }
    } suiteSetup;
}

// Фикстура с простым ключом 'А' (сдвиг 0)
struct KeyA_Fixture {
    modAlphaCipher* p;
    KeyA_Fixture() { p = new modAlphaCipher(L"А"); }
    ~KeyA_Fixture() { delete p; }
};

// Фикстура с ключом 'Б' (сдвиг +1/-1)
struct KeyB_Fixture {
    modAlphaCipher* p;
    KeyB_Fixture() { p = new modAlphaCipher(L"Б"); }
    ~KeyB_Fixture() { delete p; }
};

// Фикстура с ключом 'В' (сдвиг +2/-2)
struct KeyV_Fixture {
    modAlphaCipher* p;
    KeyV_Fixture() { p = new modAlphaCipher(L"В"); }
    ~KeyV_Fixture() { delete p; }
};

// Тесты для конструктора
SUITE(ConstructorTests)
{
    TEST(ValidKeyUppercase) {
        modAlphaCipher cipher(L"ЗАЩИТА");
    }

    TEST(ValidKeyLowercase) {
        modAlphaCipher cipher(L"ключ");
    }

    TEST(KeyWithDigits) {
        CHECK_THROW(modAlphaCipher cipher(L"КЛЮЧ123"), cipher_error);
    }

    TEST(KeyWithPunctuation) {
        CHECK_THROW(modAlphaCipher cipher(L"К,Ю.Ч"), cipher_error);
    }

    TEST(KeyWithSpaces) {
        CHECK_THROW(modAlphaCipher cipher(L"К Л Ю Ч"), cipher_error);
    }

    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cipher(L""), cipher_error);
    }

    TEST(KeyWithNonCyrillic) {
        CHECK_THROW(modAlphaCipher cipher(L"KEY"), cipher_error);
    }
}

// Тесты для encrypt
SUITE(EncryptTests)
{
    TEST_FIXTURE(KeyA_Fixture, UppercaseString) {
        std::wstring input = L"ПРОГРАММИРОВАНИЕ";
        std::wstring expected = L"ПРОГРАММИРОВАНИЕ"; // Сдвиг 0
        CHECK(expected == p->encrypt(input));
    }

    TEST_FIXTURE(KeyB_Fixture, LowercaseString) {
        std::wstring input = L"информатика";
        std::wstring expected = L"ЙОХПСНБУЙЛБ"; // Сдвиг +1
        CHECK(expected == p->encrypt(input));
    }

       
    TEST_FIXTURE(KeyV_Fixture, StringWithSpacesRoundTrip) { 
        std::wstring original_input_with_spaces = L"БЕЗ ОПАСНОСТЬ"; 
        
        std::wstring encrypted_result = p->encrypt(original_input_with_spaces); 
        
        std::wstring decrypted_result = p->decrypt(encrypted_result); 
        
        std::wstring expected_decrypted = L"БЕЗОПАСНОСТЬ"; 
        CHECK(expected_decrypted == decrypted_result); 
        
    }
    TEST_FIXTURE(KeyB_Fixture, StringWithNumbers) {
        std::wstring input = L"ДАННЫЕ895";
        CHECK_THROW(p->encrypt(input), cipher_error);
    }

    TEST_FIXTURE(KeyA_Fixture, StringWithLatin) {
        std::wstring input = L"ТЕКСТABC";
        std::wstring expected = L"ТЕКСТ"; // Латиница удаляется, сдвиг 0
        CHECK(expected == p->encrypt(input));
    }

    TEST_FIXTURE(KeyB_Fixture, EmptyText) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }

    TEST_FIXTURE(KeyV_Fixture, NoCyrillicText) {
        CHECK_THROW(p->encrypt(L"123456"), cipher_error);
    }

    TEST_FIXTURE(KeyA_Fixture, OnlySpacesText) {
        CHECK_THROW(p->encrypt(L"   "), cipher_error);
    }
}

// Тесты для decrypt
SUITE(DecryptTests)
{
    TEST_FIXTURE(KeyA_Fixture, UppercaseString) {
        std::wstring input = L"ЗАЩИТА";
        std::wstring expected = L"ЗАЩИТА"; // Сдвиг 0
        CHECK(expected == p->decrypt(input));
    }

    TEST_FIXTURE(KeyB_Fixture, DecryptEncryptedString) {
        std::wstring input = L"ЙОХПСНБУЙЛБ"; // Результат шифрования "ИНФОРМАТИКА" ключом Б
        std::wstring expected = L"ИНФОРМАТИКА";
        CHECK(expected == p->decrypt(input));
    }
    
    TEST_FIXTURE(KeyV_Fixture, StringWithoutSpaces) {
        std::wstring original = L"ГРОМ";
        std::wstring expected_encrypted = L"ЕТРО"; // Г(3)+2=5(Д - нет, Е), Р(17)+2=19(Т), О(15)+2=17(Р), М(13)+2=15(О) -> ЕТРО
        std::wstring encrypted = p->encrypt(original);
        CHECK(expected_encrypted == encrypted);
        
        std::wstring decrypted = p->decrypt(encrypted);
        CHECK(original == decrypted);
    }

    TEST_FIXTURE(KeyB_Fixture, StringWithNumbers) {
        std::wstring input = L"ЕБООЙЖ123";
        CHECK_THROW(p->decrypt(input), cipher_error);
    }

    TEST_FIXTURE(KeyA_Fixture, StringWithLatin) {
        std::wstring input = L"ТЕКСТXYZ";
        std::wstring expected = L"ТЕКСТ";
        CHECK(expected == p->decrypt(input));
    }

    TEST_FIXTURE(KeyB_Fixture, EmptyText) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }

    TEST_FIXTURE(KeyV_Fixture, NoCyrillicText) {
        CHECK_THROW(p->decrypt(L"123456"), cipher_error);
    }

    TEST_FIXTURE(KeyA_Fixture, OnlySpacesText) {
        CHECK_THROW(p->decrypt(L"   "), cipher_error);
    }
}

int main(int argc, char* argv[])
{
    return UnitTest::RunAllTests();
}