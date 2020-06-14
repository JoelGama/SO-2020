all:
	gcc servidor.c auxiliares.c -Wall -o servidor
	gcc cliente.c -Wall -o cliente
	mkfifo "Bus"
	mkfifo "Cliente"

clean:
	rm -f cliente
	rm -f Cliente
	rm -f servidor
	rm -f Bus
	rm -f log.txt
	rm -f pids.txt