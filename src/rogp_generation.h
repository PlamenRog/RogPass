
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

#ifndef rogp_generation
#define rogp_generation

#include <stdio.h>
#include "main_structures.h"

void handleErrors(void);

void serialize_entry(FILE* file, EntryPass* entries, uint32_t numEntries);

EntryPass* deserialize_entry(FILE* file, uint32_t* numEntries);

void generate_key_from_password(const char *password, unsigned char *key);

#endif
