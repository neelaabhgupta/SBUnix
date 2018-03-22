#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[], char *envp[])
{
	while(1)
	{
		if(sleep(0) == 1)
		{
			break;
		}
	}
	//write(1, "SBUNIX:>   ", 20);
	exit(1);
	return 0;
}