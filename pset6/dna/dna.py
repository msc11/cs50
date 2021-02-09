import sys
import csv


def main():
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py CSVFILE TXTFILE")

    with open(sys.argv[1], 'r') as f:
        STRlist = next(csv.reader(f))
    with open(sys.argv[2], 'r') as f:
        dna_sample = f.readline()

    dna_sample_result = ['Result']

    for STR in STRlist[1:]:
        STRlen = len(STR)
        max_cnt = 0
        cur_cnt = 0
        on = False
        i = 0
        while i < len(dna_sample) - STRlen + 1:
            if dna_sample[i:i+STRlen] == STR:
                cur_cnt += 1
                i += STRlen
            else:
                max_cnt = max(max_cnt, cur_cnt)
                cur_cnt = 0
                i += 1
        dna_sample_result.append(str(max_cnt))

    with open(sys.argv[1], 'r') as f:
        reader = csv.reader(f)

        for row in reader:
            if (dna_sample_result[1:] == row[1:]):
                print(row[0])
                return 0

        print("No match")


if __name__ == "__main__":
    main()