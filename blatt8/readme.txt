#########################################################################
#		Betriebssysteme und Rechnerarchitekturen		#
#		Sommersemester 2012					#
#		Übungsblatt 8 - Einzelabgabe, 22.06.2012		#
# 									#
# 		Soeren Kroell						#
#########################################################################


Aufgabe 8.1 a)

	Lösungen dazu befinden sich im Ordner "a". Folgende Dateien sind 
	vorhanden:
	- inc/soundex.h
	- inc/soundex-test.h
	- makefile
	- soundex.c
	- soundex-test.c
	
	Ausführbare Datei erzeugen durch "make soundex-test"
	
	Starten des Programms:
	./soundex-test <param> <param>

	Das Programm liefert ein "uebereinstimmung" oder "abweichung" 
	in Abhängigkeit der Parameter als Kommandozeilenausgabe zurück.
	
	Beispiel:
	$ ./soundex-test soundex soundeggs
	uebereinstimmung

	$ ./soundex-test mandli schockchi
	abweichung



Aufgabe 8.1 b)
	
	Lösungen dazu befinden sich im Ordner "b". Folgende Dateien sind 
	vorhanden:
	- inc/soundex.h
	- inc/souls.h
	- makefile
	- soundex.c
	- souls.c

	Ausführbare Datei erzeugen durch "make souls"

	Starten des Programms:
	./souls <verzeichnisname> <buergername>

	Das Programm gibt, sofern eine Übereinstimmung gefunden wurde, 
	den Dateinamen sowie die Größe der Datei in Bytes zurück.

	Beispiel:
	$ ./souls buerger soeren
	soeren.txt (0 Bytes)



Aufgabe 8.1 c)
	
	Lösungen dazu befinden sich im Ordner "c". Folgende Dateien sind 
	vorhanden:
	- inc/soundex.h
	- inc/souls.h
	- makefile
	- soundex.c
	- souls.c
	
	Ausführbare Datei erzeugen durch "make souls"

	Starten des Programms:
	./souls <verzeichnisname> <buergername>
	
	Das Programm gibt, sofern eine Übereinstimmung gefunden wurde, 
	den Dateinamen, die Größe der Datei in Bytes sowie den Dateipfad 
	zurück.

	Beispiel:
	$ ./souls buerger soeren
	soeren.txt (0 Bytes) (buerger/vip/soeren)
	soeren.txt (0 Bytes) (buerger/soeren)
	
