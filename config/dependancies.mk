# MinilibX
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
#   $(info 🍏)
	MLX_DIR   := lib/mlx_mac
	LIBMLX    := $(MLX_DIR)/libmlx.dylib
	LIB       += -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
	CFLAGS    += -I include/mac/
else ifeq ($(UNAME), Linux)
#    $(info 🐧)
	MLX_DIR   := lib/mlx_linux
	LIBMLX    := $(MLX_DIR)/libmlx.a
	LIB       += -L $(MLX_DIR) -lXext -lX11 -lm -lmlx
	CFLAGS    += -I include/linux/
else
	$(error "Cannot support $(UNAME)!")
endif
CFLAGS += -I $(MLX_DIR)

# Libft
LIBFT_DIR := lib/libscarf
LIBFT := $(LIBFT_DIR)/libft.a
LIB += -L $(LIBFT_DIR) -lft
CFLAGS += -I $(LIBFT_DIR)/include/

# Dependencies
DEPENDENCIES = $(LIBFT) $(LIBMLX)

