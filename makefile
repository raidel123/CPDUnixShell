all: myshell myls

myshell: main.c
	gcc -Wall -pedantic -o $@ $<

myls: myls.c
	gcc -Wall -pedantic -o $@ $<

clean:
	rm myshell myls;
