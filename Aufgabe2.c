#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int utf8_strlen(char *s) 
{
  int arrayLength = 0, stringLength = 0;
  while (s[arrayLength]) 
  {
    if ((s[arrayLength] & 0xc0) != 0x80) stringLength++;
    arrayLength++;
  }
  return stringLength;
}

main(){
    char *a = "hallo";
  	char *b = "Längentest";
	
    int length1 = strlen(a);
    int length2 = strlen(b);
    
    printf("strlen() lentgh of %s = %d \n", a,length1);
    printf("unicode-length of %s = %d \n",a, utf8_strlen(a));

    printf("strlen() lentgh of %s = %d \n", b,length2);
    printf("unicode-length of %s = %d \n",b, utf8_strlen(b));
	
}
