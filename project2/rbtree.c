// C program for Red-Black Tree article_insertion
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>


struct arxivArticle
{
  char* id;
  int file_offset;
};

struct article_node
{
    struct arxivArticle* article;
    int size;
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
void print_article(struct arxivArticle* article)
{
    printf("%s\n", article->id);
}

void initArxivArticle(struct arxivArticle *article, char* id, int offset){
  article->id = malloc(strlen(id)+1 * sizeof(char));
  strcpy(article->id, id);
  article->id[strlen(id)] = '\0';
  article->file_offset = offset;
}

void article_init_node(struct article_node *node, char* id, int offset){
  node->article = malloc(sizeof(struct arxivArticle));
  node->size = 1;
  node->left=node->right=node->parent=NULL;
  initArxivArticle(node->article, id, offset);
}

// void print_article(struct article_node* node, File* fp)
// {
//
// }

void delete_article(struct article_node* del)
{
    free(del->article->id);
    free(del->article);
    free(del);
}

void article_delete_tree(struct article_node* root)
{
    if(root != NULL)
    {
        article_delete_tree(root->left);
        article_delete_tree(root->right);
        delete_article(root);
    }
}

void word_delete_tree(struct word_node* root)
{
    if(root != NULL)
    {
        word_delete_tree(root->left);
        word_delete_tree(root->right);
        free(root->word);
        article_delete_tree(root->sub_root);
        free(root);
    }
}

void article_insert(struct article_node **root, struct arxivArticle* article);
void word_init_node(struct word_node *node, char* word, struct arxivArticle* article){
  node->word = malloc(30 * sizeof(char));
  strcpy(node->word, word);
  node->sub_root = malloc(sizeof(struct article_node));
  article_init_node(node->sub_root, article->id, article->file_offset);
  //article_insert(&node->sub_root, article);
  node->left=node->right=node->parent=NULL;
}

void word_insert_fixup(struct word_node **root, struct word_node *z);
void word_insert(struct word_node **root,struct word_node* z);
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

void word_insert(struct word_node **root, struct word_node* z)
{
    // printf("word insert\n");
    //if root is null make z as root    free(article.abstract);
    if (*root == NULL)
    {
        printf("this should only happen once\n");
        (*root) = malloc(sizeof(struct word_node));
        word_init_node((*root), z->word, z->sub_root->article);
        (*root)->color = 'B';
    }
    else
    {
        struct word_node *y = NULL;
        struct word_node *x = (*root);

        // Follow standard BST article_insert steps to first article_insert the node
        while (x != NULL)
        {
            //printf("x->article->id = %s\nz->article->id = %s \n", x->article->id, z->article->id);
            y = x;
            // printf("x(current) = %s\n", x->word);
            // printf("z(new) = %s\n", z->word);
            if (strcmp(z->word, x->word) < 0){

                // printf("moving left\n");
                x = x->left;
            } else if (strcmp(z->word, x->word) > 0) {
                // printf("moving right\n");
                x = x->right;
            }
            else {
                // printf("word already in tree, %s, %s\n", z->word, x->word);
                article_insert(&x->sub_root, z->sub_root->article);
                // delete_article(z->sub_root);
                // free(z->word);
                // free(z);

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
    if(root == NULL)
    {
        printf("root is null");
        return NULL;
    }

    struct word_node* current = root;
    while(current != NULL)
    {
        // printf("Search Current: %s\n", current->word);
        if(strcmp(current->word, search_word) == 0)
            return current;
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

void word_inorder_list(struct word_node* root, char** list)
{
    int i = 0;
    if (root == NULL)
        return;
    word_inorder_list(root->left, list);
    sprintf(list[i], "%s", root->word);
    word_inorder_list(root->right, list);
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
    // printf("article insert\n");
    // Allocate memory for new node
    struct article_node *z = (struct article_node*)malloc(sizeof(struct article_node));

    article_init_node(z, article->id, article->file_offset);
    strcpy(z->article->id, article->id);

    //if root is null make z as root    free(article.abstract);
    if (strcmp((*root)->article->id, "null") == 0)
    {
        printf("this should never happen\n");
        z->color = 'B';
        delete_article(*root);
        (*root) = z;
        (*root)->size = 1;
    }
    else
    {
        struct article_node *y = NULL;
        struct article_node *x = (*root);

        // Follow standard BST article_insert steps to first article_insert the node
        while (x != NULL)
        {
            //printf("x->article->id = %s\nz->article->id = %s \n", x->article->id, z->article->id);
            y = x;
            if (strcmp(z->article->id, x->article->id) < 0){

                //printf("moving left\n");
                x = x->left;
            } else if (strcmp(z->article->id, x->article->id) > 0) {
                //printf("moving right\n");
                x = x->right;
            }
            else {
                delete_article(z);
                // printf("duplicate, returning\n");
                return;
            }
        }
        z->parent = y;
        if( y == NULL)
            *root = z;
        else if (strcmp(z->article->id, y->article->id) < 0){
            y->left = z;
        } else if (strcmp(z->article->id, y->article->id) > 0){
            y->right = z;
        }
        z->color = 'R';

        article_insert_fixup(root,z);
        free(x);
    }
    (*root)->size += 1;

}


struct article_node* article_search(char* search_id, struct article_node* root)
{
    struct article_node* current = root;
    while(current != NULL)
    {
        if(strcmp(current->article->id, search_id) == 0)
        {
            return current;
        }
        if(strcmp(search_id, current->article->id) < 0)
            current = current->left;
        else if(strcmp(search_id, current->article->id) > 0)
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
    printf("%s ", root->article->id);
    article_inorder(root->right);
}

void article_inorder_list(struct article_node* root, char** list)
{
    int i = 0;
    if (root == NULL)
        return;
    article_inorder_list(root->left, list);
    sprintf(list[i], "%s", root->article->id);
    article_inorder_list(root->right, list);
}

//====================================== STOP TREE ===================================
struct stop_node
{
    char* stop;
    char color;
    struct stop_node *left, *right, *parent;
};

void stop_init_node(struct stop_node *node, char* stop)
{
	node->stop = malloc((strlen(stop)+1) * sizeof(char));
	strcpy(node->stop, stop);
	node->left = node->right = node->parent = NULL;
}


// Left Rotation
void stop_left_rotate(struct stop_node **root,struct stop_node *x)
{
    //y stored pointer of right child of x
    struct stop_node *y = x->right;

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


// Right Rotation (Similar to stop_left_rotate)
void stop_right_rotate(struct stop_node **root,struct stop_node *y)
{
    struct stop_node *x = y->left;
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
void stop_insert_fixup(struct stop_node **root,struct stop_node *z)
{
    // iterate until z is not the root and z's parent color is red
    while (z != *root && z->parent->color == 'R')
    {
        struct stop_node *y;

        // Find uncle and store uncle in y
        if (z->parent == z->parent->parent->left)
            y = z->parent->parent->right;
        else
            y = z->parent->parent->left;

        // If uncle is RED, do following
        // (i)  Change color of parent and uncle as BLACK
        // (ii) Change color of grandparent as RED
        // (iii) Move z to grandparent
        if (y->color == 'R')
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
                stop_right_rotate(root,z->parent->parent);
            }

            // Left-Right (LR) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Left Rotate Parent
            // (iii) Right Rotate Grand Parent
            if (z->parent == z->parent->parent->left &&
                z == z->parent->right)
            {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                stop_left_rotate(root,z->parent);
                stop_right_rotate(root,z->parent->parent);
            }

            // Right-Right (RR) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Left Rotate Grandparent
            if (z->parent == z->parent->parent->right &&
                z == z->parent->right)
            {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                stop_left_rotate(root,z->parent->parent);
            }

            // Right-Left (RL) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Right Rotate Parent
            // (iii) Left Rotate Grand Parent
            if (z->parent == z->parent->parent->right &&
                z == z->parent->left)
            {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                stop_right_rotate(root,z->parent);
                stop_left_rotate(root,z->parent->parent);
            }
        }
    }
    (*root)->color = 'B'; //keep root always black
}

// Utility function to insert newly node in RedBlack tree
void stop_insert(struct stop_node **root, char* data)
{
    // Allocate memory for new node
    struct stop_node *z = (struct stop_node*)malloc(sizeof(struct stop_node));
	stop_init_node(z, data);
     //if root is null make z as root
    if (*root == NULL)
    {
        z->color = 'B';
        (*root) = z;
    }
    else
    {
        struct stop_node *y = NULL;
        struct stop_node *x = (*root);

        // Follow standard BST insert steps to first insert the node
        while (x != NULL)
        {
            y = x;
            if (strcmp(z->stop, x->stop) < 0)
                x = x->left;
            else if (strcmp(z->stop, x->stop) > 0)
                x = x->right;
        }
        z->parent = y;
		if( y == NULL)
			*root = z;
        else if (strcmp(z->stop, y->stop) < 0)
            y->right = z;
        else if (strcmp(z->stop, y->stop) > 0)
            y->left = z;
        z->color = 'R';

        // call stop_insert_fixup to fix reb-black tree's property if it
        // is voilated due to insertion.
        stop_insert_fixup(root,z);
		free(x);
    }
}

// A utility function to traverse Red-Black tree in stop_inorder fashion
void stop_inorder(struct stop_node *root)
{
    if (root == NULL)
        return;
    stop_inorder(root->left);
    printf("%s \n", root->stop);
    stop_inorder(root->right);
}
int stop_search(char* search_id, struct stop_node** root)
{
    struct stop_node* current = *root;
    while(current != NULL)
    {
        if(strcmp(current->stop, search_id) == 0)
        {
            return 1;
        }
        if(strcmp(search_id, current->stop) < 0)
            current = current->left;
        else if(strcmp(search_id, current->stop) > 0)
            current = current->right;
        }
    return 0;
}
