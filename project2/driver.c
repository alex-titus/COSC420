// C program for Red-Black Tree insertion
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
//  and converts the character to ascii in the new file
//A Red-Black tree node structure
struct arxivArticle
{
  char* title;
  char* author;
  char* abstract;
  char* article_id;
};

struct node
{
    struct arxivArticle* article;
    char color;
    struct node *left, *right, *parent;
};

// Left Rotation
void LeftRotate(struct node **root,struct node *x)
{
    if (!x || !x->right)
        return ;
    //y stored pointer of right child of x
    struct node *y = x->right;

    //store y's left subtree's pointer as x's right child
    x->right = y->left;

    //update parent pointer of x's right
    if (x->right != NULL)
        x->right->parent = x;

    //update y's parent pointer
    y->parent = x->parent;

    // if x's parent is null make y as root of tree
    if (x->parent == NULL)
        (*root) = y;

    // store y at the place of x
    else if (x == x->parent->left)
        x->parent->left = y;
    else    x->parent->right = y;

    // make x as left child of y
    y->left = x;

    //update parent pointer of x
    x->parent = y;
}


// Right Rotation (Similar to LeftRotate)
void rightRotate(struct node **root,struct node *y)
{
    if (!y || !y->left)
        return ;
    struct node *x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent =y->parent;
    if (x->parent == NULL)
        (*root) = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

// Utility function to fixup the Red-Black tree after standard BST insertion
void insertFixUp(struct node **root, struct node *z)
{
    // iterate until z is not the root and z's parent color is red
    while (z != *root && z != (*root)->left && z != (*root)->right && z->parent->color == 'R')
    {
        struct node *y;

        // Find uncle and store uncle in y
        if (z->parent && z->parent->parent && z->parent == z->parent->parent->left)
            y = z->parent->parent->right;
        else
            y = z->parent->parent->left;

        // If uncle is RED, do following
        // (i)  Change color of parent and uncle as BLACK
        // (ii) Change color of grandparent as RED
        // (iii) Move z to grandparent
        if (!y)
            z = z->parent->parent;
        else if (y->color == 'R')
        {
            y->color = 'B';
            z->parent->color = 'B';
            z->parent->parent->color = 'R';
            z = z->parent->parent;
        }

        // Uncle is BLACK, there are four cases (LL, LR, RL and RR)
        else
        {
            // Left-Left (LL) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Right Rotate Grandparent
            if (z->parent == z->parent->parent->left &&
                z == z->parent->left)
            {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent->parent);
            }

            // Left-Right (LR) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Left Rotate Parent
            // (iii) Right Rotate Grand Parent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->left &&
                z == z->parent->right)
            {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root,z->parent);
                rightRotate(root,z->parent->parent);
            }

            // Right-Right (RR) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Left Rotate Grandparent
            if (z->parent && z->parent->parent &&
                z->parent == z->parent->parent->right &&
                z == z->parent->right)
            {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root,z->parent->parent);
            }

            // Right-Left (RL) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Right Rotate Parent
            // (iii) Left Rotate Grand Parent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->right &&
                z == z->parent->left)
            {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent);
                LeftRotate(root,z->parent->parent);
            }
        }
    }
    (*root)->color = 'B'; //keep root always black
}

// Utility function to insert newly node in RedBlack tree
void insert(struct node **root, struct arxivArticle* article)
{
    // Allocate memory for new node
    struct node *z = (struct node*)malloc(sizeof(struct node));
    z->article = article;
    z->left = z->right = z->parent = NULL;

    //if root is null make z as root
    if (*root == NULL)
    {
        z->color = 'B';
        (*root) = z;
    }
    else
    {
        struct node *y = NULL;
        struct node *x = (*root);

        // Follow standard BST insert steps to first insert the node
        while (x != NULL)
        {
            //printf("x->article->article_id = %s\nz->article->article_id = %s \n", x->article->article_id, z->article->article_id);
            y = x;
            if (strcmp(z->article->article_id, x->article->article_id) < 0){

                //printf("moving left\n");
                x = x->left;
            } else if (strcmp(z->article->article_id, x->article->article_id) > 0) {
                //printf("moving right\n");
                x = x->right;
            }
            else {
                //printf("duplicate, returning\n");
                return;
            }
        }
        z->parent = y;
        if( y == NULL)
            *root = z;
        else if (strcmp(z->article->article_id, y->article->article_id) < 0){
            y->left = z;
        } else if (strcmp(z->article->article_id, y->article->article_id) > 0){
            y->right = z;
        }
        z->color = 'R';

        insertFixUp(root,z);
    }
}

// A utility function to traverse Red-Black tree in inorder fashion
void inorder(struct node *root)
{
    //printf("%s ", root);
    if (root == NULL)
        return;
    inorder(root->left);
    printf("%s ", root->article->article_id);
    inorder(root->right);
}

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
  int n = 0;
  int clone;
  char * test;

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
      sprintf(line, "%d %s", n, line);
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
