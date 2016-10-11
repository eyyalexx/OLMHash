FOR QUESTION 1:

1. Open Terminal

2. Run the command (excluding the quotations): "gcc OLMHash.c -o Q1". This will compile the .c file.

3. In terminal, run the command (excluding the quotations): "./Q1". (This command should be used everytime the user wants to run the program.)

4. The program will generate a .txt file where the usernames and the most recent password hashes will be stored. The file will be called "OLMHash.txt".

5. Follow prompts in the terminal.



FOR QUESTION 2:

- In order for Question 2 (i.e. The Challenge-Response Authentication) to work properly, the user must have gone through Question 1.

- For question 2, the generated "OLMHash.txt", which contains usernames and their most recent password hashes, is required for the server. The server uses the stored hash values when computing f(r,h'), where h' is the hash associated with the user id in the OLMHash.txt.

1. Open Terminal

2. Run the command (excluding the quotations): "gcc Authentication.c -o Q1". This will compile the .c file.

3. In terminal, run the command (excluding the quotations): "./Q2". (This command should be used everytime the user wants to run the program.)

4. Follow prompts in the terminal. The "user ID" and "password" that you enter should match the "user ID" and "password" from Question 1, in which case you will be granted access. Otherwise, access will not be granted.
