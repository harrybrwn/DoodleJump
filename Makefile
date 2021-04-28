MKDIR=mkdir -p

all: build/DoodleJump

clean:
	$(RM) -r build

build/DoodleJump: build/Makefile
	cd build && $(MAKE)

build/Makefile: build lib/sfml
	cd build && cmake ..

build:
	$(MKDIR) build

lib/sfml:
	git clone --depth=1 -b 2.5.1 https://github.com/SFML/SFML.git $@

.PHONY: all clean
