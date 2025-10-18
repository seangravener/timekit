CC = gcc
CFLAGS = -Wall -Wextra -O2
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)

SRC_DIR = src
BIN_DIR = bin

.PHONY: all clean pomodoro time_display time_gtk

all: pomodoro time_display time_gtk

pomodoro: $(SRC_DIR)/pomodoro.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -o $(BIN_DIR)/$@ $< $(GTK_LIBS)

time_display: $(SRC_DIR)/time_display.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $<

time_gtk: $(SRC_DIR)/time_display_gtk.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -o $(BIN_DIR)/$@ $< $(GTK_LIBS)

clean:
	rm -rf $(BIN_DIR)/*