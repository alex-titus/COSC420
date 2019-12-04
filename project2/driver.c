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

  fp = fopen("./arxiv/shortened-arxiv-metadata.txt", "r");
  //fp = fopen("./arxiv/arxiv-metadata.txt", "r");

  if (fp == NULL){
      printf("File not found\n");
    exit(EXIT_FAILURE);
  }
  while ((read = getline(&line, &len, fp)) != -1) {
    int idLength = strlen(line)+1;
    char *id = malloc(idLength * sizeof(char));
    if (line[0] != '+'){
      strcpy(id, line);
      id[idLength-1] = '\0';
      printf("id: %s", id);

      getline(&line, &len, fp);
      int titleLength = strlen(line) + 1;
      char *title = malloc(titleLength * sizeof(char));
      strcpy(title, line);
      title[titleLength-1] = '\0';

      getline(&line, &len, fp);
      int authorLength = strlen(line)+1;
      char *author = malloc(authorLength * sizeof(char));
      strcpy(author, line);
      author[authorLength-1] = '\0';

      getline(&line, &len, fp);
      struct arxivArticle* article = malloc(sizeof(struct arxivArticle));
      initArxivArticle(article, idLength, titleLength, authorLength);


      strcpy(article->id, id);
      strcpy(article->author, author);
      strcpy(article->title, title);
      //print_article(article);

      free(id);
      free(title);
      free(author);

      int i, j;
      int offset = 0;
      for(i = 0; i < strlen(line);){
        int foundDelim = 0;
        while(!foundDelim){
          if ((line[i] >= 65 && line[i] <= 90))
            line[i] += 32;
          if (!((line[i] >= 48 && line[i] <= 57) || (line[i] >= 97 && line[i] <= 122)))
            foundDelim = 1;
          i++;
        }
        int wordSize = i-offset;

        if( wordSize > 2)
        {
            //printf("\nwordsize: %d, ", wordSize);
            char *insertWord = malloc((wordSize) * sizeof(char));
            strncpy(insertWord, (line + offset), wordSize);
            insertWord[wordSize-1] = '\0';

            struct word_node *new_word = (struct word_node*)malloc(sizeof(struct word_node));
            word_init_node(new_word, insertWord, article);

            article_insert(&new_word->sub_root, article);

            //printf("%s is the word\n", insertWord);
            //struct word_node* returney = word_search(insertWord, root);
            // if(returney != NULL){
            //     //printf("found word %s\n", returney->word);
            // }else
            // {
            word_insert(&root, new_word);
                //printf("\ninorder:\n");
            // }
            free(insertWord);
        }
        offset = i;
      }
      puts("\n\n\n");
      word_inorder(root);
      sleep(1);
  }else
  {
    // printf("article info:\n");
    // printf("%s", article.arword_initticle_id);
    // printf("%s", article.title);
    // printf("%s", article.author);
    //sleep(10);
    }
  }

  word_inorder(root);
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
    struct word_node *cthulu_tree = NULL;
    clock_t t0 = clock();
    int i;

    printf("proccessing search index ...\n");

    metadataInsertion(cthulu_tree);

    printf("Welcome to the muthah fukin game beech \n -1 to quit\n");
    int done = 0;
    while(!done)
    {
        char* input = malloc(100 * sizeof(char));
        printf("Search: ");
        scanf("%s", input);
        printf("%s\n", input);

        struct word_node* ret = word_search(input, cthulu_tree);
    }

    return 0;
}
