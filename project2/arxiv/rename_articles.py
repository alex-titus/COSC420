
in_file = "arxiv-citations.txt"
index_file = "indexed-citations.dat"
article_ids_file = "article_ids.dat"
hits_scores_file = "hits_scores.dab"
combined_file = "combined_file.dat"

article_indexes = {}
lookup_later = []
current_article = ""
current_id = 0
citation_dict = {}
in_citations = False
i = 0
with open(in_file, 'r') as citations:
    for line in citations:
        line = line.strip()
        if line == "-----":
            in_citations = True
        elif line == "+++++":
            in_citations = False
        elif not in_citations:
            article_indexes[line] = i
            i+=1
            current_article = article_indexes[line]
            citation_dict[current_article] = []
        elif in_citations:
            #print(f"citing {current_article}")
            try:
                current_citation = article_indexes[line]
                citation_dict[current_article].append(current_citation)#add the index of the citation to the list of citations for
            except KeyError:
                lookup_later.append((current_article, line))
    print(f"num articles before lookups {len(article_indexes)}, {i}")
    for article, line in lookup_later:
        try:
            citation = article_indexes[line]
            citation_dict[article].append(citation)
        except KeyError:
            #the cited article has no listing outside of being cited, so it needs to get a listing in citations
            article_indexes[line] = i
            i+=1
            citation = article_indexes[line]
            citation_dict[citation] = []
            citation_dict[article].append(citation)
            #print(f"{line} has no listing outside of being cited")
print(f"num articles {len(article_indexes)}, {i}")
auth_scores = {}
hub_scores = {}

with open(index_file, 'w+') as outf:
    for article, citations in citation_dict.items():
        if citations:
            outf.write(f"{article}:{citations}\n")
            hub_scores[article] = len(citations)
            for cite in citations:
                try:
                    auth_scores[cite] += 1
                except KeyError:
                    auth_scores[cite] = 1

with open(hits_scores_file, 'w+') as outf:
    for name, id in article_indexes.items():
        try:
            hub = hub_scores[id]
        except KeyError:
            hub = 0
        try:
            auth = auth_scores[id]
        except KeyError:
            auth = 0
        if hub != 0 or auth != 0:
            outf.write(f"{id}:{hub}:{auth}\n")

with open(article_ids_file, 'w+') as outf:
    for article, id in article_indexes.items():
        outf.write(f"{article}:{id}\n")

with open(combined_file, 'w+') as outf:
    for article, id in zip(article_indexes.items(), citation_dict.items()):
        if len(id[1]) == 0:
            new_line = (f"{id[0]}:{article[0]}:0\n")
        else:
            new_line = (f"{id[0]}:{article[0]}:{id[1]}\n")
        outf.write(new_line)
        #if new_line[-3] == '['
#print(citation_dict)
