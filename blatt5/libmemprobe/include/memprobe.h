/*
 * Probeweises Zugreifen auf eine Speicher-Adresse.
 * Rueckgabe: 0 -> kein Zugriff (Adresse ungueltig)
 *            1 -> Lesezugriff erfolgreich
 *            2 -> Schreibzugriff erfolgreich
 */

#ifndef __LIBMEMPROBE_H__
#define __LIBMEMPROBE_H__

extern int memprobe(void *adresse);

#endif
