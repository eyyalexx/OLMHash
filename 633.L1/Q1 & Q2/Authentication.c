#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_USER_LENGTH 32
#define MAX_PASSWORD_LENGTH 12

void pass_hash(char *in, char *out);
void E(char *in, char *out);
char *generateRandXOR(char* hash, int r);
void R(char* in, char* out, int r);

int main()
{
    char user_ID [MAX_USER_LENGTH];
    char user_pass [MAX_PASSWORD_LENGTH];
    char user_pass_hash [MAX_PASSWORD_LENGTH];
    char *temp_ID;
    char *temp_pass;
    char line[256];
    int existing_user = 0;
    int random_number;
    char* user_xor;
    char* temp_xor;

    //Open Files
    FILE *file;
    file = fopen("OLMHash.txt", "r");

    //Error Opening File
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //Initial Prompt
    printf("Enter your user ID:\n");
    scanf ("%s", user_ID);

    printf("Enter your password:\n");
    scanf ("%s", user_pass);

    //Generate a random number that comes from server
    srand(time(NULL)); // Randomize Seed

    random_number = rand();
    //printf("%i\n", random_number);

    //Hash Input Password
    pass_hash(user_pass, user_pass_hash);
    //printf ("%s\n", user_pass_hash);

    //Put hashed password and the random number through XOR function.
    //The "user_xor" will be sent to the server for comparison.
    user_xor = generateRandXOR(user_pass_hash, random_number);
    //printf("%s\n", user_xor);


    //Server Side Process:

    //Get Line From OLMHash.txt File
    while (fgets(line, sizeof(line), file))
    {

        //Get ID And Password From Line
        temp_ID = strtok (line, " ");
        temp_pass = strtok (NULL, "\n");
        //printf ("%s\n%s\n", temp_ID, temp_pass);

        //If user exists, use exiting hash with random number to calculate "temp_xor".
        if (strcmp(user_ID, temp_ID) == 0)
        {
            existing_user = 1;

            //Put exiting hashed password (in the OLMHash.txt) and the random number through XOR function.
            temp_xor = generateRandXOR(temp_pass, random_number);
            //printf("%s\n", user_xor);
            //printf("%s\n", temp_xor);
            //printf ("%s\n",  user_pass_hash);
            //printf ("%s\n", temp_pass);

            //If results match, print "Access Granted".
            if (strcmp(user_xor, temp_xor) == 0)
            {
                printf("Access Granted\n");
            }

            //If results do not match, print "Access Denied".
            if (strcmp(user_xor, temp_xor) != 0)
            {
                printf("Access Denied\n");
            }
        }
    }

    //If user does not exist, print "User ID Not Found".
    if (existing_user == 0)
    {
        //User Doesn't Exist
        printf("User ID Not Found\n");
    }

    //Close Files
    fclose(file);

    return 0;
}

//Hash Function
void pass_hash(char *in, char *out)
{

    //Adjust To Upper Case
    int i;
    for(i = 0; in[i]; i++)
    {
        in[i] = toupper(in[i]);
    }

    //Adjust Length. Has to be exactly 12 characters, so either cut or add to make 12 characters long.
    if (strlen(in) > MAX_PASSWORD_LENGTH)
    {
        strncpy(out, in, MAX_PASSWORD_LENGTH);
    }

    if (strlen(in) < MAX_PASSWORD_LENGTH)
    {
        int j;
        for (j = strlen(in); j < MAX_PASSWORD_LENGTH; j++)
        {
            in[j] = '@';
        }
    }

    // Allocates a block of memory for an array of 12 char elements, each of them char bytes (i.e. 4) long, and initializes all its bits 
    // to zero.
    char *hash = (char*)calloc(MAX_PASSWORD_LENGTH + 1, sizeof(char));

    //Loop through every 4 bytes (i.e. 4 chars) and perform the hashing function. It loops 3 times in order to cover the 3 blocks of 4
    //letters, which will add up to 12 characters.
    int k;
    for (k = 0; k < 3; k++)
    {
	E(&in[4 * k], &hash[4 * k]);
    }

    strcpy(out, hash);
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

//xor the hash with the server's random number
char *generateRandXOR(char* hash, int r)
{
        char * out = (char*)calloc(MAX_PASSWORD_LENGTH + 1, sizeof(char));
        R(hash, out, r);
        R(&hash[4], &out[4], r);
        R(&hash[8], &out[8], r);
        return out;
}

//xor function for each bundle (4 bytes)
void R(char* in, char* out, int r)
{
        out[3] = in[3] ^ (r & 255); //xor the first byte with r's first byte
        r = r >> 8; //shift r by one byte
        out[2] = in[2] ^ (r & 255);
        r = r >> 8;
        out[1] = in[1] ^ (r & 255);
        r = r >> 8;
        out[0] = in[0] ^ (r & 255);
}
