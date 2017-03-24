CC=gcc -g
objects = main.o routes.o changes.o backup.o transfer.o queue.o
headers = main.c routes.h changes.h backup.h transfer.h queue.h

myprog : $(objects)
				$(CC) -o main $(objects) -lm -lrt

main.o : main.c $(headers)
				$(CC) -c main.c -lrt
								
routes.o : routes.c
				$(CC) -c routes.c -lm

changes.o : changes.c
				$(CC) -c changes.c -lm
						
backup.o : backup.c
				$(CC) -c backup.c -lm

transfer.o : transfer.c
				$(CC) -c transfer.c -lm
				
queue.o : queue.c
				$(CC) -c queue.c -lm

clean:
				rm  $(objects)
