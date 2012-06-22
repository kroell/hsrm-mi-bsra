/**
 * @file souls.c
 * @brief BSRA Uebungsblatt 8, Aufgabe 1a - Zwei Zeichenketten auf Uebereinstimmung pruefen
 * 
 * Author: Soeren Kroell
 * Date: 22.06.2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/soundex.h"
#include "inc/soundex-test.h"

static char *suchwort;
static char *eingabe;

static __inline__ void hilfe(void) {
    write(2,HILFETEXT,strlen(HILFETEXT));
    exit(EXIT_FAILURE);
}

static __inline__ void fehler(void) {
    write(2,FEHLERTEXT,strlen(FEHLERTEXT));
    exit(EXIT_FAILURE);
}


/** 
 * @brief Vergleich der mitgegebenen Werte mittels Soundex Modul
 */
void soundexVergleich(char *value1, char *value2) {
char string1 [5];
char string2 [5];
char *soundex1 = Soundex (value1);
strcpy(string1,soundex1);
char *soundex2 = Soundex (value2);	
strcpy(string2,soundex2);

	/* Vergleiche der Soundex-Returns und Ausgabe der entsprechenden Meldung */
	if (strcmp (string1,string2) != 0) {
		printf ("abweichung\n");
	}
	else {
		printf ("uebereinstimmung\n");
	}
}


int main(int argc, char *argv[]) {
suchwort = argv[1];
eingabe = argv[2];

    if (argc == ERLAUBTE_ANZ_PARAM) {
	soundexVergleich(suchwort,eingabe);
    }
    else{
	hilfe();
    }

return 0;
}
