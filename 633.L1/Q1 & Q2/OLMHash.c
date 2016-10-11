#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN_USER_LENGTH 4
#define MAX_USER_LENGTH 32
#define MAX_PASSWORD_LENGTH 12
#define MAX_ATTEMPTS 5

void pass_hash(char *in, char *out);
void E(char *in, char *out);

//void chopN(char *str, size_t n);

int main()
{
    char user_ID [MAX_USER_LENGTH];
    char user_pass [MAX_PASSWORD_LENGTH];
    char user_pass_hash [MAX_PASSWORD_LENGTH];
    char *temp_ID;
    char *temp_pass;
    char line[256];
    char line_cpy[256];
    char new_line[256];
    int existing_user = 0;
    int attempts = 0;

    //Open Files
    FILE *file;
    FILE *temp;
    file = fopen("OLMHash.txt", "a+");
    temp = fopen("temp.txt", "w+");

    //Error Opening File
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //Initial Prompt
    printf("Enter your user ID:\n");
    scanf ("%s", user_ID);

    //Check User ID Length
    while (strlen(user_ID) < MIN_USER_LENGTH || strlen(user_ID) > MAX_USER_LENGTH)
    {
        printf("The user ID must be between 4 and 32 characters.\nPlease enter another ID:\n");
        scanf ("%s", user_ID);
    }
    
    //Get Line From OLMHash.txt File
    while (fgets(line, sizeof(line), file))
    {
        strcpy(line_cpy, line);

        //Get ID And Password From Line
        temp_ID = strtok(line, " ");
        temp_pass = strtok(NULL, "\n");

        //User exists, ask for new password and replace current password. 
        if (strcmp(user_ID, temp_ID) == 0)
        {
            existing_user = 1;

            //Get Pass From User
            printf("Enter your old password:\n");
            scanf ("%s", user_pass);

            //Compute the hash for the inputted password.
            pass_hash(user_pass, user_pass_hash);

            //Check Incorrect Password Attempts
            while (strcmp(user_pass_hash, temp_pass) != 0 && attempts != MAX_ATTEMPTS)
            {
                printf("Incorrect password. %i tries remaining.\nPlease try again:\n", MAX_ATTEMPTS - attempts);
                scanf ("%s", user_pass);
                pass_hash(user_pass, user_pass_hash);
                attempts++;
            }

            if (attempts == 5)
            {
                fputs(line_cpy, temp);
            }

            //If the password is correct, ask for a new password which replaces the old.
            if (strcmp(user_pass_hash, temp_pass) == 0)
            {
                //Get New Password
                printf("Password correct\n");
                printf("Enter your new password:\n");
                scanf ("%s", user_pass);
                pass_hash(user_pass, user_pass_hash);

                //Create New Line To Add To File
                strcpy(new_line, user_ID);
                strcat(new_line, " ");
                strcat(new_line, user_pass_hash);
                strcat(new_line, "\n");

                //Add Line To File
                fputs(new_line, temp);
            }
        }

        if (strcmp(user_ID, temp_ID) != 0)
        {
            fputs(line_cpy, temp);
        }
    }

    if (existing_user == 0)
    {
        //If the User ID does not exist, add User ID and password (i.e. the password hash) to the OLMHash.txt file.
        printf("\nNot existing user\n\n");
        printf("Enter your new password:\n");
        scanf ("%s", user_pass);
        pass_hash(user_pass, user_pass_hash);

        //Create New Line To Add To File
        strcpy(new_line, user_ID);
        strcat(new_line, " ");
        strcat(new_line, user_pass_hash);
        strcat(new_line, "\n");

        //Add Line To File
        fputs(new_line, temp);
    }

    //Displays Lockout Message To User
    if (attempts == MAX_ATTEMPTS)
    {
        printf("Too many unsuccessful attempts - your account is locked.\n");
    }

    //Close Files
    fclose(file);
    fclose(temp);

    //Delete original file and rename temp.
    remove("OLMHash.txt");
    rename("temp.txt", "OLMHash.txt" );

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

//void chopN(char *str, size_t n)
//{
//    size_t len = strlen(str);
//    if (n > len)
//        return;  // Or: n = len;
//    memmove(str, str+n, len - n + 1);
//}
