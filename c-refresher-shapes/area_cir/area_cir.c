#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  int radius = 0;
  float area = 0;

  if (argc != 2) {
    printf("You must supply 1 argument! (radius)\n");
    return 0;
  } else {
    radius = atoi(argv[1]);
    area = 3.14 * (radius*radius);
    printf("Area of circle = %.2f\n", area);
    return 1;
  }
}
