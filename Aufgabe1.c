#include <stdio.h>
#include <stdlib.h>

char *trimInPlace (char *s){
	char *c = s;              //counter welcher jeden Character durchgeht
	char found = 0;           //Ob bereits der erste nicht-leerzeichen-character gefunden wurde
	while(*c){                //Solange das ende '\0' nicht erreicht wurde
		if(*c > ' ' && !found){ //falls der character > ' ' ist, ist es ein zeichen ('\20' ist Leerzeichen, '\21' ist das erste Sonderzeichen '!')
			s = c;                //Setze den Pointer s auf c
			found = 1;
		}
		c++;
	}

	while(*c <= ' '){   //c ist am ende -> rückwärts suchen
		if(*(c-1) > ' '){ //falls die position hinter dem aktuellen Wert ein Zeichen enthält, setze das ende ('\0') auf den aktuellen Wert
			*c = '\0';
			break;
		} else {
			c--;
		}
	}
	return s;           //gib den pointer auf den "neuen" Anfang des char arrays zurück
}
main(){
  //Alle 5 möglichen Fälle:
	char s1[] = "  2 spaces  ";
	char s2[] = " 1 space ";
	char s3[] = "no space infront   ";
	char s4[] = "   no space behind";
	char s5[] = "no spaces at all";
	
	printf("s1 = |%s|\n", trimInPlace(s1));
	printf("s2 = |%s|\n", trimInPlace(s2));
	printf("s3 = |%s|\n", trimInPlace(s3));
	printf("s4 = |%s|\n", trimInPlace(s4));
	printf("s5 = |%s|\n", trimInPlace(s5));
}
