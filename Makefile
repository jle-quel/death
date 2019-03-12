NAME = war 

SRC_PATH = src
OBJ_PATH = obj
INC_PATH = inc
TRACE_PATH=/tmp/trace

SRC_NAME =			\
		main.c		\
		entry.c		\
		decrypt.c	\


OBJ_NAME = $(SRC_NAME:.c=.o)

CC = gcc 
CFLAGS = -Wall -Wextra -Werror -masm=intel -D DEBUG

RED=\033[1;31m
GREEN=\033[1;32m
NC=\033[0m

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

.PHONY: all, clean, fclean, re

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c Makefile
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_PATH)
	@echo "$(GREEN)[✓]$(NC) Source compiled : $<"
	@touch $(TRACE_PATH)
	@chmod 666 $(TRACE_PATH)

clean:
	@rm -f $(OBJ)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	@echo "$(RED)[-]$(NC) Objects cleaned"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)[-]$(NC) Program clear"
	@rm -f $(TRACE_PATH)

re: fclean
	@make

