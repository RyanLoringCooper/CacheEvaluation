CXX				= gcc
CXXFLAGS		= -Wall -O2 
SRCS			= $(patsubst src/%.c, %.c, $(wildcard src/*.c))
TESTS			= $(patsubst tests/%.c, %.c, $(wildcard tests/*.c))
OBJS			= $(SRCS:.c=.o)
TEST_OBJS		= $(TESTS:.c=.o)
OBJ_DIR			= obj/
BUILD_DIR		= bin/
VPATH			= tests src

all: tests srcs
	@echo Compiled all targets

srcs: $(OBJS) 
	@mkdir -p $(BUILD_DIR)
	@- $(foreach TEST,$(TESTS), \
		$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)$(TEST) $(patsubst %.o, $(OBJ_DIR)%.o, $(OBJS)) $(OBJ_DIR)$(TEST:.c=.o)\
	)

tests: $(OBJS) $(TEST_OBJS)
	@mkdir -p $(BUILD_DIR)
	@- $(foreach TEST,$(TESTS), \
		$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)$(TEST) $(patsubst %.o, $(OBJ_DIR)%.o, $(OBJS)) $(OBJ_DIR)$(TEST:.c=.o)\
	)

clean: 
	$(RM) -rf $(BUILD_DIR)
	$(RM) -rf $(OBJ_DIR)

%.o : %.c
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $(OBJ_DIR)$@ $<
