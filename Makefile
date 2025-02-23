# Makefile for giti project

# Default target
.PHONY: all
all: build

# Build target
.PHONY: build
build:
	@if ! pkg-config --exists libgit2; then \
		echo "Error: libgit2 not found"; \
		echo "Please install libgit2 development files"; \
		exit 1; \
	fi
	@if [ ! -f "CMakeLists.txt" ]; then \
		echo "Error: CMakeLists.txt not found"; \
		exit 1; \
	fi
	cmake -B build
	cd build && make

# Install target
.PHONY: install
install: build
	@if [ ! -f "build/giti" ]; then \
		echo "Error: giti binary not found. Build failed?"; \
		exit 1; \
	fi
	@echo "Installing Giti..."
	@sudo cp build/giti /usr/bin/giti
	@mkdir -p ~/.giti
	@if [ ! -f ~/.giti/config.json ]; then \
		cp ./config.json ~/.giti/config.json; \
		echo "Created default config at ~/.giti/config.json"; \
	else \
		echo "Config file already exists at ~/.giti/config.json"; \
	fi
	@echo "Giti installed successfully!"
	@echo "Run 'giti -h' for usage information"

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

# Uninstall target
.PHONY: uninstall
uninstall:
	@echo "Uninstalling Giti..."
	@sudo rm -f /usr/bin/giti
	@rm -rf ~/.giti
	@echo "Giti uninstalled successfully!"

.PHONY: get
get: build
	@if [ ! -f "build/giti" ]; then \
		echo "Error: giti not found"; \
		exit 1; \
	fi
	@cp build/giti ./giti
	@echo "giti binary copied to current directory"

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  build     - Build the project (default)"
	@echo "  install   - Install giti to system and create config"
	@echo "  uninstall - Remove giti from system"
	@echo "  get       - Copy the giti binary to current directory"
	@echo "  fclean    - Remove build directory and giti binary"
	@echo "  clean     - Remove giti binary"
	@echo "  help      - Show this help message"