ifeq ($(strip ${DEVKITPPC}),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

CC=${DEVKITPPC}/bin/powerpc-eabi-gcc

CFLAGS= -O2 -Wall -Wextra -Iinclude -I${DEVKITPRO}/libogc2/wii/include -DHW_RVL=1
LDFLAGS= -DGEKKO -mcpu=750 -meabi -mhard-float -L${DEVKITPRO}/libogc2/wii/lib -lwiiuse -lbte -logc -lm -mrvl

SRC=src/main.c src/utils.c src/3rdparty/haxx.c src/hbc.c src/3rdparty/runtimeiospatch.c
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

all: openmii

openmii: build/main.dol

build/main.elf: build build/3rdparty ${OBJ}
	${CC} ${OBJ} ${LDFLAGS} -Wl,-Map,$@.map -o $@

build/%.o: src/%.c
	${CC} ${CFLAGS} -c -o $@ $<

%.dol: %.elf
	elf2dol $< $@

build/3rdparty:
	mkdir -p build/3rdparty

clean:
	rm -rf build

run:
	make
	wiiload build/main.dol

.PHONY: all openmii clean run

