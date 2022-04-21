#include "engine.h"
#include "renderer.h"
#include "types__x11_events.h"
#include <stdlib.h>
#include <math.h>

static void	engine__init__input(t_engine *this)
{
	mlx_hook(this->renderer.window, X11EVENTS__KeyPress, X11MASKS__KeyPressMask,
		&inputhandler__key_press, &this->inputhandler);
	mlx_hook(this->renderer.window, X11EVENTS__KeyRelease,
		X11MASKS__KeyReleaseMask, &inputhandler__key_release,
		&this->inputhandler);
	mlx_hook(this->renderer.window, X11EVENTS__DestroyNotify,
		X11MASKS__NoEventMask, &engine__deinit, this);
}

int	engine__deinit(t_engine *this)
{
	renderer__deinit(&this->renderer);
	exit(0);
	return (0);
}
#include <stdio.h>
// camera position will be determined by parsed map result someday
// .dir = {-1, 0},
// .plane = {0, 0.66},
void	engine__init(t_engine *this, t_world *world)
{
	t_camera	camera;

	camera = (t_camera){
		.pos = {7, 16},
		.dir = {0, 1},
		.plane = {0.66, 0},
	};
	this->camera = world->camera;
	printf("cam pos %f, %f\n", this->camera.pos.x, this->camera.pos.y);
	this->camera = camera;
		printf("cam pos %f, %f\n", this->camera.pos.x, this->camera.pos.y);
	// this->camera = world->camera;
	// this->camera.pos = world->camera.pos;
	// this->camera.pos = (t_vec){5, 5};
	renderer__init(&this->renderer, world);
	this->inputhandler = inputhandler__init();
	engine__init__input(this);
}
