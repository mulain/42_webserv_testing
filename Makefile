# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/28 14:41:05 by wmardin           #+#    #+#              #
#    Updated: 2023/04/12 19:08:46 by wmardin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	webservtest
CC		=	c++
CFLAGS	=	-g -Iincl -fsanitize=address
EFLAGS	=	-Wall -Wextra -Werror -pedantic -std=c++98
RM		=	rm -rf

COLOR	=	\033[30m
RESET	=	\033[0m

SRCFILE	=	main.cpp Server.cpp Socket.cpp Utils.cpp Config.cpp
SRC		=	$(addprefix src/, $(SRCFILE))
OBJ		= 	$(addprefix obj/, $(SRCFILE:%.c=%.o))

all: $(NAME)

$(NAME):
	@$(CC) $(CFLAGS) $(EFLAGS) $(SRC) -o $(NAME)
	@echo "$(COLOR)$(NAME) compiled.$(RESET)"

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) $(EFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ) obj
	@echo "$(COLOR)$(NAME) cleaned.$(RESET)"

fclean: clean
	@$(RM) $(NAME) $(NAME).dSYM

re: fclean all

git:
	git add .
	git commit -m "auto add & push"
	git push
	@echo "$(COLOR)git auto add & push performed.$(RESET)"

mgit:
	git add .
	@read -p "Enter the commit message: " halp; \
	git commit -m "$$halp"
	git push
	@echo "$(COLOR)git auto add & push with message performed.$(RESET)"

.SILENT: $(OBJ)
