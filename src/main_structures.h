
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

#ifndef main_structures
#define main_structures
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t	 length;
    bool		 hasLower;
    bool		 hasUpper;
    bool		 hasNumbers;
    bool		 hasSpecChars;
} Password;
#define PASS_DEFAULTS { .length = 12, .hasLower = false, .hasUpper = false, .hasNumbers = false, .hasSpecChars = false}

typedef struct {
    char*    entryName;
    char*    username;
    char*    email;
    char*    url;
    char*    note;
    char*    pass;
} EntryPass;

#endif
