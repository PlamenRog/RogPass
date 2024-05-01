CFLAGS = -lcrypto -Wextra -Wall -Werror -pedantic -lm

install:
	gcc ./src/repl.c ./src/rogp_generation.c ./src/generation_utility.c ./src/print_functions.c -o rogp $(CFLAGS)

pass_gen:
	gcc ./tests/test_password_generation.c ./src/generation_utility.h ./src/generation_utility.c -o ./tests/test_password_generation $(CFLAGS)

file_gen:
	gcc ./tests/test_entry_files.c ./src/rogp_generation.c -o ./tests/test_entry_files $(CFLAGS)

enc_gen:
	gcc ./tests/test_encryption_functions.c ./src/rogp_generation.c -o ./tests/test_encryption_functions $(CFLAGS)
