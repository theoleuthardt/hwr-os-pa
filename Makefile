CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c11
LDFLAGS = -pthread
TARGET = search
SOURCES = main.c functions.c thread_functions.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = search.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build successful!"

%.o: %.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@echo "Running $(TARGET)..."
	@echo ""
	./$(TARGET)

clean:
	@echo "Cleaning up..."
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm -f thread_*.log
	@echo "Clean complete!"

.PHONY: all run clean