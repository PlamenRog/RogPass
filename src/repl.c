
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
#include <stdbool.h>
#include <ctype.h>

#include "main_structures.h"
#include "rogp_generation.h"
#include "generation_utility.h"
#include "print_functions.h"

int check_entry_count(int numEntries) {
	if (numEntries == 0) {
		printf("There aren't any available entries.\n");
		return 1;
	}
	return 0;
}

void create_password(char pass[129]) {
	while (true) {
		printf("Would you like to manually type in or generate a password?(m/g): ");
		char choice[9];
		scanf(" %8[^\n]", choice);
		for (int i = 0; i < 8; i++) {
			choice[i] = toupper(choice[i]);
		}

		if (strcmp(choice, "MANUALLY") == 0 || strcmp(choice, "M") == 0) {
			printf("Enter the pass: ");
			scanf(" %128[^\n]", pass);
			break;
		}
		else if (strcmp(choice, "GENERATE") == 0 || strcmp(choice, "G") == 0) {
			Password genPass = input_password();
			strcpy(pass, generate_password(genPass));
			break;
		}

		printf("Invalid input, try again!\n");
	}

	printf("\n");
}

// Function to dynamically allocate memory for a string
char* copyString(const char* source) {
	if (source == NULL || strcmp(source, "") == 0) {
		return NULL;
	}
	char* destination = malloc(strlen(source) + 1);
	if (destination != NULL) {
		strcpy(destination, source);
	}
	return destination;
}

EntryPass create_entry() {
	char entryName[129];
	char username[129];
	char email[129];
	char url[129];
	char note[513];
	char pass[129];

	printf("Enter the entry name (type \"BLANK\" to leave empty): ");
	scanf(" %128[^\n]", entryName);
	if (strcmp(entryName, "BLANK") == 0) {
		entryName[0] = '\0'; // Set to empty string if "BLANK" is entered
	}

	printf("Enter the username (type \"BLANK\" to leave empty): ");
	scanf(" %128[^\n]", username);
	if (strcmp(username, "BLANK") == 0) {
		username[0] = '\0'; // Set to empty string if "BLANK" is entered
	}

	printf("Enter the email (type \"BLANK\" to leave empty): ");
	scanf(" %128[^\n]", email);
	if (strcmp(email, "BLANK") == 0) {
		email[0] = '\0'; // Set to empty string if "BLANK" is entered
	}

	printf("Enter the url (type \"BLANK\" to leave empty): ");
	scanf(" %128[^\n]", url);
	if (strcmp(url, "BLANK") == 0) {
		url[0] = '\0'; // Set to empty string if "BLANK" is entered
	}

	printf("Enter the note (type \"BLANK\" to leave empty): ");
	scanf(" %512[^\n]", note);
	if (strcmp(note, "BLANK") == 0) {
		note[0] = '\0'; // Set to empty string if "BLANK" is entered
	}

	// Generate password (implement your own password generation logic)
	create_password(pass);

	// Create new EntryPass instance with dynamically allocated strings
	EntryPass newEntry = {
		copyString(entryName),
		copyString(username),
		copyString(email),
		copyString(url),
		copyString(note),
		copyString(pass)
	};

	return newEntry;
}

uint32_t select_entry(EntryPass* entries, uint32_t numEntries, const char* keyword) {
	uint32_t num;
	while (true) {
		pretty_print(entries, numEntries);
		printf("Enter the number of the entry which you'd like to %s: \n", keyword);
		printf("rogp? ");
		if (scanf("%u", &num) != 1) {
			while(getchar() != '\n');
			printf("Invalid input. Please enter an integer.\n");
		}
		while(getchar() != '\n');
		if (num < numEntries) {
			break;
		}
		printf("Number is too large.\n");
	}
	return num;
}

void show_entry(EntryPass* entries, uint32_t numEntries) {
	uint32_t num = select_entry(entries, numEntries, "expand");

	printf("Entry id %d:\n", num);
	printf("Entry name: %s\n", entries[num].entryName);
	printf("Username: %s\n", entries[num].username);
	printf("Email: %s\n", entries[num].email);
	printf("Url: %s\n", entries[num].url);
	printf("Note: %s\n", entries[num].note);
	printf("Password: %s\n", entries[num].pass);
	printf("Estimated password entropy: %f bits\n", calculate_entropy(entries[num].pass));
}

void mod_entry(EntryPass* entries, uint32_t numEntries) {
	uint32_t num = select_entry(entries, numEntries, "modify");

	char choice[11];

	while(true) {
		printf("Now select which attribute you would like to edit(username, password, etc.): ");
		scanf(" %10[^\n]", choice);

		for (int i = 0; i < 10; i++) {
			choice[i] = toupper(choice[i]);
		}


		if (strcmp(choice, "ENTRYNAME") == 0 || strcmp(choice, "ENTRY NAME") == 0) {
			printf("Enter the entry name: ");
			scanf(" %128[^\n]", entries[num].entryName);
			break;
		}
		else if (strcmp(choice, "USERNAME") == 0) {
			printf("Enter the username: ");
			scanf(" %128[^\n]", entries[num].username);
			break;
		}
		else if (strcmp(choice, "EMAIL") == 0) {
			printf("Enter the email: ");
			scanf(" %128[^\n]", entries[num].email);
			break;
		}
		else if (strcmp(choice, "URL") == 0) {
			printf("Enter the url: ");
			scanf(" %128[^\n]", entries[num].url);
			break;
		}
		else if (strcmp(choice, "NOTE") == 0) {
			printf("Enter the note: ");
			scanf(" %128[^\n]", entries[num].note);
			break;
		}
		else if (strcmp(choice, "PASSWORD") == 0 || strcmp(choice, "PASS") == 0) {
			printf("Enter the password: ");
			create_password(entries[num].pass);
			break;
		}

		printf("Invalid input, try again!\n");
	}
}

void delete_entry(EntryPass* entries, uint32_t* numEntries) {
	uint32_t num = select_entry(entries, *numEntries, "delete");

	for (uint32_t i = num; i < (*numEntries - 1); i++) {
		entries[i] = entries[i + 1];
	}

	*numEntries -= 1;

}


void print_help() {
	fprintf(stdout, "%s\n", "\n"
	        "rogpass			   manages passwords somewhat safely\n"
	        "\n"
	        "commands:\n"
	        "\n"
	        "add                               adds entry\n"
	        "show                              expands on an entry\n"
	        "mod, modify                       modifies an entry\n"
	        "del, delete                       deletes an entry\n"
	        "print                             prints surface info about all entries\n"
	        "help                              prints this message\n"
	        "write                             writes changes\n"
	        "exit                              exits the program\n"
	        "\n");
}

int main(){
	FILE* file = NULL;
	EntryPass* entries = NULL;
	uint32_t numEntries = 0;
	char masterPass[129];

	char filename[129];
	while(true) {
		char choice[7];
		printf("Create a new file or open an existing one(c/o): ");
		scanf(" %6s", choice);

		for (int i = 0; i < 6; i++) {
			choice[i] = toupper(choice[i]);
		}

		if (strcmp(choice, "CREATE") == 0 || strcmp(choice, "C") == 0) {
			printf("Enter a file name(without the extention): ");
			// 5 characters less, because .rogp might be appended
			scanf(" %123[^\n]", filename);

			size_t len = strlen(filename);
			char* last_five = filename + (len - 5);
			if (strcmp(last_five, ".rogp") != 0) {
				strcat(filename, ".rogp");
			}

			printf("Enter the master password(using this you will access your file, make sure to remember it): ");
			if (scanf(" %128[^\n]", masterPass) != 1) {
				printf("An error ocurred.");
				exit(EXIT_FAILURE);
			}

			file = fopen(filename, "w");
			if (file == NULL) {
				printf("Error creating file for writing\n");
				exit(EXIT_FAILURE);
			}

			fclose(file);
			break;
		}

		if (strcmp(choice, "OPEN") == 0 || strcmp(choice, "O") == 0) {
			printf("Enter a file name(with the extention): ");
			scanf(" %128[^\n]", filename);

			file = fopen(filename, "r");
			if (file == NULL) {
				printf("Error opening file for redacting\n");
				exit(EXIT_FAILURE);
			}

			scanf(" %128[^\n]", masterPass);

			char unsigned key[17];
			generate_key_from_password(masterPass, key);

			if (decrypt_file(filename, "temp.txt", key) != 0) {
				fprintf(stderr, "Decryption failed\n");
				return EXIT_FAILURE;
			}

			FILE* tempFile = fopen("temp.txt", "r");
			if (tempFile == NULL) {
				printf("Error opening file for reading.\n");
				return 1;
			}
			entries = deserialize_entry(tempFile, &numEntries);
			fclose(tempFile);

			remove("temp.txt");
			fclose(file);
			break;
		}

		printf("Invalid input, try again.\n");
	}

	printf("Enter a command('help' for help menu, 'exit' to quit): \n");
	char choice[7];
	bool hasSaved = true;
	while (true) {
		printf("rog> ");
		scanf(" %5s", choice);

		for (int i = 0; i < 6; i++) {
			choice[i] = toupper(choice[i]);
		}

		if (strcmp(choice, "EXIT") == 0) {
			if (!hasSaved) {
				printf("You have unsaved changes! Are you sure you want to exit(type full yes or no): ");

				char saveChoice[4];
				if(scanf(" %s", saveChoice) != 1) {
					printf("Invalid input, doing nothing.");
					continue;
				}

				for (int i = 0; i < 3; i++) {
					saveChoice[i] = toupper(saveChoice[i]);
				}

				if(strcmp(saveChoice, "YES") != 0)
					continue;
			}


			printf("Exiting...\n");
			break;
		}

		else if (strcmp(choice, "HELP") == 0) {
			print_help();
		}

		else if (strcmp(choice, "ADD") == 0) {
			EntryPass newEntry = create_entry();
			uint32_t new_size = numEntries+1;
			EntryPass* temp = realloc(entries, new_size * sizeof(EntryPass));
			if (temp == NULL) {
				fprintf(stderr, "Memory reallocation failed\n");
				free(entries);
				exit(EXIT_FAILURE);
			}
			entries = temp;
			entries[numEntries] = newEntry;
			numEntries = new_size;
			hasSaved = false;
		}


		else if (strcmp(choice, "SHOW") == 0) {
			if (check_entry_count(numEntries) != 0) {
				continue;
			}
			show_entry(entries, numEntries);
		}


		else if (strcmp(choice, "MOD") == 0 || strcmp(choice, "MODIFY") == 0) {
			if (check_entry_count(numEntries) != 0) {
				continue;
			}
			mod_entry(entries, numEntries);
			hasSaved = false;
		}


		else if (strcmp(choice, "DEL") == 0 || strcmp(choice, "DELETE") == 0) {
			if (check_entry_count(numEntries) != 0) {
				continue;
			}

			delete_entry(entries, &numEntries);
			hasSaved = false;
		}


		else if (strcmp(choice, "PRINT") == 0) {
			pretty_print(entries, numEntries);
		}

		else if (strcmp(choice, "WRITE") == 0) {

			FILE* tempFile = fopen("temp.txt", "w");
			if (file == NULL) {
				printf("Error opening file for writing.\n");
				exit(EXIT_FAILURE);
			}

			serialize_entry(tempFile, entries, numEntries);
			fclose(tempFile);

			char unsigned key[17];
			generate_key_from_password(masterPass, key);

		
			remove(filename);
			if (encrypt_file("temp.txt", filename, key) != 0) {
				fprintf(stderr, "Encryption failed\n");
				exit(EXIT_FAILURE);
			}

			hasSaved = true;
		}

		else {
			printf("Invalid input, try again.\n");
		}
	}

	return 0;
}
