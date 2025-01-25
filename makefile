# compiler of choice
CC=gcc
# enabling warnings and other compile flags
CFLAGS=-W -Wall -g
EXEC=spn
# list of sources i need make to compile
OBJS=src/main.o src/spc.o src/cli.o
DEPS=src/spc.h src/cli.h

all: $(EXEC)

# simplified rule that makes all object files from their header and source
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c  -o $@ $<

# rule to make the final executable from the created object files
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

encrypt: $(EXEC)
	./$(EXEC) -e 1110111011101110 0001001000110100
decrypt: $(EXEC)
	./$(EXEC) -d 1110001011010100 0001001000110100
# test decryption and encryption
test: $(EXEC)
	./$(EXEC) -t 1010101010101010 0001001000110100
# clean out the object files and the final executable
.PHONY: clean
clean:
	rm $(EXEC) src/*.o
