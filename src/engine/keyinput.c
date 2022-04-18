#include <stdlib.h>
#include "engine.h"
#include "renderer.h"

void inputhandler__init(t_inputhandler* this) {
  this->keyinfo = (t_inputhandler__keyinfo){true, false, false, true, false};
}
// {false, false, false, false, false}

bool inputhandler__is_movement(t_inputhandler* this) {
  t_inputhandler__keyinfo keyinfo = this->keyinfo;
  return (keyinfo.is_up_pressed || keyinfo.is_down_pressed ||
          keyinfo.is_left_pressed || keyinfo.is_right_pressed);
}

bool inputhandler__is_mouse_movement(t_inputhandler* this) {
  return (this->old_mouse.x != this->mouse.x ||
          this->old_mouse.y != this->mouse.y);
  // t_inputhandler__mouseinfo mouseinfo = this->mouseinfo;
  // return (mouseinfo.is_mouse_moved);
}
// int inputhandler__mouse(t_keycode mouse_input, t_inputhandler *this) {
// 	if (mouse_input == X11EVENTS__ButtonPress) {
// 		this->keyinfo.is_mouse_pressed = true;
// 	} else if (mouse_input == X11EVENTS__ButtonRelease) {
// 		this->keyinfo.is_mouse_pressed = false;
// 	}
// 	return (0);
// }

// FIXME: implement key library
int inputhandler__key_release(t_keycode key, t_inputhandler* this) {
  if (key == KEY_W)
    this->keyinfo.is_up_pressed = false;
  if (key == KEY_S)
    this->keyinfo.is_down_pressed = false;
  if (key == KEY_A)
    this->keyinfo.is_left_pressed = false;
  if (key == KEY_D)
    this->keyinfo.is_right_pressed = false;
  return 0;
}

int inputhandler__key_press(t_keycode key, t_inputhandler* this) {
  if (key == KEY_W)
    this->keyinfo.is_up_pressed = true;
  if (key == KEY_S)
    this->keyinfo.is_down_pressed = true;
  if (key == KEY_A)
    this->keyinfo.is_left_pressed = true;
  if (key == KEY_D)
    this->keyinfo.is_right_pressed = true;
  if (key == KEY_ESC)
    this->keyinfo.is_exit = true;

  return (0);
}
