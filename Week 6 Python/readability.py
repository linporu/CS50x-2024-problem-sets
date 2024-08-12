def main():

    # Prompt the user for some text
    text = input("Text: ")

    # Count the number of letters, words, and sentences in the text
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Compute the Coleman-Liau index
    index = compute_index(letters, words, sentences)

    # Print the grade level
    if index < 1:
        print("Before Grade 1")
    elif 1 < index and index < 17:
        print(f"Grade {index}")
    elif index > 17:
        print("Grade 16+")


# Function to count the number of letters
def count_letters(text):
    letter_counter = 0
    for i in range(len(text)):
        if str.isalpha(text[i]) == True:
            letter_counter += 1
    return letter_counter


# Function to count the number of words
def count_words(text):
    space_counter = 0
    for i in range(len(text)):
        if str.isspace(text[i]) == True:
            space_counter += 1
    return space_counter + 1  # the number of words == the number of space + 1


# Function to count the number of sentences
def count_sentences(text):
    sentence_counter = 0
    for i in range(len(text)):
        if text[i] in ['.', '!', '?']:
            sentence_counter += 1
    return sentence_counter


# Function to compute the Coleman-Liau index
def compute_index(letters, words, sentences):

    # Compute L
    L = float(letters) * 100 / float(words)

    # Compute S
    S = float(sentences) * 100 / float(words)

    # Compute index
    index = 0.0588 * L - 0.296 * S - 15.8
    return round(index)


main()
