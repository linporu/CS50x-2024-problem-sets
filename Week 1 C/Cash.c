// implement a program in C that prints the minimum coins needed to make the given amount of change

#include <cs50.h>
#include <stdio.h>

int calculate_quarters(int a);
int calculate_dimes(int a);
int calculate_nickels(int a);
int calculate_pennies(int a);

int main(void)
{
    // Prompt the user for change owed, in cents
    int cents;
    do
    {
        cents = get_int("Change owed: ");
    }
    while (cents < 0);

    // Calculate how many quarters you should give customer
    int q = calculate_quarters(cents);
    printf("q = %i\n", calculate_quarters(cents));

    // Subtract the value of those quarters from cents
    cents = cents - (q * 25);

    // Calculate how many dimes you should give customer
    int d = calculate_dimes(cents);
    printf("d = %i\n", calculate_dimes(cents));

    // Subtract the value of those dimes from remaining cents
    cents = cents - (d * 10);

    // Calculate how many nickels you should give customer
    int n = calculate_nickels(cents);
    printf("n = %i\n", calculate_nickels(cents));

    // Subtract the value of those nickels from remaining cents
    cents = cents - (n * 5);

    // Calculate how many pennies you should give customer
    int p = calculate_pennies(cents);
    printf("p = %i\n", calculate_pennies(cents));

    // Subtract the value of those pennies from remaining cents
    cents = cents - (p * 1);
    // Sum the number of quarters, dimes, nickels, and pennies used
    int sum = q + d + n + p;

    // Print that sum
    printf("sum = %i\n", sum);
}

// 計算 quarters 的函數
int calculate_quarters(int a)
{
    return a / 25;
}

// 計算 dimes 的函數
int calculate_dimes(int a)
{
    return a / 10;
}

// 計算 nickels 的函數
int calculate_nickels(int a)
{
    return a / 5;
}

// 計算 pennies 的函數
int calculate_pennies(int a)
{
    return a / 1;
}
