

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so
SYMLINK = libft_malloc.so


GCC = gcc

F = -Wall -Wextra -Werror -I./libft/inc


LIB_PATH = libft

LIB = $(LIB_PATH)/libft.a \

#******************************************************************************#
# HEADER
#******************************************************************************#
INC_FILE_NAME = 
INC_DIR = inc
INC = $(addprefix $(INC_DIR)/, $(INC_FILE_NAME))


#******************************************************************************#
# SOURCES/OBJECT
#******************************************************************************#

SRC_FILE_NAME = 	malloc.c \
					main.c \

SRC_DIR = src
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILE_NAME))


OBJ_FILE_NAME = $(SRC_FILE_NAME:.c=.o)
OBJ_DIR = .obj
OBJ = $(addprefix $(OBJ_DIR)/, $(OBJ_FILE_NAME))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	echo $@
	echo $<
	$(GCC) $(F) -o $@ -c $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

#******************************************************************************#

.PHONY: all clean fclean re bug debug

all: $(NAME) $(SYMLINK)

$(NAME): $(OBJ) $(INC)
		make -C libft/
		$(GCC) $(F) -o $(NAME) $(OBJ) $(LIB)

$(SYMLINK):
	#ln -s $(SHARED) $(SYMLINK)

clean:
	make clean -C libft/
	rm -rf $(OBJ)

fclean: clean
	make fclean -C libft/
	rm -f $(NAME) a.out

re: fclean all

#
#$(LIB):
#	make -C libft/
#



# bug:
# 	gcc -g $(F) -o $(NAME) push_swap.c list_push_back.c ps_error.c print_stacks.c make_ss.c make_rr.c make_rrr.c pre_validate.c \
# 	init_var.c read_flags.c operation_push_back.c frst_sort_3_a.c next_sort_3_a.c \
# 	next_sort_3_b.c last_sort_3_b.c add_function.c optimization.c sorting_logic.c  print_operation.c $(INCL)

bug:
	gcc -g $(F) -o $(NAME_2) list_push_back.c add_function.c ps_error.c print_stacks.c make_ss.c make_rr.c make_rrr.c pre_validate.c init_var.c read_flags.c operation_push_back.c free_memory.c checker.c reading_commands.c $(INCL)

debug: bug
	lldb $(NAME_2) -- 2 3 4

