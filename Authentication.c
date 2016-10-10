#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char * generateRandXOR(char* hash, int r);
void R(char* in, char* out, int r);
void pass_hash(char *in, char *out);
void E(char *in, char *out);
void chopN(char *str, size_t n);

const int MAX_PASSWORD_LENGTH = 12;

int main()
{
    char user_ID [32];
    char user_pass [12];
    char user_pass_hash [12];
    char *temp_ID;
    char *temp_pass;
    char line[256];
    int existing_user = 0;
    int random_number;
    const int MAX_PASSWORD_LENGTH = 12;
    char* user_xor;
    char* temp_xor;



    //open files
    FILE *file;
    file = fopen("OLMHash.txt", "r");

    //error opening file
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //initial prompt
    printf("Enter your user ID:\n");
    scanf ("%s", user_ID);

    printf("Enter your password:\n");
    scanf ("%s", user_pass);

      //generate a random 32 bit number that comes from server
    srand(time(NULL)); // randomize seed

    random_number = rand();
    printf("%i\n", random_number);


    //hash password
    pass_hash(user_pass, user_pass_hash);
    printf ("%s\n", user_pass_hash);

    //put through XOR function with rand number
    user_xor = generateRandXOR(user_pass, random_number);
    //printf("%s\n", user_xor);

    //send password to server

    //server side
    //get line from file
    while (fgets(line, sizeof(line), file))
    {

        //get ID and passwod from line
        temp_ID = strtok (line," ");
        temp_pass = strtok (NULL,"\n");

        //printf ("%s\n%s\n", temp_ID, temp_pass);

        //user exists - ask for password and replace current
        if (strcmp(user_ID, temp_ID) == 0)
        {

            existing_user = 1;
            //put hash through XOR function with 32 bit rand number
            temp_xor = generateRandXOR(temp_pass, random_number);
            printf("%s\n", user_xor);
            printf("%s\n", temp_xor);


            //printf ("%s\n",  user_pass_hash);
            //printf ("%s\n", temp_pass);

                      //if pass correct - Access Granted
            if (strcmp(user_xor, temp_xor) == 0)
            {
                //get new password
                printf("Access Granted\n");
            }

            //if pass incorrect - Access Denied
            if (strcmp(user_xor, temp_xor) != 0)
            {
                printf("Access Denied\n");
            }
        }
    }

    //not existing user - print user not found
    if (existing_user == 0)
    {
        //user doesn't exist
        printf("User ID not found\n");
    }

    //close files
    fclose(file);

    return 0;
}

//hash funbction
void pass_hash(char *in, char *out)
{

    //adjust to upper case
    int i;
    for(i = 0; in[i]; i++)
    {
        in[i] = toupper(in[i]);
    }

    //adjust length - cut or add \0
    if (strlen(in) > 12)
    {
        strncpy(out, in, 12);
    }

      if (strlen(in) < 12)
    {
        int j;
        for (j = strlen(in); j < 12; j++)
        {
            in[j] = '@';
        }
    }

    //encryption
    char sub1[3];
    char sub1_hash[32];
    char sub2[3];
    char sub2_hash[32];
    char sub3[3];
    char sub3_hash[32];

    //split into three substrings
    memcpy(sub1, in, 4);
    chopN(in, 4);
    memcpy(sub2, in, 4);
    chopN(in, 4);
    memcpy(sub3, in, 4);

     //use E function
    E(sub1, sub1_hash);
    E(sub2, sub2_hash);
    E(sub3, sub3_hash);

    //concatenate strings
    strcpy(out, sub1_hash);
    strcat(out, sub2_hash);
    strcat(out, sub3_hash);
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


void chopN(char *str, size_t n)
{
    size_t len = strlen(str);
    if (n > len)
        return;  // Or: n = len;
    memmove(str, str+n, len - n + 1);
}

//xor the hash with the server's random number
char * generateRandXOR(char* hash, int r)
{
        char * out = (char*)calloc(MAX_PASSWORD_LENGTH, sizeof(char));
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
