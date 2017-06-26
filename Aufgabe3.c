#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <search.h>

static const int MIN_ARTICLE_SIZE = 300;

struct node {
	char		*w;
	int	 	count;
	struct node	*l, *r;
} *tree;


void incr(struct node **root, char *w) {
	if (root == NULL || w == NULL)
		return;
	if (*root == NULL) {
		struct node *n;
		int l = strlen(w);

		n = (struct node *) malloc(sizeof(struct node));
		if (n == NULL)
			return;
		n->w = (char *) malloc(l+1);
		if (n->w == NULL)
			return;
		strcpy(n->w,w);
		n->count = 1;
		n->l = NULL;
		n->r = NULL;

		*root = n;
		return;
	}
	int c = strcmp((*root)->w,w);
	if (c == 0) {
		((*root)->count)++;
		return;
	}
	if (c < 0)
		incr(&((*root)->r),w);
	else
		incr(&((*root)->l),w);
}

/* traverse checkt ob count > 50 und schreibt direkt ins file */
void traverse(struct node *r, char *key, FILE *fp) {
	if (r == NULL)
		return;
	
	traverse(r->l, key, fp);
	char str[128];
	sprintf(str, "%d", r->count);
	fwrite(str, sizeof(char) * strlen(str), 1, fp);
	fwrite(" ", sizeof(char), 1, fp);
	fwrite(r->w, sizeof(char) * strlen(r->w), 1, fp);
	fwrite("\n", sizeof(char), 1, fp);
	fflush(fp);

	traverse(r->r, key, fp);
}


void words(char *s)
{
	int state = 0;
	char c;
	char *start;

	while (c = *s) {
		if (isalpha(c)) {
			if (!state) {
				start = s;
				state = 1;
			}
		} else {
			state = 0;
			if (start) {
				*s = '\0';
				incr(&tree,start);
				*s = c;
			}
			start = NULL;
		}
		s++;
	}
}

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

int main() {
	FILE *fp;
	char line[1024];
	int debug = 0;
	

	/* öffne dblp.xml zum lesen */
	fp = fopen("dblp.xml", "r");
	if (fp == NULL) {
		perror("Error opening file");
		return (-1);
	}
	/* TODO: Geeignete Datenstruktur zum speichern von den Keys und den zugehörigen Titeln anlegen */
	//ENTRY titleToken;
	//ENTRY *searchTitleToken=malloc(sizeof(ENTRY));
	//(void) hcreate(MAX_HASH_SIZE);

	tree = NULL;

	/* validArticle wird auf 1 gestzt, sobald ein key="x/y/z" Attribut gefunden wurde.
	 Ist validArticle == 1, so wird bei jeder neuen line statt nach einem "<article" nach einem "<title>" gesucht */
	int validArticle = 0;

	char *title = (char *) malloc(sizeof(char) * 512);
	char *key = (char *) malloc(sizeof(char) * 32);
	char *lastkey = (char *) malloc(sizeof(char) * 32);

	int journals = 0;
	/* lies Zeile für Zeile */
	while (fgets(line, 1024, fp) != NULL) {
		if (debug)
			printf("line: %s", line);

		if (!validArticle) {
			/* check for the beginning of an <article> Tag */
			if (strstr(line, "<article") != NULL) {
				if (debug)
					printf("line enthaelt article");

				/* <article wurde gefunden, teste ob es sich um ein journal handelt */
				if (strstr(line, "key=\"journals/") != NULL) {
					/* finde den index von 'key="' und erhöhe ihn um 5, um die 5 buchstaben key=" zu "überspringen", danach um 9, um journals/ zu überspringen */
					char *substr = "key=\"";
					int index = findSubstring(line, substr) + 5 + 9;
					if (debug)
						printf(", index after key=\"journals/: %d\n", index);

					/* finde den index vom end-tag "> */
					substr = "\">";
					int endIndex = findSubstring(line, substr);

					/* setze den pointer von key auf den ersten Char nach journals/ */
					key = &line[index];

					/* setze das Ende des Strings auf das " */
					line[endIndex] = '\0';

					/* überprüfe ob key 3 Teile enthält */
					if (findSubstring(key, "/") == -1) {
						/* no valid key */
						lastkey = NULL;
						break;
					} else {
						/* key hat das Format x/y/z */
						/* setze das Ende so, dass key nurnoch y enthält und /z weggeschnitten wird */
						key[findSubstring(key, "/")] = '\0';
						if(debug)printf("key: %s\n", key);
						
						if(lastkey == NULL){
							strcpy(lastkey, key);
						} else {
							if(strcmp(lastkey, key) == 0){
								journals++;
								/* behalte tree bei, selber key */
							} else {
								/* TODO: lege neuen tree an und printe alten */
								if(journals >= MIN_ARTICLE_SIZE){
									FILE *fpOut;
									char *concat = malloc(strlen(key) * sizeof(char) + 17 * sizeof(char));
									strcpy(concat, "./wordcloud/");
									strcat(concat, key);
									strcat(concat, ".txt");
									fpOut = fopen(concat, "wb");
									if (fpOut == NULL) {
										perror("Error opening file");
										return;
									}
									traverse(tree, lastkey, fpOut);
									printf("exported tree to %s\n", concat);
									free(concat);
									fclose(fpOut);
									tree=NULL;
									journals = 0;
								}
								strcpy(lastkey, key);
							}
						}

						/* ab hier enthält key nurnoch den validen Artikelnamen */
						validArticle = 1;
						
					}
				}
			}
		} else {
			if (strstr(line, "<title>") != NULL) {
				if (debug)
					printf("title found\n");
				/* wurde <title> gefunden, so extrahiere den Titel bis </title> */

				/* finde den index vom </title> tag */
				char *substr = "</title>";
				int index = findSubstring(line, substr);

				/* setze das Ende des Strings auf den letzten Char vorm < */
				line[index] = '\0';

				/* lasse *title auf den Anfang des Titels zeigen */
				title = &line[10];
				if(debug)printf("title: %s\n", title);

				validArticle = 0;
				/* setze validArticle wieder auf 0, damit nach "<article" gesucht wird */

				/* Ab hier enthält Key den Artikelnamen und title den zugehörigen Titel */
				
				words(title);

			}
		}

	}
	if(validArticle && journals >= MIN_ARTICLE_SIZE){
		FILE *fpOut;
		char *concat = malloc(strlen(key) * sizeof(char) + 17 * sizeof(char));
		strcpy(concat, "./wordcloud/");
		strcat(concat, key);
		strcat(concat, ".txt");
		//printf("concat: %s\n", concat);
		fpOut = fopen(concat, "wb");
		if (fpOut == NULL) {
			perror("Error opening file");
			printf("last record, key: %s", key);
			return;
		}
		traverse(tree, lastkey, fpOut);
		printf("exported tree to %s\n", concat);
		fclose(fpOut);
		free(concat);
	}
	fclose(fp);

	/* TODO: Überprüfe für jeden key, ob die zugehörige Zeitschrift >500 Titel enthält.*/
	/* TODO: Zerlege Titel aller großen Zeitschriften und erstelle Wort-Statistik */
	/* TODO: Erstelle geeignete Statistik zum Erstellen von Word-Clouds für die ersten x (vllt. 30?) Worte */

	return (0);
}
