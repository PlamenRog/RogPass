
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
#include <string.h>

#include "main_structures.h"

// Function to find the maximum of three integers
int max_of_three(int a, int b, int c) {
    int max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

void pretty_print(EntryPass* entry, uint32_t numEntries) {
    // Calculate the maximum length of each field in all entries
    int max_entryName_length = 0;
    int max_url_length = 0;

    for (uint32_t i = 0; i < numEntries; ++i) {
        int entryName_length = strlen(entry[i].entryName);
        int url_length = strlen(entry[i].url);

        if (entryName_length > max_entryName_length) max_entryName_length = entryName_length;
        if (url_length > max_url_length) max_url_length = url_length;
    }

    // Calculate the width of each column
    int width_number = max_of_three(6, 5, 6); // Minimum width 6
    int width_entryName = max_of_three(15, max_entryName_length, 15); // Minimum width 15
    int width_url = max_of_three(10, max_url_length, 10); // Minimum width 10

    // Print the header
    printf("| %-*s | %-*s | %-*s |\n", width_number, "ID", width_entryName, "Entry name", width_url, "URL");
    printf("|");
    for (int i = 0; i < width_number + 2; i++) printf("-");
    printf("|");
    for (int i = 0; i < width_entryName + 2; i++) printf("-");
    printf("|");
    for (int i = 0; i < width_url + 2; i++) printf("-");
    printf("|\n");

    // Print each entry.
    for (uint32_t i = 0; i < numEntries; ++i) {
        printf("| %-*d | %-*s | %-*s |\n", width_number, i, width_entryName, entry[i].entryName, width_url, entry[i].url);
    }
}

