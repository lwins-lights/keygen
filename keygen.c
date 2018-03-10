#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

#include "lib/sha256/sha256.h"

char *saltRelLoc = "/.keygen/salt";
char saltLoc[999] = {0};

char *usage =
	"  -h:\n"
	"    Print this message and exit.\n"
	"  -g [string]:\n"
	"    Generate the key corresponding to the given string.\n"
	"  -s [salt]:\n"
	"    Set the salt.";

void init() {
	
	char *home;
	
	home = getenv("HOME");
	strcpy(saltLoc, home);
	strcat(saltLoc, saltRelLoc);

}

void printUsageAndExit(char **argv) {

    fprintf(stderr, "Usage: %s [options]\n%s\n", argv[0], usage);
    exit(0);

}

void genKeyAndExit(char *str) {

	SHA256_CTX ctx;
	u8 hash[999];
	char res[999], buff[999], salt[999];
	int i;
	FILE *fin;

	fin = fopen(saltLoc, "r");
	if (fin == NULL || fscanf(fin, "%s", salt) == EOF) {
		fprintf(stderr, "Cannot find the salt file. Please use -s to set it first.\n");
		exit(0);
	}
	strcpy(buff, str);
	strcat(buff, salt);
	sha256_init(&ctx);
	sha256_update(&ctx, (const u8 *)buff, strlen(buff));
	sha256_final(&ctx, hash);
	for (i = 0; i < 32; i++)
		sprintf(res + (2 * i), "%02x", *(hash + i));
	printf("%s\n", res);
	exit(0);

}

void setSaltAndExit(char *str) {

	FILE *fout;
	
	fout = fopen(saltLoc, "w"); 
	if (fout == NULL) {
		fprintf(stderr, "keygen is not installed correctly.");
		exit(0);
	}
	fprintf(fout, "%s\n", str);
	fclose(fout);
	exit(0);

}

void parseArguments(int argc, char **argv) {

	char c;
	
	optind = 0;
	while ((c = getopt(argc, argv, "hg:s:")) != EOF) {
		switch (c) {
			case 'h':
				printUsageAndExit(argv);
				break;
			case 'g':
				genKeyAndExit(optarg);
				break;
			case 's':
				setSaltAndExit(optarg);
				break;
			default:
				printUsageAndExit(argv);
		}
	}
	printUsageAndExit(argv);

}

int main(int argc, char **argv) {

	init();
	parseArguments(argc, argv);
	return 0;

}
