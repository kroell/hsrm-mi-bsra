/*
 * libmemprobe: Probeweises Zugreifen auf beliebige Adressen.
 * Zugriffe auf illegale Adressen werden durch einen
 * Signal-Handler und mit setjmp()/longjmp() abgefangen.
 */


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>
#include <assert.h>

#include "../include/memprobe.h"


/*
 * Zuletzt gesicherter Prozessorzustand:
 */
static jmp_buf prozessor_status;

/*
 * Installieren des Signal Handlers: Ohne besondere
 * Massnahmen fuert ein Signal in der Regel zum 
 * sofortigen Programmabbruch. Mit Hilfe eines
 * "Signal Handlers" kann man Signale jedoch abfangen.
 * Diese Routine installiert einen Signal Handler fuer
 * ein gegebenes Signal:
 */
static int register_signal_handler(
    int sig_nr,                 /* Nummer des Signals */
    void (*signalhandler)(int)) /* Aufzurufender Handler (Funktionszeiger) */
{
    sigset_t s;
    sigemptyset (&s);
    sigaddset (&s, sig_nr);;
    sigprocmask(SIG_UNBLOCK, &s, 0);

    if(signal(sig_nr, signalhandler) == SIG_ERR)
        return -1;
    return 0;
}

/*
 * Der eigentliche Signal Handler: Wird vom Betriebssystem
 * aufgerufen wenn das Programm ein "Signal" erhalten hat.
 * Hier: Der Handler wird fuer das Signal "Segment
 * Violation" (SIGSEGV) registriert, d.h. er wird immer
 * dann aufgerufen, wenn das Programm einen
 * unererlaubten Speicherzugriff gemacht hat.
 */
static void handle_sig(int signr)
{
    /*
     * (siehe "man assert")
     * Dieser Signal Handler wird fuer SIGSEGV
     * registriert. singnr muss deshalb gleich SIGSEGV
     * sein, oder irgend etwas ist hier oberfaul....
     */
    assert(signr == SIGSEGV);
    /*
     * Registriere den Signal Handler fuer das
     * naechste Signal gleich wieder:
     */
    register_signal_handler(signr, handle_sig);

    /*
     * Stelle den zuletzt mit setjmp() gesicherten
     * Prozessorzustand wieder her:
     */
    longjmp(prozessor_status, 1);
}

/*
 * Probeweises Zugreifen auf eine Speicher-Adresse.
 * Rueckgabe: 0 -> kein Zugriff (Adresse ungueltig)
 *            1 -> Lesezugriff erfolgreich
 *            2 -> Schreibzugriff erfolgreich
 */

int memprobe(void *adresse)
{
    volatile char sinke;
    char *probierzeiger;
    volatile int ergebnis = 0;

    probierzeiger = (char*)adresse;

    /*
     * Registriere den Signal Handler fuer
     * SIGSEGV (= Speicherschutzverletzung).
     */
    if(register_signal_handler(SIGSEGV, handle_sig) < 0)
    {
        perror("sigaction");
        exit(1);
    }

    /*
     * Rette den Prozessorzustand: Programm macht hier
     * weiter, wenn eine Speicherschutzverletzung
     * aufgetreten ist.
     */
    if(!setjmp(prozessor_status))
    {
        /* versuche einen Lesezugriff */
        sinke = *probierzeiger;
        /* hat geklappt -> ergebnis = 1 */
        ergebnis = 1;
        /* versuche einen Schreibzugriff: schreibe
         * zuvor gelesenes Datum zur�ck, damit der
         * Speicherinhalt intakt bleibt. */
        *probierzeiger = sinke;
        /* hat geklappt -> ergebnis = 2 */
        ergebnis = 2;
    }
    /* Je nachdem, ob das Signal beim Lesezugriff,
    * beim Schreibzugriff oder gar nicht ausgeloest wurde,
    * hat ergebnis jetzt den Wert 0, 1 oder 2 */
    return(ergebnis);
}

