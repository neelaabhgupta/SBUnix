#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<cstring.h>


uint64_t stoi(char* s)
{
	uint64_t i = 0;
	uint64_t l = 0;
	while(1)
	{
		if(s[l] == '\0')
		{
			break;
		}
		i = i*10 + (s[l] - '0');
		l++;
	}
	return i;
}

int main(int argc, char *argv[], char *envp[]) 
{	
	clrscr();
	//puts("sbush> ");
	#if 0
	size_t size = 4096;
	uint64_t *address = (uint64_t*)malloc(size);
	*(address + 511) = 1;
	free((void*)address);
	free((void*)address);
	//*(address + 511) = 1;
	uint64_t *address1 = (uint64_t*)malloc(size);
	*(address1 + 511) = 1;
	free((void*)address1);
	free((void*)address1);
	//int d = 5;
	char a[128] = {'a'};
	write(1, a, 128);
	printf("Hello First Printf Run \n");
	#endif
	#if 0
	printf("#####################----Welcome!----#############################\n");
	char *file = "bin/ls";
	execvpe(file, NULL, NULL);
	execvpe("bin/ps", NULL, NULL);
	//execvpe("bin/sleep", NULL,NULL);
	char command[512];
	//char token1[128];
	//char token2[128];
	//char token3[128];
	printf("SBUNIX:>  \ndsgygcydg");
	//char *str = "Hello\n";
	//write(1, command, 512);
	#endif
	#if 1

	printf("SBUNIX:> ");
	
	//command
	//int m=0;
	while(1){
		char command[512];
		int ret=0;
		while(1){
			ret=read(0, command, 512);
			if(ret){
				break;
			}
		}
		//printf("%s",command);
		int flag = 0;
		for(int i = 0; i < 512; i++)
		{
			if(command[i] != '\0')
			{
				flag = 1;
				break;
			}
		}
		if(flag)
			parser(command);
		for(int i=0;i<512;i++)
		{
			command[i]='\0';
		}
	}
	#endif
	
	//#endif
	#if 0
	char* string1 = "Parent//";
	char* string2 = "Child";
	pid_t pid = fork();
	if(pid == 0)
	{
		write(0, string2, 20);
	}
	else
	{
		write(0, string1, 20);	
		uint64_t syscall_num = 3;
		__asm__ volatile("mov %0, %%r12;\n"
			"int $0x80;\n"
			:
			:"g"(syscall_num)
			);
	}
	#endif
	while(1);
	return 0;
}
void parser(char command[]){
		char token1[128];
		char token2[128];
		char token3[128];
		#if 1
		int t = 0;
		int flag1=1;
		int flag2=0;
		int flag3=0;
		int i=0;
		int j=0;
		if(flag1 && !flag2 && !flag3 ){
			for(i=0; i<512; i++){
				if(flag1==1 && command[i]=='\0'){
					flag2=1;
					flag1=0;
					token1[t]='\0';
					break;
				}
				if(flag1==1 && command[i]==' '){
					flag2=1;
					flag1=0;
					token1[t]='\0';
					break;
				}
				if(flag1==1){
					token1[t] = command[i];
					t++;
				}

			}
		}
		j=i+1;
		t=0;
		if(!flag1 && flag2 && !flag3){
			for(i=j; i<512; i++){
				if(flag2==1 && command[i]=='\0'){
					flag3=1;
					flag2=0;
					token2[t]='\0';
					break;
				}
				if(flag2==1 && command[i]==' '){
					flag3=1;
					flag2=0;
					token2[t]='\0';
					break;
				}
				if(flag2==1){
					token2[t] = command[i];
					t++;
				}

			}

		}
		j=i+1;
		t=0;
		if(!flag1 && !flag2 && flag3){
			for(i=j; i<512; i++){
				if(flag3==1 && command[i]=='\0'){
					flag3=0;
					token3[t]='\0';
					break;
				}
				if(flag3==1 && command[i]==' '){
					flag3=0;
					token3[t]='\0';
					break;
				}
				if(flag3==1){
					token3[t] = command[i];
					t++;
				}

			}

		}
		token1[0]=token1[0];
		token2[0]=token2[0];
		token3[0]=token3[0];
		if(!mystrcmp(token1,"cd")){
			//printf("cd and now pass token 2 to cd binary");
			//set_arg_path(token2);
			//execvpe("bin/cd", NULL, NULL);
		}
		else if(!mystrcmp(token1,"ls")){
			//printf("ls and now pass token 2 to ls binary");
		}
		else if(!mystrcmp(token1,"kill")){
			if (!mystrcmp(token2,"-9")){	
				//printf("kill -9 and now pass token 2 to kill binary");
				set_arg_val(stoi(token3));
				//kill();
				execvpe("bin/kill", NULL, NULL);
			}
			
		}
		else if(!mystrcmp(token1,"sleep")){
			
			//printf("sleep and now pass token 2 to sleep binary");
			set_arg_val(stoi(token2));
			execvpe("bin/sleep", NULL, NULL);
			
		}
		else if(!mystrcmp(token1,"ps")){
			
			//printf("ps and call ps sleep binary");
			execvpe("bin/ps", NULL, NULL);
			//getpid();
			//getppid();
			
		}
		else if(!mystrcmp(token1,"cat")){
			
			//printf("cat and now pass token 2 to cat binary");
			
		}
		else if(!mystrcmp(token1,"echo")){
			
			//printf("echo and now pass token 2 to echo binary");
			set_arg_path(token2);
			//echo();
			execvpe("bin/echo", NULL, NULL);
			
		}
		else if(!mystrcmp(token1, "clear")){
			clrscr();
		}
		/*else{
			printf("Command not Supported\n");
		}*/
		/*printf("%s ",token1);
		printf("%s ",token2);
		printf("%s ",token3);*/
		//write(1,"\n",1);
		for(int i=0;i<128;i++){
			token1[i]=token2[i]=token3[i]='\0';
		}
		//printf("SBUNIX:> ");
		#endif
		
	}


