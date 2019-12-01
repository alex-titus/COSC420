#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include "rbtree.c"
#include "matlib.h"

#include <time.h>

#define NB_ELEMS 10

int convIntToStr(char * str, int x){
  sprintf(str, "%d", x);
  return (strlen(str));
}

/* Drier program to test above function*/
int main()
{
    srand(time(NULL));
    struct node *root = NULL;
    clock_t t0 = clock();
    struct arxivArticle testArticle1;
    struct arxivArticle testArticle2;
    struct arxivArticle testArticle3;
    struct arxivArticle testArticle4;
    struct arxivArticle testArticle5;
    struct arxivArticle testArticle6;
    testArticle1.article_id = "test";
    testArticle2.article_id = "test";
    testArticle3.article_id = "test1";
    testArticle4.article_id = "test2";
    testArticle5.article_id = "a";
    testArticle6.article_id = "test4";
    insert(&root, &testArticle1);
    insert(&root, &testArticle2);
    insert(&root, &testArticle3);
    insert(&root, &testArticle4);
    insert(&root, &testArticle5);
    insert(&root, &testArticle6);
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
