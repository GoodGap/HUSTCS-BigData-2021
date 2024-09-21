# we assume that the utilities from RISC-V cross-compiler (i.e., riscv64-unknown-elf-gcc and etc.)
# are in your system PATH. To check if your environment satisfies this requirement, simple use 
# `which` command as follows:
# $ which riscv64-unknown-elf-gcc
# if you have an output path, your environment satisfy our requirement.

# ---------------------	macros --------------------------
CROSS_PREFIX 	:= riscv64-unknown-elf-
CC 				:= $(CROSS_PREFIX)gcc
AR 				:= $(CROSS_PREFIX)ar
RANLIB        	:= $(CROSS_PREFIX)ranlib

SRC_DIR        	:= .
OBJ_DIR 		:= obj
SPROJS_INCLUDE 	:= -I.  

HOSTFS_ROOT := hostfs_root
ifneq (,)
  march := -march=
  is_32bit := $(findstring 32,$(march))
  mabi := -mabi=$(if $(is_32bit),ilp32,lp64)
endif

CFLAGS        := -Wall -Werror -gdwarf-3 -fno-builtin -nostdlib -D__NO_INLINE__ -mcmodel=medany -g -Og -std=gnu99 -Wno-unused -Wno-attributes -fno-delete-null-pointer-checks -fno-PIE $(march) -fno-omit-frame-pointer
COMPILE       	:= $(CC) -MMD -MP $(CFLAGS) $(SPROJS_INCLUDE)

#---------------------	utils -----------------------
UTIL_CPPS 	:= util/*.c

UTIL_CPPS  := $(wildcard $(UTIL_CPPS))
UTIL_OBJS  :=  $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(UTIL_CPPS)))


UTIL_LIB   := $(OBJ_DIR)/util.a

#---------------------	kernel -----------------------
KERNEL_LDS  	:= kernel/kernel.lds
KERNEL_CPPS 	:= \
	kernel/*.c \
	kernel/machine/*.c \
	kernel/util/*.c

KERNEL_ASMS 	:= \
	kernel/*.S \
	kernel/machine/*.S \
	kernel/util/*.S

KERNEL_CPPS  	:= $(wildcard $(KERNEL_CPPS))
KERNEL_ASMS  	:= $(wildcard $(KERNEL_ASMS))
KERNEL_OBJS  	:=  $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(KERNEL_CPPS)))
KERNEL_OBJS  	+=  $(addprefix $(OBJ_DIR)/, $(patsubst %.S,%.o,$(KERNEL_ASMS)))

KERNEL_TARGET = $(OBJ_DIR)/riscv-pke


#---------------------	spike interface library -----------------------
SPIKE_INF_CPPS 	:= spike_interface/*.c

SPIKE_INF_CPPS  := $(wildcard $(SPIKE_INF_CPPS))
SPIKE_INF_OBJS 	:=  $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(SPIKE_INF_CPPS)))


SPIKE_INF_LIB   := $(OBJ_DIR)/spike_interface.a


#---------------------	user   -----------------------
USER_CPPS 		:= user/app_shell.c user/user_lib.c

USER_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_CPPS)))

USER_TARGET 	:= $(HOSTFS_ROOT)/bin/shell

USER_E_CPPS 		:= user/app_ls.c user/user_lib.c

USER_E_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_E_CPPS)))

USER_E_TARGET 	:= $(HOSTFS_ROOT)/bin/app_ls

USER_M_CPPS 		:= user/app_mkdir.c user/user_lib.c

USER_M_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_M_CPPS)))

USER_M_TARGET 	:= $(HOSTFS_ROOT)/bin/app_mkdir

USER_T_CPPS 		:= user/app_touch.c user/user_lib.c

USER_T_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_T_CPPS)))

USER_T_TARGET 	:= $(HOSTFS_ROOT)/bin/app_touch

USER_C_CPPS 		:= user/app_cat.c user/user_lib.c

USER_C_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_C_CPPS)))

USER_C_TARGET 	:= $(HOSTFS_ROOT)/bin/app_cat

USER_O_CPPS 		:= user/app_echo.c user/user_lib.c

USER_O_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_O_CPPS)))

USER_O_TARGET 	:= $(HOSTFS_ROOT)/bin/app_echo

USER_PWD_CPPS 		:= user/app_pwd.c user/user_lib.c

USER_PWD_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_PWD_CPPS)))

USER_PWD_TARGET 	:= $(HOSTFS_ROOT)/bin/app_pwd

USER_WAIT_CPPS 		:= user/app_wait.c user/user_lib.c

USER_WAIT_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_WAIT_CPPS)))

USER_WAIT_TARGET 	:= $(HOSTFS_ROOT)/bin/app_wait

USER_COW_CPPS 		:= user/app_cow.c user/user_lib.c

USER_COW_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_COW_CPPS)))

USER_COW_TARGET 	:= $(HOSTFS_ROOT)/bin/app_cow

USER_SEM_CPPS 		:= user/app_semaphore.c user/user_lib.c

USER_SEM_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_SEM_CPPS)))

USER_SEM_TARGET 	:= $(HOSTFS_ROOT)/bin/app_semaphore

USER_BM_CPPS 		:= user/app_singlepageheap.c user/user_lib.c

USER_BM_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_BM_CPPS)))

USER_BM_TARGET 	:= $(HOSTFS_ROOT)/bin/app_singlepageheap

USER_PGF_CPPS 		:= user/app_sum_sequence.c user/user_lib.c

USER_PGF_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_PGF_CPPS)))

USER_PGF_TARGET 	:= $(HOSTFS_ROOT)/bin/app_sum_sequence

USER_BT_CPPS 		:= user/app_print_backtrace.c user/user_lib.c

USER_BT_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_BT_CPPS)))

USER_BT_TARGET 	:= $(HOSTFS_ROOT)/bin/app_print_backtrace

USER_ERRORLINE_CPPS 		:= user/app_errorline.c user/user_lib.c

USER_ERRORLINE_OBJS  		:= $(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(USER_ERRORLINE_CPPS)))

USER_ERRORLINE_TARGET 	:= $(HOSTFS_ROOT)/bin/app_errorline

#------------------------targets------------------------
$(OBJ_DIR):
	@-mkdir -p $(OBJ_DIR)	
	@-mkdir -p $(dir $(UTIL_OBJS))
	@-mkdir -p $(dir $(SPIKE_INF_OBJS))
	@-mkdir -p $(dir $(KERNEL_OBJS))
	@-mkdir -p $(dir $(USER_OBJS))
	@-mkdir -p $(dir $(USER_E_OBJS))
	@-mkdir -p $(dir $(USER_M_OBJS))
	@-mkdir -p $(dir $(USER_T_OBJS))
	@-mkdir -p $(dir $(USER_C_OBJS))
	@-mkdir -p $(dir $(USER_O_OBJS))
	@-mkdir -p $(dir $(USER_PWD_OBJS))
	@-mkdir -p $(dir $(USER_WAIT_OBJS))
	@-mkdir -p $(dir $(USER_COW_OBJS))
	@-mkdir -p $(dir $(USER_SEM_OBJS))
	@-mkdir -p $(dir $(USER_BM_OBJS))
	@-mkdir -p $(dir $(USER_PGF_OBJS))
	@-mkdir -p $(dir $(USER_BT_OBJS))
	@-mkdir -p $(dir $(USER_ERRORLINE_OBJS))
	
$(OBJ_DIR)/%.o : %.c
	@echo "compiling" $<
	@$(COMPILE) -c $< -o $@

$(OBJ_DIR)/%.o : %.S
	@echo "compiling" $<
	@$(COMPILE) -c $< -o $@

$(UTIL_LIB): $(OBJ_DIR) $(UTIL_OBJS)
	@echo "linking " $@	...	
	@$(AR) -rcs $@ $(UTIL_OBJS) 
	@echo "Util lib has been build into" \"$@\"
	
$(SPIKE_INF_LIB): $(OBJ_DIR) $(UTIL_OBJS) $(SPIKE_INF_OBJS)
	@echo "linking " $@	...	
	@$(AR) -rcs $@ $(SPIKE_INF_OBJS) $(UTIL_OBJS)
	@echo "Spike lib has been build into" \"$@\"

$(KERNEL_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(SPIKE_INF_LIB) $(KERNEL_OBJS) $(KERNEL_LDS)
	@echo "linking" $@ ...
	@$(COMPILE) $(KERNEL_OBJS) $(UTIL_LIB) $(SPIKE_INF_LIB) -o $@ -T $(KERNEL_LDS)
	@echo "PKE core has been built into" \"$@\"

$(USER_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"
	@cp $@ $(OBJ_DIR)

$(USER_E_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_E_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_E_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_M_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_M_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_M_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_T_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_T_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_T_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_C_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_C_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_C_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_O_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_O_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_O_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_PWD_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_PWD_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_PWD_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"
	
$(USER_WAIT_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_WAIT_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_WAIT_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_COW_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_COW_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_COW_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_SEM_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_SEM_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_SEM_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_BM_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_BM_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_BM_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_PGF_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_PGF_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_PGF_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_BT_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_BT_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_BT_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"

$(USER_ERRORLINE_TARGET): $(OBJ_DIR) $(UTIL_LIB) $(USER_ERRORLINE_OBJS)
	@echo "linking" $@	...	
	-@mkdir -p $(HOSTFS_ROOT)/bin
	@$(COMPILE) --entry=main $(USER_ERRORLINE_OBJS) $(UTIL_LIB) -o $@
	@echo "User app has been built into" \"$@\"


-include $(wildcard $(OBJ_DIR)/*/*.d)
-include $(wildcard $(OBJ_DIR)/*/*/*.d)

.DEFAULT_GOAL := $(all)

all: $(KERNEL_TARGET) $(USER_TARGET) $(USER_E_TARGET) $(USER_M_TARGET) $(USER_T_TARGET) $(USER_C_TARGET) $(USER_O_TARGET) $(USER_PWD_TARGET) $(USER_WAIT_TARGET) $(USER_COW_TARGET) $(USER_SEM_TARGET) $(USER_BM_TARGET) $(USER_PGF_TARGET) $(USER_BT_TARGET) $(USER_ERRORLINE_TARGET)
.PHONY:all

run: $(KERNEL_TARGET) $(USER_TARGET) $(USER_E_TARGET) $(USER_M_TARGET) $(USER_T_TARGET) $(USER_C_TARGET) $(USER_O_TARGET) $(USER_PWD_TARGET) $(USER_WAIT_TARGET) $(USER_COW_TARGET) $(USER_ERRORLINE_TARGET) $(USER_SEM_TARGET) $(USER_BM_TARGET) $(USER_PGF_TARGET) $(USER_BT_TARGET)
	spike -p2 $(KERNEL_TARGET) /bin/shell

# need openocd!
gdb:$(KERNEL_TARGET) $(USER_TARGET)
	spike --rbb-port=9824 -H $(KERNEL_TARGET) $(USER_TARGET) &
	@sleep 1
	openocd -f ./.spike.cfg &
	@sleep 1
	riscv64-unknown-elf-gdb -command=./.gdbinit

# clean gdb. need openocd!
gdb_clean:
	@-kill -9 $$(lsof -i:9824 -t)
	@-kill -9 $$(lsof -i:3333 -t)
	@sleep 1

objdump:
	riscv64-unknown-elf-objdump -d $(KERNEL_TARGET) > $(OBJ_DIR)/kernel_dump
	riscv64-unknown-elf-objdump -d $(USER_TARGET) > $(OBJ_DIR)/user_dump

test:
	readelf -W -S $(USER_TARGET) > $(OBJ_DIR)/readelf_dump
	dwarfdump -l $(USER_TARGET) > $(OBJ_DIR)/dwarf_dump
	readelf -wl -W $(USER_TARGET) > $(OBJ_DIR)/readelf_debug_line_dump

cscope:
	find ./ -name "*.c" > cscope.files
	find ./ -name "*.h" >> cscope.files
	find ./ -name "*.S" >> cscope.files
	find ./ -name "*.lds" >> cscope.files
	cscope -bqk

format:
	@python ./format.py ./

clean:
	rm -fr ${OBJ_DIR} ${HOSTFS_ROOT}/bin
