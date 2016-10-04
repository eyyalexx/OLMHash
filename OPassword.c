#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_COMBINATIONS 456976


void generate();
void E(char *in, char *out);
char *hash_process(char *password);
void searchHash(char *fname, char *hash);

char key[5];
char *filename = "combinations.txt";

int main()
{
	printf("\nGenerating combinations.\n");
	//create all possible hashes
	generate();
	printf("Done!\n");
	printf("Enter hash key: ");
	fgets (key, 5, stdin);

	searchHash(filename, key);

}


void searchHash(char *fname, char *hash){
	FILE *fp;
	char temp[512];
	int line_num = 1;
	int find_result = 0;
	char *lineOut[12];

	//open file
	if((fp = fopen(fname, "r")) == NULL) {
		exit(1);
	}

	//find
	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, hash)) != NULL) {
			printf("The corresponding password is on line: %d", line_num);
			printf("\n%s\n", temp);
			if(strcmp(temp, hash) == 0){
				break;
			}

			find_result++;
	
		}
		line_num++;
	}

	//close file
	if(fp) fclose(fp);

}



void generate(){

	//open file.
	FILE *file = fopen("combinations.txt", "w+");

	if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

   	
	char* alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


	char * str = calloc(4 + 1, sizeof(char));
	char * str2 = calloc(4 + 1, sizeof(char));
    
    for (int i = 0; i < NUM_COMBINATIONS ; i++) {
        int j = i;
        for (int k = 0; k < 4; k++) {
            str[k] = alpha[j % 26];
            j = j / 26;
        }

        str2 = hash_process(str);        
        fprintf(file, "%s : %s\n", str2, str);


    }

    //close file.
    fclose(file);


}

char *hash_process(char *password)
{
	// Allocates a block of memory for an array of 13 char elements, each of them char bytes (i.e. 4) long, and initializes all its bits 
        // to zero.
	char *hash = (char*)calloc(4 + 1, sizeof(char));

	//Loop through every 4 bytes (i.e. 4 chars) and perform the hashing function.
     int i;
	for (i = 0; i < 4; i++)
	{
		E(&password[4*i], &hash[4*i]);
	}

	return hash;
}

/********************* E function *************************/
// DES replacement cipher
// The function E takes 4 bytes from *in as input and
// writes 4 bytes to *out
void E(char *in, char *out)
{
    out[0]=(in[0]&0x80)^(((in[0]>>1)&0x7F)^((in[0])&0x7F));
    out[1]=((in[1]&0x80)^((in[0]<<7)&0x80))^(((in[1]>>1)&0x7F)^((in[1])&0x7F));
    out[2]=((in[2]&0x80)^((in[1]<<7)&0x80))^(((in[2]>>1)&0x7F)^((in[2])&0x7F));
    out[3]=((in[3]&0x80)^((in[2]<<7)&0x80))^(((in[3]>>1)&0x7F)^((in[3])&0x7F));
}