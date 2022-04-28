CC = gcc
CFLAGS = -g -Wall
TARGET = menun
TEST = test



all: $(TARGET) $(TEST) a

a:
	@echo "Practica One"
	./menun


$(TARGET) :$(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

$(READ) :$(TEST).c
	$(CC) $(CFLAGS) -o $(READ) $(READ).c


clean:
	$(RM) $(TARGET) $(TEST)

