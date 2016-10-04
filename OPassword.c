#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void generate();

int main()
{
	printf("Generating combinations.\n");
	generate();
	printf("Done!\n");
}



void generate(){

	//open file.
	FILE *file = fopen("combinations.txt", "w+");

	if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

	char* alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	char * str = calloc(4 + 1, sizeof(char));
    
    for (int i = 0; i < 1679616 ; i++) {
        int j = i;
        for (int k = 0; k < 4; k++) {
            str[k] = alpha[j % 36];
            j = j / 36;
        }
        
        fprintf(file, "%s\n", str);
    }

    //close file.
    fclose(file);


}