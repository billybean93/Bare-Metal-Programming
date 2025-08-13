#--------------------------------------Makefile-------------------------------------

CFILES = $(wildcard kernel/*.c) $(wildcard uart/*.c) $(wildcard CLI/*.c) $(wildcard mailbox/*.c)
OFILES = $(patsubst kernel/%.c,build/%.o,$(patsubst uart/%.c,build/%.o,$(patsubst CLI/%.c,build/%.o,$(patsubst mailbox/%.c,build/%.o,$(CFILES)))))
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -I kernel -I uart -I CLI -I mailbox

all: clean kernel8.img run

# ensure build dir exists
build:
	mkdir -p build

build/boot.o: kernel/boot.S | build
	aarch64-none-elf-gcc $(GCCFLAGS) -c kernel/boot.S -o build/boot.o

# compile C from each folder into build/*.o
build/%.o: kernel/%.c | build
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

build/%.o: uart/%.c | build
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

build/%.o: CLI/%.c | build
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

build/%.o: mailbox/%.c | build
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: build/boot.o $(OFILES)
	aarch64-none-elf-ld -nostdlib build/boot.o $(OFILES) -T kernel/link.ld -o build/kernel8.elf
	aarch64-none-elf-objcopy -O binary build/kernel8.elf kernel8.img

clean:
	rm -f kernel8.elf build/* *.o *.img

# Run emulation with QEMU
run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio

