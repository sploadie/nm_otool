# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/11/20 16:35:37 by tgauvrit          #+#    #+#              #
#    Updated: 2016/09/02 15:13:47 by tgauvrit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME1 =			ft_nm
NAME2 =			ft_otool

CC =			clang

FLAGS =			-Wall -Werror -Wextra -fPIC

HEADERS =		-I ./include -I $(LIBFT_DIR)

LIBRARIES =		-L$(LIBFT_DIR) -l$(LIBFT_NAME)

LIBFT_NAME =	ft

LIBFT =			$(LIBFT_DIR)lib$(LIBFT_NAME).a

LIBFT_DIR =		./libft/

SRC_DIR =		./src/

OBJ_DIR_NAME =	obj
OBJ_DIR =		./obj/

SHARED_FILES =	file_data multiprint swap sections print_symbols
FILENAMES1 :=	nm_main nm
FILENAMES2 :=	otool_main

OBJ_PATHS1 :=	$(addsuffix .o,$(SHARED_FILES) $(FILENAMES1))
OBJ_PATHS1 :=	$(addprefix $(OBJ_DIR),$(OBJ_PATHS1))

OBJ_PATHS2 :=	$(addsuffix .o,$(SHARED_FILES) $(FILENAMES2))
OBJ_PATHS2 :=	$(addprefix $(OBJ_DIR),$(OBJ_PATHS2))

OBJ_PATHS :=	$(addsuffix .o,$(SHARED_FILES) $(FILENAMES1) $(FILENAMES2))
OBJ_PATHS :=	$(addprefix $(OBJ_DIR),$(OBJ_PATHS))

all: $(NAME1) $(NAME2)

$(NAME1): $(LIBFT) $(OBJ_PATHS1)
	$(CC) $(FLAGS) $(HEADERS) $(LIBRARIES) $(OBJ_PATHS1) -o $(NAME1)

$(NAME2): $(LIBFT) $(OBJ_PATHS2)
	$(CC) $(FLAGS) $(HEADERS) $(LIBRARIES) $(OBJ_PATHS2) -o $(NAME2)

$(OBJ_PATHS): $(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@/bin/mkdir -p $(OBJ_DIR)
	$(CC) -c $(FLAGS) $(HEADERS) $< -o $@

$(LIBFT):
	(cd $(LIBFT_DIR) && make)

clean:
	-/bin/rm -f $(OBJ_PATHS)
	/usr/bin/find . -name "$(OBJ_DIR_NAME)" -maxdepth 1 -type d -empty -delete

fclean: clean
	-/bin/rm -f $(NAME1) $(NAME2)

re: fclean all
