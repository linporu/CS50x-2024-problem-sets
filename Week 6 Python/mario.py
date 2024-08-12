from cs50 import get_int


def main():

    # 詢問高度，n < 1 時重複詢問
    while True:
        n = get_int("Height: ")
        if n > 0 and n < 9:
            break

    # 打出金字塔
    for i in range(n):
        print_space(n - i - 1)
        print_brick(i + 1)
        print()


# 打出空格的函數
def print_space(length):
    for i in range(length):
        print(" ", end="")


# 打出磚塊的函數
def print_brick(length):
    for i in range(length):
        print("#", end="")


main()
