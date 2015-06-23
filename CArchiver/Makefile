all:huffmancode.o bufferedbitfile.o huffman.o main.o
	gcc -std=c99  huffmancode.o bufferedbitfile.o huffman.o main.o -o carchiver
huffmancode.o:huffmancode.c
	gcc -std=c99 -c huffmancode.c
huffman.o:huffmancode.o bufferedbitfile.o huffman.c
	gcc -std=c99 -c huffman.c
bufferedbitfile.o:bufferedbitfile.c
	gcc -std=c99 -c bufferedbitfile.c
main.o:main.c
	gcc -std=c99 -c main.c
clean:
	rm -rf *.o carchiver
