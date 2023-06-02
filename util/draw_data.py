import argparse
import matplotlib.patches as patches
import matplotlib.pyplot as plt
import csv

def draw_rectangle(rectangle, axes):

    id = rectangle[0]
    x = rectangle[1]
    y = rectangle[2]
    w = rectangle[3]
    h = rectangle[4]

    rect_patch = patches.Rectangle(
        (x, y), w, h,
        linewidth=1, edgecolor='r', facecolor='none'
    )
    axes.add_patch(rect_patch)
    axes.text(x + w / 2, y + h / 2, id, weight='bold', fontsize=5, ha='center', va='center')


def main():
    parser = argparse.ArgumentParser(description = 'AAA Assignment Data Visualization Generator')
    parser.add_argument('data_file', metavar = 'Data File', help = 'Path to the data file')
    args = parser.parse_args()

    rect_csv = open(args.data_file, "r")
    rect_reader = csv.reader(rect_csv)

    fig, ax = plt.subplots()
    max_x = 0
    max_y = 0

    for row in rect_reader:
        rect = [int(row[0]), float(row[1]), float(row[2]), float(row[3]) - float(row[1]), float(row[4]) - float(row[2])]

        if rect[2] + rect[4] > max_y:
            max_y = rect[2] + rect[4]

        if rect[1] + rect[3] > max_x:
            max_x = rect[1] + rect[3]
         
        draw_rectangle(rect, ax)

    plt.xlim([-10, max_x + 10])
    plt.ylim([-10, max_y + 10])
    print(f"Saving to {args.data_file[:-4]}.pdf\n")
    plt.savefig(f"{args.data_file[:-4]}_dataviz.pdf")
    plt.close()

if __name__ == "__main__":
    main()
