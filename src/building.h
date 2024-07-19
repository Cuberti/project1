#ifndef BUILDING_H
#define BUILDING_H

#include "defines.h"
#include "world.h"
#include <stdio.h>
#include <stdlib.h>

/* void building_run(cell_t **world); */
void bulding_update(int *mode, building_t building, cell_t **world, Camera2D camera, building_t **target_building);
void building_render(int mode, Camera2D camera, cell_t **world, building_t *target_building);

#endif