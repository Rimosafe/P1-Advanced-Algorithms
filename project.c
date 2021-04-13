#include <stdio.h>
#include <stdlib.h>


typedef struct {
  char* text;
  char* pattern;
  char* occurrences;
  int text_size;
  int pattern_size;
  int kmp_count;
} StringMatch;

void operations_handler();
StringMatch* newStringMatching();
void freeStringMatching();
StringMatch* read_pattern();
StringMatch* read_text();
StringMatch* naive_search();
StringMatch* kmp_search();
int* compute_pi_table();


int main(){
  operations_handler();
  return 0;
}

StringMatch* newStringMatching() {
  int initial_size;
  StringMatch* str_match = malloc(sizeof(StringMatch));

  if(str_match == NULL)
    return NULL;

  initial_size = 1;

  str_match->text = malloc(initial_size*sizeof(char));
  if(str_match->text == NULL) {
    free(str_match);
    return NULL;
  }

  str_match->pattern = malloc(initial_size*sizeof(char));
  if(str_match->pattern == NULL) {
    free(str_match->text);
    free(str_match);
    return NULL;
  }

  str_match->occurrences = malloc(initial_size*sizeof(char));
  if(str_match->occurrences == NULL) {
    free(str_match->text);
    free(str_match->pattern);
    free(str_match);
    return NULL;
  }

  str_match->kmp_count = 0;
  str_match->text_size = initial_size;
  str_match->pattern_size = initial_size;

  return str_match;
}

void freeStringMatching(StringMatch* str_match) {
  free(str_match->text);
  free(str_match->pattern);
  free(str_match->occurrences);
  free(str_match);
}

void operations_handler() {

  StringMatch *s = newStringMatching();
  int c;

  while(EOF != (c = getchar())) {
    switch (c) {
      case 'T':
        c = getchar();
        s = read_text(s);
        break;

      case 'N':
        c = getchar();
        s = read_pattern(s);
        s = naive_search(s);
        printf("%s\n", s->occurrences);
        break;

      case 'K':
        c = getchar();
        s = read_pattern(s);
        s = kmp_search(s);
        printf("%s\n", s->occurrences);
        printf("%d\n", s->kmp_count);
        break;

      case 'B':
        c = getchar();
        s = read_pattern(s);
        break;

      case 'X':
        freeStringMatching(s);
        return;

      default:
        break;
    }
  }
}


StringMatch* read_text(StringMatch* s) {
  int i;
  int c;
  int size;
  i = 0;
  size = 1;
  c = getchar();

  while('\n' != c) {
    if(i == size){
      size = size * 2;
      s->text = realloc(s->text, size*sizeof(char));
    }

    s->text[i] = (char)c;
    i++;
    c = getchar();
  }

  if(i == size){
    size = size * 2;
    s->text = realloc(s->text, size*sizeof(char));
  }

  s->text[i] = '\0';
  s->text_size = i;

  return s;
}


StringMatch* read_pattern(StringMatch* s) {
  int i;
  int c;
  int size;
  i = 0;
  size = 1;
  c = getchar();

  while('\n' != c) {
    if(i == size){
      size = size * 2;
      s->pattern = realloc(s->pattern, size*sizeof(char));
    }

    s->pattern[i] = (char)c;
    i++;
    c = getchar();
  }

  if(i == size){
    size = size * 2;
    s->pattern = realloc(s->pattern, size*sizeof(char));
  }

  s->pattern[i] = '\0';
  s->pattern_size = i;

  return s;
}

StringMatch* resolve_output(StringMatch *s, int occurence, int index) {
  s->occurrences[index] = occurence + '0';
  s->occurrences[index + 1] = ' ';
  return s;
}


StringMatch* naive_search(StringMatch *s) {
  int i;
  int size;
  int count;
  count = 0;
  size = 1;

  for(i = 0; i < s->text_size; i++) {
    int j;
    j = 0;

    while (j < s->pattern_size && s->pattern[j] == s->text[i+j] && i + j < s->text_size) {
      j++;
    }

    if(j == s->pattern_size) {

      if(count + 1 >= size) {
        size = size * 2;
        s->occurrences = realloc(s->occurrences, size*sizeof(char));
      }

      s = resolve_output(s, i, count);
      count+=2;

    }
  }

  if(count == size){
    size = size * 2;
    s->occurrences = realloc(s->occurrences, size*sizeof(char));
  }

  s->occurrences[count] = '\0';
  return s;
}


StringMatch* kmp_search(StringMatch *s) {
  int initial_size;
  int *pi_table;
  int i;
  int j;
  int size;
  int count;

  initial_size = 1;
  pi_table = malloc(initial_size*sizeof(int));
  pi_table =  compute_pi_table(s, pi_table);

  count = 0;
  size = 1;
  i = 0;
  j = 0;

  while(i < s->text_size) {



    if(s->text[i] == s->pattern[j]) {
      i++;
      j++;
      s->kmp_count++;
    }

    if(j == s->pattern_size) {

      if(count + 1 >= size) {
        size = size * 2;
        s->occurrences = realloc(s->occurrences, size*sizeof(char));
      }

      s = resolve_output(s, i-j, count);
      count+=2;
      j = pi_table[j-1];

      /* Last text char coincide with last pattern char */
      if(i != s->text_size) s->kmp_count++;

    }

    else if(i < s->text_size && s->pattern[j] != s->text[i]){
      s->kmp_count++;

      if(j > 0){
        j = pi_table[j-1];
      }
      else i++;
    }
  }

  if(count == size){
    size = size * 2;
    s->occurrences = realloc(s->occurrences, size*sizeof(char));
  }

  s->occurrences[count] = '\0';
  return s;

}


int* compute_pi_table(StringMatch* s, int* pi_table) {

  int size;
  int i;
  int j;
  size = 2;
  i = 1;
  j = 0;

  pi_table[0] = j;

  pi_table = realloc(pi_table, size*sizeof(int));
  size = size * 2;

  while(i < s->pattern_size) {

    if(s->pattern[i] == s->pattern[j]) {

      if(i == size) {
        pi_table = realloc(pi_table, size*sizeof(int));
        size = size*2;
      }

      j++;
      pi_table[i] = j;
      i++;
    }

    else {

      if(j > 0) {
        if(i == size) {
          pi_table = realloc(pi_table, size*sizeof(int));
          size = size*2;
        }

        j = pi_table[j-1];
      }

      else{
        if(i == size) {
          pi_table = realloc(pi_table, size*sizeof(int));
          size = size*2;
        }

        pi_table[i] = 0;
        i++;
      }

    }

  }

  return pi_table;
}
