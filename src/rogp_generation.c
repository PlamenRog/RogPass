
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

#include "main_structures.h"

void handleErrors(void)
{
    fprintf(stderr, "Error occurred\n");
    exit(1);
}

// Serialization functions

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



// Cryptography functions

#define KEY_SIZE 32  // AES-256 key size

void generate_key_from_password(const char *password, unsigned char *key) {
    if (PKCS5_PBKDF2_HMAC(password, strlen(password), NULL, 0, 10000, EVP_sha256(), 32, key) != 1) {
        handleErrors();
    }
}
