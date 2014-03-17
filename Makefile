CC:=gcc

SOURCES ?= $(wildcard src/*.c)
OBJS    ?= $(addprefix , $(SOURCES:.c=.o))

#----------------------------------------
#	Softwares Version
#	
#	MAJOR.MINOR.PATCH
#		1. MAJOR version when you make incompatible API changes,
#		2. MINOR version when you add functionality in a backwards-compatible manner, and
#		3. PATCH version when you make backwards-compatible bug fixes.
#----------------------------------------
VERSION := 0.1.0

#----------------------------------------
#	Common CFLAGS
#----------------------------------------
CFLAGS += -W -Wall -Wshadow
CFLAGS += -O2 -g -fno-omit-frame-pointer
CFLAGS += -fno-strict-aliasing
CFLAGS += -I. -Iinclude/ 
CFLAGS += -D'RELEASE="$(VERSION)"'

LDFLAGS +=

TARGET := monitor

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "[BIN] $@ version:$(VERSION)"
	@$(CC) $^ $(LDFLAGS) -o $@

$(OBJS): %.o: %.c 
	@echo "[CC] $<"
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJS) $(TARGET)

format:
	@astyle --style=linux -s4 -n src/*.c
	@astyle --style=linux -s4 -n include/*.h


install: all
	@echo "update $(TARGET)"
	@rsync -Cravp $(TARGET) /usr/local/bin/

.PHONY: clean format install
