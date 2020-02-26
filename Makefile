PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS = main.o allocator.o utest.o low_level.o

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2 -Werror -Wall -Wextra  -Wstrict-prototypes -Wmissing-prototypes
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all: blockAllocator

blockAllocator:	$(OBJS)
	$(CXX) -o $@ $^

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr blockAllocator $(OBJS)
	
#test: blockAllocator	
#	./blockAllocator
