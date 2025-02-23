# Makefile for giti project

# Default target
.PHONY: all
all: build

# Build target
.PHONY: build
build:
	@if ! pkg-config --exists libgit2; then \
		echo "libgit2 not found"; \
		echo "Please install libgit2"; \
		exit 1; \
	fi
	@if [ ! -f "CMakeLists.txt" ]; then \
		echo "CMakeLists.txt not found"; \
		exit 1; \
	fi
	cmake -B build
	cd build && make

# Clean target
.PHONY: clean
clean:
	@if [ -f "giti" ]; then \
		rm giti; \
	fi

.PHONY: fclean
fclean: clean
	@if [ -d "build" ]; then \
		rm -rf build; \
	fi

.PHONY: get
get: build
	@if [ ! -f "build/giti" ]; then \
		echo "giti not found"; \
		exit 1; \
	fi
	@cp build/giti ./giti
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
