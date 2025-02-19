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
	rm giti

.PHONY: fclean
fclean: clean
	rm -rf build

.PHONY: get
get: build
	@if [ ! -f "build/build/giti" ]; then \
		echo "giti not found"; \
		exit 1; \
	fi
	@cp build/build/giti ./giti
	@echo "giti binary copied to current directory"

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  build  - Build the project (default)"
	@echo "  get    - Copy the giti binary to the current directory"
	@echo "  fclean - Remove build directory and giti binary"
	@echo "  clean  - Remove giti binary"
	@echo "  run    - Run the giti executable"
	@echo "  help   - Show this help message"
