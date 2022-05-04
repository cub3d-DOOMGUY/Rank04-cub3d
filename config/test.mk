# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: youkim <youkim@student.42seoul.kr>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/04 13:12:18 by youkim            #+#    #+#              #
#    Updated: 2022/05/04 13:25:42 by youkim           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

try: all
	@set -e ;\
		MAP=$$(python3 -c "from random import shuffle;\
			from pathlib import Path;m=list(Path('map').iterdir());\
			shuffle(m);print(m.pop())");\
		echo 🗺️ running $$MAP;\
		./$(NAME) $$MAP
# ./$(NAME) asset/map/mandatory.cub

test: all
	@set -e ;\
		for cub in map/**/*; do\
			echo "🚨 $${cub%.*}" ;\
			! ./$(NAME) $$cub;\
			echo ;\
		done
	@echo ✅ all invalid maps failed successfully

#./$(NAME) $$cub ;\

norm: norminette
norminette:
	@set -e ;\
		for dir in lib/libscarf src include; do\
			norminette $$dir ;\
			echo ✅ $$dir: OK! ;\
		done
	@echo ✅ norminette passed successfully

