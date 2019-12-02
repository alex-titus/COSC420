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

int metadataInsertion(struct node *root)
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen("./arxiv/arxiv-metadata.txt", "r");

  if (fp == NULL){
    exit(EXIT_FAILURE);
  }
  struct arxivArticle article;
  while ((read = getline(&line, &len, fp)) != -1) {
    if (line[0] != '+'){
      strcpy(article.article_id, line);
      getline(&line, &len, fp);
      strcpy(article.title, line);
      getline(&line, &len, fp);
      strcpy(article.author, line);
      getline(&line, &len, fp);
      strcpy(article.abstract, line);
    }

    printf("article info:\n");
    printf("%s\n", article.author);
    printf("%s\n", article.abstract);
    printf("%s\n", article.title);
    printf("%s\n", article.article_id);
    sleep(1);
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
    struct node *root = NULL;
    clock_t t0 = clock();
    int i;
    /*
    for(i = 0; i < 1000; i++){
      char test[20];
      convIntToStr(test, i);
      article.article_id = test;
      article.abstract = test;
      article.author = test;
      article.title = test;
      insert(&root, &article);
      free(article.abstract);
      free(article.article_id);
      free(article.author);
      free(article.title);
    }
    */
    clock_t t1 = clock();
    printf("inorder Traversal Is :\n");
    inorder(root);
    printf("\n");
    float time_taken = (float)(t1 - t0) / CLOCKS_PER_SEC * 1000;
	  printf("insertion took %fms -> %f us/elem\n",
		time_taken,
		time_taken / NB_ELEMS * 1000);
    return 0;
}
