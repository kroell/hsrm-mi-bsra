#ifndef SOULS_H
    #define SOULS_H
    #define ERLAUBTE_ANZ_PARAM 3
    #define PATH_MAX 100
    #define HILFETEXT "Richtiger Aufruf lautet: ./souls <verzeichnis> <buergername>\n\n"
    #define FEHLERTEXT "Allgeimeiner Fehler!\n"

	extern int soundexVergleich (char *value1, char *value2);
	extern void souLs (char * dirname, char * suchbegriff);
	

    #ifdef EBUG
        #define debug(s) printf("DEBUG: "); printf s;
    #else
        #define debug(s) {}-
    #endif
#endif

