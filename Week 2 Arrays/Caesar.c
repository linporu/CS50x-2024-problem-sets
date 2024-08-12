#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaration
bool only_digits(string s);
string encryption(string plaintext, int key);

int main(int argc, string argv[])
{
    // Make sure program was run with just one command-line argument
    if (argc != 2)
    {
        printf("More or less than one command-line argument.\n");
        return 1;
    }

    // Make sure every character in argv[1] is a digit
    if (!only_digits(argv[1]))
    {
        printf("Not every character is a digit.\n");
        return 1;
    }

    // Convert argv[1] from a `string` to an `int`
    int key = atoi(argv[1]);

    // Prompt user for plaintext
    string plaintext = get_string("Plaintext:  ");

    // Encrypt the plaintext
    string ciphertext = encryption(plaintext, key);

    // Print ciphertext
    printf("Ciphertext: %s", ciphertext);

    // Exit
    printf("\n");
    return 0;
}

// Function to check whether every character in argv[1] is a digit
bool only_digits(string s)
{
    for (int i = 0, len = strlen(s); i < len; i++)
    {
        if (isdigit(s[i]) == false)
        {
            return false;
        }
    }
    return true;
}

// Function to encrypt the plaintext
string encryption(string plaintext, int key)
{
    int len = strlen(plaintext);
    string ciphertext;
    ciphertext = malloc(len + 1);

    // Encryption for each character
    for (int i = 0; i < len; i++)
    {
        if (isupper(plaintext[i]))
        {
            ciphertext[i] = (plaintext[i] - 'A' + key) % 26 + 'A';
        }
        else if (islower(plaintext[i]))
        {
            ciphertext[i] = (plaintext[i] - 'a' + key) % 26 + 'a';
        }
        else
        {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[len] = '\0';
    return ciphertext;
}
