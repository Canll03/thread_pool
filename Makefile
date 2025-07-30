CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -pthread

SRCS = threadPool.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = threadPoolDemo

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

# 1            $<              表示第一个匹配的依赖
# 2            $@            表示目标
# 3            $^              所有依赖
# 4           %.o: %.c     自动匹配
