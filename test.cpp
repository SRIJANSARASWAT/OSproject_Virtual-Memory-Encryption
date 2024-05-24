#include <iostream>        // For input and output operations
#include <fstream>         // For file input and output operations
#include <iomanip>         // For setting output format (e.g., width, fill)
#include <windows.h>       // For memory management functions on Windows
#include <openssl/conf.h>  // For OpenSSL configuration
#include <openssl/evp.h>   // For OpenSSL high-level cryptographic API
#include <openssl/err.h>   // For OpenSSL error handling
#include <chrono>          // For high-resolution clock operations
#include <ctime>           // For time-related functions
#include <cstring>         // For C-style string functions

#define PAGE_SIZE 4096     // Define the size of a memory page

// Function to handle errors and print them
void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

// Function to encrypt or decrypt a memory page
void encrypt_decrypt_page(void *addr, size_t len, bool encrypt, const unsigned char *key, const unsigned char *iv) {
    int outlen, tmplen;
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();  // Create a new cipher context
    if (!ctx)
        handleErrors(); 

    // Initialize the cipher context for encryption or decryption
    if (encrypt) {
        if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1)
            handleErrors();
    } else {
        if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1)
            handleErrors();
    }

    // Encrypt or decrypt the data
    if (encrypt) {
        if (EVP_EncryptUpdate(ctx, (unsigned char *)addr, &outlen, (unsigned char *)addr, len) != 1)
            handleErrors();
    } else {
        if (EVP_DecryptUpdate(ctx, (unsigned char *)addr, &outlen, (unsigned char *)addr, len) != 1)
            handleErrors();
    }

    // Finalize the encryption or decryption
    if (encrypt) {
        if (EVP_EncryptFinal_ex(ctx, (unsigned char *)addr + outlen, &tmplen) != 1)
            handleErrors();
    } else {
        if (EVP_DecryptFinal_ex(ctx, (unsigned char *)addr + outlen, &tmplen) != 1)
            handleErrors();
    }

    EVP_CIPHER_CTX_free(ctx);  // Free the cipher context
}

// Function to allocate memory securely
void *secure_malloc(size_t size) {
    size_t rounded_size = ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;  // Round size to nearest page size
    void *ptr = VirtualAlloc(NULL, rounded_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);  // Allocate memory
    if (ptr == nullptr) {
        std::cerr << "Memory allocation failed\n";
        return nullptr;
    }

    std::memset(ptr, 0, rounded_size);  // Initialize allocated memory to zero

    return ptr;
}

// Function to free memory securely
void secure_free(void *ptr, size_t size, const unsigned char *key, const unsigned char *iv) {
    encrypt_decrypt_page(ptr, size, false, key, iv);  // Decrypt memory content before freeing
    VirtualFree(ptr, 0, MEM_RELEASE);  // Free the memory
}

// Function to simulate memory usage
void simulate_memory_usage() {
   const char *text = "Hello, encrypted memory!";
   size_t size = strlen(text) + 1;
   unsigned char *key = (unsigned char *)"0123456789012345";
   unsigned char *iv = (unsigned char *)"0123456789012345";

   void *ptr = secure_malloc(size);  // Allocate memory

   if (ptr != nullptr) {
       std::strcpy((char *)ptr, text);  // Copy text to allocated memory
       encrypt_decrypt_page(ptr, size, true, key, iv);  // Encrypt the memory content
       std::cout << "Encrypted Memory Content: " << (char *)ptr << std::endl;
       secure_free(ptr, size, key, iv);  // Free the memory
   }
}

// Function to measure memory usage
void measure_memory_usage() {
   size_t size = PAGE_SIZE * 10;
   unsigned char *key = (unsigned char *)"0123456789012345";
   unsigned char *iv = (unsigned char *)"0123456789012345";

   void *ptr = secure_malloc(size);  // Allocate memory

   if (ptr != nullptr) {
       std::cout << "Memory allocated successfully\n";
       secure_free(ptr, size, key, iv);  // Free the memory
   } else {
       std::cerr << "Memory allocation failed\n";
   }
}

// Function to demonstrate encryption and decryption
void demonstrate_encryption_decryption() {
   const char *text = "Hello, encrypted memory!";
   size_t size = strlen(text) + 1;
   unsigned char *key = (unsigned char *)"0123456789012345";
   unsigned char *iv = (unsigned char *)"0123456789012345";

   void *ptr = secure_malloc(size);  // Allocate memory

   if (ptr != nullptr) {
       std::strcpy((char *)ptr, text);  // Copy text to allocated memory
       std::cout << "Original Memory Content: " << (char *)ptr << std::endl;
       encrypt_decrypt_page(ptr, size, true, key, iv);  // Encrypt the memory content
       std::cout << "Encrypted Memory Content: " << (char *)ptr << std::endl;
       encrypt_decrypt_page(ptr, size, false, key, iv);  // Decrypt the memory content
       std::cout << "Decrypted Memory Content: " << (char *)ptr << std::endl;
       secure_free(ptr, size, key, iv);  // Free the memory
   }
}

// Function to generate random data and fill memory with it
void generate_random_data(void *ptr, size_t size) {
   srand(static_cast<unsigned>(time(NULL)));  // Seed random number generator
   for (size_t i = 0; i < size; ++i) {
       ((char *)ptr)[i] = rand() % 256;  // Fill memory with random data
   }
}

// Function to perform random data encryption
void perform_random_data_encryption() {
    const size_t size = PAGE_SIZE * 10;
    unsigned char *key = (unsigned char *)"0123456789012345";
    unsigned char *iv = (unsigned char *)"0123456789012345";

    void *ptr = secure_malloc(size);  // Allocate memory

    if (ptr != nullptr) {
        generate_random_data(ptr, size);  // Generate random data
        encrypt_decrypt_page(ptr, size, true, key, iv);  // Encrypt the random data
        std::cout << "Random data generated and encrypted successfully\n";
        secure_free(ptr, size, key, iv);  // Free the memory
    }
}

// Function to save encrypted data to a file
void save_encrypted_data(const void *ptr, size_t size, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);  // Open file for binary writing
    if (file.is_open()) {
        file.write(reinterpret_cast<const char *>(ptr), size);  // Write data to file
        std::cout << "Encrypted data saved to file: " << filename << std::endl;
    } else {
        std::cerr << "Failed to open file for writing\n";
    }
}

// Function to load encrypted data from a file and decrypt it
void load_and_decrypt_data(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);  // Open file for binary reading
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();  // Get file size
        file.seekg(0, std::ios::beg);

        unsigned char *key = (unsigned char *)"0123456789012345";
        unsigned char *iv = (unsigned char *)"0123456789012345";
        
        void *ptr = secure_malloc(size);  // Allocate memory
        if (ptr != nullptr) {
            file.read(reinterpret_cast<char *>(ptr), size);  // Read data from file
            encrypt_decrypt_page(ptr, size, false, key, iv);  // Decrypt the data
            std::cout << "Decrypted Data from File: " << std::endl;
            std::cout << reinterpret_cast<const char *>(ptr) << std::endl;
            secure_free(ptr, size, key, iv);  // Free the memory
        }
    } else {
        std::cerr << "Failed to open file for reading\n";
    }
}

// Function to dump memory content in hexadecimal and ASCII format
void memory_dump(const void *ptr, size_t size) {
    const unsigned char *data = static_cast<const unsigned char *>(ptr);
    std::cout << "Memory Dump (Hexadecimal and ASCII):\n";
    for (size_t i = 0; i < size; i += 16) {
        std::cout << std::setw(4) << std::setfill('0') << i << "  ";  // Print offset
        for (size_t j = 0; j < 16; ++j) {
            if (i + j < size) {
                std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(data[i + j]) << " ";  // Print hex
            } else {
                std::cout << "   ";
            }
        }
        std::cout << " ";
        for (size_t j = 0; j < 16; ++j) {
            if (i + j < size) {
                char c = data[i + j];
                std::cout << (isprint(c) ? c : '.');  // Print ASCII or dot for non-printable characters
            }
        }
        std::cout << std::endl;
    }
}

// Function to get a 16-character key from the user
unsigned char* get_user_key() {
    static unsigned char key[16];
    std::cout << "Enter 16-character key: ";
    std::cin >> std::setw(16) >> key;  // Read key
    return key;
}

// Function to get a 16-character IV from the user
unsigned char* get_user_iv() {
    static unsigned char iv[16];
    std::cout << "Enter 16-character IV: ";
    std::cin >> std::setw(16) >> iv;  // Read IV
    return iv;
}

// Function to encrypt and decrypt user input
void encrypt_decrypt_user_input() {
    std::string input;
    std::cout << "Enter text to encrypt: ";
    std::cin.ignore();  // Ignore previous newline
    std::getline(std::cin, input);  // Read user input
    size_t size = input.size() + 1;
    
    unsigned char *key = get_user_key();
    unsigned char *iv = get_user_iv();

    void *ptr = secure_malloc(size);  // Allocate memory
    if (ptr != nullptr) {
        std::strcpy((char *)ptr, input.c_str());  // Copy user input to memory
        encrypt_decrypt_page(ptr, size, true, key, iv);  // Encrypt the memory content
        std::cout << "Encrypted User Input: " << (char *)ptr << std::endl;
        encrypt_decrypt_page(ptr, size, false, key, iv);  // Decrypt the memory content
        std::cout << "Decrypted User Input: " << (char *)ptr << std::endl;
        secure_free(ptr, size, key, iv);  // Free the memory
    }
}


// Function to get user option
int get_user_option() {
   int option;
   std::cout << "\n";
   std::cout << "1. Simulate memory usage\n";
   std::cout << "2. Measure memory usage\n";
   std::cout << "3. Demonstrate encryption and decryption\n";
   std::cout << "4. Save encrypted data to file\n";
   std::cout << "5. Load encrypted data from file and decrypt\n";
   std::cout << "6. Memory dump\n";
   std::cout << "7. Encrypt/Decrypt user input\n";
   std::cout << "8. Exit\n";
   std::cout << "Enter option: ";
   std::cin >> option;
   while(std::cin.fail() || option < 1 || option > 8) {
       std::cin.clear();
       std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
       std::cout << "Invalid option. Please enter a number between 1 and 8: ";
       std::cin >> option;
   }
   return option;
}

// Main function
int main() {
    OpenSSL_add_all_algorithms();  // Initialize OpenSSL algorithms

    int option;
    do {
        option = get_user_option();  // Get user option
        switch (option) {
            case 1:
                simulate_memory_usage();
                break;
            case 2:
                measure_memory_usage();
                break;
            case 3:
                demonstrate_encryption_decryption();
                break;
            case 4: {
                const char *text = "Hello, encrypted memory!";
                size_t size = strlen(text) + 1;
                unsigned char *key = (unsigned char *)"0123456789012345";
                unsigned char *iv = (unsigned char *)"0123456789012345";

                void *ptr = secure_malloc(size);  // Allocate memory
                if (ptr != nullptr) {
                    std::strcpy((char *)ptr, text);  // Copy text to memory
                    encrypt_decrypt_page(ptr, size, true, key, iv);  // Encrypt the memory content
                    save_encrypted_data(ptr, size, "encrypted_data.bin");  // Save encrypted data to file
                    secure_free(ptr, size, key, iv);  // Free the memory
                }
                break;
            }
            case 5:
                load_and_decrypt_data("encrypted_data.bin");  // Load and decrypt data from file
                break;
            case 6: {
                const char *text = "Hello, encrypted memory!";
                size_t size = strlen(text) + 1;
                unsigned char *key = (unsigned char *)"0123456789012345";
                unsigned char *iv = (unsigned char *)"0123456789012345";

                void *ptr = secure_malloc(size);  // Allocate memory
                if (ptr != nullptr) {
                    std::strcpy((char *)ptr, text);  // Copy text to memory
                    memory_dump(ptr, size);  // Dump memory content
                    secure_free(ptr, size, key, iv);  // Free the memory
                }
                break;
            }
            case 7:
                encrypt_decrypt_user_input();  // Encrypt and decrypt user input
                break;
            case 8:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    } while (option != 8);

    EVP_cleanup();  // Clean up OpenSSL

    return 0;
}
