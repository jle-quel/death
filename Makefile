###############################################################################
### INIT 
################################################################################

WAR = src/war
LOAD = src/load

################################################################################
### RULES
################################################################################

.PHONY: all clean fclean re war load

all: war load

war:
	make -C $(WAR)

load:
	make -C $(LOAD)

clean:
	make -C $(WAR) clean
	make -C $(LOAD) clean

fclean: clean
	make -C $(WAR) fclean
	make -C $(LOAD) fclean

re: fclean all
