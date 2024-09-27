OUT := circle-scores
CXXFLAGS := -Wall -Wextra -std=c++11 -O2
LIBS := -lcurl

SRC := $(wildcard src/*.cpp)
OBJ := $(subst src/,build/,$(SRC:.cpp=.o))

_ := $(if $(filter clean,$(MAKECMDGOALS)), \
	$(info rm -rf build $(OUT)) \
	$(shell rm -rf build $(OUT)))

all: $(OUT)

$(OUT): $(OBJ)
	$(CXX) -o $(OUT) $(OBJ) $(LIBS)

$(OBJ): | build

build:
	mkdir $@

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@:

.PHONY: all clean
