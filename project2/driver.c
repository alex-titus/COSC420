// C program for Red-Black Tree insertion
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

#include "rbtree.c"
#include "matlib.h"

#include <time.h>

#define NB_ELEMS 10

int convIntToStr(char * str, int x){
  sprintf(str, "%d", x);
  return (strlen(str));
}

int citationsIndexer()
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int clone;

  fp = fopen("./arxiv/arxiv-citations.txt", "r");
  clone = open("./arxiv/arxiv-citations-indexed.txt", O_WRONLY | O_CREAT, 0770);

  if (fp == NULL){
    exit(EXIT_FAILURE);
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    if (line[0] == '-'){
      /* We are currently looking at a citations, must skip all lines til a '+' */
      do {
        getline(&line, &len, fp);
      } while (line[0] != '+');
    } else {
      write(clone, line, read);
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
    struct node *root = NULL;
    clock_t t0 = clock();
    /*
    Do Computational Work Here

    clock_t t1 = clock();
    printf("inorder Traversal Is :\n");
    inorder(root);
    printf("\n");
    float time_taken = (float)(t1 - t0) / CLOCKS_PER_SEC * 1000;
	  printf("insertion took %fms -> %f us/elem\n",
		time_taken,
		time_taken / NB_ELEMS * 1000);
    */
    return 0;
}
