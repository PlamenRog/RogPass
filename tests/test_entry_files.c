
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

#include "../src/main_structures.h"
#include "../src/rogp_generation.h"

int main() {
    EntryPass entries[] = {
        {"Entry1", "User1", "user1@example.com", "example.com", "Note for Entry1", "pass1"},
        {"Entry2", "User2", "user2@example.com", "example.com", "Note for Entry2", "pass2"},
        {"Entry3", "User3", "user3@example.com", "example.com", "Note for Entry3", "pass3"}
    };

    FILE* outFile = fopen("entries.txt", "w");
    if (outFile == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }

    int numEntries = sizeof(entries) / sizeof(entries[0]);
    serialize_entry(outFile, entries, numEntries);
    fclose(outFile);

    FILE* inFile = fopen("entries.txt", "r");
    if (inFile == NULL) {
        printf("Error opening file for reading.\n");
        return 1;
    }

    uint32_t deserializedNumEntries;
    EntryPass* deserializedEntries = deserialize_entry(inFile, &deserializedNumEntries);
    fclose(inFile);

    if (deserializedEntries != NULL) {
        printf("Deserialized %d entries from file:\n", deserializedNumEntries);
        for (uint32_t i = 0; i < deserializedNumEntries; i++) {
            printf("Entry Name: %s\n", deserializedEntries[i].entryName);
            printf("Username: %s\n", deserializedEntries[i].username);
            printf("Email: %s\n", deserializedEntries[i].email);
            printf("URL: %s\n", deserializedEntries[i].url);
            printf("Note: %s\n", deserializedEntries[i].note);
            printf("Note: %s\n", deserializedEntries[i].pass);
            printf("\n");
        }
    }

    return 0;
}

