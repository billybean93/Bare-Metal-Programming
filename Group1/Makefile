#--------------------------------------Makefile-------------------------------------


CFILES = $(wildcard ./kernel/*.c)
BITMAPS  = $(wildcard ./bitmap/*.c)
CLIFILES = $(wildcard ./CLI/*.c)
BIRDFILES = $(wildcard ./bird/*.c)
MAILBOXFILES = $(wildcard ./mailbox/*.c)
FONTFILES = $(wildcard ./font/*.c)
TIMERFILES = $(wildcard ./timer/*.c)
FRAMEBUFFERFILES = $(wildcard ./framebuffer/*.c)
MAPFILES = $(wildcard ./map/*.c)
GAMEFILES = $(wildcard ./game/*.c)

# Include bitmap files in the build process
# The bitmap files are compiled to object files and linked into the final binary
# Without this, bitmap array would be undefined in kernel.c
OFILES   = $(CFILES:./kernel/%.c=./build/%.o) \
           $(BITMAPS:./bitmap/%.c=./build/%.o) \
		   $(BIRDFILES:./bird/%.c=./build/%.o) \
		   $(CLIFILES:./CLI/%.c=./build/%.o) \
		   $(MAILBOXFILES:./mailbox/%.c=./build/%.o) \
		   $(FONTFILES:./font/%.c=./build/%.o) \
		   $(FRAMEBUFFERFILES:./framebuffer/%.c=./build/%.o) \
		   $(TIMERFILES:./timer/%.c=./build/%.o) \
		   $(MAPFILES:./map/%.c=./build/%.o) \
		   $(GAMEFILES:./game/%.c=./build/%.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib

all: clean  kernel8.img run1
mac: clean_mac uart0_build kernel8.img run_mac


# Compile bitmap data files
./build/%.o: ./map/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/%.o: ./game/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/%.o: ./timer/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/%.o: ./bird/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@


uart0_build: ./uart/uart0.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./uart/uart0.c -o ./build/uart.o

./build/boot.o: ./kernel/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./kernel/boot.S -o ./build/boot.o

./build/%.o: ./kernel/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/%.o: ./CLI/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/%.o: ./framebuffer/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/%.o: ./font/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/%.o: ./mailbox/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/%.o: ./bitmap/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@


kernel8.img: ./build/boot.o ./build/uart.o $(OFILES)
	aarch64-none-elf-ld -nostdlib ./build/boot.o ./build/uart.o $(OFILES) -T ./kernel/link.ld -o ./build/kernel8.elf
	aarch64-none-elf-objcopy -O binary ./build/kernel8.elf kernel8.img

clean:
	del .\build\kernel8.elf .\build\*.o *.img

clean_mac: 
	rm -f ./build/kernel8.elf ./build/*.o *.img

# Run emulation with QEMU
run1: 
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio

run0: 
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial stdio

run_mac: 
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio 
