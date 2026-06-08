# ─────────────────────────────────────────────────────────────
# Makefile — Password Strength Checker
# ─────────────────────────────────────────────────────────────

CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -O2
LDFLAGS = -lm
TARGET  = password_checker
SRC     = password_checker.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)
	@echo "✔ Build successful! Run with: ./$(TARGET)"

clean:
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
	@echo "✔ Installed to /usr/local/bin/$(TARGET)"

uninstall:
	rm -f /usr/local/bin/$(TARGET)

.PHONY: all clean install uninstall
