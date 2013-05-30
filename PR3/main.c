/* main.c
 *
 * IN DIESER DATEI MUSS NICHTS VERÄNDERT WERDEN!!
 * Zur Bearbeitung der Aufgabe siehe client.c
 *
 * Diese Datei enthält die nötige Funktionalität, um Daten über das MQTT-
 * Protokoll mit einem Broker zu kommunizieren. Dafür müssen beim Starten des
 * Programms Host, Port und (optional) eine ID zur Identifizierung des Clients
 * angegeben werden.
 * Die Funktionen in dieser Datei kümmern sich um den Verbindungsaufbau und das
 * Empfangen und senden von Nachrichten. Dafür rufen sie die Funktionen aus der
 * client.c auf.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>
#include <pthread.h>

#include "client.h"

const char *topic_name = "techgi4/msgboard";

pthread_mutex_t mutex;

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *mosq_message)
{
	pthread_mutex_lock(&mutex);
	message_received(mosq_message->payload, mosq_message->payloadlen);
	pthread_mutex_unlock(&mutex);
	
	fflush(stdout);
}

void on_connect(struct mosquitto *mosq, void *userdata, int result)
{
	if(!result){
		// subscribe to chat topic on successful connect.
		mosquitto_subscribe(mosq, NULL, topic_name, 2);
	}else{
		printf("Connect failed\n");
	}
}

void on_subscribe(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	printf("Connected\n", mid, granted_qos[0]);
}

void on_log(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	printf("%s\n", str);
}

int main(int argc, char *argv[])
{
    int port, client_id, buffer_size;
	int rc;
	struct mosquitto *mosq;
	
	void* buffer;
	
	char line[1024];
	const char *name;
	
	//prepare arguments
	if(argc < 3) {
	    printf("usage: %s host port [id]\n", argv[0]);
	    return 0;
	}
	
	port = atoi(argv[2]);
	client_id = -1;
	if(argc > 3) {
	    client_id = atoi(argv[3]);
	}
	
	pthread_mutex_init(&mutex, NULL);
	
	name = init(client_id);
	
    //start mosquitto stuff
	mosquitto_lib_init();

	mosq = mosquitto_new(name, true, NULL);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_subscribe_callback_set(mosq, on_subscribe);
	mosquitto_message_callback_set(mosq, on_message);

	rc = mosquitto_connect_async(mosq, argv[1], port, 60);
	mosquitto_loop_start(mosq);

	while(1) {
	    fgets(line, 1024, stdin);
	    
	    pthread_mutex_lock(&mutex);
	    buffer = message_entered(line, &buffer_size);
	    pthread_mutex_unlock(&mutex);
	    
	    int sent_mid = -1;
	    mosquitto_publish(mosq, &sent_mid, topic_name, buffer_size, buffer, 0, false);
	    
	    message_sent(buffer, buffer_size);
	}
	
	mosquitto_disconnect(mosq);
	mosquitto_loop_stop(mosq, false);

	mosquitto_lib_cleanup();
	
	cleanup();
	
	return 0;
}
