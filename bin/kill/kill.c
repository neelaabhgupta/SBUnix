#include<stdlib.h>
#include<signal.h>

int main(int argc, char *argv[], char *envp[])
{
	kill(0, 1);
	//write(1, "SBUNIX:>   ", 20);
	exit(0);
}