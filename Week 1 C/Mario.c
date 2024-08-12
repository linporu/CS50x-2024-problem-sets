// Implement a program in C that recreates that pyramid, using hashes (#) for bricks,

#include <cs50.h>
#include <stdio.h>

void print_space(int length);
void print_brick(int length);

int main(void)
{
    // 詢問高度 n，n < 1 時重複詢問
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1);

    // 打出金字塔
    for (int i = 0; i < n; i++)
    {
        print_space(n - i + 1);
        print_brick(i + 1);
        printf("\n");
    }
}

// 空格函數 print_space
void print_space(int length)
{
    for (int i = 0; i < length; i++)
    {
        printf(" ");
    }
}

// 磚塊函數 print_brick
void print_brick(int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("#");
    }
}
