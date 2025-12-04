all: clean search

run: search
	@echo "Running search..."
	@echo ""
	./search

clean:
	@echo "Cleaning up..."
	rm -f main.o functions.o thread_functions.o
	rm -f search
	rm -f thread_*.log
	@echo "Clean complete!"

search: main.o functions.o thread_functions.o
	@echo "Linking search..."
	gcc -pthread -o search main.o functions.o thread_functions.o
	@echo "Build successful!"

main.o: main.c search.h
	@echo "Compiling main.c..."
	gcc -Wall -Wextra -pthread -std=c11 -c main.c -o main.o

functions.o: functions.c search.h
	@echo "Compiling functions.c..."
	gcc -Wall -Wextra -pthread -std=c11 -c functions.c -o functions.o

thread_functions.o: thread_functions.c search.h
	@echo "Compiling thread_functions.c..."
	gcc -Wall -Wextra -pthread -std=c11 -c thread_functions.c -o thread_functions.o

.PHONY: all run clean