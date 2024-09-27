OUT := circle-scores
CXXFLAGS := -Wall -Wextra -std=c++11 -O2
LIBS := -lcurl

include config.mk

SRC := \
	src/args.cpp \
	src/base_request.cpp \
	src/credentials.cpp \
	src/log.cpp \
	src/main.cpp \
	src/post_request.cpp \
	src/processor.cpp \
	src/rankings.cpp \
	src/recent_scores.cpp \
	src/score.cpp \
	src/sig_handler.cpp \
	src/stdout_score_receiver.cpp \
	src/time.cpp \
	src/token.cpp \
	src/uleb128.cpp

ifneq ($(ENABLE_DISCORD_HOOK),0)
SRC += src/discord_score_receiver.cpp
CXXFLAGS += -DENABLE_DISCORD_HOOK
endif
ifneq ($(ENABLE_LAMS),0)
SRC += src/lams_score_receiver.cpp
CXXFLAGS += -DENABLE_LAMS
endif

OBJ := $(subst src/,build/,$(SRC:.cpp=.o))

_ := $(if $(filter clean,$(MAKECMDGOALS)), \
	$(info rm -rf build $(OUT)) \
	$(shell rm -rf build $(OUT)))

all: $(OUT)

$(OUT): $(OBJ)
	$(CXX) -o $(OUT) $(OBJ) $(LIBS)

$(OBJ): config.mk | build

build:
	mkdir $@

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@:

.PHONY: all clean
