#!/bin/sh

echo "::: Trage zwei Fische für Toni ein :::"
./fisch -n toni landmann
./fisch -n toni scheidewind
./fisch -l

echo "::: Trage zwei Fische für David ein :::"
./fisch -n david deins
./fisch -n david dzwei

echo "::: Erhöhe Davids Pensionskosten um 2 Euro :::"
./fisch -z david 2
./fisch -l

./fisch -l toni
./fisch -l david

echo "::: Rechnung Toni :::"
hd fischfile.dat
./fisch -d toni
hd fischfile.dat
echo "::: Rechnung David:::"
./fisch -d david
echo "::: Rechnungen wurden ausgegeben, jetzt sollte Fehler auftreten :::"
./fisch -l

