
publish: publish.c Makefile
	gcc publish.c -Wall -g -lpaho-mqtt3cs -o publish

activate: activate.c Makefile
	gcc activate.c -Wall -g -lpaho-mqtt3cs -o activate

