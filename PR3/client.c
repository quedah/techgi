/* client.c
 *
 * Zur Bearbeitung der Aufgabe müssen die Funktionen in dieser Datei
 * implementiert werden.
 * Diese Funktionen werden durch die Implementierung zu den entsprechenden Zeit-
 * punkten aufgerufen.
 */

#include <stdlib.h>

/* wird einmal nach start des Programms aufgerufen und bekommt die ID übergeben
 * (d.i. das 3. Kommandozeilen-Argument). Diese ID kann genutzt werden, um den
 * Client innerhalb des Systems zu identifizieren.
 * Die Funktion soll falls nötig Initialisierungslogik enthalten und muss einen
 * eindeutigen Namen zurückgeben, mit dem sich der Client beim Broker anmeldet.
 *
 * Argumente:
 *   cid - die ID, die beim Start des Programms angegeben wurde. (-1, falls
 *         keine ID angegeben wurde)
 *
 * Rückgabewert:
 *   Der Name des clients als c-String. Dieser sollte für jeden Client ein-
 *   deutig sein.
 */
const char* init(int cid) {
    //TODO: implement
    return "Testclient 1";
}

/* wird einmal vor Ende des Programms aufgerufen. Diese Funktion kann ggf.
 * genutzt werden, um Ressourcen freizugeben.
 */
void cleanup() {
    //TODO: Implement
}

/* wird aufgerufen, sobald der Client eine Nachricht empfängt.
 * 
 * Argumente:
 *   data - Pointer auf die empfangenen Daten
 *   size - die Länge der Daten
 */
void message_received(void *data, size_t size) {
    //TODO: Implement
}

/* wird aufgerufen, sobald im Terminal eine Nachricht eingegeben und mit <Enter>
 * bestätigt wurde. Eine Implementierung dieser Funktion muss die übergebene
 * Nachricht ggf. mit Zusatzinformationen in einen geeigneten Puffer schreiben
 * und einen Pointer auf diesen Speicher zurückgeben. Zusätzlich muss die Größe
 * dieses Puffers in den durch 'buffer_size' referenzierten Speicher geschrieben
 * werden.
 *
 * Argumente:
 *   message        - die eingegebene Zeichenkette (max. 1023 Zeichen)
 *   buffer_size    - zeigt auf den Speicher, in den die Größe des Rückgabe-
 *                    Puffers geschrieben werden muss
 *
 * Rückgabewert:
 *   Die Funktion muss einen Pointer auf die zu sendenden Daten zurückgeben. Die
 *   Größe dieser Daten muss in den durch 'buffer_size' referenzierten Speicher
 *   geschrieben werden.
 */
void* message_entered(char *message, int *buffer_size) {
    //TODO: Implement
    *buffer_size = 0; //bisher keine Daten
    return NULL;
}

/* wird aufgerufen, sobald die durch 'message_entered' zurückgegebenen Daten
 * verschickt wurden.
 * Hier können ggf. reservierte Resourcen freigegeben werden.
 * 
 * Argumente:
 *   buffer         - Zeiger auf die Daten; wurde von 'message_entered'
 *                    zurückgegeben
 *   buffer_size    - Größe des Puffers; wurde in 'message_entered' gesetzt
 */
void message_sent(void* buffer, int buffer_size) {
    //TODO: (falls nötig) Implement
}

