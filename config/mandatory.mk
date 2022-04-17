.PHONY: all clean fclean re

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ) $(DEPENDENCIES)
	$(CC) $(CFLAGS) -I $^ $(LIB) -o $@
	@echo 🚪 compiled $@


all: $(NAME)

clean:
	@rm -f $(OBJ)
	@echo "🗑 cleaned $(NAME)'s object files"

fclean: clean
	@rm -f $(NAME)
	@echo 🗑 cleaned $(NAME)

re: fclean all

$(LIBFT):
	make -C $(LIBFT_DIR)

$(LIBMLX):
	make -C $(MLX_DIR)
