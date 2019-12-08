// C program for Red-Black Tree insertion
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<mpi.h>
#include "rbtree.c"

//don't add stopwords to tree
// * search return article id's
// * after nodes return search results, root node has to parse the author lists and titles
//parse the citations
// * filter the subgraphs
//  pagerank

#define NB_ELEMS 10

int convIntToStr(char * str, int x){
    sprintf(str, "%d", x);
    return (strlen(str));
}

void find_offset(int rank, int num_nodes, MPI_File* file, int* chunk)
{//sets the offset of file to where the node should start
    //and returns the length of how many bytes it should read
    MPI_File_seek(*file, 0, MPI_SEEK_END);
    MPI_Offset file_len;
    MPI_File_get_size(*file, &file_len);
    MPI_Offset starting_point = (file_len / num_nodes) * rank;
    MPI_Offset ending_point = (file_len / num_nodes) * (rank + 1) -1;

    MPI_Status status;
    char c = ' ';
    int num_plusi = 0;
    //printf("Node#%d starting at %d\n", rank, starting_point);
    MPI_File_seek(*file, starting_point, MPI_SEEK_SET);
    //printf("%d start: %d\N", rank, starting_point);
    int nbytes = 1;
    if(rank != 0)
    {
        while( num_plusi < 6 && c != '\0' && nbytes >0)
        {
            // if(rank == 3)
            // printf("%d: %c\n", rank, c);
            if(c=='+')
            num_plusi++;
            else
            num_plusi = 0;
            //MPI_File_seek(file, -2, MPI_SEEK_CUR);
            nbytes = MPI_File_read(*file, &c, 1, MPI_CHAR, &status);
            //printf("\tNode #%d s reading char %c\n", rank, c);
        }
        MPI_File_get_position(*file, &starting_point);
        //MPI_File_seek(*file, starting_point+ 1, MPI_SEEK_SET);
        //    printf("\n\noffset before seek is %d\n", starting_point);
        //MPI_File_get_position(*file, &starting_point);
        //printf("offset after seek is %d\n\n", starting_point);
    }
    //printf("Node#%d starting at %d\n", rank, starting_point);
    nbytes = num_plusi = 0;
    c = ' ';
    MPI_File_seek(*file, ending_point, MPI_SEEK_SET);
    while( num_plusi < 6 && c != '\0' && nbytes > 0)
    {
        // if(rank == 3)
        // printf("%d: %c\n", rank, c);
        if(c=='+')
        num_plusi++;
        else
        num_plusi = 0;
        nbytes = MPI_File_read(*file, &c, 1, MPI_CHAR, &status);
        //printf("\tNode #%d e reading char %c\n", rank, c);
    }
    //MPI_File_seek(*file, 1, MPI_SEEK_CUR);
    MPI_File_get_position(*file, &ending_point);

    // if(rank == num_nodes)
    //     ending_point = file_len;
    // printf("end %d, file len : %d\n",ending_point, file_len);
    chunk[0] = starting_point + 1;
    chunk[1] = ending_point + 1;
}

// MPI_Status status;
//
// MPI_Offset words_pointer_pos = 0;
// //rewind(words_fp);
//
// pass_num = 0;
// while (words_pointer_pos <= endpoint)
// {
//     MPI_File_get_position(mpi_words_file, &words_pointer_pos);
//     c = ' ';
//     i = 10;
//     while(c != '\n' && c != 0)
//     {
//         MPI_File_read(mpi_words_file, &c, 1, MPI_CHAR, &status);
//         word[i++] = c;
//     }
//     //MPI_File_seek(mpi_words_file, 1, MPI_SEEK_CUR);
//     word[i] = 0;
//     int word_len = strlen(word+ 10) - 1;
//     word[word_len+ 10] = 0;
//     //printf("checking %s\n", word+10);
//     // print_char_vals(word+10);
//     strcpy(word2, word+10);
//     //int word_len = strlen(word2);
//
//     word_num++;

void read_stopwords(struct stop_node* root, char* stopwords_file_name)
{
    MPI_Comm world = MPI_COMM_WORLD;
    int rank, world_size;
    MPI_Comm_rank(world, &rank);
    MPI_Comm_size(world, &world_size);

    MPI_File mpi_words_file;
    MPI_File_open(
        world,
        stopwords_file_name,
        MPI_MODE_RDONLY,
        MPI_INFO_NULL,
        &mpi_words_file
    );

    //FILE* test = fopen("test_offset.txt", "r");
    int chunk[2];
    find_offset(rank, world_size, &mpi_words_file, chunk);
    //printf("node #%d start %d | end %d\n", rank, chunk[0], chunk[1]);

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(stopwords_file_name, "r");
    //fp = fopen("./arxiv/arxiv-metadata.txt", "r");

    if (fp == NULL){
        printf("Stopwords file not found\n");
        exit(EXIT_FAILURE);
    }
    fseek(fp, chunk[0], SEEK_SET);
    while ((read = getline(&line, &len, fp)) != -1 && ftell(fp) < chunk[1]) {
        stop_insert(&root, line);
    }
}

int metadataInsertion(struct word_node* root, char* meta_file)
{
    MPI_Comm world = MPI_COMM_WORLD;
    int rank, world_size;
    MPI_Comm_rank(world, &rank);
    MPI_Comm_size(world, &world_size);

    MPI_File mpi_words_file;
    MPI_File_open(
        world,
        meta_file,
        MPI_MODE_RDONLY,
        MPI_INFO_NULL,
        &mpi_words_file
    );

    //FILE* test = fopen("test_offset.txt", "r");
    int chunk[2];
    find_offset(rank, world_size, &mpi_words_file, chunk);
    printf("node #%d start %d | end %d\n", rank, chunk[0], chunk[1]);

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(meta_file, "r");
    //fp = fopen("./arxiv/arxiv-metadata.txt", "r");

    if (fp == NULL){
        printf("meta file not found\n");
        exit(EXIT_FAILURE);
    }
    fseek(fp, chunk[0], SEEK_SET);
    while ((read = getline(&line, &len, fp)) != -1 && ftell(fp) < chunk[1]) {
        printf("Line: %s", line);
        int idLength = strlen(line)+1;
        char *id = malloc(idLength * sizeof(char));
        if (line[0] != '+'){
            int file_offset = ftell(fp);
            strcpy(id, line);
            id[idLength-1] = '\0';
            struct arxivArticle* article = malloc(sizeof(struct arxivArticle));
            initArxivArticle(article, id, file_offset);

            //don't neccessarily to copy all of the lines, but it's not a super big deal
            getline(&line, &len, fp);
            getline(&line, &len, fp);
            getline(&line, &len, fp);

            int i;
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
                    printf("%s has %d articles\n", insertWord, new_word->sub_root->size);
                    //printf("\ninorder:\n");
                    // }
                    free(insertWord);
                }
                offset = i;
            }
            //puts("\n\n\n");
            //word_inorder(root);
            free(article->id);
            free(article);
            //sleep(1);
        }
        free(id);

    }

    //word_inorder(root);
    fclose(fp);
    if (line){
        free(line);
    }
    //exit(EXIT_SUCCESS);
    return 0;
}


/* Drier program to test above function*/
int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);
    MPI_Comm world = MPI_COMM_WORLD;
    int rank, world_size;
    MPI_Comm_rank(world, &rank);
    MPI_Comm_size(world, &world_size);


    umask(0);
    struct word_node *cthulu_tree = NULL;

    printf("proccessing search index ...\n");
    char meta_file[100] = "./arxiv/shortened-arxiv-metadata.txt\0";
    metadataInsertion(cthulu_tree, meta_file);
    if(rank == 0)
    printf("Welcome to the muthah fukin game beech \n -1 to quit\n");
    int done = 0;
    while(!done)
    {
        char* input = malloc(100 * sizeof(char));
        if(rank == 0){
            printf("Search: ");
            scanf("%s", input);
        }
        MPI_Bcast(
            input,
            strlen(input),
            MPI_CHAR,
            0,
            world);
        printf("Node #%d: Searching for %s\n",rank, input);
        struct word_node* ret;
        word_search(input, cthulu_tree, ret);
        printf("return word%s\n", ret->word);
        char** list = malloc(ret->sub_root->size * sizeof(char));
        article_inorder_list(ret->sub_root, list);
        int i;
        for(i = 0; i < 10 ; i++)
            printf("%s, ", list[i]);
        if(strcmp(input, "-1") == 0)
            done = 1;
            //search_results
    }
    MPI_Finalize();
    return 0;
}
