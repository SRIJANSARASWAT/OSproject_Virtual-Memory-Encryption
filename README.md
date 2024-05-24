# Memory Encryption and Decryption Project

## Description

This project demonstrates various functionalities related to memory encryption and decryption using the OpenSSL library. The main functionalities include simulating memory usage, measuring memory usage, encrypting and decrypting memory, saving and loading encrypted data, performing memory dumps, and more.

## Features

1. **Simulate Memory Usage**: Encrypt and decrypt a sample memory content.
2. **Measure Memory Usage**: Allocate and free memory securely.
3. **Demonstrate Encryption and Decryption**: Show the process of encrypting and decrypting a sample memory content.
4. **Save Encrypted Data to File**: Save encrypted memory content to a file.
5. **Load Encrypted Data from File and Decrypt**: Load and decrypt data from a file.
6. **Memory Dump**: Display memory content in hexadecimal and ASCII format.
7. **Encrypt/Decrypt User Input**: Encrypt and decrypt user-provided input.
8. **Exit**: Exit the program.

## Prerequisites

- C++ Compiler (g++)
- OpenSSL Library
- Windows Operating System

## Installation

1. **Download and Install OpenSSL**

   - Go to the [OpenSSL website](https://www.openssl.org/) and download the appropriate version for Windows.
   - Install OpenSSL and make a note of the installation directory (e.g., `C:/OpenSSL-Win64`).

2. **Set Up Environment Variables**

   - Add the OpenSSL `bin` directory to your system's `PATH` environment variable.

   ```sh
   set PATH=%PATH%;C:\OpenSSL-Win64\bin
   ```

## Compilation

1. **Navigate to the Project Directory**

   Open a terminal or command prompt and navigate to the project directory:

   ```sh
   cd "/d/College Codes/Sem4/C++/project" #it's in my device
   ```

2. **Compile the Project**

   Compile the project using the g++ compiler:

   ```sh
   g++ -o project.exe project.cpp -LC:/OpenSSL-Win64/lib -lssl -lcrypto -IC:/OpenSSL-Win64/include
   ```

3. **Verify the Compiled Executable**

   Ensure that the executable file has been created:

   ```sh
   ls
   ```

   You should see `project.exe` listed in the directory.

## Running the Program

1. **Execute the Compiled Program**

   Run the executable:

   ```sh
   ./project.exe
   ```

   If the program runs successfully, you will see a menu with various options corresponding to the features listed above.

## Usage

When the program is running, you will be prompted with a menu to select an option. Enter the number corresponding to the desired functionality and follow any additional prompts.

### Example

Here is an example of how to run the program and simulate memory usage:

1. **Run the Executable:**

   ```sh
   ./project.exe
   ```

2. **Enter `1` to Select "Simulate Memory Usage":**

   ```
   Enter option: 1
   ```

3. **You Will See the Encrypted Memory Content Displayed:**

   ```
   Encrypted Memory Content: ~╡╠ó↓÷<╥╠xN/äs£i╗♦╞¡î 1₧♦TÜ α∩Φ
   ```

### Options

1. **Simulate Memory Usage**

   Encrypt and decrypt a sample memory content.

2. **Measure Memory Usage**

   Allocate and free memory securely.

3. **Demonstrate Encryption and Decryption**

   Show the process of encrypting and decrypting a sample memory content.

4. **Save Encrypted Data to File**

   Save encrypted memory content to a file named `encrypted_data.bin`.

5. **Load Encrypted Data from File and Decrypt**

   Load and decrypt data from `encrypted_data.bin`.

6. **Memory Dump**

   Display memory content in hexadecimal and ASCII format.

7. **Encrypt/Decrypt User Input**

   Encrypt and decrypt user-provided input.

8. **Exit**

   Exit the program.

## Troubleshooting

- **OpenSSL Not Found**: Ensure the OpenSSL library is correctly installed and the paths to the library and include files are correct.
- **Compilation Errors**: Verify that the project files and OpenSSL installation are in the correct locations.
- **Memory Allocation Issues**: Ensure sufficient memory is available on your system.



## Author

Srijan Saraswat
Divyanshu Chander
Devansh Moudgil
## Acknowledgments

- OpenSSL Project (https://www.openssl.org/)
