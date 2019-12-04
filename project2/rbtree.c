// C program for Red-Black Tree article_insertion
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


struct arxivArticle
{
  char* title;
  char* author;
  char* article_id;
};

struct article_node
{
    struct arxivArticle* article;
    char color;
    struct article_node *left, *right, *parent;
};

struct word_node
{
    char* word;
    struct article_node* sub_root;
    char color;
    struct word_node *left, *right, *parent;
};

void initArxivArticle(struct arxivArticle *article, int id, int title, int author){
  article->article_id = malloc(id * sizeof(char));
  article->title = malloc(title * sizeof(char));
  article->author = malloc(author * sizeof(char));
}

void article_init_node(struct article_node *node){
  node->article = malloc(3 * sizeof(char*));
  node->left=node->right=node->parent=NULL;
  initArxivArticle(node->article, 0, 0, 0);
}

void word_init_node(struct word_node *node){
  node->word = malloc(30 * sizeof(char));
  strcpy(node->word, "cthulu");
  node->sub_root = malloc(sizeof(struct article_node));
  article_init_node(node->sub_root);
  strcpy(node->sub_root->article->article_id, "cthulu");
  node->left=node->right=node->parent=NULL;
}

void article_insert(struct article_node **root, struct arxivArticle* article);
void word_insert_fixup(struct word_node **root, struct word_node *z);
void word_insert(struct word_node **root, char* word, struct arxivArticle* article);
struct word_node* word_search(char* search_word, struct word_node* root);

void word_right_rotate(struct word_node **root,struct word_node *y);
void word_left_rotate(struct word_node **root,struct word_node *x);

void word_insert_fixup(struct word_node **root, struct word_node *z)
{
    // iterate until z is not the root and z's parent color is red
    while (z != *root && z != (*root)->left && z != (*root)->right && z->parent->color == 'R')
    {
        struct word_node *y;

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
                word_right_rotate(root,z->parent->parent);
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
                word_left_rotate(root,z->parent);
                word_right_rotate(root,z->parent->parent);
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
                word_left_rotate(root,z->parent->parent);
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
                word_right_rotate(root,z->parent);
                word_left_rotate(root,z->parent->parent);
            }
        }
    }
    (*root)->color = 'B'; //keep root always black
}

void word_insert(struct word_node **root, char* word, struct arxivArticle* article)
{
    // Allocate memory for new node
    struct word_node *z = (struct word_node*)malloc(sizeof(struct word_node));
    word_init_node(z);

    strcpy(z->word, word);
    article_insert(&z->sub_root, article);
    //if root is null make z as root    free(article.abstract);
    if (*root == NULL)
    {
        z->color = 'B';
        (*root) = z;
    }
    else
    {
        struct word_node *y = NULL;
        struct word_node *x = (*root);

        // Follow standard BST article_insert steps to first article_insert the node
        while (x != NULL)
        {
            //printf("x->article->article_id = %s\nz->article->article_id = %s \n", x->article->article_id, z->article->article_id);
            y = x;
            if (strcmp(z->word, x->word) < 0){

                //printf("moving left\n");
                x = x->left;
            } else if (strcmp(z->word, x->word) > 0) {
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
        else if (strcmp(z->word, y->word) < 0){
            y->left = z;
        } else if (strcmp(z->word, y->word) > 0){
            y->right = z;
        }
        z->color = 'R';

        word_insert_fixup(root,z);
        free(x);
        //free(y);
        //free(z->word);
        //free(z->sub_root);
        //free(z);
    }

}


// Left Rotation
void word_left_rotate(struct word_node **root,struct word_node *x)
{
    if (x == NULL || x->right == NULL)
        return;
    //y stored pointer of right child of x
    struct word_node *y = x->right;

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
    else
        x->parent->right = y;

    // make x as left child of y
    y->left = x;

    //update parent pointer of x
    x->parent = y;
}

// Right Rotation (Similar to article_left_rotate)
void word_right_rotate(struct word_node **root,struct word_node *y)
{
    if (!y || !y->left)
        return ;
    struct word_node *x = y->left;
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

struct word_node* word_search(char* search_word, struct word_node* root)
{
    struct word_node* current = root;
    while(current != NULL)
    {
        if(strcmp(current->word, search_word) == 0)
        {
            return current;
        }
        if(strcmp(search_word, current->word) < 0)
            current = current->left;
        else if(strcmp(search_word, current->word) > 0)
            current = current->right;
        }
    return NULL;//if the loop exits without returning that means the data it's     looking for isn't in the tree
}

void word_inorder(struct word_node *root)
{
    //printf("%s ", root);
    if (root == NULL)
        return;
    word_inorder(root->left);
    printf("%s ", root->word);
    word_inorder(root->right);
}
//===================================== ARTICLE TREE ===================================
// Left Rotation
void article_left_rotate(struct article_node **root,struct article_node *x)
{
    if (!x || !x->right)
        return ;
    //y stored pointer of right child of x
    struct article_node *y = x->right;

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


// Right Rotation (Similar to article_left_rotate)
void article_right_rotate(struct article_node **root,struct article_node *y)
{
    if (!y || !y->left)
        return ;
    struct article_node *x = y->left;
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

// Utility function to fixup the Red-Black tree after standard BST article_insertion
void article_insert_fixup(struct article_node **root, struct article_node *z)
{
    // iterate until z is not the root and z's parent color is red
    while (z != *root && z != (*root)->left && z != (*root)->right && z->parent->color == 'R')
    {
        struct article_node *y;

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
                article_right_rotate(root,z->parent->parent);
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
                article_left_rotate(root,z->parent);
                article_right_rotate(root,z->parent->parent);
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
                article_left_rotate(root,z->parent->parent);
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
                article_right_rotate(root,z->parent);
                article_left_rotate(root,z->parent->parent);
            }
        }
    }
    (*root)->color = 'B'; //keep root always black
}

// Utility function to article_insert newly node in RedBlack tree
void article_insert(struct article_node **root, struct arxivArticle* article)
{
    // Allocate memory for new node
    struct article_node *z = (struct article_node*)malloc(sizeof(struct article_node));
    article_init_node(z);

    //z->article->article_id = article->article_id;
    strcpy(z->article->article_id, article->article_id);
    strcpy(z->article->author, article->author);
    strcpy(z->article->title, article->title);

    //if root is null make z as root    free(article.abstract);
    if (*root == NULL)
    {
        z->color = 'B';
        (*root) = z;
    }
    else
    {
        struct article_node *y = NULL;
        struct article_node *x = (*root);

        // Follow standard BST article_insert steps to first article_insert the node
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

        article_insert_fixup(root,z);
        free(x);
        free(y);
        free(z->article->article_id);
        free(z->article->author);
        free(z->article->title);
        free(z->article);
        free(z);
    }

}


struct article_node* article_search(char* search_id, struct article_node* root)
{
    struct article_node* current = root;
    while(current != NULL)
    {
        if(strcmp(current->article->article_id, search_id) == 0)
        {
            return current;
        }
        if(strcmp(search_id, current->article->article_id) < 0)
            current = current->left;
        else if(strcmp(search_id, current->article->article_id) > 0)
            current = current->right;
        }
    return NULL;//if the loop exits without returning that means the data it's     looking for isn't in the tree
}

// A utility function to traverse Red-Black tree in article_inorder fashion
void article_inorder(struct article_node *root)
{
    //printf("%s ", root);
    if (root == NULL)
        return;
    article_inorder(root->left);
    printf("%s ", root->article->article_id);
    article_inorder(root->right);
}


// ========================== WORD TREE ============================
