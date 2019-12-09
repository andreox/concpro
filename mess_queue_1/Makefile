all : main client server

client : client.c header.h
	gcc -o client client.c

server : server.c header.h
	gcc -o server server.c -lpthread

main : main.c header.h
	gcc -o main main.c

clean :
	rm -f *.o
	rm -f main client server