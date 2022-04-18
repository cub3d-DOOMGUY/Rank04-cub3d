#ifndef ENGINE_H
#define ENGINE_H

#include "types__engine.h"
#include "types__renderer.h"

//@func
/*
** < draw.c > */

void	renderer__draw(t_renderer *info);
void	clear_grid(int grid[HEIGHT][WIDTH]);
/*
** < init.c > */

void	engine__init(t_engine *this);
/*
** < keyinput.c > */

void	inputhandler__init(t_inputhandler *this);
bool	inputhandler__is_movement(t_inputhandler *this);
int		inputhandler__key_release(t_keycode key, t_inputhandler *this);
int		inputhandler__key_press(t_keycode key, t_inputhandler *this);
/*
** < movement.c > */

void	engine__move_player(t_engine *e);
/*
** < run.c > */

void	engine__refresh(t_engine *this);
int		engine__loop(t_engine *this);
void	engine__run(t_engine *this);
#endif
