ifeq ($(strip ${DEVKITPPC}),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

CC=${DEVKITPPC}/bin/powerpc-eabi-gcc
CFLAGS=-O2 -Wall -Wextra -Iinclude
LDFLAGS=-DGEKKO -mcpu=750 -meabi -mhard-float

CFLAGS=-I${DEVKITPRO}/libogc2/wii/include -DHW_RVL=1
LDFLAGS=-L${DEVKITPRO}/libogc2/wii/lib -lwiiuse -lbte -logc -lm -mrvl

SRC=src/main.c
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

all: openmii

openmii: build/main.dol

build/main.elf: build ${OBJ}
	${CC} ${OBJ} ${LDFLAGS} -Wl,-Map,$@.map -o $@

build/%.o: src/%.c
	${CC} ${CFLAGS} -c -o $@ $<

%.dol: %.elf
	elf2dol $< $@

build:
	mkdir -p build

clean:
	rm -rf build

.PHONY: all openmii

