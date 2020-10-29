GPP = g++ -D _GLIBCXX_USE_CXX11_ABI=0 -m32 -std=c++11 -Ilib -Isrc
GCC = gcc -D _GLIBCXX_USE_CXX11_ABI=0 -m32 -Ilib -Isrc
LINK = $(GPP) -lstdc++
OUTFILE = "./YALP-MTA.so"

COMPILE_FLAGS = -c -O3 -fPIC -w

all: build

clean:
	-rm -f *~ *.o *.so
  
static: GPP = g++ -D _GLIBCXX_USE_CXX11_ABI=0 -m32 -std=c++11 -Ilib -Isrc -static-libgcc -static-libstdc++
static: GCC = gcc -D _GLIBCXX_USE_CXX11_ABI=0 -m32 -Ilib -Isrc -static-libgcc -static-libstdc++
static: LINK = $(GPP)
static: all

build: clean
	$(GPP) $(COMPILE_FLAGS) ./lib/sdk/*.cpp
	$(GPP) $(COMPILE_FLAGS) ./src/*.cpp
	$(LINK) -fshort-wchar -shared -o $(OUTFILE) *.o
