#include <stdio.h>
#include <string.h> 

void split(char *src, const char *separator, char **dest, int *num) {
     char *pNext;
     int count = 0;
     if (src == NULL || strlen(src) == 0)
         return;
     if (separator == NULL || strlen(separator) == 0)
         return;   
     pNext = strtok(src, separator);
     while(pNext != NULL) {
         *dest++ = pNext; 
         ++count;
         pNext = strtok(NULL, separator);  
     }    
     *num = count;
}
/*
int main(int argc, char **argv){
    char str[128];
    strcpy(str,  "192.168.0.33:9090");

    char *ipport[2] = {0};
    int num = 0;
    split(str, ":", ipport, &num);

    for(int i = 0;i < num; i ++) {
        printf("%s\n",ipport[i]);
    }
    return 0;
} 
*/

