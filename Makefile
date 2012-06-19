CXX=clang++
# CFLAGS+= -stdlib=libc++ -lc++abi
CFLAGS+= -std=c++11 -O3
HEADERS= $(wildcard *.h)

listcomp: main.cc $(HEADERS)
	$(CXX) $(CFLAGS) main.cc -o $@

clean:
	rm listcomp