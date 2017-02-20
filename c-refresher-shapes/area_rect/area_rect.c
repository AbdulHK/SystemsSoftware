#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  int width = 0;
  int length = 0;
  int area = 0;

  if (argc != 3) {
    printf("You must supply 2 arguments (widht and length)!\n");
    return 0;
  } else {
    width = atoi(argv[1]);
    length = atoi(argv[2]);
    area = width * length;
    printf("Area of rectangle = %d\n", area);
    return 1;
  }
}
