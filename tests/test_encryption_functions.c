
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

#include "../src/rogp_generation.h"

int main() {
    // TODO: Leaving them commented for a bit
    //const char *inputFile = "entries.txt";
    //const char *encryptedFile = "entries.rogp";
    //const char *decryptedFile = "decrypted.txt";
    const char password[] = "pass";
    char unsigned key[33];


    generate_key_from_password(password, key);

    printf("Encryption and decryption completed successfully.\n");

    return 0;
}

