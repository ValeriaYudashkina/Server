CXX=g++
SRC_DIR=src
OBJ_DIR=obj
INCLUDE_DIR=include
PROJECT=server
STATIC=server_static
SANITIZED=server_san
DEBUG_BIN=$(PROJECT)_debug

CXXFLAGS=-O2 -Wall -DNDEBUG -std=c++17 -I./$(INCLUDE_DIR)
DBGFLAGS=-g -Og -I./$(INCLUDE_DIR)
SANFLAGS=-fsanitize=address -fsanitize=leak -fsanitize=undefined

LDFLAGS=-lboost_program_options -lcryptopp

SOURCES := $(SRC_DIR)/main.cpp $(SRC_DIR)/Interface.cpp $(SRC_DIR)/Logger.cpp $(SRC_DIR)/UserDatabase.cpp $(SRC_DIR)/DataProcessor.cpp $(SRC_DIR)/Authenticator.cpp $(SRC_DIR)/Server.cpp

OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

DEPS := $(INCLUDE_DIR)/Interface.h $(INCLUDE_DIR)/Logger.h $(INCLUDE_DIR)/UserDatabase.h $(INCLUDE_DIR)/DataProcessor.h $(INCLUDE_DIR)/Authenticator.h $(INCLUDE_DIR)/Server.h

.PHONY: all clean format static sanitize debug help

all: $(PROJECT)

static: $(STATIC)

sanitize: CXXFLAGS := $(DBGFLAGS) $(SANFLAGS)
sanitize: LDFLAGS += $(SANFLAGS)
sanitize: clean $(SANITIZED)

$(PROJECT) : $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(STATIC) : $(OBJECTS)
	$(CXX) -static $^ $(LDFLAGS) -o $@

$(SANITIZED) : $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(DEBUG_BIN) : $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	@mkdir -p $(OBJ_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

debug: CXXFLAGS := $(DBGFLAGS)
debug: clean $(DEBUG_BIN)

format:
	astyle $(SRC_DIR)/*.cpp $(INCLUDE_DIR)/*.h

clean:
	rm -f $(PROJECT) $(STATIC) $(SANITIZED) $(DEBUG_BIN) $(OBJ_DIR)/*.o *.orig
	@rmdir $(OBJ_DIR) 2>/dev/null || true