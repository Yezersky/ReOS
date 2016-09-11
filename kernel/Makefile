SHELL=/bin/bash

default: /tmp/ReOS/kernel/debug/kernel.bin

include ../cpp.mk

THOR_FLAGS=-DCONFIG_HISTORY=y

# Ask GCC for the crtbegin and crtend files
CRTBEGIN_OBJ:=$(shell $(CXX) $(KERNEL_CPP_FLAGS_64) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CXX) $(KERNEL_CPP_FLAGS_64) -print-file-name=crtend.o)

# Compile the assembly code

$(eval $(call kernel_compile_assembly_folder,src))

# Compile all the kernel CPP code

$(eval $(call compile_cpp_folder,src))
$(eval $(call compile_cpp_folder,src/net))
$(eval $(call compile_cpp_folder,src/drivers))
$(eval $(call compile_cpp_folder,src/fs))
$(eval $(call compile_cpp_folder,src/vfs))

# Compile all the ACPICA C code

$(eval $(call acpica_folder_compile,dispatcher))
$(eval $(call acpica_folder_compile,events))
$(eval $(call acpica_folder_compile,executer))
$(eval $(call acpica_folder_compile,namespace))
$(eval $(call acpica_folder_compile,parser))
$(eval $(call acpica_folder_compile,hardware))
$(eval $(call acpica_folder_compile,tables))
$(eval $(call acpica_folder_compile,utilities))

-include $(D_FILES)

# Remove special files from O_FILES
O_FILES := $(filter-out /tmp/ReOS/kernel/debug/src/crti.s.o,$(O_FILES))
O_FILES := $(filter-out /tmp/ReOS/kernel/debug/src/crtn.s.o,$(O_FILES))

# COmpute the link files order
LINK_O_FILES=/tmp/ReOS/kernel/debug/src/crti.s.o $(CRTBEGIN_OBJ) $(O_FILES) $(CRTEND_OBJ) /tmp/ReOS/kernel/debug/src/crtn.s.o

/tmp/ReOS/kernel/debug/kernel.bin: $(LINK_O_FILES)
	@ mkdir -p /tmp/ReOS/kernel/debug/
	@ echo -e "$(MODE_COLOR)[debug]$(NO_COLOR) Link $(FILE_COLOR)$@$(NO_COLOR)"
	@ $(CXX) $(KERNEL_LINK_FLAGS) $(KERNEL_CPP_FLAGS_64) -o $@.o $(LINK_O_FILES)
	@ $(OC) -R .note -R .comment -O binary --set-section-flags .bss=alloc,load,contents $@.o $@

clean:
	@ echo -e "Remove compiled files (deps/objects)"
	@ rm -rf /tmp/ReOS/kernel/debug
	@ rm -f /tmp/ReOS/kernel/crti.o
	@ rm -f /tmp/ReOS/kernel/crts.o
	@ echo -e "Remove bin files"
	@ rm -f /tmp/ReOS/kernel/*.bin
