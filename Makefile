CC=gcc
CFLAGS=-Wall
LFLAGS=

SRC=$(wildcard tests/*.c)
EXE=$(patsubst %.c,%.exe, $(SRC))

.PHONY: clean all

%.exe: %.c
	@echo "=> Testing $(patsubst tests/%.c,%.h,$<)"
	@echo "   Compiling target $<"
	@$(CC) -o $@ $< $(CFLAGS) $(LFLAGS)

	@echo "   Running target $@"
	@(./$@ && echo "==> TEST SUCCEED!") || echo "==> TEST FAILED!"
	@echo ""

all: $(EXE)

clean:
	rm -rf $(EXE)