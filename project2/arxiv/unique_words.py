in_file = "arxiv-metadata.txt"
stop_words = []
with open("stopwords") as sw:
    for line in sw:
        stop_words.append(line)

unique_words = []
i = 0
with open(in_file, 'r') as citations:
    for line in citations:
        line = line.strip()
        if line == "++++++":
            i = 0
        elif i == 2:
            for word in line.split(" "):
                if word not in stop_words:
                    if word not in unique_words:
                        unique_words.append(word)
        i = (i + 1) % 5
#print(unique_words, "\n")
print(len(unique_words))

#number of unique words 465284
