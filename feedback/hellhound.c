
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

void error(char *msg){
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(1);
}

int main(int argc, char* argv[])
{
	char *phrase = argv[1];
	char *vars[] = {"RSS_FEED=http://www.spiegel.de/international/index.rss", NULL};
	FILE *f = fopen("stories.txt", "w");
	if(!f) {
		error("Can't open stories.txt");
	}

	pid_t pid = fork();
	if (pid == -1) {
		error("Can't fork process");
	}

	if(!pid) {
		if(dup2(fileno(f), 1) == -1) {
			error("Can't redirect Standard Output");
		}
		if (execle("/usr/bin/python", "/usr/bin/python", "./dogriffiths-rssgossip-f0bb346/rssgossip.py", phrase, NULL, vars) == -1) {
			error("Can't run script");
		}
	}

	int pid_status;
	if(waitpid(pid, &pid_status, 0) == -1) {
		error("Error waiting for child process");
	}

	return 0;
}