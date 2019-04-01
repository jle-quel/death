################################################################################
### INIT
################################################################################

NAME		= ../../load 

SRC_PATH_C	= src
SRC_PATH_ASM	= ../utils/src/asm

OBJ_PATH_C	= obj/c
OBJ_PATH_ASM	= obj/asm

INC_PATH	= inc

CC		= gcc
AS		= nasm
CFLAGS		= -Wall -Wextra -Werror -I $(INC_PATH) -g3 -fsanitize=address
ASFLAGS		= -f elf64

################################################################################
### OBJECTS 
################################################################################

OBJ_NAME_1	= main.o					\

OBJ_NAME_2	= rc4.o						\

OBJ		= $(addprefix $(OBJ_PATH_C)/,$(OBJ_NAME_1))	\
		  $(addprefix $(OBJ_PATH_ASM)/,$(OBJ_NAME_2))	\

################################################################################
### RULES
################################################################################

.PHONY: all clean fclean re

all: obj $(NAME)

obj:
	mkdir -p obj
	mkdir -p obj/c
	mkdir -p obj/asm

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_PATH_C)/%.o: $(SRC_PATH_C)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH_ASM)/%.o: $(SRC_PATH_ASM)/%.s Makefile
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf obj
	rm -rf $(NAME)

re: fclean all
