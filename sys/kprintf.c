#include <sys/kprintf.h>
#include <stdarg.h>
#include <sys/defs.h>
//void kprintf(const char *fmt, ...)

//Citation - https://www.hackerearth.com/practice/notes/know-our-printf-variable-number-of-arguments-to-a-function/

int vp_counter = 0;
char *current_vp;

#define video_loc (0xFFFFFFFF80000000 + 0xB8000)
//#define video_loc (0xB8000)
void endpage()
{
    char *temp = (char*)video_loc;
    while(temp != (char*)(video_loc+3680))
    {
        *temp = *(temp+160);
        temp++;
    }
    current_vp = (char*)(video_loc+3680);
    vp_counter = 3680;
    char *temp1 = current_vp;
    while(temp1 != (char*)video_loc+3840)
    {
        *temp1 = '\0';
        temp1++;
    }
}

void kprintf(char *fmt, ...)
{
    va_list vl;
    //char *p;
    va_start(vl, fmt);
    //int counter =0;
    //int buffer1=3998;
    //char *current_vp = (char*)video_loc;
    //current_vp=current_vp+buffer1;
    current_vp = (char*)video_loc;
    current_vp = current_vp + vp_counter;
    for (int l = 0; fmt[l] != '\0'; l++) 
    {
        if (fmt[l] != '%') 
        {
            if(fmt[l] == (char)10)
            {
                int buffer = 160 - vp_counter%160;
                if(current_vp == (char*)(video_loc+3840))
                {
                    endpage();
                }
                current_vp = current_vp + buffer;
                vp_counter = vp_counter + buffer;
            }
            else if(fmt[l] == (char)9)
            {
                if(current_vp == (char*)(video_loc+3840))
                {
                    endpage();
                }
                current_vp = current_vp + 8;
            }
            else if(fmt[l] == (char)13)
            {
                if(current_vp == (char*)(video_loc+3840))
                {
                    endpage();
                }
                int buffer1 = vp_counter%160;
                current_vp = current_vp - buffer1;
                vp_counter = vp_counter - buffer1;
            }
            else
            {
                if(current_vp == (char*)(video_loc+3840))
                {
                    endpage();
                }
                *current_vp++ = fmt[l];
                *current_vp++ = 0x00;
                vp_counter += 2;
            }
            continue;
        }
        switch (fmt[++l])
        {
            case 's':
            {
                char *val = va_arg(vl, char *);
                for (;*val != '\0'; val++)
                {
                    /*if(*val=='\n'){
                        int buffer = 160 - vp_counter%160;
                        current_vp = current_vp + buffer;
                        vp_counter = vp_counter + buffer;
                    }*/   
                    if(current_vp == (char*)(video_loc+3840))
                    {
                        endpage();
                    } 
                    *current_vp++ = *val;
                    *current_vp++ = 0x00;
                    vp_counter += 2;
                }
            }
            break;
            case 'c':
            {
                char val = va_arg(vl, int);
                if(current_vp == (char*)(video_loc+3840))
                {
                    endpage();
                }
                *current_vp++ = (char)val;
                *current_vp++ = 0x00;
                vp_counter += 2;
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
                    if(current_vp == (char*)(video_loc+3840))
                    {
                        endpage();
                    }
                    *current_vp++ = '0';
                    *current_vp++ = 0x00;
                    vp_counter += 2;
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
                        if(current_vp == (char*)(video_loc+3840))
                        {
                            endpage();
                        }
                        *current_vp++ = temp[i--];
                        *current_vp++ = 0x00;
                        vp_counter += 2;
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
                    if(current_vp == (char*)(video_loc+3840))
                    {
                        endpage();
                    }
                    *current_vp++ = '0';
                    *current_vp++ = 0x00;
                    vp_counter += 2;
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
                        if(current_vp == (char*)(video_loc + 3840))
                        {
                            endpage();
                        }
                        *current_vp++ = temp[i--];
                        *current_vp++ = 0x00;
                        vp_counter += 2;
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
                    if(current_vp == (char*)(video_loc+3840))
                    {
                        endpage();
                    }
                    *current_vp++ = temp[i--];
                    *current_vp++ = 0x00;
                    vp_counter += 2;
                }
            }
            break;
        }
    } 
    va_end(vl);
    //vp_counter++;
}  

void kprintf_bsp()
{
    current_vp = current_vp - 2;
    *current_vp = ' ';
    //*(current_vp + 4) = 0x0F;
    vp_counter -= 2;
}


void clrscr(){
    //kprintf("hello");
    //kprintf("Current video_loc is: %p", (uint64_t)current_vp);
    
    while((uint64_t)current_vp!=video_loc){
        current_vp = current_vp - 2;
        *current_vp = ' ';
        vp_counter -= 2;
    }
    kprintf("\t\t\t\t\t\tWELCOME to SBUNIX!\n");
    kprintf("For Instructions Please refer to The ReadME File. Press hit Enter after every command\n");
}