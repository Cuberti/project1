#ifndef WORLD_H
#define WORLD_H

#include "defines.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void temperature_layer_generate(cell_t **world);
void grass_layer_generate(cell_t **world);
void snow_layer_generate(cell_t **world);
void sand_layer_generate(cell_t **world);
void tree_layer_generate(cell_t **world);
void water_layer_generate(cell_t **world);
void rock_layer_generate(cell_t **world);
void stick_layer_generate(cell_t **world);
void stone_layer_generate(cell_t **world);
void nations_layer_generate(cell_t **world, int *nations_count);
void ore_layer_generate(int type, int min_value, int max_value, int chance, int min_size, int max_size, cell_t **world);
void world_update(Camera2D camera, cell_t **target_cell, cell_t **world);
void world_render(cell_t **world, Camera2D camera, Texture2D texture, cell_t *target_cell);

#endif