# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tgauvrit <tgauvrit@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/09/14 13:15:51 by tgauvrit          #+#    #+#              #
#    Updated: 2016/09/14 13:17:48 by tgauvrit         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	gcc test_facile.c -o test_facile
	gcc -m32 test_facile.c -o test_facile_32-bit
	gcc test_moins_facile.c -o test_moins_facile
	gcc -m32 test_moins_facile.c -o test_moins_facile_32-bit

clean:
	rm -f test_facile test_facile_32-bit test_moins_facile test_moins_facile_32-bit

re: clean all