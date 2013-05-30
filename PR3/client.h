/* client.h
 *
 * Deklaration der Callbacks
 * Für Hinweise zur Implementierung siehe Kommentare in der client.c
 */

const char* init(int cid);

void cleanup();

void message_received(void *data, size_t size);

void* message_entered(char* message, int *buffer_size);

void message_sent(void* buffer, int buffer_size);
