# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Project structure
SRCDIR := src
OBJDIR := obj
BINDIR := bin
TARGET := $(BINDIR)/game

# Source files
SOURCES := $(SRCDIR)/main.cpp \
           $(SRCDIR)/character.cpp \
           $(SRCDIR)/test_object.cpp \
           $(SRCDIR)/chat/chat.cpp \
           $(SRCDIR)/tilemap/tilemap.cpp

# Object files
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Header files (for dependency tracking)
HEADERS := $(SRCDIR)/character.hpp \
           $(SRCDIR)/test_object.hpp \
           $(SRCDIR)/game_object.hpp \
           $(SRCDIR)/chat/chat.hpp \
           $(SRCDIR)/tilemap/tilemap.hpp

# Default target
all: $(TARGET)

# Create the target executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they don't exist
$(BINDIR):
	@mkdir -p $(BINDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/chat
	@mkdir -p $(OBJDIR)/tilemap

# Clean build files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Clean and rebuild
rebuild: clean all

# Run the game
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: rebuild

# Release build with optimizations
release: CXXFLAGS += -O3 -DNDEBUG
release: rebuild

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the game (default)"
	@echo "  clean    - Remove all build files"
	@echo "  rebuild  - Clean and rebuild"
	@echo "  run      - Build and run the game"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build with optimizations"
	@echo "  help     - Show this help message"

.PHONY: all clean rebuild run debug release help

# Include dependency files if they exist
-include $(OBJECTS:.o=.d)

# Generate dependency files
$(OBJDIR)/%.d: $(SRCDIR)/%.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM -MT $(@:.d=.o) $< > $@