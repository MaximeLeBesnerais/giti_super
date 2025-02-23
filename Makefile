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
	@echo "Adding 'GITI_CONFIG' environment variable to your shell profile"
	@if [ -f ~/.bashrc ]; then \
		grep -qx "export GITI_CONFIG=~/.giti/config.json" ~/.bashrc || echo "export GITI_CONFIG=~/.giti/config.json" >> ~/.bashrc; \
	fi
	@if [ -f ~/.zshrc ]; then \
		grep -qx "export GITI_CONFIG=~/.giti/config.json" ~/.zshrc || echo "export GITI_CONFIG=~/.giti/config.json" >> ~/.zshrc; \
	fi
	@if [ -f ~/.profile ]; then \
		grep -qx "export GITI_CONFIG=~/.giti/config.json" ~/.profile || echo "export GITI_CONFIG=~/.giti/config.json" >> ~/.profile; \
	fi
	@if [ -f ~/.bash_profile ]; then \
		grep -qx "export GITI_CONFIG=~/.giti/config.json" ~/.bash_profile || echo "export GITI_CONFIG=~/.giti/config.json" >> ~/.bash_profile; \
	fi
	@echo "Attempted to add 'GITI_CONFIG' environment variable to your shell profile"
	@echo "Giti installed successfully!"
	@echo "Run 'giti' for usage information"

# Update target
.PHONY: update
update:
	@echo "Updating Giti..."
	var=$$(git pull); \
	if [ "$$var" = "Already up to date." ]; then \
		echo "Giti is already up to date!"; \
	else \
		@echo "Giti is being updated..."; \
		@make build; \
		sudo cp build/giti /usr/bin/giti; \
		echo "Giti updated successfully!"; \
	fi

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