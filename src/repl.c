
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

void trim_newline(char* str) {
	size_t len = strlen(str);
	if (len > 0 && str[len - 1] == '\n') {
		str[len - 1] = '\0';
	}
}

// fgets wrapper to check return value
void safe_fgets(char* buffer, size_t size) {
	while (true) {
		if (fgets(buffer, size, stdin) == NULL) {
			clearerr(stdin);
			printf("Input error, please try again: ");
			continue;
		}
		trim_newline(buffer);
		break;
	}
}

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
		safe_fgets(choice, sizeof(choice));

		for (int i = 0; choice[i]; i++) choice[i] = toupper(choice[i]);

		if (strcmp(choice, "MANUALLY") == 0 || strcmp(choice, "M") == 0) {
			printf("Enter the pass: ");
			safe_fgets(pass, 129);
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

char* copyString(const char* source) {
	if (source == NULL || strcmp(source, "") == 0) return NULL;
	char* destination = malloc(strlen(source) + 1);
	if (destination != NULL) strcpy(destination, source);
	return destination;
}

EntryPass create_entry() {
	char entryName[129], username[129], email[129], url[129], note[513], pass[129];

	printf("Enter the entry name (type \"BLANK\" to leave empty): ");
	safe_fgets(entryName, sizeof(entryName));
	if (strcmp(entryName, "BLANK") == 0) entryName[0] = '\0';

	printf("Enter the username (type \"BLANK\" to leave empty): ");
	safe_fgets(username, sizeof(username));
	if (strcmp(username, "BLANK") == 0) username[0] = '\0';

	printf("Enter the email (type \"BLANK\" to leave empty): ");
	safe_fgets(email, sizeof(email));
	if (strcmp(email, "BLANK") == 0) email[0] = '\0';

	printf("Enter the url (type \"BLANK\" to leave empty): ");
	safe_fgets(url, sizeof(url));
	if (strcmp(url, "BLANK") == 0) url[0] = '\0';

	printf("Enter the note (type \"BLANK\" to leave empty): ");
	safe_fgets(note, sizeof(note));
	if (strcmp(note, "BLANK") == 0) note[0] = '\0';

	create_password(pass);

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
	char buffer[20];
	uint32_t num;

	while (true) {
		pretty_print(entries, numEntries);
		printf("Enter the number of the entry which you'd like to %s: \n", keyword);
		printf("rogp? ");
		safe_fgets(buffer, sizeof(buffer));

		if (sscanf(buffer, "%u", &num) != 1) {
			printf("Invalid input. Please enter an integer.\n");
			continue;
		}

		if (num < numEntries) break;
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
	char choice[20];

	while (true) {
		printf("Now select which attribute you would like to edit(username, password, etc.): ");
		safe_fgets(choice, sizeof(choice));

		for (int i = 0; choice[i]; i++) choice[i] = toupper(choice[i]);

		if (strcmp(choice, "ENTRYNAME") == 0 || strcmp(choice, "ENTRY NAME") == 0) {
			printf("Enter the entry name: ");
			safe_fgets(entries[num].entryName, 129);
			break;
		}
		else if (strcmp(choice, "USERNAME") == 0) {
			printf("Enter the username: ");
			safe_fgets(entries[num].username, 129);
			break;
		}
		else if (strcmp(choice, "EMAIL") == 0) {
			printf("Enter the email: ");
			safe_fgets(entries[num].email, 129);
			break;
		}
		else if (strcmp(choice, "URL") == 0) {
			printf("Enter the url: ");
			safe_fgets(entries[num].url, 129);
			break;
		}
		else if (strcmp(choice, "NOTE") == 0) {
			printf("Enter the note: ");
			safe_fgets(entries[num].note, 513);
			break;
		}
		else if (strcmp(choice, "PASSWORD") == 0 || strcmp(choice, "PASS") == 0) {
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
	(*numEntries)--;
}

void print_help() {
	fprintf(stdout, "%s\n", "\n"
			"rogpass                           manages passwords somewhat safely\n"
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

int main() {
	FILE* file = NULL;
	EntryPass* entries = NULL;
	uint32_t numEntries = 0;
	char masterPass[129], filename[129], command[20];
	bool hasSaved = true;

	while (true) {
		char choice[10];
		printf("Create a new file or open an existing one(c/o): ");
		safe_fgets(choice, sizeof(choice));

		for (int i = 0; choice[i]; i++) {
			choice[i] = toupper(choice[i]);
		}

		if (strcmp(choice, "CREATE") == 0 || strcmp(choice, "C") == 0) {
			printf("Enter a file name(without the extension): ");
			safe_fgets(filename, sizeof(filename));
			if (strlen(filename) < 5 || strcmp(filename + strlen(filename) - 5, ".rogp") != 0) {
				strcat(filename, ".rogp");
			}

			printf("Enter the master password(using this you will access your file, make sure to remember it): ");
			safe_fgets(masterPass, sizeof(masterPass));

			file = fopen(filename, "w");
			if (!file) {
				printf("Error creating file for writing\n"); exit(EXIT_FAILURE);
			}
			fclose(file);
			break;
		}

		if (strcmp(choice, "OPEN") == 0 || strcmp(choice, "O") == 0) {
			printf("Enter a file name(with the extension): ");
			safe_fgets(filename, sizeof(filename));

			file = fopen(filename, "r");
			if (!file) {
				printf("Error opening file for redacting\n"); exit(EXIT_FAILURE);
			}

			printf("Enter the master password:	");
			safe_fgets(masterPass, sizeof(masterPass));

			unsigned char key[17];
			generate_key_from_password(masterPass, key);

			if (decrypt_file(filename, "temp.txt", key) != 0) {
				fprintf(stderr, "Decryption failed\n");
				return EXIT_FAILURE;
			}

			FILE* tempFile = fopen("temp.txt", "r");
			if (!tempFile) {
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

	while (true) {
		printf("rog> ");
		safe_fgets(command, sizeof(command));
		for (int i = 0; command[i]; i++) {
			command[i] = toupper(command[i]);
		}

		if (strcmp(command, "EXIT") == 0) {
			if (!hasSaved) {
				printf("You have unsaved changes! Are you sure you want to exit(type full yes or no): ");
				char saveChoice[10];
				safe_fgets(saveChoice, sizeof(saveChoice));
				for (int i = 0; saveChoice[i]; i++) {
					saveChoice[i] = toupper(saveChoice[i]);
				}
				if (strcmp(saveChoice, "YES") != 0) continue;
			}
			printf("Exiting...\n");
			break;
		}
		else if (strcmp(command, "HELP") == 0) {
			print_help();
		}
		else if (strcmp(command, "ADD") == 0) {
			EntryPass newEntry = create_entry();
			EntryPass* temp = realloc(entries, (numEntries + 1) * sizeof(EntryPass));
			if (!temp) {
				fprintf(stderr, "Memory reallocation failed\n");
				free(entries);
				exit(EXIT_FAILURE);
			}
			entries = temp;
			entries[numEntries++] = newEntry;
			hasSaved = false;
		}
		else if (strcmp(command, "SHOW") == 0) {
			if (!check_entry_count(numEntries)) {
				show_entry(entries, numEntries);
			}
		}
		else if (strcmp(command, "MOD") == 0 || strcmp(command, "MODIFY") == 0) {
			if (!check_entry_count(numEntries)) {
				mod_entry(entries, numEntries);
				hasSaved = false;
			}
		}
		else if (strcmp(command, "DEL") == 0 || strcmp(command, "DELETE") == 0) {
			if (!check_entry_count(numEntries)) {
				delete_entry(entries, &numEntries);
				hasSaved = false;
			}
		}
		else if (strcmp(command, "PRINT") == 0) {
			pretty_print(entries, numEntries);
		}
		else if (strcmp(command, "WRITE") == 0) {
			FILE* tempFile = fopen("temp.txt", "w");
			if (!tempFile) {
				printf("Error opening file for writing.\n");
				exit(EXIT_FAILURE);
			}
			serialize_entry(tempFile, entries, numEntries);
			fclose(tempFile);

			unsigned char key[17];
			generate_key_from_password(masterPass, key);

			remove(filename);
			if (encrypt_file("temp.txt", filename, key) != 0) {
				fprintf(stderr, "Encryption failed\n");
				exit(EXIT_FAILURE);
			}
			remove("temp.txt");
			hasSaved = true;
		}
		else printf("Invalid input, try again.\n");
	}

	return 0;
}
