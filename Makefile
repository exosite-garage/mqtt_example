
publish: publish.c Makefile
	gcc publish.c -Wall -g -lpaho-mqtt3cs -o publish
