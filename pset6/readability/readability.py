import cs50


def main():
    text = cs50.get_string("Text:")

    N = 0  # num words
    L = 0  # num letters
    S = 0  # num sentences

    text = text.split()
    N = len(text)
    for word in text:
        for t in word:
            if (t == ".") or (t == "!") or (t == "?"):
                S += 1
            elif str.isalpha(t):
                L += 1

    print("#words:", N, "#sentences:", S, "#letters:", L)

    index = 0.0588 * (L / (N / 100.0)) - 0.296 * (S / (N / 100.0)) - 15.8
    index_rnd = round(index)

    if index < 1:
        print("Before Grade 1")
    elif index_rnd >= 16:
        print("Grade 16+")
    else:
        print("Grade", index_rnd)


if __name__ == "__main__":
    main()