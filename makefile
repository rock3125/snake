
SOURCE := main.c

OBJ := obj/main.o

.PHONY : build clean

# sudo apt-get install libncurses-dev
build :
	rm -f *.o obj/*.o snake
	gcc $(SOURCE) -c -fPIC
	mv *.o ./obj/
	gcc -o snake $(OBJ) -lncurses

clean :
	rm -f *.o obj/*.o snake
