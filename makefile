CC = g++

all:	scrap

scrap: main.o ui.o map.o being.o item.o game.o event.o rng.o
	$(CC) -o $@ $^ -lncurses

scrap.tar.gz: *.cpp *.h makefile readme.txt
	tar cfz scrap.tar.gz *.cpp *.h makefile readme.txt

clean:
	rm -f scrap *.o

main.o: main.cpp main.h ui.h map.h being.h item.h game.h event.h rng.h
ui.o:	ui.cpp ui.h
map.o:	map.cpp map.h ui.h
being.o:	being.cpp being.h main.h ui.h map.h item.h event.h rng.h
item.o:	item.cpp item.h
game.o:	game.cpp game.h main.h ui.h map.h being.h item.h event.h
event.o:	event.cpp event.h
rng.o:	rng.cpp rng.h
