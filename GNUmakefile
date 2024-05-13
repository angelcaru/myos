# Nuke built-in rules and variables.
override MAKEFLAGS += -rR
 
# This is the name that our final kernel executable will have.
# Change as needed.
override KERNEL := myos
 
# Convenience macro to reliably declare user overridable variables.
define DEFAULT_VAR =
    ifeq ($(origin $1),default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1),undefined)
        override $(1) := $(2)
    endif
endef
 
# It is suggested to use a custom built cross toolchain to build a kernel.
# We are using the standard "cc" here, it may work by using
# the host system's toolchain, but this is not guaranteed.
override DEFAULT_KCC := x86_64-elf-gcc
$(eval $(call DEFAULT_VAR,KCC,$(DEFAULT_KCC)))
 
# Same thing for "ld" (the linker).
override DEFAULT_KLD := x86_64-elf-ld
$(eval $(call DEFAULT_VAR,KLD,$(DEFAULT_KLD)))
 
# User controllable C flags.
override DEFAULT_KCFLAGS := -ggdb -O3 -pipe
$(eval $(call DEFAULT_VAR,KCFLAGS,$(DEFAULT_KCFLAGS)))
 
# User controllable C preprocessor flags. We set none by default.
override DEFAULT_KCPPFLAGS :=
$(eval $(call DEFAULT_VAR,KCPPFLAGS,$(DEFAULT_KCPPFLAGS)))
 
# User controllable nasm flags.
override DEFAULT_KNASMFLAGS := -F dwarf -g
$(eval $(call DEFAULT_VAR,KNASMFLAGS,$(DEFAULT_KNASMFLAGS)))
 
# User controllable linker flags. We set none by default.
override DEFAULT_KLDFLAGS :=
$(eval $(call DEFAULT_VAR,KLDFLAGS,$(DEFAULT_KLDFLAGS)))
 
# Internal C flags that should not be changed by the user.
override KCFLAGS += \
    -Wall \
    -Wextra \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fPIE \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone
 
# Internal C preprocessor flags that should not be changed by the user.
override KCPPFLAGS := \
    -I src \
    $(KCPPFLAGS) \
    -MMD \
    -MP
 
# Internal linker flags that should not be changed by the user.
override KLDFLAGS += \
    -m elf_x86_64 \
    -nostdlib \
    -static \
    -pie \
    --no-dynamic-linker \
    -z text \
    -z max-page-size=0x1000 \
    -T linker.ld
 
# Internal nasm flags that should not be changed by the user.
override KNASMFLAGS += \
    -Wall \
    -f elf64
 
# Use "find" to glob all *.c, *.S, and *.asm files in the tree and obtain the
# object and header dependency file names.
override CFILES := $(shell cd src && find -L * -type f -name '*.c')
override ASFILES := $(shell cd src && find -L * -type f -name '*.S')
override NASMFILES := $(shell cd src && find -L * -type f -name '*.asm')
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))
 
# Default target.
.PHONY: all
all: image.iso

image.iso: bin/$(KERNEL) ./limine/limine
	# Create a directory which will be our ISO root.
	mkdir -p iso_root
 
	# Copy the relevant files over.
	mkdir -p iso_root/boot
	cp -v bin/myos iso_root/boot/
	mkdir -p iso_root/boot/limine
	cp -v limine.cfg limine/limine-bios.sys limine/limine-bios-cd.bin \
    	  limine/limine-uefi-cd.bin iso_root/boot/limine/
 
	# Create the EFI boot tree and copy Limine's EFI executables over.
	mkdir -p iso_root/EFI/BOOT
	cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
	cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/
 
	# Create the bootable ISO.
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin \
    	    -no-emul-boot -boot-load-size 4 -boot-info-table \
        	--efi-boot boot/limine/limine-uefi-cd.bin \
	        -efi-boot-part --efi-boot-image --protective-msdos-label \
    	    iso_root -o image.iso
 
	# Install Limine stage 1 and 2 for legacy BIOS boot.
	./limine/limine bios-install image.iso
 
./limine/limine:
	# Download the latest Limine binary release for the 7.x branch.
	git clone https://github.com/limine-bootloader/limine.git --branch=v7.x-binary --depth=1
 
	# Build "limine" utility.
	make -C limine
 
# Link rules for the final kernel executable.
bin/$(KERNEL): GNUmakefile linker.ld $(OBJ)
	mkdir -p "$$(dirname $@)"
	$(KLD) $(OBJ) $(KLDFLAGS) -o $@
 
# Include header dependencies.
-include $(HEADER_DEPS)

# Metaprogramming
meta/build/font: meta/font.c
	mkdir -p meta/build
	# Host GCC
	gcc -Wall -Wextra -o $@ $< -lm

src/font-data.h: meta/build/font
	./$< > $@

# Compilation rules for *.c files.
obj/%.c.o: src/%.c GNUmakefile src/font-data.h
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@
 
# Compilation rules for *.S files.
obj/%.S.o: src/%.S GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(KCC) $(KCFLAGS) $(KCPPFLAGS) -c $< -o $@
 
# Compilation rules for *.asm (nasm) files.
obj/%.asm.o: src/%.asm GNUmakefile
	mkdir -p "$$(dirname $@)"
	nasm $(KNASMFLAGS) $< -o $@
 
# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf bin obj iso_root image.iso

