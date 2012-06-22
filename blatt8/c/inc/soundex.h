#ifndef SOUNDEX_H
	#define SOUNDEX_H

		extern char * Soundex (char * name);

#endif
#ifdef EBUG
	#define debug(s) printf("DEBUG: "); printf s;
#else
	#define debug(s) {}
#endif

