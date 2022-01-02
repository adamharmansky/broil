all:
	cc broil.c -lImlib2 -o broil

run: all
	./broil troll.webp -s 100

install: all
	install broil /usr/bin

clean:
	rm -f broil
