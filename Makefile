SRC_PATH=./
SRC_FILE=main.cc MessageHeader.cc MessageParse.cc FileReader.cc 
OBJECT=FileReader.o MessageHeader.o MessageParse.o main.o 
PROJECT=chttp

$(PROJECT): $(OBJECT)
	g++ -o $(PROJECT) $(OBJECT) -L/usr/lib64/crazy -lcrazy_net -I./ -g
$(OBJECT): $(SRC_FILE)
	g++ -c $(SRC_FILE) -I./ -g

.PHONY : clean
clean:
	-rm $(PROJECT) $(OBJECT)
