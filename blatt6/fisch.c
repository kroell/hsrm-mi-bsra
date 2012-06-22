#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "inc/fisch.h"

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

void liste(char *besitzer)
{
    Fischsatz fisch;
    int fd;
    char *name = "Besitzer";
    char *fischname = "Fisch";

    /* Datei öffnen, bei Fehler beenden */
    if((fd = open(FISCHFILE,O_RDONLY)) < 0)
	fehler();

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
    printf("\n+--------------------------------------------------------------------+\n");
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

    fd = open(FISCHFILE, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if(fd < 0)
        fehler();
 
    if(write(fd,(void*)&neu, FISCHSIZE) != FISCHSIZE)
	fehler();
    
    close(fd);
    return;
}

void erhoehe(char *besitzer, int betrag)
{
    int fd, anzahl = 0;
    Fischsatz fisch;

    if (betrag < 0)
	betrag = 1;

    /* Datei öffnen, bei Fehler beenden */
    if((fd = open(FISCHFILE,O_RDWR)) < 0)
	fehler();
	
    while((read(fd, &fisch, FISCHSIZE)) > 0)
    {
		
	if (!strncmp(fisch.besitzer, besitzer,20))
	{
	    fisch.preis += betrag;
	    anzahl++;
			
	    /* Leseoffset zurücksetzen um an gleiche stelle zu schreiben */
	    lseek(fd,-FISCHSIZE, SEEK_CUR);
			
	    if (write(fd, &fisch, FISCHSIZE) < 0)
		fehler();
	}
    }

    close(fd);
    printf("Preis für %d Fisch(e) erhoeht!\n", anzahl);
    return;
}

void rechnung(char *besitzer)
{ 
    int fd, sum=0;
    Fischsatz fisch;	

    /* Fischfile verschieben */	
    if(rename(FISCHFILE, TMPFILE) != 0)
	fehler();
    
    /* Filedescriptor holen */
    if((fd = open(TMPFILE,O_RDONLY)) < 0)
	fehler();
    	
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
	    neu(fisch.besitzer,fisch.fischname,fisch.preis);
	}		
    }

    /* Aufräumen */		
    remove(TMPFILE);

    /* Ausgabe */
    printf("Rechnung für %s: \t %-5d Euro\n",besitzer,sum);
    return;
}

int main(int argc, char *argv[])
{
    if (2 <= argc)
    {
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
