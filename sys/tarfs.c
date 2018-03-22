#include <sys/tarfs.h>
#include <sys/kprintf.h>
#include <sys/init_pd.h>

uint64_t string_to_integer(char *p){
	uint64_t i;
	i = 0;
	while(*p>='0' && *p<='9'){
		i=i*10+(*p-'0');
		p++;
	}
	return i;
}

uint64_t power (uint64_t num, uint64_t pow){
	if(pow == 0){
		return 1;
	}
	else {
		return num*power(num, pow-1);
	}
}


uint64_t oct2dec(uint64_t oct){
    uint64_t dec=0; 
    uint64_t i = 0;
    while(oct != 0){
        dec = dec + (oct%10) * power(8,i);
        i++;
        oct = oct/10;
    }
    return dec;
}

int compare_string(char *dest, char *src){
    int i=0;
    int flag=0;
    while(dest[i]!='\0' || src[i]!='\0'){
        if(dest[i]==src[i]){
            i++;
            continue;
        }
        else{
            flag=1;
            return flag;
            
        }
    }
    return 0;
}

uint64_t find_executable(char *path){
	uint64_t t_size;
	HEAD *head = (HEAD *)&_binary_tarfs_start;
	while(head < (HEAD*)&_binary_tarfs_end && compare_string(head->name, "")){
		//kprintf("HELLO FROM INSIDE\n");
		t_size = oct2dec(string_to_integer(head->size));
		if(compare_string(path,head->name)==0){
			if(compare_string("0",head->typeflag)==0){
				//kprintf("TYPE:%s", head->typeflag);
				return (uint64_t)(head + 1);
			}
			else{
				return 0;
			}
		}
		if(t_size>0){
			if(t_size%512!=0){
				t_size=(t_size/512)*512;
            	t_size+=512;
            }
			head = (HEAD*)((uint64_t)head + t_size + 512);
		}
		else{
			head = head+1;
		}
	}
	return 0;
}