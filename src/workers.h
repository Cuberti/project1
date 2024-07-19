#ifndef WORKERS_H
#define WORKERS_H

#include "defines.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

void workers_run(workers_t *workers, int workers_count);

void workers_update(mode_t *mode, workers_t **workers, int *workers_count, Camera2D camera, cell_t **world, int target_nation, workers_t ***target_workers, int *target_workers_count);

void workers_render(mode_t mode, workers_t *workers, int workers_count, workers_t **target_workers, int target_workers_count, int target_nation, cell_t **world, Camera2D camera);

#endif