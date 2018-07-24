

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
NAME = libft_malloc_$(HOSTTYPE).so
SYMLINK = libft_malloc.so


GCC = gcc

F = -Wall -Wextra -Werror
FSHARED = -shared -fPIC 


#*** libft ********************************************************************#
LIB_PATH = libft
LIB_INC = $(LIB_PATH)/include
LIB = $(LIB_PATH)/libft.a


#******************************************************************************#
# HEADER
#******************************************************************************#
#INC_FILE_NAME = malloc.h
INC_DIR = include
#INC = $(addprefix $(INC_DIR)/, $(INC_FILE_NAME))


#******************************************************************************#
# SOURCES/OBJECT
#******************************************************************************#

SRC_FILE_NAME = 	malloc.c \

SRC_DIR = src
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILE_NAME))


OBJ_FILE_NAME = $(SRC_FILE_NAME:.c=.o)
OBJ_DIR = .obj
OBJ = $(addprefix $(OBJ_DIR)/, $(OBJ_FILE_NAME))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(GCC) $(F) -o $@ -c $< -I$(INC_DIR) -I$(LIB_INC)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

#** rules **********************************************************************#

.PHONY: all clean fclean re bug debug

all: $(NAME) $(SYMLINK)

$(NAME): $(LIB) $(OBJ) $(INC)
		$(GCC) $(F) $(FSHARED) $(OBJ) $(LIB) -o $(NAME)

$(SYMLINK):
	ln -s $(NAME) $(SYMLINK)
	
$(LIB): libft.all

clean: libft.clean
	rm -rf $(OBJ)

fclean: libft.fclean clean
	rm -f $(NAME) a.out

re: fclean all


#** rules * libft **************************************************************#
libft.all:
	make -C $(LIB_PATH)/ all

libft.clean:
	make -C $(LIB_PATH)/ clean

libft.fclean:
	make -C $(LIB_PATH)/ fclean

libft.re:
	make -C $(LIB_PATH)/ re


test: all
	gcc -c src/main.c -o .obj/main.o
	gcc .obj/main.o $(OBJ) $(LIB)  -o a.out 
	



# bug:
# 	gcc -g $(F) -o $(NAME) push_swap.c list_push_back.c ps_error.c print_stacks.c make_ss.c make_rr.c make_rrr.c pre_validate.c \
# 	init_var.c read_flags.c operation_push_back.c frst_sort_3_a.c next_sort_3_a.c \
# 	next_sort_3_b.c last_sort_3_b.c add_function.c optimization.c sorting_logic.c  print_operation.c $(INCL)

bug:
	gcc -g $(F) -o $(NAME_2) list_push_back.c add_function.c ps_error.c print_stacks.c make_ss.c make_rr.c make_rrr.c pre_validate.c init_var.c read_flags.c operation_push_back.c free_memory.c checker.c reading_commands.c $(INCL)

debug: bug
	lldb $(NAME_2) -- 2 3 4

