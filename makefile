run:main.o MessageHeader.o FileReader.o
	g++ -o run main.o MessageHeader.o FileReader.o -L/usr/lib64/crazy -lcrazy_net
main.o:main.cc
	g++ -c main.cc -I./
MessageHeader.o:MessageHeader.cc
	g++ -c MessageHeader.cc -I./
FileReader.o:FileReader.cc
	g++ -c FileReader.cc -I./
clean:
	rm run main.o MessageHeader.o FileReader.o
