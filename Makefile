
pubsub: pubsub.c Makefile
	gcc pubsub.c -Wall -g -lpaho-mqtt3cs -o pubsub

activate: activate.c Makefile
	gcc activate.c -Wall -g -lpaho-mqtt3cs -o activate

