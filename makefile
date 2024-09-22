# compiler of choice
CC=gcc
# enabling warnings and other compile flags
CFLAGS=-W -Wall -g
EXEC=spc
# list of sources i need make to compile
OBJS=src/main.o src/spc.o
DEPS=src/spc.h

all: $(EXEC)

# simplified rule that makes all object files from their header and source
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c  -o $@ $<

# rule to make the final executable from the created object files
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(OBJS) -o $(EXEC)

# clean out the object files and the final executable
clean:
	rm $(EXEC) src/*.o
