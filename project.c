#include <stdio.h>
#include <stdlib.h>


typedef struct {
  char* text;
  char* pattern;
  char* occurrences;
  int text_size;
  int pattern_size;
} StringMatch;

void operations_handler();
StringMatch* newStringMatching();
void freeStringMatching();
StringMatch* read_pattern();
StringMatch* read_text();
StringMatch* naive_search();


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
