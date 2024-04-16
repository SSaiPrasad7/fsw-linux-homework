client1:
	gcc task1/client_multi_threaded.c -o client1

client2:
	gcc task2/client_multi_threaded.c -o client2

clean:
	rm -f client1 client2