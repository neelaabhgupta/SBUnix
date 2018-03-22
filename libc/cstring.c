#include<cstring.h>
#include<unistd.h>
#include<sys/init_pd.h>

size_t strlen(char* input)
{
    int i = 0;
    while(input[i++] != '\0'){}
    return i-1;
}

int mystrcmp(char *dest, char *src)
{
    int i = 0;
    int flag = 0;
    while(dest[i] != '\0' || src[i] != '\0')
    {
        if(dest[i] == src[i])
        {
            i++;
            continue;
        }
        else
        {
            flag = 1;
            return flag;            
        }
    }
    return 0;
}

void strcopy(char* source, char* destination)
{
	int i = 0;
	while(source[i] != '\0')
	{
		destination[i] = source[i];
		i++;
	}
	destination[i] = '\0';
}

void clearstring(char* s, int len)
{
	for(int i = 0; i < len; i++)
	{
		s[i] ='\0';	
	}
}

