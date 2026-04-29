CPP = g++
OPTIONS = -D_JUDGE_ -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Wno-sign-compare -std=c++17

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
TARGET = bin/minirpg_console

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CPP) $(OBJS) -o $(TARGET) $(OPTIONS)

%.o: %.cpp
	$(CPP) -c $< $(OPTIONS) -o $@

clean:
	rm -f src/*.o $(TARGET)