#define _XOPEN_SOURCE
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <ctype.h>
#include <crypt.h>
#include <unistd.h>
#include <string.h>

void checkCrypts(char*, int);
int compareCrypts(char*);



int compareCrypts(char* word){
  char passwords[11][29] = {
    "$1$ab$tYl84YhDM6bmCOuCusTKS.",
    "$1$ab$DC1ifnjzzu8Za5qEt96Kq0",
    "$1$ab$DqPMoPPboZ2HTH7RfiqQs.",
    "$1$ab$0I4CGceZU1wOu9PO3qn2p/",
    "$1$ab$FPyWVGc2x83IsQ7.q775k1",
    "$1$ab$T/Cabrtf2TgOYXhZFlRct/",
    "$1$ab$/rOLL5LFn/3ZIa2TFnN4G.",
    "$1$ab$N9t.xxEuc93HGa.twsZuP.",
    "$1$ab$Po1AuQSRCorWXHi8cIOhK/",
    "$1$ab$s2uqC2lMivvj9IX5PQ8KO/",
    "$1$ab$tThrUGNaCZBr224TYwxw2."
  };

  int i;
  for(i = 0; i <= 10; i++){
    char *crypted_word = (crypt(word, "$1$ab$"));
    if (strcmp(crypted_word, passwords[i]) == 0){
      printf("Matched %s with crypt %s to %s\n",
            word, crypted_word, passwords[i]);
    }
  }
  free(crypted_word);
}

void checkCrypts(char* word, int size){
  // Check to see if current word matches any
  compareCrypts(word);
  /* Create a new word that will be large enough to hold both a N digit prefix
  and N digit suffix for incrementing the word */
  char* prefix_buffered_word;
  char* suffix_buffered_word;
  int i, j, buffered_size, first_zero;
  int word_size = strlen(word);
  int ffix_size = size;

  /* Now create our new buffered word, with only enough size needed for
  both suffix and prefix */
  buffered_size = (word_size + ffix_size);
  prefix_buffered_word = malloc(buffered_size * sizeof(char));
  suffix_buffered_word = malloc(buffered_size * sizeof(char));
  for (i = 0; i < buffered_size; i++){
    prefix_buffered_word[i] = '0';
  }
  prefix_buffered_word[buffered_size] = '\0';
  memmove(prefix_buffered_word+ffix_size, word, word_size);
  memmove(suffix_buffered_word, word, word_size);
  for(i = word_size; i < buffered_size; i++){
    suffix_buffered_word[i] = '0';
  }
  printf("prefix word: %s, suffix word: %s\n", prefix_buffered_word, suffix_buffered_word);
}

int main(int argc, char **argv) {
  char* word = "fluffy";
  //checkCrypts(word, 2);
  compareCrypts("fluffy");
  return 0;
}
