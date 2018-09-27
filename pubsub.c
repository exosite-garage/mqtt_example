#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <unistd.h>
#include <time.h>
#include "MQTTClient.h"

#define ADDRESS             "ssl://<hostname>:<port>"
#define TOKEN               "<MURANO_TOKEN>"
#define CLIENTID            ""
#define TOPIC               "$resource/data_in"
#define QOS                 1
#define TIMEOUT             10000L
#define PRINT_CONFIRMATION   0 // change to 1 if delivery messages are to be printed to stdout

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    if (PRINT_CONFIRMATION)
        printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
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

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    time_t t;
    srand((unsigned) time(&t));

    while (1) {
        char payload[16];

        sprintf(payload, "{\"001\": %d}", rand() % 100);
        pubmsg.payload = payload;
        pubmsg.payloadlen = strlen(payload);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        // printf("Waiting for up to %d seconds for publication of %s\n"
        //         "on topic %s for client with ClientID: %s\n",
        //         (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Message with delivery token %d delivered: %s\n", token, payload);
        sleep(1);
    }
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}