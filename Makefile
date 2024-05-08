explorer: build/main.o build/fs.o
	mkdir -p bin
	$(CC) -I src/fs.h $^ -o bin/$@

build/main.o:
	mkdir -p build
	$(CC) -c $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) src/main.c -o $@

build/fs.o:
	mkdir -p build
	$(CC) -c $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) src/fs.c -o $@

clean:
	rm -rv build bin || exit 0
