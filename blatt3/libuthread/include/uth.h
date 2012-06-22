#ifndef __UTH_H__
#define __UTH_H__

/**
 *  @file       uth.h
 *  @brief      Micro Thread Bibliothek
 *
 * Dies ist eine Bibliothek zur Unterstuetzung von einfachem, @b kooperativem Multi-Threading
 * auf Anwenderebene.
 *
 */

/**
 * @brief Initialisieren der Micro Thread Bibliothek
 *
 * Diese Funktion initilisiert die Micro Thread Bibliothek und startet
 * den ersten Thread. Dieser kann dann weitere Threads erzeugen und starten.
 * Diese Funktion kehrt erst zurueck, wenn alle Threads der Threadbibliothek beendet sind.
 *
 * @param num Maximale Anzahl an Threads, die erzeugt werden koennen
 * @param entry Eintrittspunkt des ersten Threads
 * @retval nichts
 */

void uth_init(unsigned int num, void (*entry)(void));


/**
 * @brief Erzeugen eines Thread
 *
 * Diese Funktion erzeugt einen neuen Thread. Die noetigen Datenstrukturen werden
 * angelegt und ein @b handle wird zurueckgeliefert, mit dem der Thread kuenftig
 * referenziert werden kann. Falls nicht genuegend Ressourcen vorhanden sind, um
 * den Thread anzulegen (z.B. zu wenig Speicher fuer den Stack), so liefert die
 * Funktion @c NULL zurueck.
 *
 * @param entry Eintrittspunkt des neuen Thread
 * @retval handle Handle zum Referenzieren des Threads
 * @retval NULL Erzeugen des Threads schlug fehl
 */

void* uth_thread_create(void (*entry)(void));


/**
 * @brief Loeschen eines Thread
 *
 * Diese Funktion loescht einen Thread. Die verwendeten Datenstrukturen werden
 * freigegeben. Der zu loeschende wird durch sein @b handle referenziert. Es
 * kann sich hierbei auch um den Thread des Aufrufers handeln. Wenn der letzte
 * Thread der Micro Thread Bibliothek geloescht wird, kehrt die Funktion @c uth_init()
 * zu ihrem Aufrufer zurueck.
 *
 * @param handle Handle des zu loeschenden Threads
 * @retval 0 Thread erfolgreich geloescht
 * @retval -1 @c handle identifiziert keinen bekannten Thread
 */

int uth_thread_terminate(void* handle);


/**
 * @brief Eigenes Thread-Handle ermitteln
 *
 * Diese Funktion liefert den Handle des Threads des Aufrufers zurueck. Die Identifikation
 * des Threads erfolgt anhand des Stackpointers des Aufrufers. Dieser muss innerhalb eines
 * fuer jeden Thread individuellen Adressbereiches liegen.
 *
 * @retval handle Handle des Threads des Aufrufers
 * @retval 0 Aufruf von einem Thread, der nicht unter der Verwaltung der Micro Thread Bibliothek liegt
 */

void* uth_get_handle(void);


/**
 * @brief Thread-Umschaltung
 *
 * Diese Funktion blockiert den Thread des Aufrufers und schaltet zu einem (in der Regel andern)
 * Thread um, der daraufhin aktiv wird. Wird @c handle als @c NULL uebergeben, so wird der naechste
 * Thread anhand einer FIFO-Reihenfolge bestimmt. Falls es nur einen Thread im System gibt, so kann
 * dies auch der Aufrufer selbst sein. Wird mit @c handle der Thread-Handle eines existierenden Thread
 * angegeben, so wird dieser Thread als naechster ausgefuehrt. Falls dieser nicht gueltig existiert, wird
 * wieder in FIFO-Reihenfolge fortgefahren.
 *
 * @param handle Handle des naechsten zu aktivierenden Threads. \b NULL falls das System die Auswahl treffen soll
 * @retval nichts
 */

void uth_thread_next(void *handle);

/**
 * @brief Eigenen Thread terminieren (Selbstmord)
 *
 * Dieses Makro beendet den Thread des Aufrufers
 *
 * @retval nichts
 */

#define uth_suicide() uth_thread_terminate(uth_get_handle())

#endif
