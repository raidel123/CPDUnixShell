all: myshell myls

myshell: main.c
	gcc -w -o $@ $<

myls: myls.c
	gcc -w -o $@ $<

clean:
	rm myshell myls;
