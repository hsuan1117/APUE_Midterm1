CC = gcc
CFLAG = -std=c11 -O2 -Wall
TARGET = q1
SRCS = q1.c
OBJS = q1.o
TARGET_2 = q2
SRCS_2 = q2.c
OBJS_2 = q2.o

all: clean $(TARGET) $(TARGET_2)

$(TARGET): $(OBJS)
	$(CC) $(CFLAG) -o $(TARGET) $(OBJS)

$(TARGET_2): $(OBJS_2)
	$(CC) $(CFLAG) -o $(TARGET_2) $(OBJS_2)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(OBJS_2) $(TARGET_2) *.out
