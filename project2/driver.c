// C program for Red-Black Tree insertion
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

#include "rbtree.c"


#include <time.h>

#define NB_ELEMS 10

int convIntToStr(char * str, int x){
  sprintf(str, "%d", x);
  return (strlen(str));
}

int metadataInsertion(struct word_node* root)
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen("./arxiv/arxiv-metadata.txt", "r");

  if (fp == NULL){
    exit(EXIT_FAILURE);
  }
  while ((read = getline(&line, &len, fp)) != -1) {
    struct arxivArticle* article = malloc(sizeof(struct arxivArticle));
    initArxivArticle(article);
    if (line[0] != '+'){
      strcpy(article->article_id, line);
      printf("line: %s", article->article_id);
      getline(&line, &len, fp);
      strcpy(article->title, line);
      getline(&line, &len, fp);
      strcpy(article->author, line);
      getline(&line, &len, fp);
      int i, j;
      int offset = 0;
      for(i = 0; i < strlen(line);){
        int foundDelim = 0;
        while(!foundDelim){
          if (!((line[i] >= 48 && line[i] <= 57) || (line[i] >= 65 && line[i] <= 90) || (line[i] >= 97 && line[i] <= 122))){
            foundDelim = 1;
          }
          i++;
        }
        int wordSize = i-offset - 1;

        if( wordSize > 1)
        {
            printf("wordsize %d", wordSize);
            char *insertWord = malloc((wordSize + 1) * sizeof(char));
            strncpy(insertWord, (line + offset), wordSize);
            insertWord[wordSize] = '\0';
            printf("%s is the word\n", insertWord);
            struct word_node* returney = word_search(insertWord, root);
            if(returney != NULL)
                printf("found word %s\n", returney->word);
            else
            {
                word_insert(&root, insertWord, article);
                printf("\ninorder:\n");
                word_inorder(root);
            }
        }
        offset = i;
      }
  }else
  {
    // printf("article info:\n");
    // printf("%s", article.article_id);
    // printf("%s", article.title);
    // printf("%s", article.author);
    sleep(10);
    }
  }

  fclose(fp);
  if (line){
    free(line);
  }
  exit(EXIT_SUCCESS);
}


/* Drier program to test above function*/
int main()
{
    umask(0);
    srand(time(NULL));
    struct word_node *root = malloc(sizeof(struct word_node));
    word_init_node(root);
    clock_t t0 = clock();
    int i;

    metadataInsertion(root);

    clock_t t1 = clock();
    printf("inorder Traversal Is :\n");
    word_inorder(root);
    printf("\n");
    float time_taken = (float)(t1 - t0) / CLOCKS_PER_SEC * 1000;
	  printf("insertion took %fms -> %f us/elem\n",
		time_taken,
		time_taken / NB_ELEMS * 1000);
    return 0;
}
