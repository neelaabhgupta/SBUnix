#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/defs.h>
//#include <sys/init_pd.h>

char * getMallocAddress(int size)
{
    char * tmp_addr=(char*)malloc(size);
    return tmp_addr;

}

int printf(const char *format, ...)
{   
    char * buffer_to_print= getMallocAddress(4096);
    #if 1
    int len=0;
    va_list vl;
    va_start(vl, format);
    for (int l = 0; format[l] != '\0'; l++) 
    {   
        if (format[l] != '%') 
        {
            if(format[l] == (char)10)
            {
                buffer_to_print[len++]=(char)10;
            }
            else
            {
                buffer_to_print[len++]=format[l];
            }
            continue;
        }
        switch (format[++l])
        {
            case 's':
            {
                char *val = va_arg(vl, char *);
                for (;*val != '\0'; val++)
                {
                    buffer_to_print[len++]= *val;
                }
            }
            break;
            case 'c':
            {
                char val = va_arg(vl, int);
                buffer_to_print[len++]= val;
            }
            break;
            case 'd':
            {
                int i = 0;
                char temp[1024] = {};
                int mod = 0;
                int val = va_arg(vl, int);
                if(val == 0)
                {
                    buffer_to_print[len++]='0';
                }
                else
                {
                    while(val)
                    {
                        mod = val%10;       
                        temp[i] = mod + '0';
                        val = val/10;
                        i++;
                    }
                    i--;
                    while(i>=0)
                    {
                        buffer_to_print[len++]=temp[i--];
                    }
                }
            }
            break;
            case 'x':
            {
                int i = 0;
                unsigned long long int k = va_arg(vl, unsigned long long int);
                char temp[1024] = {};
                int mod = 0;
                if(k == 0)
                {
                    buffer_to_print[len++]='0';
                }
                else
                { 
                    while(k)
                    {
                        mod = k%16;        
                        switch(mod)
                        {
                            case 0:temp[i] = '0';
                                    break;
                            case 1:temp[i] = '1';
                                    break;
                            case 2:temp[i] = '2';
                                    break;
                            case 3:temp[i] = '3';
                                    break;
                            case 4:temp[i] = '4';
                                    break;
                            case 5:temp[i] = '5';
                                    break;
                            case 6:temp[i] = '6';
                                    break;
                            case 7:temp[i] = '7';
                                    break;
                            case 8:temp[i] = '8';
                                    break;  
                            case 9:temp[i] = '9';
                                    break;
                            case 10:temp[i] = 'A';
                                    break;
                            case 11:temp[i] = 'B';
                                    break;  
                            case 12:temp[i] = 'C';
                                    break;
                            case 13:temp[i] = 'D';
                                    break;
                            case 14:temp[i] = 'E';
                                    break;
                            case 15:temp[i] = 'F';
                                break;  
                        }
                        k = k/16;
                        i++;
                    }
                    i--;
                    while(i>=0)
                    {
                        
                        buffer_to_print[len++]=temp[i--];
                    }
                }           
            }
            break;
            case 'p':
            {
                
                void* a = va_arg(vl, void*);
                unsigned long long int k = (unsigned long long int)a;
                char temp[1024] = {};
                int mod = 0;
                int i = 0; 
                while(k)
                {
                    mod = k%16;        
                    switch(mod)
                    {
                        case 0:temp[i] = '0';
                                break;
                        case 1:temp[i] = '1';
                                break;
                        case 2:temp[i] = '2';
                                break;
                        case 3:temp[i] = '3';
                                break;
                        case 4:temp[i] = '4';
                                break;
                        case 5:temp[i] = '5';
                                break;
                        case 6:temp[i] = '6';
                                break;
                        case 7:temp[i] = '7';
                                break;
                        case 8:temp[i] = '8';
                                break;  
                        case 9:temp[i] = '9';
                                break;
                        case 10:temp[i] = 'A';
                                break;
                        case 11:temp[i] = 'B';
                                break;  
                        case 12:temp[i] = 'C';
                                break;
                        case 13:temp[i] = 'D';
                                break;
                        case 14:temp[i] = 'E';
                                break;
                        case 15:temp[i] = 'F';
                        break;  
                    }
                    k = k/16;
                    i++;
                }
                i--;
                while(i>=0)
                {   

                    buffer_to_print[len++]=temp[i--];
                }
            }
            break;
        }
    } 
    va_end(vl);
    write(1,buffer_to_print,len);
    //free((void*)buffer_to_print);
    return len;
    #endif
    //return 0;
}

 /*int putchar(int c)
 {
    int ret = printf("%c", c);
    return ret;
 }

 int puts(const char *s)
 {
    int ret = printf("%s", s);
    return ret;
 }*/

 char* gets(char *s)
 {
    char str[512];
    for(int i = 0; i < 512; i++)
    {
        str[i] = '\0';
    }
    int ret = 0;
    while(1)
    {
        ret=read(0, str, 512);
        if(ret)
        {
            break;
        }
    }
    
    if(str[0] == '\0')
    {
        return NULL;
    }
    else
    {
        int i = 0;
        while(1)
        {
            if(str[i] == '\0')
            {
                s[i] = '\0';
                break;
            }
            s[i] = str[i];
            i++;
        }
        return s;
    }
 }