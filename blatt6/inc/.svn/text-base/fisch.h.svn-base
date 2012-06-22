#ifndef FISCH_H
#define FISCH_H
#define FISCHFILE  "fischfile.dat"
#define TMPFILE  "tmpfile.dat"
#define GRUNDPREIS 17
#define HILFETEXT \
       "aufruf: ./fisch <param> [option]\n\n" \
       "-l [name]\t listet " FISCHFILE " [nur Eintraege von name]\n" \
       "-n name fisch \t legt einen neuen Eintrag fuer fisch unter name an\n" \
       "-z name betrag \t erhoeht den Preis bei allen Fischen von name " \
       "um betrag\n" \
       "-d name \t gibt die Rechnung fuer name aus und loescht alle seine " \
       "Fischbestaende aus " FISCHFILE "\n"
#define FEHLERTEXT "Fehler bei Dateioperation mit " FISCHFILE " (nicht vorhanden, korrumpiert o.a.)!\n"

typedef struct fis {
    char besitzer[20];
    char fischname[20];
    int preis;
} Fischsatz;

extern void liste(char *besitzer);
extern void neu(char *besitzer, char *fisch, int preis);
extern void erhoehe(char *besitzer, int betrag);
extern void rechnung(char *besitzer);

const int FISCHSIZE = sizeof(Fischsatz);
#endif
