
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
#include <openssl/evp.h>
#include <openssl/aes.h>

#include "main_structures.h"

void handleErrors(void)
{
    fprintf(stderr, "Error occurred\n");
    exit(1);
}

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

/*
char* qstrdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *new_s = malloc(len);
    if (new_s == NULL) return NULL;
    memcpy(new_s, s, len);
    return new_s;
}
*/

EntryPass* deserialize_entry(FILE* file, uint32_t* numEntries) {
    if (file == NULL) {
        printf("Error: File pointer is NULL.\n");
        return NULL;
    }

    // Count the number of lines (entries) in the file
    int count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    rewind(file); // Reset file position indicator

    // Allocate memory for array of EntryPass structs
    EntryPass* entries = (EntryPass*)malloc(count * sizeof(EntryPass));
    if (entries == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Read data from file and deserialize into EntryPass structs
    for (int i = 0; i < count; i++) {
        char entryName[128], username[128], email[128], url[128], note[128], pass[128];

        fscanf(file, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", entryName, username, email, url, note, pass);

        entries[i].entryName = strdup(entryName);
        entries[i].username = strdup(username);
        entries[i].email = strdup(email);
        entries[i].url = strdup(url);
        entries[i].note = strdup(note);
		entries[i].pass = strdup(pass);
    }

    *numEntries = count;
    return entries;
}

void generate_key_from_password(const char *password, unsigned char *key) {
    if (PKCS5_PBKDF2_HMAC(password, strlen(password), NULL, 0, 10000, EVP_sha256(), 32, key) != 1) {
        handleErrors();
    }
}

void encrypt_file(const char *inputFile, const char *outputFile, const unsigned char *key) {
    FILE *inFile = fopen(inputFile, "rb");
    FILE *outFile = fopen(outputFile, "wb");
    if (!inFile || !outFile) {
        perror("Error opening file");
        exit(1);
    }

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL) != 1) handleErrors();

    int blockSize = EVP_CIPHER_block_size(EVP_aes_256_cbc());
    unsigned char *buffer = malloc(blockSize);
    if (!buffer) handleErrors();

    unsigned char *outBuffer = malloc(blockSize + blockSize);
    if (!outBuffer) handleErrors();

    int len;
    int outLen;

    while ((len = fread(buffer, 1, blockSize, inFile)) > 0) {
        if (EVP_EncryptUpdate(ctx, outBuffer, &outLen, buffer, len) != 1) handleErrors();
        fwrite(outBuffer, 1, outLen, outFile);
    }

    if (EVP_EncryptFinal_ex(ctx, outBuffer, &outLen) != 1) handleErrors();
    fwrite(outBuffer, 1, outLen, outFile);

    EVP_CIPHER_CTX_free(ctx);

    free(buffer);
    free(outBuffer);

    fclose(inFile);
    fclose(outFile);
}

void decrypt_file(const char *inputFile, const char *outputFile, const unsigned char *key) {
    FILE *inFile = fopen(inputFile, "rb");
    FILE *outFile = fopen(outputFile, "wb");
    if (!inFile || !outFile) {
        perror("Error opening file");
        exit(1);
    }

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL) != 1) handleErrors();

    int blockSize = EVP_CIPHER_block_size(EVP_aes_256_cbc());
    unsigned char *buffer = malloc(blockSize);
    if (!buffer) handleErrors();

    unsigned char *outBuffer = malloc(blockSize + blockSize);
    if (!outBuffer) handleErrors();

    int len;
    int outLen;

    while ((len = fread(buffer, 1, blockSize, inFile)) > 0) {
        if (EVP_DecryptUpdate(ctx, outBuffer, &outLen, buffer, len) != 1) handleErrors();
        fwrite(outBuffer, 1, outLen, outFile);
    }

    if (EVP_DecryptFinal_ex(ctx, outBuffer, &outLen) != 1) handleErrors();
    fwrite(outBuffer, 1, outLen, outFile);

    EVP_CIPHER_CTX_free(ctx);

    free(buffer);
    free(outBuffer);

    fclose(inFile);
    fclose(outFile);
}

