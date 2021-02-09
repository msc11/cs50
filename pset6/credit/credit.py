import cs50


def main():
    intnum = cs50.get_int("Number:")
    number = str(intnum)

    key = int(number[:2])
    if (key == 34) or (key == 37):
        validity_check(number, "AMEX")
    elif (key >= 51) and (key <= 55):
        validity_check(number, "MASTERCARD")
    elif number[0] == "4":
        validity_check(number, "VISA")
    else:
        print("INVALID")


def validity_check(n, name):
    if (len(n) == 15) and (name == "AMEX"):
        valid = checksum(n)
    elif (len(n) == 16) and (name == "MASTERCARD"):
        valid = checksum(n)
    elif (len(n) == 13 or len(n) == 16) and (name == "VISA"):
        valid = checksum(n)
    else:
        print("INVALID")

    if valid:
        print(name)
    else:
        print("INVALID")


def checksum(n):
    result = 0

    for i, digit in enumerate(n[::-1]):
        if i % 2 == 0:
            result += int(digit)
        else:
            buf = int(digit) * 2
            result += (buf // 10) + (buf % 10)

    return result % 10 == 0


if __name__ == "__main__":
    main()