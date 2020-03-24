CC=gcc
CFLAGS=-Wall -std=c11
LFLAGS=

SRC=$(wildcard tests/*.c)
EXE=$(patsubst %.c,%.exe, $(SRC))

.PHONY: clean all

%.c: %.h
%.cpp: %.h

#%.exe: %.c
%.exe: %.cpp
	@echo "=> Testing $(patsubst tests/%.c,%.h,$<)"
	@echo "   Compiling target $<"
	@$(CC) -o $@ $< $(CFLAGS) $(LFLAGS)

	@echo "   Running target $@"
	@(./$@ && echo "==> TEST SUCCEED!") || echo "==> TEST FAILED!"
	@echo ""

all: $(EXE)

clean:
	rm -rf $(EXE)