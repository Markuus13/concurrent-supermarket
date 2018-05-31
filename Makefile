CC=gcc
CFLAGS= -Wall -std=c11

all: compile exec clean

compile:
	@echo "Bulding objects..."
	$(CC) $(CFLAGS) ./src/main.c -o build/main
	@echo "Done Building!"

exec:
	@echo "Running main..."
	@echo "\n\n\n"
	@build/./main
	@echo "\n\n\n"
	@echo "Done running!"

clean:
	@echo "Cleaning..."
	@rm -rf build
	@mkdir build
	@echo "Done cleaning!"
