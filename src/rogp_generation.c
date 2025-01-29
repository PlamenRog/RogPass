
 /*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, using version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include "main_structures.h"

void handleErrors(void)
{
    fprintf(stderr, "Error occurred\n");
    exit(1);
}

/* Serialization functions */
void serialize_entry(FILE* file, EntryPass* entries, uint32_t numEntries) {
    if (file == NULL) {
        printf("Error: File pointer is NULL.\n");
        return;
    }

    for (uint32_t i = 0; i < numEntries; i++) {
        fprintf(file, "%s|%s|%s|%s|%s|%s\n", entries[i].entryName, entries[i].username,
                entries[i].email, entries[i].url, entries[i].note, entries[i].pass);
    }
}

char* qstrdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *new_s = malloc(len);
    if (new_s == NULL) return NULL;
    memcpy(new_s, s, len);
    return new_s;
}

EntryPass* deserialize_entry(FILE* file, uint32_t* numEntries) {
    if (file == NULL) {
        printf("Error: File pointer is NULL.\n");
        return NULL;
    }

    // get the number entries in the file
    int count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    rewind(file); // reset file position indicator

    // Allocate memory for array of EntryPass structs
    EntryPass* entries = (EntryPass*)malloc(count * sizeof(EntryPass));
    if (entries == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // read data from file and deserialize into EntryPass structs
    for (int i = 0; i < count; i++) {
        char entryName[128], username[128], email[128], url[128], note[128], pass[128];

        fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", entryName, username, email, url, note, pass);

        entries[i].entryName = qstrdup(entryName);
        entries[i].username = qstrdup(username);
        entries[i].email = qstrdup(email);
        entries[i].url = qstrdup(url);
        entries[i].note = qstrdup(note);
        entries[i].pass = qstrdup(pass);
    }

    *numEntries = count;
    return entries;
}



/* Cryptography functions */

#define AES_BLOCK_SIZE 16 // 16 bytes -> 128 bits

void generate_key_from_password(const char *password, unsigned char *key) {
    if (PKCS5_PBKDF2_HMAC(password, strlen(password), NULL, 0, 10000, EVP_sha256(), AES_BLOCK_SIZE, key) != 1) {
        handleErrors();
    }
}

void generate_random_iv(unsigned char *iv) {
    RAND_bytes(iv, AES_BLOCK_SIZE);
}

int encrypt_file(const char *inputFileName, const char *outputFileName, const unsigned char *key) {
    FILE *inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        perror("Error opening input file");
        return -1;
    }

    FILE *outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        perror("Error opening output file");
        fclose(inputFile);
        return -1;
    }

    EVP_CIPHER_CTX *ctx;
    unsigned char iv[AES_BLOCK_SIZE];

    // create and initialize the context
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        perror("Error creating cipher context");
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    generate_random_iv(iv);  // generate random IV

    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1) {
        perror("Error initializing encryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    unsigned char inBuffer[AES_BLOCK_SIZE];
    unsigned char outBuffer[AES_BLOCK_SIZE + EVP_MAX_BLOCK_LENGTH];
    int bytesRead = 0;
    int bytesWritten = 0;
    int finalWritten = 0;

    // write IV to the beginning of the output file
    fwrite(iv, 1, AES_BLOCK_SIZE, outputFile);

    while ((bytesRead = fread(inBuffer, 1, AES_BLOCK_SIZE, inputFile)) > 0) {
        if (EVP_EncryptUpdate(ctx, outBuffer, &bytesWritten, inBuffer, bytesRead) != 1) {
            perror("Error encrypting data");
            EVP_CIPHER_CTX_free(ctx);
            fclose(inputFile);
            fclose(outputFile);
            return -1;
        }
        fwrite(outBuffer, 1, bytesWritten, outputFile);
    }

    if (EVP_EncryptFinal_ex(ctx, outBuffer, &finalWritten) != 1) {
        perror("Error finalizing encryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }
    fwrite(outBuffer, 1, finalWritten, outputFile);

    EVP_CIPHER_CTX_free(ctx);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

int decrypt_file(const char *inputFileName, const char *outputFileName, const unsigned char *key) {
    FILE *inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        perror("Error opening input file");
        return -1;
    }

    FILE *outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        perror("Error opening output file");
        fclose(inputFile);
        return -1;
    }

    EVP_CIPHER_CTX *ctx;
    unsigned char iv[AES_BLOCK_SIZE];

    // read IV from the beginning of the input file
    if (fread(iv, 1, AES_BLOCK_SIZE, inputFile) != AES_BLOCK_SIZE) {
        perror("Error reading IV from input file");
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        perror("Error creating cipher context");
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv) != 1) {
        perror("Error initializing decryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    unsigned char inBuffer[AES_BLOCK_SIZE + EVP_MAX_BLOCK_LENGTH];
    unsigned char outBuffer[AES_BLOCK_SIZE];
    int bytesRead = 0;
    int bytesWritten = 0;
    int finalWritten = 0;

    while ((bytesRead = fread(inBuffer, 1, AES_BLOCK_SIZE + EVP_MAX_BLOCK_LENGTH, inputFile)) > 0) {
        if (EVP_DecryptUpdate(ctx, outBuffer, &bytesWritten, inBuffer, bytesRead) != 1) {
            perror("Error decrypting data");
            EVP_CIPHER_CTX_free(ctx);
            fclose(inputFile);
            fclose(outputFile);
            return -1;
        }
        fwrite(outBuffer, 1, bytesWritten, outputFile);
    }

    if (EVP_DecryptFinal_ex(ctx, outBuffer, &finalWritten) != 1) {
        perror("Error finalizing decryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }
    fwrite(outBuffer, 1, finalWritten, outputFile);

    EVP_CIPHER_CTX_free(ctx);
    fclose(outputFile);
    fclose(inputFile);

    return 0;
}
