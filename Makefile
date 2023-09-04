OUT:=circle-scores
CXXFLAGS:=-Wall -Wextra -std=c++11 -O2
LIBS:=-lcurl

SRC:=$(wildcard src/*.cpp)
OBJ:=$(subst src/,build/,$(SRC:.cpp=.o))

all: $(OUT)

$(OUT): $(OBJ)
	$(CXX) -o $(OUT) $(OBJ) $(LIBS)

$(OBJ): | build

build:
	mkdir $@

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf build $(OUT)

.PHONY: all clean
