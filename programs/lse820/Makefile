.PHONY: default clean

CURRENT_DIR=$(notdir $(shell pwd))
EXEC_NAME=lse820

default: link

include ../../cpp.mk

$(eval $(call program_compile_cpp_folder,$(CURRENT_DIR),src))
$(eval $(call program_link_executable,$(CURRENT_DIR),$(EXEC_NAME)))

clean:
	@ echo -e "Remove compiled files"
	@ rm -rf /tmp/ReOS/programs/$(CURRENT_DIR)/debug
