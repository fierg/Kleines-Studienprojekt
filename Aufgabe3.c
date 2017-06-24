#include <stdio.h>
#include <string.h>

int findSubstring(char *str, char *substr) {
     int i = 0;
     int d = 0;
	 /* setze Index i auf jedes Zeichen von str und prüfe, ob von i aus substr gefunden wurde */
	 for (i = 0; i < strlen(str) - strlen(substr); i++) {
		 int found = 1; 
		 for (d = 0; d < strlen(substr); d++) {
			 if (str[i + d] != substr[d]) {
				 /* sobald ein character nicht übereinstimmt, setze found = 0 und suche ab dem nächsten Zeichen in str*/
				 found = 0; 
				 break;
			 }
		 }
		 if (found == 1) {
			 return i;
		 }
	 }
	 return -1;
} 

char *getSubstringToChar(char *str, char c){
	/* kopiere str in out.
		benutze den index i und erhöhe ihn mithilfe von str solange, bis ein " gefunden wurde
		setze das Ende vom kopierten String out auf den gefunden index */
	int i = 0;
	char *out;
	strcpy(out, str);
	while(*str != c){
		i++;
		str++;
	}
	out[i] = '\0';
	return out;
}

int isValid(char *key){
	int valid = 1;
	
	/* TODO: Überprüfe ob key vom Format [a-zA-Z]+/[a-zA-Z]+/[a-zA-Z]+ ist */
	
	return valid;
}

int main(){
   FILE *fp;
   char line[512];
   int debug = 1;

   /* öffne dblp.xml zum lesen */
   fp = fopen("dblp_smallTest.xml" , "r");
   if(fp == NULL){
      perror("Error opening file");
      return(-1);
	}
	
	/* TODO: Geeignete Datenstruktur zum speichern von den Keys und den zugehörigen Titeln anlegen */
	
   /* validArticle wird auf 1 gestzt, sobald ein key="x/y/z" Attribut gefunden wurde.
	  Ist validArticle == 1, so wird bei jeder neuen line statt nach einem "<article" nach einem "<title>" gesucht */
	int validArticle = 0;
	
   /* lies Zeile für Zeile */
   while( fgets (line, 512, fp) != NULL ){
		if(debug)printf("line: %s\n", line);
		
		if(!validArticle){
			/* check for the beginning of an <article> Tag */
			if (strstr(line, "<article") != NULL) {
				if(debug)printf("line enthaelt article");
				
				/* <article wurde gefunden 
					finde den index von 'key="' und erhöhe ihn um 5, um die 5 buchstaben key=" zu "überspringen" */
				char *substr = "key=\"";
				int index = findSubstring(line, substr) + 5;
				if(debug)printf(", index after key=\": %d\n", index);
				
				/* wurde ein key gefunden, so extrahiere den substring bis zum nächsten " */
				if(index != -1){
					char *key = getSubstringToChar(&line[index], '"');
					if(debug)printf("key: %s\n", key);
					
					/* TODO: implementation von "isValid" */
					
					if(isValid(key)){
						validArticle = 1;
						/* TODO: key zu journal zuordnen und irgendwo speichern */
					} else {
						
					}
				}
			}
		}else {
			if(strstr(line, "<title>") != NULL){
				/* wurde <title> gefunden, so enthält die nächste zeile den gesamten Titel */
				char *title = fgets (line, 512, fp);
				/* TODO: Speichere den titel und ordne ihn dem letzten key zu */
			}
			/* setze validArticle wieder auf 0, damit nach "<article" gesucht wird */
			validArticle = 0;
		}
	}
   fclose(fp);
   
   /* TODO: Überprüfe für jeden key, ob die zugehörige Zeitschrift >500 Titel enthält.*/
   /* TODO: Zerlege Titel aller großen Zeitschriften und erstelle Wort-Statistik */
   /* TODO: Erstelle geeignete Statistik zum Erstellen von Word-Clouds für die ersten x (vllt. 30?) Worte */
   
   return(0);
}