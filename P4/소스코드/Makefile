CC = gcc
CFLAGS = -g -Wall
TARGET = a.out
OBJS = \
       ui.o\
       page-replacement-algorithms.o\
       main.o\
 

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

ui.o : ui.c
	$(CC) -c -o ui.o ui.c

page-replacement-algorithms.o : page-replacement-algorithms.c
	$(CC) -c -o page-replacement-algorithms.o page-replacement-algorithms.c 	

main.o : main.c
	$(CC) -c -o main.o main.c

clean:
	rm $(OBJECT) $(TARGET)
