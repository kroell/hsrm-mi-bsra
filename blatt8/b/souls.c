/**
 * @file souls.c
 * @brief BSRA Uebungsblatt 8, Aufgabe 1b - Verzeichnisse nach Namen durchsuchen
 * 
 * Author: Soeren Kroell
 * Date: 22.06.2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "inc/soundex.h"
#include "inc/souls.h"


static __inline__ void hilfe(void) {
    write(2,HILFETEXT,strlen(HILFETEXT));
    exit(EXIT_FAILURE);
}

static __inline__ void fehler(void) {
    write(2,FEHLERTEXT,strlen(FEHLERTEXT));
    exit(EXIT_FAILURE);
}


/** 
 * @brief Vergleich der mitgegebenen Werte mittels Soundex und Rueckgabe true(1) bzw false(0)
 */
int soundexVergleich(char *value1, char *value2) {
char string1 [5];
char string2 [5];
int abweichung = 0;
int uebereinstimmung = 1;
char *soundex1 = Soundex (value1);
strcpy(string1,soundex1);
char *soundex2 = Soundex (value2);	
strcpy(string2,soundex2);

	/* Vergleiche der Soundex-Returns und Ausgabe der entsprechenden Meldung */
	if (strcmp (string1,string2) != 0) {
		return abweichung;
	}
	else {
		return uebereinstimmung;
	}
}


int main(int argc, char *argv[]) {
DIR *dir;
struct dirent *eintrag;
struct stat statbuf;
char pfadpuffer[PATH_MAX], *dateiname;
char * dirname = argv[1];
char * suchbegriff = argv[2];
char * dateiendung = ".txt";
int check = 0;

	if (argc != ERLAUBTE_ANZ_PARAM) {
		hilfe();
		printf("Aufruf: %s verzeichnis\n", dirname);
		exit(1);
	}

	dir = opendir(dirname);
 
	if (dir == NULL) {
		perror(dirname);
		exit(2);
	}

	strcpy(pfadpuffer, dirname);
	strcat(pfadpuffer, "/");
	dateiname = pfadpuffer + strlen(pfadpuffer);
	
	while (1) {
		eintrag = readdir(dir);
		if (eintrag == NULL) break;
		if (strcmp(eintrag->d_name, ".")==0 ||
			strcmp(eintrag->d_name, "..")==0) continue;
			strcpy(dateiname, eintrag->d_name);
		if (stat(pfadpuffer, &statbuf) == -1) {
			perror(pfadpuffer);
		} 
		else {
			/* Nur Ausgabe von Dateien mit .txt Endung */
			if (strstr(dateiname, dateiendung)){

				/* .txt Endung aus Dateinamen entfernen */
				dateiname[strlen(dateiname)-4] = 0;
	
				/* Suchbegriff und Dateiname vergleichen */
				check = soundexVergleich(suchbegriff, dateiname);
				/* wenn check true, dann Ausgabe der Datei */
				if (check == 1){
					printf("%s%s (%ld Bytes)\n",dateiname,dateiendung, statbuf.st_size);
				}	
			}
		}
	}
	closedir(dir);

return 0;
}
