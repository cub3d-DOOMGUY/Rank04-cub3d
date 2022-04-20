#include "engine.h"
#include "renderer.h"
#include "std__math.h"
#include <math.h>

const extern int	g_worldmap[24][24];
extern int			texture[8][TEX_HEIGHT * TEX_WIDTH];

int	shade_color(int color, double divide)
{
	if (divide <= 1.)
		return (color);
	return (((int)(((0xFF0000 & color) >> 16) / divide) << 16)
		+ ((int)(((0x00FF00 & color) >> 8) / divide) << 8)
		+ ((int)((0x0000FF & color) / divide)));
}

int	distance_shade(int color, double distance)
{
	return (shade_color(color, distance / 1.5));
}

t_colors	get_color(t_ivec *map, bool is_hit_y_side)
{
	const int		index = g_worldmap[map->y][map->x];
	int				result;
	const t_colors	colors[] = {
		COLOR__YELLOW,
		COLOR__RED,
		COLOR__GREEN,
		COLOR__BLUE,
		COLOR__WHITE,
	};

	if (index > 4)
		result = colors[0];
	else
		result = colors[index];
	if (is_hit_y_side)
		result /= 2;
	return (result);
}

// calculate lowest and highest pixel to fill in current stripe
void	renderer__draw__vertical_wall(t_renderer *this,
									int lineheight,
									int color,
									int x)
{
	int	draw_start;
	int	draw_end;
	int	y;

	draw_start = math__max(-lineheight / 2 + HEIGHT / 2, 0);
	draw_end = math__min(lineheight / 2 + HEIGHT / 2, HEIGHT - 1);
	y = draw_start - 1;
	while (++y < draw_end)
		this->buf[y][x] = color;
}

void	floordata__raycast__set_raydir_vector(t_floordata *this, t_camera *camera)
{
	this->ray_dir0 = (t_vec){camera->dir.x - camera->plane.x, camera->dir.y
		- camera->plane.y};
	this->ray_dir1 = (t_vec){camera->dir.x + camera->plane.x, camera->dir.y
		+ camera->plane.y};
}

void	floordata__raycast__set_row_distance(t_floordata *this, int current_y)
{
	int		position_from_center;
	float	vertical_camera_position;

	position_from_center = current_y - HEIGHT / 2;
	vertical_camera_position = 0.5 * HEIGHT;
	this->rowDistance = vertical_camera_position / position_from_center;
}

void	floordata__raycast__set_floor_vectors(t_floordata *this, t_camera *camera)
{
	this->floorStep.x = this->rowDistance * (this->ray_dir1.x
			- this->ray_dir0.x) / WIDTH;
	this->floorStep.y = this->rowDistance * (this->ray_dir1.y
			- this->ray_dir0.y) / WIDTH;
	this->floor.x = camera->pos.x + this->rowDistance * this->ray_dir0.x;
	this->floor.y = camera->pos.y + this->rowDistance * this->ray_dir0.y;
}

void	floordata__raycast__set_delta_texture_vector(t_floordata *this)
{
	this->cell.x = (int)(this->floor.x);
	this->cell.y = (int)(this->floor.y);
	this->deltaT.x = (int)(TEX_WIDTH * (this->floor.x
				- this->cell.x)) & (TEX_WIDTH - 1);
	this->deltaT.y = (int)(TEX_HEIGHT * (this->floor.y
				- this->cell.y)) & (TEX_HEIGHT - 1);
	this->floor.x += this->floorStep.x;
	this->floor.y += this->floorStep.y;
}

void	floordata__draw__checkerboard(t_floordata *this)
{
	bool	checkerboard;

	checkerboard = ((int)this->floor.x + (int)this->floor.y) % 2;
	if (checkerboard)
	{
		this->floorTexture = WOOD;
		this->ceilingTexture = GRAYSTONE;
	}
	else
	{
		this->floorTexture = REDBRICK;
		this->ceilingTexture = MOSSY;
	}
}

void	renderer__draw__floor(t_renderer *this, t_floordata *vecs,
		int current_x, int current_y)
{
	int	color;

	color = texture[vecs->floorTexture][(int)(TEX_WIDTH * vecs->deltaT.y
			+ vecs->deltaT.x)];
	color = (color >> 1) & 8355711; // make a bit darker
	color = distance_shade(color, vecs->rowDistance);
	this->buf[current_y][current_x] = color;
	color = texture[vecs->ceilingTexture][(int)(TEX_WIDTH * vecs->deltaT.y
			+ vecs->deltaT.x)];
	color = (color >> 1) & 8355711; // make a bit darker
	color = distance_shade(color, vecs->rowDistance);
	this->buf[HEIGHT - current_y - 1][current_x] = color;
}

// FLOOR CASTING
void	renderer__raycast__floor(t_renderer *this, t_camera *camera)
{
	t_floordata	floordata;
	int			y;
	int			x;

	y = HEIGHT / 2 - 1;
	while (++y < HEIGHT)
	{
		floordata__raycast__set_raydir_vector(&floordata, camera);
		floordata__raycast__set_row_distance(&floordata, y);
		floordata__raycast__set_floor_vectors(&floordata, camera);
		x = -1;
		while (++x < WIDTH)
		{
			floordata__raycast__set_delta_texture_vector(&floordata);
			floordata__draw__checkerboard(&floordata);
			renderer__draw__floor(this, &floordata, x, y);
		}
	}
}

void	walldata__raycast__set_dda_vector(t_walldata *this, t_camera *camera, int current_x)
{
	this->camera_x = dda__normalized_plane_x(current_x);
	this->ray_dir = camera__ray_dir_at_position(camera, this->camera_x);
	this->map_pos = camera__to_pos_at_map(camera);
	this->delta_dist = dda__dist_to_next_closest_grid(&this->ray_dir);
	this->step = dda__initial_step(camera, &this->map_pos, &this->ray_dir, &this->delta_dist);
	dda__advance_step_until_hit(&this->step, &this->map_pos, &this->delta_dist);
	this->perpWallDist = dda__perpendicular_dist_to_closest_grid(
		&this->step, camera, &this->map_pos, &this->ray_dir);
}

void	walldata__draw__set_wall_data(t_walldata *this, t_camera *camera)
{
	this->lineheight = (int)(HEIGHT / this->perpWallDist * 1);
	this->draw_start = math__max(-this->lineheight / 2 + HEIGHT / 2, 0);
	this->draw_end = math__min(this->lineheight / 2 + HEIGHT / 2, HEIGHT - 1);
	if (this->step.is_hit_y_side == 0)
		this->wallx = camera->pos.y + this->perpWallDist * this->ray_dir.y;
	else
		this->wallx = camera->pos.x + this->perpWallDist * this->ray_dir.x;
	this->wallx -= floor(this->wallx);
}

void	walldata__draw__set_texture_data(t_walldata *this)
{
	this->texX = (int)(this->wallx * (double)TEX_WIDTH);
	if (this->step.is_hit_y_side == 0 && this->ray_dir.x > 0)
		this->texX = TEX_WIDTH - this->texX - 1;
	if (this->step.is_hit_y_side == 1 && this->ray_dir.y < 0)
		this->texX = TEX_WIDTH - this->texX - 1;
	this->step_val = 1.0 * TEX_HEIGHT / this->lineheight;
	this->texPos = (this->draw_start - HEIGHT / 2 + this->lineheight / 2) * this->step_val;
}

int	walldata__draw__wall_texture(t_walldata *this)
{
	int	texY;
	int	texnum;
	int	color;

	texY = (int)this->texPos & (TEX_HEIGHT - 1);
	this->texPos += this->step_val;
	texnum = g_worldmap[this->map_pos.x][this->map_pos.y] - 1;
	color = texture[texnum][TEX_HEIGHT * texY + this->texX];
	color = distance_shade(color, this->perpWallDist);
	// if (this->step.is_hit_y_side && (this->step.y_sign == POSITIVE))
	// 	color = 0;
		//color = (color >> 1) & 8355711;
	return (color);
}

void	renderer__raycast__wall(
	t_renderer* this, t_camera* camera, double zbuffer[WIDTH])
{
	t_walldata	walldata;
	int			x;
	int			y;

	x = -1;
	while (++x < WIDTH)
	{
		walldata__raycast__set_dda_vector(&walldata, camera, x);
		zbuffer[x] = walldata.perpWallDist;
		walldata__draw__set_wall_data(&walldata, camera);
		walldata__draw__set_texture_data(&walldata);
		y = walldata.draw_start - 1;
		while (++y < walldata.draw_end)
			this->buf[y][x] = walldata__draw__wall_texture(&walldata);
	}
}

void	renderer__raycast(t_renderer *this, t_camera *camera)
{
	double	zBuffer[WIDTH];

	renderer__raycast__floor(this, camera);
	renderer__raycast__wall(this, camera, zBuffer);
}
