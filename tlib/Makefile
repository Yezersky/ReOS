default: /tmp/ReOS/tlib/debug/libtlib.a

include ../cpp.mk

$(eval $(call tlib_compile_cpp_folder,src))
$(eval $(call tlib_compile_assembly_folder,src))

# Compile the assembly code

O_FILES := $(filter-out /tmp/ReOS/tlib/debug/src/crti.s.o,$(O_FILES))
O_FILES := $(filter-out /tmp/ReOS/tlib/debug/src/crtn.s.o,$(O_FILES))

test:
	@ echo $(O_FILES)

/tmp/ReOS/tlib/debug/libtlib.a: /tmp/ReOS/tlib/debug/src/crti.s.o /tmp/ReOS/tlib/debug/src/crtn.s.o $(O_FILES)
	@ mkdir -p /tmp/ReOS/tlib/debug/
	@ echo -e "$(MODE_COLOR)[debug]$(NO_COLOR) Link (tlib) $(FILE_COLOR)$@$(NO_COLOR)"
	@ ${AR} rcs debug/libtlib.a $(O_FILES)

-include $(D_FILES)

clean:
	@ echo -e "Remove compiled files (deps/objects)"
	@ rm -rf /tmp/ReOS/tlib/debug
