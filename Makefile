arch ?= x86_64
kernel := build/kernel-$(arch).bin
iso := build/os-$(arch).iso

GCCPARAMS = -std=c++11 -mno-red-zone -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore 
GCCINCLUDES = -Isrc -Isrc/drivers

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
assembly_source_files := $(wildcard src/arch/$(arch)/*.S)
assembly_object_files := $(patsubst src/arch/$(arch)/%.S, \
	build/arch/$(arch)/%.o, $(assembly_source_files))
	
c_source_files := $(wildcard src/*.cpp) $(wildcard src/drivers/*.cpp)
c_object_files := $(patsubst src/%.cpp, \
	build/%.o, $(c_source_files))

.PHONY: all clean run iso

all: $(kernel)

clean:
	@rm -rf build

run: $(iso)
	@qemu-system-x86_64 -cdrom $(iso) # -show-cursor or qemu 2.9.0 required for mouse to work 

debug: $(iso)
	@qemu-system-x86_64 -d int -no-reboot -cdrom $(iso)
	
iso: $(iso)

# build bootable iso image
$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
	@rm -r build/isofiles

# bulid kernel binary
$(kernel): $(assembly_object_files) $(c_object_files) $(linker_script)
	@ld -n -T $(linker_script) -o $(kernel) $(assembly_object_files) $(c_object_files)

# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.S
	@mkdir -p $(shell dirname $@)
	@as --64 $< -o $@
	
# compile c++ files
build/%.o: src/%.cpp
	@mkdir -p $(shell dirname $@)
	@g++ $(GCCPARAMS) $(GCCINCLUDES) -c  $< -o $@	