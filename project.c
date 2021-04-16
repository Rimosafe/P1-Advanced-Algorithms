#include <stdio.h>
#include <stdlib.h>


typedef struct {
  char* text;
  char* pattern;
  int text_size;
  int pattern_size;
} StringMatch;

void operations_handler();
StringMatch* newStringMatching();
void freeStringMatching();
StringMatch* read_pattern();
StringMatch* read_text();
void naive_search();
void kmp_search();
int* compute_pi_table();
void boyer_moore_search();
int* pre_process_bad_char_rule();
int* z_values();
int max();


int main(){
  operations_handler();
  return 0;
}


StringMatch* newStringMatching(){
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


  str_match->text_size = initial_size;
  str_match->pattern_size = initial_size;

  return str_match;
}


void freeStringMatching(StringMatch* str_match){
  free(str_match->text);
  free(str_match->pattern);
  free(str_match);
}


void operations_handler(){

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
        naive_search(s);
        break;

      case 'K':
        c = getchar();
        s = read_pattern(s);
        kmp_search(s);
        break;

      case 'B':
        c = getchar();
        s = read_pattern(s);
        boyer_moore_search(s);
        break;

      case 'X':
        freeStringMatching(s);
        return;

      default:
        break;
    }
  }
}


StringMatch* read_text(StringMatch* s){
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


StringMatch* read_pattern(StringMatch* s){
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


void naive_search(StringMatch *s){
  int i;

  for(i = 0; i < s->text_size; i++) {
    int j;
    j = 0;

    while (j < s->pattern_size && s->pattern[j] == s->text[i+j] && i + j < s->text_size) {
      j++;
    }

    if(j == s->pattern_size) {
      printf("%d ", i);
    }
  }

  printf("\n");
}


void kmp_search(StringMatch *s){
  int initial_size;
  int *pi_table;
  int kmp_count;
  int i;
  int j;

  initial_size = 1;
  kmp_count = 0;
  i = 0;
  j = 0;

  pi_table = malloc(initial_size*sizeof(int));
  pi_table =  compute_pi_table(s, pi_table);

  while(i < s->text_size) {
    if(s->text[i] == s->pattern[j]) {
      i++;
      j++;
      kmp_count++;
    }

    if(j == s->pattern_size) {
      printf("%d ", i-j);
      j = pi_table[j-1];
      /* Last text char coincide with last pattern char */
      if(i != s->text_size) kmp_count++;
    }

    else if(i < s->text_size && s->pattern[j] != s->text[i]){
      kmp_count++;

      if(j > 0){
        j = pi_table[j-1];
      }
      else i++;
    }
  }

  printf("\n%d \n", kmp_count);
  free(pi_table);
}


int* compute_pi_table(StringMatch* s, int* pi_table){
  int size;
  int i;
  int j;
  size = 1;
  i = 1;
  j = 0;


  pi_table[0] = j;

  while(i < s->pattern_size){
    if(s->pattern[i] == s->pattern[j]){

      if(i == size){
        size = size*2;
        pi_table = realloc(pi_table, size*sizeof(int));
      }

      j++;
      pi_table[i] = j;
      i++;
    }

    else{

      if(j > 0){
        if(i == size){
          size = size*2;
          pi_table = realloc(pi_table, size*sizeof(int));
        }

        j = pi_table[j-1];
      }

      else{

        if(i == size){
          size = size*2;
          pi_table = realloc(pi_table, size*sizeof(int));
        }

        pi_table[i] = 0;
        i++;
      }
    }
  }
  return pi_table;
}


int max(int a, int b){

  return (a > b) ? a : b;
}


char* reverse_string(StringMatch* s, char* reverse){

  int i, j;

  j = s->pattern_size - 1;

  for(i = 0; i < s->pattern_size; i++) {
      reverse[i] = s->pattern[j];
      j--;
  }

  reverse[s->pattern_size] = '\0';

  return reverse;
}


int* z_algorithm(StringMatch* s, int* sgs_table){

  int i;
  int j;
  int left;
  int right;
  char* reverse;

  right = 0;
  left = 0;

  reverse = malloc((s->pattern_size+1)*sizeof(char));
  reverse = reverse_string(s, reverse);

  for(i = 1; i < s->pattern_size; i++){

    if(i > right){
      left = i;
      right = i;

      while(right < s->pattern_size && reverse[right-left] == reverse[right]) right++;

      sgs_table[s->pattern_size - i - 1] = right - left;
      right--;
    }

    else{
      j = i - left;

      if(sgs_table[j] < right - i - 1) sgs_table[i] = sgs_table[s->pattern_size - j - 1];

      else{
        left = i;

        while(right < s->pattern_size && reverse[right-left] == reverse[right]) right++;

        sgs_table[s->pattern_size - i - 1] = right - left;
        right--;
      }
    }
  }

  free(reverse);
  return sgs_table;
}


int* pre_process_bad_char_rule(StringMatch* s, int* bc_table){

  int i;
  int a_size;

  a_size = 128;

  for(i = 0; i < a_size; i++){
    bc_table[i] = s->pattern_size;
  }

  for(i = 0; i < s->pattern_size; i++){
    bc_table[(int)s->pattern[i]] = max(1, s->pattern_size - i - 1);
  }

  return bc_table;
}

int* pre_process_good_suffix_rule(StringMatch* s, int* gs_table, int* z_values){
  return gs_table;
}

void boyer_moore_search(StringMatch *s){
  int a_size;
  int *z_values;
  int *bc_table;
  int *gs_table;
  int i;
  int j;
  int comp;

  comp = 0;
  a_size = 128;

  z_values = malloc(s->pattern_size*sizeof(int));
  gs_table = malloc(s->pattern_size*sizeof(int));
  bc_table = malloc(a_size*sizeof(int));

  z_values[s->pattern_size - 1] = s->pattern_size;
  for(i = 0; i < s->pattern_size - 1; i++) {
    z_values[i] = 0;
  }

  for(i = 1; i < s->pattern_size; i++) {
    gs_table[i] = s->pattern_size;
  }

  pre_process_bad_char_rule(s, bc_table);
  z_algorithm(s, z_values);
  pre_process_good_suffix_rule(s, gs_table, z_values);

  printf("\n");
  for(i = 0; i < s->pattern_size; i++) {
    printf("%d ", i);
  }
  printf("\n");
  for(i = 0; i < s->pattern_size; i++) {
    printf("%c ", s->pattern[i]);
  }
  printf("\n");
  for(i = 0; i < s->pattern_size; i++) {
    printf("%d ", z_values[i]);
  }
  printf("\n");

  i = 0;

  /*while(i < s->text_size - s->pattern_size) {

    j = s->pattern_size - 1;


    while(j >= 0 && s->pattern[j] == s->text[i+j]) {
      printf("comparing %c with %c\n", s->pattern[j], s->text[i+j]);
      comp++;
      j--;
    }

    if(j < 0){
      printf("%d ", i);
      i += max(sgs_table[0], bc_table[(int)s->text[i + s->pattern_size - 1]]);
    }

    else {
      printf("comparing %c with %c\n", s->pattern[j], s->text[i+j]);
      comp++;
      i += max(sgs_table[s->pattern_size - j + 1], j - bc_table[(int)s->text[i + j]]);
    }

  }*/

  printf("\n%d \n", comp);
  free(gs_table);
  free(z_values);
  free(bc_table);

}
