#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Declaration
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int compute_index(int letters, int words, int sentences);

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Print the numbers of letters, words and sentences
    printf("%i, %i, %i\n", letters, words, sentences);

    // Compute the Coleman-Liau index
    int index = compute_index(letters, words, sentences);
    printf("index = %i\n", index);

    // Print the grade level
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (1 < index && index < 17)
    {
        printf("Grade %i\n", index);
    }
    else if (17 < index)
    {
        printf("Grade 16+\n");
    }
}

// Function to count the number of letters
int count_letters(string text)
{
    int letters = 0;

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isalpha(text[i]))
        {
            letters += 1;
        }
    }

    return letters;
}

// Function to count the number of words

int count_words(string text)
{

    int space = 0;

    // Count the number of space
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isspace(text[i]))
        {
            space += 1;
        }
    }

    // the number of words == the number of space + 1
    int words = space + 1;

    return words;
}

// Function to count the number of sentences
int count_sentences(string text)
{
    int sentences = 0;

    // Count the number of punctuation except '
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences += 1;
        }
    }

    return sentences;
}

// Function to compute the Coleman-Liau index
int compute_index(int letters, int words, int sentences)
{
    // Compute L
    float L = (float) letters * 100 / words;
    printf("L = %f\n", L);

    // Compute S
    float S = (float) sentences * 100 / words;
    printf("S = %f\n", S);

    // Compute index
    float index = 0.0588 * L - 0.296 * S - 15.8;
    printf("index = %f\n", index);

    return round(index);
}
