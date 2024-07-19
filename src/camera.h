#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include "defines.h"

void camera_init(Camera2D *camera);
void camera_update(Camera2D *camera);
void camera_zoom_control(float *zoom);
void camera_target_control(Vector2 *target, float zoom);

#endif