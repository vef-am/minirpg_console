CXX = g++
CXXFLAGS = -D_JUDGE_ -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Wno-sign-compare -std=c++17 -MMD -MP

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(OBJS:.o=.d)
TARGET = bin/minirpg_console
TEST_EFFECT_TARGET = bin/test_effect
TEST_EFFECT_SRCS = tests/test_effect.cpp src/effect.cpp

.PHONY: all test-effect run-test-effect clean

all: $(TARGET)

test-effect: $(TEST_EFFECT_TARGET)

run-test-effect: $(TEST_EFFECT_TARGET)
	$(TEST_EFFECT_TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o $@ $(CXXFLAGS)

$(TEST_EFFECT_TARGET): $(TEST_EFFECT_SRCS)
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@ $(CXXFLAGS)

src/%.o: src/%.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

clean:
	rm -f src/*.o $(DEPS) $(TARGET) $(TEST_EFFECT_TARGET)

-include $(DEPS)