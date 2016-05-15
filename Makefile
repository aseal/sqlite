BUILD_DIR = bin
C_SRCS = $(wildcard *.c)
C_EXECS = $(patsubst %.c,$(BUILD_DIR)/%,$(C_SRCS))

all: bin_exists $(C_EXECS)

bin_exists:
	mkdir -p $(BUILD_DIR)
#all: try

#try: try.c
#	gcc try.c -l sqlite3 -o try
$(BUILD_DIR)/%: %.c
	$(CC) $(CFLAGS) -o $@ $< -lsqlite3
