
def main():
    while True:
        try:
            height = int(input("Height:"))
        except ValueError:
            continue

        if (height < 1) | (height > 8):
            continue
        else:
            break

    for i in range(1, height + 1, 1):
        blocks = " " * (height - i) + "#" * i + " " * 2 + "#" * i
        print(blocks)


if __name__ == "__main__":
    main()