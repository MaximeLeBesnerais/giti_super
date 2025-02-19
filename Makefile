# Makefile for giti project

# Default target
.PHONY: all
all: build

# Build target
.PHONY: build
build:
	@if [ ! -f "CMakeLists.txt" ]; then \
		echo "CMakeLists.txt not found"; \
		exit 1; \
	fi
	cmake -B build
	cd build && make

# Clean target
.PHONY: clean
clean:
	rm -rf build

# Run target
.PHONY: run
run:
	@if [ ! -f "build/build/giti" ]; then \
		echo "giti not found"; \
		exit 1; \
	fi
	./build/build/giti

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  build  - Build the project (default)"
	@echo "  clean  - Remove build directory"
	@echo "  run    - Run the giti executable"
	@echo "  help   - Show this help message"
