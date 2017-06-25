#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

char** getTitleTokens(char* key) {
        char ** sub_str = malloc(20 * sizeof(char*));
        for (int i =0 ; i < 10; ++i)
            sub_str[i] = malloc(30 * sizeof(char));

        int i = 0;
        char *p = strtok (key, " ");

        while (p != NULL)
        {
                sub_str[i++] = p;
                p = strtok (NULL, "/");
        }
        return sub_str;
}



int main(){
   FILE *fp;
   char line[1024];
   int debug = 0;

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
	
	char *key;
	char *title;
	
   /* lies Zeile für Zeile */
   while( fgets (line, 1024, fp) != NULL ){
		if(debug)printf("line: %s", line);
		
		if(!validArticle){
			/* check for the beginning of an <article> Tag */
			if (strstr(line, "<article") != NULL) {
				if(debug)printf("line enthaelt article");
				
				/* <article wurde gefunden, teste ob es sich um ein journal handelt */
				if(strstr(line, "key=\"journals/") != NULL){
					/* finde den index von 'key="' und erhöhe ihn um 5, um die 5 buchstaben key=" zu "überspringen", danach um 9, um journals/ zu überspringen */
					char *substr = "key=\"";
					int index = findSubstring(line, substr) + 5 + 9;
					if(debug)printf(", index after key=\"journals/: %d\n", index);

					/* finde den index vom end-tag "> */
					substr = "\">";
					int endIndex = findSubstring(line, substr);

					/* setze den pointer von key auf den ersten Char nach journals/ */
					key = &line[index];

					/* setze das Ende des Strings auf das " */
					line[endIndex] = '\0';

					/* überprüfe ob key 3 Teile enthält */
					if(findSubstring(key, "/") == -1){
						/* no valid key */
						break;
					} else {
						/* key hat das Format x/y/z */
						/* setze das Ende so, dass key nurnoch y enthält und /z weggeschnitten wird */
						key[findSubstring(key, "/")] = '\0';
						printf("key: %s\n", key);

						/* ab hier enthält key nurnoch den validen Artikelnamen */
						validArticle = 1;
					}
				}
			}
		}else {
			if(strstr(line, "<title>") != NULL){
				if(debug)printf("title found\n");
				/* wurde <title> gefunden, so extrahiere den Titel bis </title> */
				
				/* finde den index vom </title> tag */
				char *substr = "</title>";
				int index = findSubstring(line, substr);
				
				/* setze das Ende des Strings auf den letzten Char vorm < */
				line[index] = '\0';
				
				/* lasse *title auf den Anfang des Titels zeigen */
				title = &line[7];
				printf("title: %s\n", title);
				
				validArticle = 0;
				/* setze validArticle wieder auf 0, damit nach "<article" gesucht wird */
				
				
				/* Ab hier enthält Key den gesamten Artikelstring x/y/z und title den zugehörigen Titel */
				/* tokenize: */
				char *t = strtok(title, " .,;:<>/\\+*~#'^°!\"§$%&(){}[]?`´|-_");
				while(t != NULL) {
					printf("token: %s\n", t);
					
					/* TODO: Speichere die Wörter und ordne ihn dem letzten key zu */
					
					t = strtok(NULL, " .,;:<>/\\+*~#'^°!\"§$%&(){}[]?`´|-_");
				}
			}		
		}
		
	}
   fclose(fp);
   
   /* TODO: Überprüfe für jeden key, ob die zugehörige Zeitschrift >500 Titel enthält.*/
   /* TODO: Zerlege Titel aller großen Zeitschriften und erstelle Wort-Statistik */
   /* TODO: Erstelle geeignete Statistik zum Erstellen von Word-Clouds für die ersten x (vllt. 30?) Worte */
   
   return(0);
}
