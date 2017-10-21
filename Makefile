CXX				= gcc
LIBS			= -lm -lc
CXXFLAGS		= -Wall -O0 -Wno-unused-but-set-variable $(LIBS)
PRG_SUFFIX		= .bin
TEST_SUFFIX		= .test
SRCS			= $(patsubst src/%.c, %.c, $(wildcard src/*.c))
SRC_OBJS		= $(SRCS:.c=.o)
PRGS			= $(patsubst %.c, %, $(SRCS))
HEADERS			= $(patsubst src/headers/%.c, %.c, $(wildcard src/headers/*.c))
HEADER_OBJS		= $(HEADERS:.c=.o)
TESTS			= $(patsubst tests/%.c, %.c, $(wildcard tests/*.c))
TEST_OBJS		= $(TESTS:.c=.o)
TEST_PRGS		= $(patsubst %.c, %, $(TESTS))
BINS			= $(patsubst %, %$(PRG_SUFFIX), $(PRGS))
TEST_BINS		= $(patsubst %, %$(TEST_SUFFIX), $(TEST_PRGS))
OBJ_DIR			= obj/
BUILD_DIR		= bin/
VPATH			= tests src src/headers

.SECONDEXPANSION:
BIN = $@

all: srcs tests

srcs: $(BINS)
	@echo Compiled sources

tests: $(TEST_BINS)
	@echo Compiled tests

%$(PRG_SUFFIX): $(SRC_OBJS)  $(HEADER_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)$(BIN) $(patsubst %.o, $(OBJ_DIR)%.o, $(patsubst %$(PRG_SUFFIX), %.o, $@)) $(patsubst %.o, $(OBJ_DIR)%.o, $(HEADER_OBJS))

%$(TEST_SUFFIX): $(TEST_OBJS) $(HEADER_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)$(BIN) $(patsubst %.o, $(OBJ_DIR)%.o, $(patsubst %$(TEST_SUFFIX), %.o, $@)) $(patsubst %.o, $(OBJ_DIR)%.o, $(HEADER_OBJS))

clean: 
	$(RM) -rf $(BUILD_DIR)
	$(RM) -rf $(OBJ_DIR)

%.o : %.c
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $(OBJ_DIR)$@ $<
