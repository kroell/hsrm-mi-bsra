#ifndef SOUNDEX-TEST_H
    #define SOUNDEX-TEST_H
    #define ERLAUBTE_ANZ_PARAM 3
    #define HILFETEXT "Richtiger Aufruf lautet: ./soundex-test <param> <param>\n\n"
    #define FEHLERTEXT "Allgeimeiner Fehler!\n"

	extern void soundexVergleich(char *value1, char *value2);

    #ifdef EBUG
        #define debug(s) printf("DEBUG: "); printf s;
    #else
        #define debug(s) {}-
    #endif
#endif

