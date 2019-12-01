
in_file = "arxiv-citations.txt"
out_file = "indexed-citations.dat"
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
with open(out_file, 'w+') as of:
    for article, citations in citation_dict.items():
        if citations:
            of.write(f"{article}:{citations}\n")
#print(citation_dict)
