main : main.c procedure.o monitor_sc.o
	gcc -o main main.c procedure.o monitor_sc.o

procedure.o : procedure.c
	gcc -c procedure.c

monitor_sc.o : monitor_sc.c
	gcc -c monitor_sc.c 
	
clean :
	rm -f *.o
	

