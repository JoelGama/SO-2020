all:
	gcc argusd.c auxiliares.c funcionalidades.c -Wall -o argusd
	gcc argus.c -Wall -o argus
	mkfifo "Bus"
	mkfifo "Cliente"

clean:
	rm -f argus
	rm -f Cliente
	rm -f argusd
	rm -f Bus
	rm -f log.txt
	rm -f pids.txt
	rm -f output.txt