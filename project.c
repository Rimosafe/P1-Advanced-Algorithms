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
StringMatch* read_text();
StringMatch* read_pattern();
void naive_search();
void kmp_search();
int* compute_pi_table();
int max();
char* reverse_string();
int* reverse_array();
int* z_algorithm();
int* bad_char_rule();
int* l_prime_array();
int* capital_l_prime_array();
int* good_suffix_rule();
void boyer_moore_search();

int main(){
  operations_handler();
  return 0;
}

void operations_handler(){

  StringMatch *s = newStringMatching();
  int c;

  while(EOF != (c = getchar())) {
    switch (c) {
      case 'T':
        c = getchar(); /*Read space*/
        s = read_text(s); /*Read text*/
        break;

      case 'N':
        c = getchar(); /*Read space*/
        s = read_pattern(s); /*Read pattern*/
        naive_search(s); /*Naive algorithm*/
        break;

      case 'K':
        c = getchar(); /*Read space*/
        s = read_pattern(s); /*Read pattern*/
        kmp_search(s); /*Knuth-Morris-Pratt algorithm*/
        break;

      case 'B':
        c = getchar(); /*Read space*/
        s = read_pattern(s); /*Read pattern*/
        boyer_moore_search(s); /*Boyer Moore algorithm*/
        break;

      case 'X':
        freeStringMatching(s);
        return;

      default:
        break;
    }
  }
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

StringMatch* read_text(StringMatch* s){
  int i;
  int c;
  int size;
  i = 0;
  size = 1;
  c = getchar(); /*First char of text*/

  while('\n' != c) {
    if(i == size){ /*Case where was reached the maximum space allocated. Resize array*/
      size = size * 2;
      s->text = realloc(s->text, size*sizeof(char));
    }

    s->text[i] = (char)c;
    i++;
    c = getchar();
  }

  if(i == size){ /*Finish reading but need to insert '\0'. In case of need, increase memory space to insert '\0'*/
    size = size * 2;
    s->text = realloc(s->text, size*sizeof(char));/*Increase memory space to insert '\0'*/
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
  c = getchar(); /*First char of pattern*/

  while('\n' != c) {
    if(i == size){ /*Case where was reached the maximum space allocated. Resize array*/
      size = size * 2;
      s->pattern = realloc(s->pattern, size*sizeof(char));
    }

    s->pattern[i] = (char)c;
    i++;
    c = getchar();
  }

  if(i == size){ /*Finish reading but need to insert '\0'. In case of need, increase memory space to insert '\0'*/
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

    if(j == s->pattern_size) { /*Match found*/
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
  pi_table =  compute_pi_table(s, pi_table); /*Preprocessing*/

  while(i < s->text_size) {
    if(s->text[i] == s->pattern[j]) { /*Character match*/
      i++;
      j++;
      kmp_count++;
    }

    if(j == s->pattern_size) { /*Match found*/
      printf("%d ", i-j);
      j = pi_table[j-1];

      if(i != s->text_size) kmp_count++; /* Last text char coincide with last pattern char */

    }

    else if(i < s->text_size && s->pattern[j] != s->text[i]){ /*Mismatch found*/
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
    if(s->pattern[i] == s->pattern[j]){ /*Match*/

      if(i == size){ /*Case where was reached the maximum space allocated. Resize array*/
        size = size*2;
        pi_table = realloc(pi_table, size*sizeof(int));
      }

      j++;
      pi_table[i] = j;
      i++;
    }

    else{ /*Mismatch*/

      if(j > 0){
        if(i == size){ /*Case where was reached the maximum space allocated. Resize array*/
          size = size*2;
          pi_table = realloc(pi_table, size*sizeof(int));
        }

        j = pi_table[j-1];
      }

      else{ /*Immediate mismatch*/

        if(i == size){ /*Case where was reached the maximum space allocated. Resize array*/
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

int* reverse_array(int size, int* array, int* reverse){
  int i, j;

  j = size - 1;

  for(i = 0; i < size; i++) {
      reverse[i] = array[j];
      j--;
  }

  return reverse;
}

int* z_algorithm(StringMatch* s, int* z_array){

  int i;
  int j;
  int left;
  int right;
  char* reverse;

  right = 0;
  left = 0;

  reverse = malloc((s->pattern_size+1)*sizeof(char));
  reverse = reverse_string(s, reverse);

  z_array[0] = s->pattern_size;
  for(i = 0; i < s->pattern_size - 1; i++) {
    z_array[i] = 0;
  }

  for(i = 1; i < s->pattern_size; i++){

    if(i > right){ /*Outside Z-box*/
      left = i;
      right = i;

      while(right < s->pattern_size && reverse[right-left] == reverse[right]) right++;

      z_array[i] = right - left;
      right--;
    }

    else{ /*Inside Z-box*/
      j = i - left;

      if(z_array[j] < right - i + 1) z_array[i] = z_array[j]; /* Use values that are already computed*/

      else{
        left = i;

        while(right < s->pattern_size && reverse[right-left] == reverse[right]) right++;

        z_array[i] = right - left;
        right--;
      }
    }
  }

  free(reverse);

  return z_array;
}

int* bad_char_rule(StringMatch* s, int* bc_table){

  int i;
  int a_size;

  a_size = 128;

  for(i = 0; i < a_size; i++){ /*Set all occurences to -1*/
    bc_table[i] = -1;
  }

  for(i = 0; i < s->pattern_size; i++){ /*Set value to the last occurence of the character*/
    bc_table[(int)s->pattern[i]] = i;
  }

  return bc_table;
}

int* l_prime_array(StringMatch* s, int* lp, int* n_table){

  int i;

  for(i = 0; i < s->pattern_size; i++){
    lp[i] = 0;
  }

  for(i = 0; i < s->pattern_size; i++){
    if(n_table[i] == i + 1) lp[s->pattern_size - i - 1] = i + 1;
  }

  for(i = s->pattern_size - 2; i >= 0; i--) {
    if(lp[i] == 0) lp[i] = lp[i+1];
  }
  return lp;
}

int* capital_l_prime_array(StringMatch* s, int* clp, int* n_table){

  int i, j;

  for(i = 0; i < s->pattern_size; i++){
    clp[i] = 0;
  }

  for(i = 0; i < s->pattern_size - 1; i++){
    j = s->pattern_size - n_table[i];
    if(j < s->pattern_size) clp[j] = i + 1;
  }
  return clp;
}

int* good_suffix_rule(StringMatch *s, int* gs_table, int* llp,  int* clp){

  int i;

  for(i = 0; i < s->pattern_size; i++){
    if(clp[i] > 0) gs_table[i] = s->pattern_size - clp[i];
    else gs_table[i] = s->pattern_size - llp[i];
  }

  return gs_table;
}

void boyer_moore_search(StringMatch *s){
  int a_size;
  int *z_values, *n_table, *bc_table, *l_prime, *capital_l_prime, *gs_table;
  int i, j, comp, bc_shift, gs_shift, shift;

  comp = 0;
  a_size = 128;

  bc_table = malloc(a_size*sizeof(int));
  gs_table = malloc(s->pattern_size*sizeof(int));
  z_values = malloc(s->pattern_size*sizeof(int));
  n_table = malloc(s->pattern_size*sizeof(int));
  l_prime = malloc(s->pattern_size*sizeof(int));
  capital_l_prime = malloc(s->pattern_size*sizeof(int));

  bc_table = bad_char_rule(s, bc_table); /*Bad character table*/
  z_values = z_algorithm(s, z_values); /*Z values computed from reversed pattern*/
  n_table = reverse_array(s->pattern_size, z_values, n_table); /*N values*/
  l_prime = l_prime_array(s, l_prime, n_table); /* l*(i) */
  capital_l_prime = capital_l_prime_array(s, capital_l_prime, n_table); /* L*(i) */
  gs_table = good_suffix_rule(s, gs_table, l_prime, capital_l_prime); /*Strong good suffix table*/

  i = 0;

  while(i <= s->text_size - s->pattern_size) {

    j = s->pattern_size - 1;
    shift = 1;

    while(j >= 0 && s->pattern[j] == s->text[i+j]) { /*While characters of pattern and text are matching*/
      comp++;
      j--;
    }

    if(j < 0){ /*Match found*/
      printf("%d ", i);
      shift = gs_table[0];
    }

    else { /*Mismatch found*/
      comp++;
      bc_shift = max(shift, j - bc_table[(int)s->text[i + j]]);
      if(j < s->pattern_size - 1) {
        gs_shift = max(shift, gs_table[j+1]);
      }
      else{ /*First comparison mismatch*/
        gs_shift = 1;
      }
      shift = max(bc_shift, gs_shift);
    }
    i += shift;
  }

  printf("\n%d \n", comp);

  free(bc_table);
  free(gs_table);
  free(z_values);
  free(n_table);
  free(l_prime);
  free(capital_l_prime);
}
