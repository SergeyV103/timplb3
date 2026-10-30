#include <UnitTest++/UnitTest++.h>
#include <locale>
#include <codecvt>
#include <cwctype>
#include <string>
#include <vector>
#include "tableCipher.h" 

namespace {
    struct TestSuiteSetup {
        TestSuiteSetup() {
            std::locale::global(std::locale("ru_RU.UTF-8"));
        }
    } suiteSetup;
}

struct Key3_Fixture {
    TableCipher* p;
    Key3_Fixture() { p = new TableCipher(3); }
    ~Key3_Fixture() { delete p; }
};

struct Key2_Fixture {
    TableCipher* p;
    Key2_Fixture() { p = new TableCipher(2); }
    ~Key2_Fixture() { delete p; }
};

struct Key4_Fixture {
    TableCipher* p;
    Key4_Fixture() { p = new TableCipher(4); }
    ~Key4_Fixture() { delete p; }
};

SUITE(ConstructorTests)
{
    TEST(ValidKeyPositive) { TableCipher cipher(3); }
    TEST(ValidKeyOne) { TableCipher cipher(1); }
    TEST(NegativeKey) { CHECK_THROW(TableCipher cipher(-5), table_cipher_error); }
    TEST(ZeroKey) { CHECK_THROW(TableCipher cipher(0), table_cipher_error); }
    TEST(KeyTooLarge) { CHECK_THROW(TableCipher cipher(1001), table_cipher_error); }
}

SUITE(EncryptTests)
{
    TEST_FIXTURE(Key3_Fixture, UppercaseString) {
        std::wstring input = L"ПРОГРАММИРОВАНИЕ";
        std::wstring expected = L"ОАИВИРРМОНПГМРАЕ"; 
        CHECK(expected == p->encrypt(input));
    }

    TEST_FIXTURE(Key2_Fixture, LowercaseString) {
        std::wstring input = L"информатика";
        std::wstring expected = L"номткифраиа"; 
        CHECK(expected == p->encrypt(input));
    }

    TEST_FIXTURE(Key3_Fixture, StringWithNumbers) {
        CHECK_THROW(p->encrypt(L"ПРОГРАММА123"), table_cipher_error);
    }

    TEST_FIXTURE(Key3_Fixture, EmptyText) {
        CHECK_THROW(p->encrypt(L""), table_cipher_error);
    }

    TEST(KeyShorterThanKey) {
        TableCipher cipher(5);
        CHECK_THROW(cipher.encrypt(L"ОБЛА"), table_cipher_error);
    }
}

SUITE(DecryptTests)
{
    TEST_FIXTURE(Key3_Fixture, UppercaseString) {
        std::wstring input = L"ОАИВИРРМОНПГМРАЕ";
        std::wstring expected = L"ПРОГРАММИРОВАНИЕ";
        CHECK(expected == p->decrypt(input));
    }

    TEST_FIXTURE(Key3_Fixture, StringWithNumbers) {
        CHECK_THROW(p->decrypt(L"H3LL0W0RLD"), table_cipher_error);
    }

    TEST_FIXTURE(Key3_Fixture, EmptyText) {
        CHECK_THROW(p->decrypt(L""), table_cipher_error);
    }

    TEST(KeyShorterThanKey) {
        TableCipher cipher(6);
        CHECK_THROW(cipher.decrypt(L"ОБЛАК"), table_cipher_error);
    }

    TEST_FIXTURE(Key2_Fixture, RoundTrip) {
        std::wstring original = L"ИНТЕРНЕТ";
        std::wstring encrypted = p->encrypt(original);
        std::wstring decrypted = p->decrypt(encrypted);
        CHECK(original == decrypted);
    }
}

int main(int argc, char* argv[])
{
    return UnitTest::RunAllTests();
}