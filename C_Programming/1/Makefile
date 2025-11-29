SHELL = cmd
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
TARGET = hello_aiot.exe
BUILDDIR = build

# Find all C files in src folder
SOURCES = $(wildcard src/*.c)

# Default target - build the program
build: $(BUILDDIR)/$(TARGET)

# Create executable in build folder
$(BUILDDIR)/$(TARGET): $(SOURCES)
	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)
	$(CC) $(CFLAGS) $(SOURCES) -o $(BUILDDIR)/$(TARGET)
	@echo "Build successful: $(BUILDDIR)/$(TARGET)"

# Clean build artifacts
clean:
	@del /S /Q $(BUILDDIR) 2>nul || exit 0
	@echo "Clean complete"
# Rebuild everything
rebuild: clean build
