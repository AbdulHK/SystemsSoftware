#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* dec_to_roman(int input) {
  static char roman_value[8] = "\0";
  int i = 12;
  int decimal[] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
  char *symbol[] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D",
    "CM", "M"};  
    
  while(input != 0) {
    while(input/decimal[i]) {
      strcat(roman_value, symbol[i]);
      input -= decimal[i];
    }
    i--;
  }
  
  return roman_value;
}

void write_to_file(char* roman_value) {
  FILE *file = fopen("data.txt", "w");
  
  if (file == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }
  
  fprintf(file, "%s", roman_value);
  
  fclose(file);
}

int main() {
  write_to_file(dec_to_roman(2012));
  return 0;
}
