CC=gcc
CXX=g++
MAKE=make
CMAKE=cmake
NPM=npm

CFLAGS=-Wall -march=native -mtune=native -std=gnu89 -Ilibs/webview/dist/usr/include $(shell pkg-config --cflags webkitgtk-6.0)
CXXFLAGS=$(CFLAGS)
LDFLAGS=-Llibs/webview/dist/usr/lib64 -lwebview $(shell pkg-config --libs webkitgtk-6.0) -lstdc++

SRC=$(wildcard src/**/*.c src/*.c)
OBJS=$(SRC:.c=.o)

OUTPUT=webview-example

all: libs css $(OUTPUT)

libs: libs/webview/dist/usr/lib64/libwebview.a

libs/webview/dist/usr/lib64/libwebview.a:
	cd libs/webview && mkdir -p build dist
	cd libs/webview/build && $(CMAKE) -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ..
	cd libs/webview/build && $(MAKE)
	cd libs/webview/build && $(MAKE) DESTDIR=../dist install
	cd libs/webview/dist/usr/lib64 && rm *.so

css:
	mkdir -p src/resources/dist
	$(NPM) run build

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

run: all
	./$(OUTPUT)

format: $(SRC)
	bash -c 'find . -type f -name *.c | grep -v libs | while read -r filename; do clang-format -i $$filename; done'

clean: $(OUTPUT)
	rm $(OUTPUT) $(OBJS)
	rm -rf src/resources/dist

clean-libs: libs
	rm -rf libs/webview/build libs/webview/dist
