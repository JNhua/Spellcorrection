INC_DIR:= ./inc/ ./inc/tcpnet/ ./inc/thread/ ./inc/timer/ ./inc/cache/ ./inc/spellcorrect/
SRC_DIR:= ./src ./src/tcpnet ./src/thread ./src/timer ./src/cache/ ./src/spellcorrect/
SRCS:=$(wildcard ./src/*.cpp) $(wildcard ./src/tcpnet/*.cpp) $(wildcard ./src/thread/*.cpp) $(wildcard ./src/timer/*.cpp) $(wildcard ./src/cache/*.cpp) $(wildcard ./src/spellcorrect/*.cpp)

OBJS:= $(patsubst %.cpp, %.o, $(SRCS))
LIBS:= -lpthread -ljson

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:=./bin/main

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
