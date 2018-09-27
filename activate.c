#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <unistd.h>
#include "MQTTClient.h"

/*
	Below, there are three parameters you must customize in order to
	activate a new device:

	 - <hostname> - Get this information from your Murano Product settings.
                    Strip off the protocol and the trailing slash.
					Ex.

						mqtt://as98jn80n2n9b00000.exosite.io/

						    becomes

						as98jn80n2n9b00000.exosite.io

	 - <port> 	  - This information is obtained in the Murano Product settings.
					The value is either 8883 (default) or 443.

	 - <id>       - This is the unique id (often the serial number) of your device.
					Ex.

						00:f4:c2:95:ac:01

    Example #defines:

    #define ADDRESS             "ssl://f5330e4s8cho0000.m2.exosite.io:8883"
    #define ACTIVATE_TOPIC      "$provision/clib-test-10"
*/

#define ADDRESS             "ssl://<hostname>:<port>"
#define ACTIVATE_TOPIC      "$provision/<id>"
#define CLIENTID            ""
#define TOKEN               ""
#define PAYLOAD             ""
#define QOS                 1
#define TIMEOUT             10000L

volatile MQTTClient_deliveryToken deliveredtoken;

int activated = 0;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    printf("Provisioning successful.\n");
    printf("Murano token: ");
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    activated = 1;
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);

    if (rc != MQTTCLIENT_SUCCESS)
    {
        printf("MQTTClient_create() failed, return code %d\n", rc);
        exit(-1);
    }
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = "";
    conn_opts.password = TOKEN;
    conn_opts.ssl = &ssl_opts;
    conn_opts.ssl->trustStore = "./trusted.crt";

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    printf("connecting...\n");
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    printf("publishing activation request...\n");
    MQTTClient_publishMessage(client, ACTIVATE_TOPIC, &pubmsg, &token);

    printf("sleeping until murano responds with token...\n");
    while(!activated) usleep(2500);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
