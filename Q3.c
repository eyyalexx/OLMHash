#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TABLE_LENGTH 456976
#define MAX_PASSWORD_LENGTH 12
#define OPASS_LENGTH 4

void fill_tables(char *letterpassword, int tableindex, int max);

char password_table[TABLE_LENGTH][OPASS_LENGTH + 1];
char hash_table[TABLE_LENGTH][OPASS_LENGTH + 1];
int tableindex = 0;

int OPass()
{
	// Allocates a block of memory for an array of 5 char elements, each of them char bytes (i.e. 1) long, and initializes all its bits 
        // to zero.
	char *password = (char*)calloc(OPASS_LENGTH + 1, sizeof(char));

	//From the allocated array, set the last character to null.
	password[OPASS_LENGTH] = '\0';

	fill_tables(password, 0, 4);

	while (1)
	{
		char *hash = (char*)calloc(OPASS_LENGTH + 1, sizeof(char));
		printf("Enter the hash of a 4-character password. Type Type 0 to exit:\n");
		scanf("%5s", hash);
		
		char c;		
		do
		{
			c = getchar();
		} while (c != '\n' && c != '\0');

		fflush(stdin);

		if (strcmp("0", hash) == 0)
		{
			return -1;
		}

		if (strlen(hash) != 4)
		{
			printf("The hash input must be 4 characters.\n");
			free(hash);
			continue;
		}

		int found = 0;
		for (int i = 0; i < TABLE_LENGTH; i++)
		{
			if(strcmp(hash_table[i], hash) == 0)
			{
				printf("The hash %s is associated with the password %s.\n", hash, password_table[i]);
				found = 1;
				break;

				//return password_table[i];
			}
		}

		if (found == 0)
		{
			printf("The hash %s is not associated with a password.\n", hash);
		}
	}

	return 0;
}

// DES Replacement Cipher
// The function E takes 4 bytes from *in as input and writes 4 bytes to *out.
void E(char *in, char *out)
{
	out[0] = (in[0] & 0x80) ^ (((in[0] >> 1) & 0x7F) ^ ((in[0]) & 0x7F));
	out[1] = ((in[1] & 0x80) ^ ((in[0] << 7) & 0x80)) ^ (((in[1] >> 1) & 0x7F) ^ ((in[1]) & 0x7F));
	out[2] = ((in[2] & 0x80) ^ ((in[1] << 7) & 0x80)) ^ (((in[2] >> 1) & 0x7F) ^ ((in[2]) & 0x7F));
	out[3] = ((in[3] & 0x80) ^ ((in[2] << 7) & 0x80)) ^ (((in[3] >> 1) & 0x7F) ^ ((in[3]) & 0x7F));
}

char *hash_process(int n, char *password)
{
	// Allocates a block of memory for an array of 13 char elements, each of them char bytes (i.e. 4) long, and initializes all its bits 
        // to zero.
	char *hash = (char*)calloc(MAX_PASSWORD_LENGTH + 1, sizeof(char));

	//Loop through every 4 bytes (i.e. 4 chars) and perform the hashing function.
        int i;
	for (i = 0; i < n; i++)
	{
		E(&password[4*i], &hash[4*i]);
	}

	return hash;
}

void fill_tables(char *letterpassword, int tableindex, int max)
{

	FILE *file1 = fopen("combinations.txt", "w+");
	//FILE *file2 = fopen("hashes.txt", "w+");

	//FILE *fp1;
	//FILE *fp2;
	//fp1=fopen("/home/pass.txt", "r");
	//fp2=fopen("/home/hash.txt", "r");

	if (tableindex >= max)
	{
		strncpy(password_table[tableindex], letterpassword, OPASS_LENGTH);
		char *hash = hash_process(1, letterpassword);
    
		//fprintf(file1, "%s\n", letterpassword);
		//fprintf(file2, "%s\n", hash);
		printf("%s : %s\n", letterpassword, hash);

		strncpy(hash_table[tableindex], hash, OPASS_LENGTH);
		tableindex++;
	}

	else
	{
		for (int i = 0; i < 26; i++)
		{
			char letter = 'A' + i;
			letterpassword[tableindex] = letter;
			fill_tables(letterpassword, tableindex + 1, max);
		}
	}

	//fclose(file1);
	//fclose(file2);
}

int main()
{
	OPass();
	return 0;
}
