in_file = "arxiv-metadata.txt"
stop_words = []



with open("stopwords") as sw:
    for line in sw:
        stop_words.append(line)

i = 0
with open(in_file, 'r') as citations:
    for line in citations:
        line = line.strip()
            if line == "++++++":
                i = 0
            elif i == 2:
                for word in line.split(" ")
