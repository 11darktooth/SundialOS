
GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -fpermissive 
ASPARAMS = --32

objects = bin/loader.o \
		  bin/gdt.o \
		  bin/hardwarecommunication/port.o \
		  bin/stdlib/stdio.o \
		  bin/drivers/driver.o \
		  bin/hardwarecommunication/interruptstubs.o \
		  bin/hardwarecommunication/interrupts.o \
		  bin/hardwarecommunication/pci.o \
		  bin/drivers/keyboard.o \
		  bin/drivers/mouse.o \
		  bin/kernel.o

bin/%.o: src/%.cpp
	mkdir -p $(@D)
	i686-elf-g++ $(GPPPARAMS) -o $@ -c $<
	
bin/%.o: src/%.s
	mkdir -p $(@D)
	i686-elf-as $(ASPARAMS) -o $@  $<

SundialOS.bin: linker.ld $(objects)
	#i686-elf-ld $(LDPARAMS) -T $< -o $@ $(objects)
	i686-elf-g++ -T $< -o $@ -ffreestanding -O2 -nostdlib $(objects) -lgcc

.PHONY: clean
clean:
	rm -rf bin SundialOS.bin SundialOS.iso 

SundialOS.iso: SundialOS.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot
#	echo 'set timeout=0' > iso/boot/grub/grub.cfg
#	echo 'set default=0' >> iso/boot/grub/grub.cfg
#	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "SundialOS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/SundialOS.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run-qemu: SundialOS.iso
	qemu-system-i386 -cdrom SundialOS.iso &

run: SundialOS.iso
	VirtualBox --startvm SundialOS &
