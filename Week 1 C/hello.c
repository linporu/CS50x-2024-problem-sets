#include <cs50.h>
#include <stdio.h>

void print_space(int length);
void print_brick(int length);

int main(void)
{
    string name = get_string("What is your name?\n");
    printf("hello, %s\n", name);
}
