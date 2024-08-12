# implement a program in C that prints the minimum coins needed to make the given amount of change

from cs50 import get_float


def main():

    # Prompt the user for change owed, in dollars
    while True:
        dollars = get_float("Change owed: ")
        if dollars > 0:
            break

    # Convert dollars to cents
    cents = dollars * 100

    # Calculate how many quarters you should give customer
    q = cents // 25

    # Subtract the value of those quarters from cents
    cents -= q * 25

    # Calculate how many dimes you should give customer
    d = cents // 10

    # Subtract the value of those dimes from remaining cents
    cents -= d * 10

    # Calculate how many nickels you should give customer
    n = cents // 5

    # Subtract the value of those nickels from remaining cents
    cents -= n * 5

    # Calculate how many pennies you should give customer
    p = cents // 1

    # Sum the number of quarters, dimes, nickels, and pennies used
    sum = q + d + n + p

    # Print that sum
    print(f"{sum:.0f}")


main()
