CC=gcc -g
objects = main.o areaOfRectangle.o areaOfSquare.o areaOfCircle.o area_of_triangle.o
headers = main.c areaOfRectangle.h areaOfSquare.h areaOfCircle.h area_of_triangle.h

myprog : $(objects)
				$(CC) -o areaProg $(objects) -lm

main.o : main.c $(headers)
				$(CC) -c main.c

areaOfRectangle.o : areaOfRectangle.c
				$(CC) -c areaOfRectangle.c -lm

areaOfSquare.o : areaOfSquare.c
				$(CC) -c areaOfSquare.c -lm

areaOfCircle.o : areaOfCircle.c
				$(CC) -c areaOfCircle.c -lm

areaOfTriangle : area_of_triangle.c
				$(CC) -c area_of_triangle.c -lm

clean:
				rm areaProg $(objects)
