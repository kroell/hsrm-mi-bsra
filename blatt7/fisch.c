#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "inc/fisch.h"
#include "inc/dijkstra.h"


static int ivals[] = {1,1,0};
/* Nummern der Semaphore */
static int mutex = 0;
static int db = 1;
static int nLeser = 2;

static __inline__ void hilfe(void)
{
    write(2,HILFETEXT,strlen(HILFETEXT));
    exit(EXIT_FAILURE);
}

static __inline__ void fehler(void)
{
    write(2,FEHLERTEXT,strlen(FEHLERTEXT));
    exit(EXIT_FAILURE);
}
/* Die Funktionen stehen hier nur als Referenz (1 zu 1 aus VL8 Seite 19-20
   abgetippt), um zu sehen, wie man die P&V-Operationen benutzt. Als Hinweis:
   mutex dient einzig und allein dazu, die Variable nLeser vor konkurrierenden
   Zugriffen zu schützen.

void leser(void)
{
    while(1)
    {
	P(mutex);
	nLeser++;
	if (nLeser == 1) P(db); // erster Leser reserviert DB
	V(mutex);

        // datenbestand_lesen

	P(mutex);
	nLeser--;
	if (nLeser == 0) V(db); // letzter Leser gibt DB wieder frei
	V(mutex);

	// gelesene_daten_verarbeiten
    }
}

void schreiber(void)
{
    while (1)
    {
	// daten bereitstellen
	P(db); // exklusiven Datenbankzugriff anfordern

	// daten schreiben

	V(db); // Datenbankzugriff wieder freigeben
    }
}*/

void liste(char *besitzer)
{
    Fischsatz fisch;
    int fd;
    char *name = "Besitzer";
    char *fischname = "Fisch";

    /* Vor konkurrierenden Schreibzugriffen schützen ANFANG*/
    debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
    P(mutex);
    V(nLeser);
    if (Stand(nLeser) == 1) P(db); /* Wenn jemand schreibt, warten */
    V(mutex);
    debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));

    /* Datei öffnen, bei Fehler beenden */
    if((fd = open(FISCHFILE,O_RDONLY)) < 0)
    {
	P(nLeser); /* Semaphore zurücksetzen */
	if (Stand(nLeser) == 0) V(db);
	debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
	fehler();
    }

    /* Ausgabe Tablle mit Kopf Beitzer, Fisch und Preis */
    printf("\n+--------------------------------------------------------------------+");
    printf("\n| %-20s | %-20s | %-20s |",name,fischname,"Preis");
    printf("\n+--------------------------------------------------------------------+");

    /* wenn fisch.dat Datensaetze enthaelt */
    while(read(fd, &fisch, FISCHSIZE) > 0)
    {
	/* wird beim Aufruf kein Besitzer angegeben*/
	if (besitzer == NULL)
	{
	    printf("\n| %-20s   %-20s   %-5d EURO \t     |",fisch.besitzer,fisch.fischname,fisch.preis);
	}
	else
	{
	    if (!strncmp(fisch.besitzer, besitzer,20))
		printf("\n| %-20s   %-20s   %-5d EURO \t     |",fisch.besitzer,fisch.fischname,fisch.preis);
	}
    }
    
    close(fd);
    /*if(getchar() != '\0');  Bewusst hängen lassen */
    P(mutex);
    P(nLeser); /* Semaphore zurücksetzen */
    if (Stand(nLeser) == 0) V(db);
    V(mutex);
    /* Vor konkurrierenden Schreibzugriffen schützen ENDE */

    printf("\n+--------------------------------------------------------------------+\n");
    debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
    return;
}

void neu(char *besitzer, char *fisch, int preis)
{
    int fd, i;
    Fischsatz neu;

    for (i=0; i < 19; ++i)
	neu.besitzer[i] = besitzer[i];
    neu.besitzer[19] = '\0';
    for (i=0; i < 19; ++i)
	neu.fischname[i] = fisch[i];
    neu.fischname[19] = '\0';
    neu.preis = preis < 0 ? GRUNDPREIS : preis;

    /* vor konkurrierenden Lese- und Schreibzugriffen schützen ANFANG */
    debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
    P(db);
    debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));

    fd = open(FISCHFILE, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if(fd < 0)
    {
	V(db); /* Freigabe */
	debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
        fehler();
    }
 
    if(write(fd,(void*)&neu, FISCHSIZE) != FISCHSIZE)
    {

	V(db); /* Freigabe */
	debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
	fehler();
    }
    
    close(fd);
    /*if(getchar() != '\0') ; Hängen lassen */
    V(db);/* Freigabe */
    debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
    /* vor konkurrierenden Lese- und Schreibzugriffen schützen ENDE */
    return;
}

void erhoehe(char *besitzer, int betrag)
{
    int fd, anzahl = 0;
    Fischsatz fisch;

    if (betrag < 0)
	betrag = 1;

    /* vor konkurrierenden Lese- und Schreibzugriffen schützen ANFANG */
    P(db);
    debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));

    /* Datei öffnen, bei Fehler beenden */
    if((fd = open(FISCHFILE,O_RDWR)) < 0)
    {
	V(db);
	debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
	fehler();
    }

    while((read(fd, &fisch, FISCHSIZE)) > 0)
    {
		
	if (!strncmp(fisch.besitzer, besitzer,20))
	{
	    fisch.preis += betrag;
	    anzahl++;
			
	    /* Leseoffset zurücksetzen um an gleiche stelle zu schreiben */
	    lseek(fd,-FISCHSIZE, SEEK_CUR);
			
	    if (write(fd, &fisch, FISCHSIZE) < 0)
	    {
		V(db);
		debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
		fehler();
	    }
	}
    }

    close(fd);
    /*if(getchar() != '\0') ; Hängen lassen */
    V(db);
    /* vor konkurrierenden Lese- und Schreibzugriffen schützen ENDE */

    printf("Preis für %d Fisch(e) erhoeht!\n", anzahl);
    debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
    return;
}

void rechnung(char *besitzer)
{ 
    int fd, sum=0;
    Fischsatz fisch;

    /* Vor konkurrierenden Lese- und Schreibzugriffen schützen ANFANG */
    P(db);
    debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));

    /* Fischfile verschieben */	
    if(rename(FISCHFILE, TMPFILE) != 0)
    {
	V(db);
	debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
	fehler();
    }

    /* Filedescriptor holen */
    if((fd = open(TMPFILE,O_RDONLY)) < 0)
    {
	V(db);
	debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
	fehler();
    }

    /* Über tmpfile iterieren */
    while(read(fd, &fisch, FISCHSIZE))
    {
	/* printf("Besitzer: %s, Name:%s, Preis: %d\n",fisch.besitzer, fisch.fischname, fisch.preis); */
	/* Dabei prüfen ob *besitzer drin steht */
	if (!strncmp(fisch.besitzer,besitzer,20))
	{
	    sum += fisch.preis; /* preis summieren */
	}	
	else /* wenn struct nich von besitzer -> in fischfile schreiben */
	{
	    int fd;
	    fd = open(FISCHFILE, O_CREAT | O_WRONLY | O_APPEND, 0666);
	    if(fd < 0)
	    {
		V(db);
		debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
		fehler();
	    }
	    if(write(fd,(void*)&fisch, FISCHSIZE) != FISCHSIZE)
	    {
		V(db);
		debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
		fehler();
	    }
	    close(fd);
	}		
    }

    /* Aufräumen */		
    remove(TMPFILE);
    /*if(getchar() != '\0') ; Hängen lassen */
    V(db);
    debug(("werte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));
    /* vor konkurrierenden Lesezugriffen schützen ENDE */

    /* Ausgabe */
    printf("Rechnung für %s: \t %-5d Euro\n",besitzer,sum);
    return;
}

int main(int argc, char *argv[])
{
    if (2 <= argc)
    {
	/* Semaphormenge erstellen */
	int num = sizeof(ivals) / sizeof(ivals[0]);
	if (PVinit(num,ivals) != 0)
	{
	    printf("Fehler beim Erstellen der Semaphore");
	    fehler();
	}
	
	debug(("initialwerte fuer mutex, nLeser, db: %d, %d, %d\n",Stand(mutex),Stand(nLeser),Stand(db)));

	if (!strncmp(argv[1],"-l",2))
	{
	    if (argc == 2)
		liste(NULL);
	    else  if (argc == 3)
		liste(argv[2]);
	    else
		hilfe();
	}
	else if (!strncmp(argv[1],"-n",2))
	{
	    if (argc == 4)
		neu(argv[2],argv[3],GRUNDPREIS);
	    else
		hilfe();
	}
	else if (!strncmp(argv[1],"-z",2))
	{
	    if (argc == 4)
		erhoehe(argv[2], strtol(argv[3], (char **) NULL, 10));
	    else
		hilfe();
	}
	else if (!strncmp(argv[1],"-d",2))
	{
	    if (argc == 3)
		rechnung(argv[2]);
	    else
		hilfe();
	}
	else
	    hilfe();
    }
    else
	hilfe();

    return 0;
}
