#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/aes.h>

const std::string FILENAME = "passwords.txt";
const int KEY_SIZE = 32;

struct PasswordEntry {
    std::string website;
    std::string username;
    std::string password;
};

std::string encrypt(const std::string& plaintext, const std::string& key) {
    std::string ciphertext;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr);

    int len;
    int ciphertext_len;

    ciphertext.resize(plaintext.size() + AES_BLOCK_SIZE);

    EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(&ciphertext[0]), &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&ciphertext[ciphertext_len]), &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    ciphertext.resize(ciphertext_len);
    return ciphertext;
}

std::string decrypt(const std::string& ciphertext, const std::string& key) {
    std::string plaintext;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char*>(key.c_str()), nullptr);

    int len;
    int plaintext_len;

    plaintext.resize(ciphertext.size());

    EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]), &len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&plaintext[plaintext_len]), &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(plaintext_len);
    return plaintext;
}

std::string generateRandomPassword(int length) {
    std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;':,.<>?";
    std::string password;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < length; ++i) {
        password += chars[std::rand() % chars.length()];
    }

    return password;
}

std::string readMasterPassword() {
    std::string masterPassword;
    std::cout << "Enter master password: ";
    std::getline(std::cin, masterPassword);
    return masterPassword;
}

void savePasswords(const std::vector<PasswordEntry>& passwords, const std::string& masterPassword) {
    std::ofstream file(FILENAME);
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::string encryptedMasterPassword = encrypt(masterPassword, masterPassword);

    file << encryptedMasterPassword << std::endl;

    for (const auto& entry : passwords) {
        std::string encryptedEntry = encrypt(entry.website + "\n" + entry.username + "\n" + entry.password, masterPassword);
        file << encryptedEntry << std::endl;
    }

    file.close();
}

std::vector<PasswordEntry> loadPasswords(const std::string& masterPassword) {
    std::vector<PasswordEntry> passwords;
    std::ifstream file(FILENAME);
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return passwords;
    }

    std::string encryptedMasterPassword;
    std::getline(file, encryptedMasterPassword);

    std::string decryptedMasterPassword = decrypt(encryptedMasterPassword, masterPassword);

    if (decryptedMasterPassword != masterPassword) {
        std::cerr << "Invalid master password." << std::endl;
        return passwords;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string decryptedEntry = decrypt(line, masterPassword);
        std::istringstream iss(decryptedEntry);
        std::string website, username, password;
        std::getline(iss, website);
        std::getline(iss, username);
        std::getline(iss, password);
        passwords.push_back({ website, username, password });
    }

    file.close();
    return passwords;
}

void printPasswords(const std::vector<PasswordEntry>& passwords) {
    std::cout << "Website\t\tUsername\tPassword" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    for (const auto& entry : passwords) {
        std::cout << std::left << std::setw(15) << entry.website << "\t" << std::setw(15) << entry.username << "\t" << entry.password << std::endl;
    }
    std::cout << std::endl;
}

void addPassword(std::vector<PasswordEntry>& passwords) {
    std::string website, username, password;
    std::cout << "Enter website: ";
    std::getline(std::cin, website);
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter password (leave empty to generate a random password): ";
    std::getline(std::cin, password);

    if (password.empty()) {
        int length;
        std::cout << "Enter password length: ";
        std::cin >> length;
        std::cin.ignore(); // Ignore the newline character left in the input stream
        password = generateRandomPassword(length);
    }

    passwords.push_back({ website, username, password });
    std::cout << "Password added successfully." << std::endl;
}

void updatePassword(std::vector<PasswordEntry>& passwords) {
    std::string website;
    std::cout << "Enter website to update: ";
    std::getline(std::cin, website);

    auto it = std::find_if(passwords.begin(), passwords.end(), [&](const PasswordEntry& entry) {
        return entry.website == website;
    });

    if (it != passwords.end()) {
        std::string username, password;
        std::cout << "Enter new username: ";
        std::getline(std::cin, username);
        std::cout << "Enter new password (leave empty to generate a random password): ";
        std::getline(std::cin, password);

        if (password.empty()) {
            int length;
            std::cout << "Enter password length: ";
            std::cin >> length;
            std::cin.ignore(); // Ignore the newline character left in the input stream
            password = generateRandomPassword(length);
        }

        it->username = username;
        it->password = password;
        std::cout << "Password updated successfully." << std::endl;
    }
    else {
        std::cout << "Website not found." << std::endl;
    }
}

void deletePassword(std::vector<PasswordEntry>& passwords) {
    std::string website;
    std::cout << "Enter website to delete: ";
    std::getline(std::cin, website);

    auto it = std::find_if(passwords.begin(), passwords.end(), [&](const PasswordEntry& entry) {
        return entry.website == website;
    });

    if (it != passwords.end()) {
        passwords.erase(it);
        std::cout << "Password deleted successfully." << std::endl;
    }
    else {
        std::cout << "Website not found." << std::endl;
    }
}

int main() {
    std::string masterPassword = readMasterPassword();
    std::vector<PasswordEntry> passwords = loadPasswords(masterPassword);

    int choice;
    do {
        std::cout << "Password Manager" << std::endl;
        std::cout << "1. View Passwords" << std::endl;
        std::cout << "2. Add Password" << std::endl;
        std::cout << "3. Update Password" << std::endl;
        std::cout << "4. Delete Password" << std::endl;
        std::cout << "5. Quit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Ignore the newline character left in the input stream

        switch (choice) {
            case 1:
                printPasswords(passwords);
                break;
            case 2:
                addPassword(passwords);
                break;
            case 3:
                updatePassword(passwords);
                break;
            case 4:
                deletePassword(passwords);
                break;
            case 5:
                savePasswords(passwords, masterPassword);
                std::cout << "Passwords saved successfully. Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Try again." << std::endl;
                break;
        }

        std::cout << std::endl;
    } while (choice != 5);

    return 0;
}
