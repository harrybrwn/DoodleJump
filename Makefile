CXX=gcc
CMAKE=cmake
RMFLAGS=-r
RMDIR=$(RM)

# Windows settings
ifeq ($(OS), Windows_NT)
RMFLAGS=/S /Q
RMDIR=rmdir
CMAKE=cmake -G "MinGW Makefiles"
endif

all: build/DoodleJump

clean:
	$(RM) $(RMFLAGS) build

build/DoodleJump: build/Makefile
	$(MAKE) -C build

build/Makefile: build lib/sfml
	cd build && $(CMAKE) ..

build:
	mkdir build

lib/sfml:
	git clone --depth=1 -b 2.5.1 https://github.com/SFML/SFML.git $@

.PHONY: all clean
