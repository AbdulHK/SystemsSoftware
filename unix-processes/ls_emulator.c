#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main( int argc, char *argv[] )  {

	int link[2];
	pid_t pid;
	char foo[4096];

	if (pipe(link)==-1) {
		exit(27);
	}

	if ((pid = fork()) == -1) {
		exit(27);
	}

	if(pid == 0) {

		dup2 (link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		execl("/bin/ls", "ls", "-1", (char *)0);
		exit(27);

	} else {

		close(link[1]);

		int nbytes = read(link[0], foo, sizeof(foo));

		if ( argc == 2 ) {
			printf("Finding... %s\n",argv[1]);

			for(int i = 0; i < sizeof(foo); i++)  {
				if(strcmp(argv[1], &foo[i]) == 0 ) {
					printf("Found file: %c\n", foo[i]);
				}
			}
			wait(NULL);

		} else {

			printf("Output: (%.*s)\n", nbytes, foo);
			wait(NULL);
		}

	}
	return 0;
}
