#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void pass_hash(char *in, char *out);
void E(char *in, char *out);
void chopN(char *str, size_t n);

int main()
{
    char user_ID [32];
    char user_pass [12];
    char user_pass_hash [12];
    char *temp_ID;
    char *temp_pass;
    char line[256];
    char line_cpy[256];
    char new_line[256];
    int existing_user = 0;
    const int max_attempts = 5;
    int attempts = 0;

    //open files
    FILE *file;
    FILE *temp;
    file = fopen("OLMHash.txt", "a+");
    temp = fopen("temp.txt", "w+");

    //error opening file
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //initial prompt
    printf("Enter your user ID:\n");
    scanf ("%s", user_ID);

    //check ID length
    while (strlen(user_ID) < 4 || strlen(user_ID) > 32)
    {
        printf("The user ID must be between 4 and 32 characters.\nPlease enter another ID:\n");
        scanf ("%s", user_ID);
    }
    
    //get line from file
    while (fgets(line, sizeof(line), file))
    {
        strcpy(line_cpy, line);

        //get ID and passwod from line
        temp_ID = strtok(line, " ");
        temp_pass = strtok(NULL, "\n");

        //user exists - ask for password and replace current
        if (strcmp(user_ID, temp_ID) == 0)
        {
            existing_user = 1;

            //get pass from user
            printf("Enter your old password:\n");
            scanf ("%s", user_pass);

            //compute hash of user_pass
            pass_hash(user_pass, user_pass_hash);

            //check incorrect password attempts
            while (strcmp(user_pass_hash, temp_pass) != 0 && attempts != max_attempts)
            {
                printf("Incorrect password. %i tries remaining.\nPlease try again:\n", max_attempts-attempts);
                scanf ("%s", user_pass);
                pass_hash(user_pass, user_pass_hash);
                attempts++;
            }

            if (attempts == 5)
            {
                fputs(line_cpy, temp);
            }

            //if pass correct - ask for new pass
            if (strcmp(user_pass_hash, temp_pass) == 0)
            {
                //get new password
                printf("Password correct\n");
                printf("Enter your new password:\n");
                scanf ("%s", user_pass);
                pass_hash(user_pass, user_pass_hash);

                //create new line to add to file
                strcpy(new_line, user_ID);
                strcat(new_line, " ");
                strcat(new_line, user_pass_hash);
                strcat(new_line, "\n");

                //add line to file
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
        //user doesn't exist - add username and password
        printf("\nNot existing user\n\n");
        printf("Enter your new password:\n");
        scanf ("%s", user_pass);
        pass_hash(user_pass, user_pass_hash);

        //create new line to add to file
        strcpy(new_line, user_ID);
        strcat(new_line, " ");
        strcat(new_line, user_pass_hash);
        strcat(new_line, "\n");

        //add line to file
        fputs(new_line, temp);
    }

    //displays lockout message to user
    if (attempts == max_attempts)
    {
        printf("Too many unsuccessful attempts - your account is locked\n");
    }

    //close files
    fclose(file);
    fclose(temp);

    //delete original file and rename temp
    remove("OLMHash.txt");
    rename("temp.txt", "OLMHash.txt" );

    return 0;
}

//hash function
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

    char *hash = (char*)calloc(12+1, sizeof(char));

    int k;
    for (k = 0; k < 3; k++)
    {
	E(&in[4*k], &hash[4*k]);
    }

    //concatenate strings
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


void chopN(char *str, size_t n)
{
    size_t len = strlen(str);
    if (n > len)
        return;  // Or: n = len;
    memmove(str, str+n, len - n + 1);
}

