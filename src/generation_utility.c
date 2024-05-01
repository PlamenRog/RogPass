
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
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <openssl/rand.h>

#include "generation_utility.h"

char get_single_char_input(const char *prompt) {
    printf("%s", prompt);
    char choice;
    scanf(" %1c", &choice);
	while(getchar() != '\n');
    return toupper(choice);
}

void get_password_preference(const char *prompt, bool *passwordAttribute) {
    char choice;
    while (true) {
        choice = get_single_char_input(prompt);
        if (choice == 'Y') {
            *passwordAttribute = true;
			printf("The password will have that attribute\n");
            break;
        } else if (choice == 'N') {
			printf("The password won't have that attribute\n");
            break;
        }
        printf("\nInvalid input. Please enter 'y' or 'n'.\n\n");
    }
}

Password input_password() {
	Password pass = PASS_DEFAULTS; // NOTE: See main_structures.h

    while (true) {
        printf("Enter the password length (max is 128): ");
        if (scanf("%u", &pass.length) == 1) {
            while(getchar() != '\n'); // Clear input buffer
            if (pass.length <= 128) {
                break;
            }
            printf("Number is too large.\n");
        } else {
            // Clear input buffer
            while(getchar() != '\n');
            printf("Invalid input. Please enter an integer.\n");
        }
    }
    printf("The password length will be: %u.\n", pass.length);
	printf("\n");

	// Handles lower case
    get_password_preference("Enter whether the password should have lower case letters (y/n): ", &pass.hasLower);
    printf("\n");

	// Handles upper case
    get_password_preference("Enter whether the password should have UPPER CASE LETTERS (y/n): ", &pass.hasUpper);
    printf("\n");

	// Handles numbers
    get_password_preference("Enter whether the password should have numbers (y/n): ", &pass.hasNumbers);
    printf("\n");

	// Handles special characters case
    get_password_preference("Enter whether the password should have special symbols (y/n): ", &pass.hasSpecChars);
    printf("\n");

	return pass;
}

const char* lower = "abcdefghijklmnopqrstuvwxyz";
const char* upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* nums = "0123456789";
const char* specs = "~`!@#$%^&*-_+=(){}[];:'\"\\.,<>/?";

// Very basic calculation, more accurate methods exist but its all bullshit anyway, mostly for aesthetic
double calculate_entropy(char* pass) {
	uint32_t charsetLength = 0;

	bool checkedLow = false;
	bool checkedUp = false;
	bool checkedNum = false;
	bool checkedSpecs = false;
	for (uint32_t i = 0; i < strlen(pass); i++) {
		if (strchr(lower, pass[i]) != NULL && !checkedLow) {
			charsetLength += 26;
		}
		else if (strchr(upper, pass[i]) != NULL && !checkedUp) {
			charsetLength += 26;
		}
		else if (strchr(nums, pass[i]) != NULL && !checkedNum) {
			charsetLength += 10;
		}
		else if (strchr(specs, pass[i]) != NULL && !checkedSpecs) {
			charsetLength += 32;
		}
	}

	return strlen(pass) * (log(charsetLength) / log(2));;
}
char* generate_password(Password pass) {
    char charset[95] = "";
    if (pass.hasLower) {
        strcat(charset, lower);
    }
    if (pass.hasUpper) {
        strcat(charset, upper);
    }
    if (pass.hasNumbers) {
        strcat(charset, nums);
    }
    if (pass.hasSpecChars) {
        strcat(charset, specs);
    }

	if(!(pass.hasLower || pass.hasUpper || pass.hasNumbers || pass.hasSpecChars) || pass.length == 0) {
		return "";
	}

    const uint32_t charsetLength = strlen(charset); // Calculate charset length after all characters are added

    char *randomString = (char *)malloc((pass.length + 1) * sizeof(char));
    //const uint32_t stringLen = strlen(randomString);


	for (uint32_t i = 0; i < pass.length; i++) {
		int randElem;
		RAND_bytes((unsigned char*)&randElem, sizeof(randElem));
		randElem = abs(randElem) % charsetLength;

		randomString[i] = charset[randElem];
	}


    return randomString;
}

