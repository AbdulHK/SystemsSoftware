#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  int base = 0;
  int height = 0;
  float area = 0;

  if (argc != 3) {
    printf("You must supply 2 arguments! (base and height)\n");
    return 0;
  } else {
    base = atoi(argv[1]);
    height = atoi(argv[2]);
    area = (base * height)/2;
    printf("Area of rectangle = %.2f\n", area);
    return 1;
  }
}
