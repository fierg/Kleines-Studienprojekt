#include <stdio.h>
#include <string.h>
#include <regex.h>

static const char *PATTERN = "[a-zA-Z]+\\/[a-zA-Z]+\\/[a-zA-Z]+";

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

int match(char *string, const char *pattern)
{
    int    status;
    regex_t    re;

    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return(0);      /* Report error. */
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return(0);      /* Report error. */
    }
    return(1);
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
				
				/* finde den index vom end-tag "> */
				substr = "\">";
				int endIndex = findSubstring(line, substr);
				
				/* setze das Ende des Strings auf das " */
				line[endIndex] = '\0';
				
				/* setze den pointer von key auf den ersten Char des Keys */
				char *key = &line[index];
				printf("key: %s\n", key);
				
				/* überprüfe ob key 3 Teile enthält */
				if(match(key, PATTERN)){
					/* falls ja, so wurde ein valider Aritkel gefunden */
					validArticle = 1;
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
				char *title = &line[7];
				printf("title: %s\n", title);
				
				/* TODO: Speichere den titel und ordne ihn dem letzten key zu */
				
				validArticle = 0;
				/* setze validArticle wieder auf 0, damit nach "<article" gesucht wird */
			}		
		}
	}
   fclose(fp);
   
   /* TODO: Überprüfe für jeden key, ob die zugehörige Zeitschrift >500 Titel enthält.*/
   /* TODO: Zerlege Titel aller großen Zeitschriften und erstelle Wort-Statistik */
   /* TODO: Erstelle geeignete Statistik zum Erstellen von Word-Clouds für die ersten x (vllt. 30?) Worte */
   
   return(0);
}
