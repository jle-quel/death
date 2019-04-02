################################################################################
### INIT
################################################################################

NAME			= war

SRC_PATH_C		= src/c
SRC_PATH_S		= src/asm

OBJ_PATH_C		= obj/c
OBJ_PATH_S		= obj/asm

INC_PATH		= inc

CC			= gcc
AS			= nasm
CFLAGS			= -Wall -Wextra -Werror -masm=intel -D LOGGER -I $(INC_PATH)
ASFLAGS			= -f elf64

################################################################################
### OBJECTS
################################################################################

OBJ_NAME_PRELOAD	= preload.o						\
			  main.o						\

OBJ_NAME_ENTRY		= __entry.o						\

OBJ_NAME_WAR		= antivirus.o						\
			  init.o						\
			  find.o						\
			  host.o						\
			  criteria.o						\
			  text.o						\
			  note.o						\
			  header.o						\
			  injection.o						\
			  autodestruction.o					\
			  replicate.o						\
			  execution.o						\

OBJ_NAME_LIB_C		= syscall.o						\
			  tools.o						\

OBJ_NAME_LIB_S		= stub.o						\

OBJ_NAME_EXIT		= __exit.o						\

OBJ			= $(addprefix $(OBJ_PATH_C)/,$(OBJ_NAME_PRELOAD))	\
			  $(addprefix $(OBJ_PATH_C)/,$(OBJ_NAME_ENTRY))		\
			  $(addprefix $(OBJ_PATH_C)/,$(OBJ_NAME_WAR))		\
			  $(addprefix $(OBJ_PATH_C)/,$(OBJ_NAME_LIB_C))		\
			  $(addprefix $(OBJ_PATH_S)/,$(OBJ_NAME_LIB_S))		\
			  $(addprefix $(OBJ_PATH_C)/,$(OBJ_NAME_EXIT))		\

################################################################################
### RULES
################################################################################

.PHONY: all clean fclean re

all: obj $(NAME)

obj:
	echo $(OBJ)
	mkdir -p obj/c obj/asm
	touch /tmp/logger
	chmod 666 /tmp/logger

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_PATH_C)/%.o: $(SRC_PATH_C)/%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH_S)/%.o: $(SRC_PATH_S)/%.s Makefile
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf obj
	rm -rf /tmp/logger
	rm -rf $(NAME)

re: fclean all
