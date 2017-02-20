/**
 * Program allowing for computation of area of some shapes.
 *  
 * Compile using following command:
 * gcc main.c area_rect.c area_cir.c area_tri.c -o bin/main
 * 
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "area_rect.h"
#include "area_cir.h"
#include "area_tri.h"

int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 4) {
    printf("Wrong number of arguments\n");
    return 0;
  }
  
  if (strcmp(argv[1], "rect") == 0) {
    if (argc == 4) {
      int width = atoi(argv[2]);
      int length = atoi(argv[3]);
      printf("Area rectangle: %.2f\n", area_rect(width, length));
    } else {
      printf ("Incorrect number of arguments (width, length)\n");
    }
  } else if(strcmp(argv[1], "cir") == 0) {
    if (argc == 3) {
      int radius = atoi(argv[2]);
      printf("Area circle: %.2f\n", area_cir(radius));
    } else {
      printf ("Incorrect number of arguments (radius)\n");
    }
  } else if(strcmp(argv[1], "tri") == 0) {
    if (argc == 4) {
      int base = atoi(argv[2]);
      int height = atoi(argv[3]);
      printf("Area triangle: %.2f\n", area_tri(base, height));
    } else {
      printf ("Incorrect number of arguments (base, height)\n");
    }
  } else {
    printf("Error! No matching shape\n");
  }
  return 0;
}
