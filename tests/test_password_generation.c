
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
#include <stdbool.h>
#include "../src/generation_utility.h"

int main() {
    Password pass = {10, false, false, false, false};

    printf("hasNothing:\n %s\n\n", generate_password(pass));

    pass.hasLower = true;
    printf("hasLower:\n %s\n\n", generate_password(pass));

    pass.hasUpper = true;
    printf("hasLower & hasUpper:\n %s\n\n", generate_password(pass));

    pass.hasNumbers = true;
    printf("hasLower & hasUpper & hasNumbers:\n %s\n\n", generate_password(pass));

    pass.hasSpecChars = true;
    printf("hasLower & hasUpper & hasNumbers & hasSpecChars:\n %s\n\n", generate_password(pass));

    return EXIT_SUCCESS;
}
