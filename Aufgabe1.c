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

	if(!found){					//nur leerzeichen gefunden oder leerer String
		s[0] = '\0';
		return s;
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

char *trimCopy(char *s){

	char *c = s;
	int start = 0;
	while(*c <= ' '){	//finde startindex des ersten nicht-leerzeichen characters
		c++;
		if(*c == '\0'){	//Wenn Ende erreicht -> nur Leerzeichen oder leerer String
			char * out = (char *) malloc(sizeof(char));
			out[0] = '\0';
			return out;	
		}
		start++;
	}
	
	int length = 1;		//berechne gesamte Länge ohne bevorstehende Leerzeichen
	while(*c){
		length++;
		c++;
	}

	while(*c <= ' '){	//ziehe nachstehende Leerzeichen von der Länge ab
		length--;
		c--;
	}

	//hier: length = Anzahl characters ohne '\0'

	char *out = (char *) malloc(sizeof(char) * (length+1));	//Speicher für length+1 allozieren
	out[length] = '\0';	//ende setzen
	
	c = &s[start];			//c auf den startindex des ersten chars setzen
	int i = 0;
	while (1){
  	out[i++] = *c++;	//character für character kopieren
		if(i == length){
			break;
		}
	}
	return out;
}

main(){
  //Alle 7 möglichen Fälle:
	char s1[] = "  2 spaces  ";	//8
	char s2[] = " 1 space ";		//7
	char s3[] = "no space infront   "; //16
	char s4[] = "   no space behind"; //15
	char s5[] = "no spaces at all";	//16
	char s6[] = "     ";
	char s7[] = ""; //0

	printf("s1 = |%s|\n", trimCopy(s1));
	printf("s2 = |%s|\n", trimCopy(s2));
	printf("s3 = |%s|\n", trimCopy(s3));
	printf("s4 = |%s|\n", trimCopy(s4));
	printf("s5 = |%s|\n", trimCopy(s5));
  	printf("s6 = |%s|\n", trimCopy(s6));
	printf("s7 = |%s|\n", trimCopy(s7));

	printf("s1 = |%s|\n", trimInPlace(s1));
	printf("s2 = |%s|\n", trimInPlace(s2));
	printf("s3 = |%s|\n", trimInPlace(s3));
	printf("s4 = |%s|\n", trimInPlace(s4));
	printf("s5 = |%s|\n", trimInPlace(s5));
  	printf("s6 = |%s|\n", trimInPlace(s6));
	printf("s7 = |%s|\n", trimInPlace(s7));
}
