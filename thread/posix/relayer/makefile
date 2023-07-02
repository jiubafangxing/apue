CFLAGS+=-pthread
LDFLAGS+=-pthread
all: relayer

relayer:  relayer.o main.o
	gcc -g  $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o relay
