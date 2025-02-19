.PHONY: all clean

all:
    @if [ ! -f "CMakeLists.txt" ]; then \
        echo "CMakeLists.txt not found"; \
        exit 1; \
    fi
    @mkdir -p Buildfiles
    @cd Buildfiles && cmake .. && make
    @cp Buildfiles/build/giti .

clean:
    @rm -rf Buildfiles
    @rm -f giti
