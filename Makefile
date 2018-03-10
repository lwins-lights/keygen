CC		= cc
CFLAGS		= -ansi -pedantic -Wall -Wextra -O3 -g3
INSTALL_PATH	= /usr/local/bin

keygen: keygen.o lib/sha256/sha256.o
	$(CC) $(CFLAGS) -o keygen keygen.o lib/sha256/sha256.o

keygen.o: keygen.c
	$(CC) $(CFLAGS) -c -o keygen.o keygen.c

lib/sha256/sha256.o: lib/sha256/sha256.c
	$(CC) $(CFLAGS) -c -o lib/sha256/sha256.o lib/sha256/sha256.c
	
.PHONY: all
all: keygen

.PHONY: clean
clean: 
	rm keygen
	rm keygen.o
	rm lib/sha256/sha256.o

.PHONY: install
install: keygen
	sudo cp keygen $(INSTALL_PATH)/
	mkdir ~/.keygen
	chmod 700 ~/.keygen

.PHONY: uninstall
uninstall:
	sudo rm $(INSTALL_PATH)/keygen
	rm -rf ~/.keygen

