#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "defines.h"
#include "camera.h"
#include "world.h"
#include "building.h"
#include "workers.h"

int main()
{
    cell_t **world = malloc(sizeof(cell_t*) * WORLD_SIZE);
    if (!world) exit(1);
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        world[i] = malloc(sizeof(cell_t) * WORLD_SIZE);
        if (!world[i]) exit(1);
    }
    Camera2D camera;
    building_t building = (building_t)
    {
        .type = STOCK,
        .material = 
        {
            (material_t)
            {
                .type = NON,
                .value = 0
            },

            (material_t)
            {
                .type = NON,
                .value = 0
            },

            (material_t)
            {
                .type = NON,
                .value = 0
            },

            (material_t)
            {
                .type = NON,
                .value = 0
            },
        },
        .time = 5,
        .bt = NULL,
    };
    mode_t mode;
    mode.mode = NON;
    mode.sub_mode = NON;

    cell_t *target_cell = NULL;
    building_t *target_building = NULL;

    workers_t **target_workers = malloc(1);
    int target_workers_count = 0;

    int target_nation = -1;

    int nations_count = 0;

    workers_t *workers = malloc(1);
    int workers_count = 0;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "game");
    SetTargetFPS(60);

    Texture2D texture = LoadTexture("sprites/blocks.png");

    temperature_layer_generate(world);
    grass_layer_generate(world);
    snow_layer_generate(world);
    sand_layer_generate(world);
    tree_layer_generate(world);
    water_layer_generate(world);
    rock_layer_generate(world);
    stick_layer_generate(world);
    stone_layer_generate(world);

    nations_layer_generate(world, &nations_count);

    target_nation = GetRandomValue(0, nations_count - 1);
    
    ore_layer_generate(COAL, 50, 100, 50, 5, 7, world);
    ore_layer_generate(COPPER, 60, 120, 70, 5, 7, world);
    ore_layer_generate(IRON, 40, 80, 30, 3, 5, world);
    ore_layer_generate(OIL, 20, 40, 10, 3, 5, world);
    ore_layer_generate(GLD, 20, 30, 10, 3, 5, world);
    ore_layer_generate(TITANIUM, 10, 20, 20, 3, 3, world);

    camera_init(&camera);

    double time_speed = 1.0f;
    clock_t begin_time = clock();
    clock_t update_time = clock();

    while (!WindowShouldClose())
    {
        update_time = clock();

        if ( ((-(begin_time - update_time)) / CLOCKS_PER_SEC) >= time_speed )
        {
            workers_run(workers, workers_count);

            begin_time = clock();
        }

        if (IsKeyPressed(KEY_R)) // Reload world.
        {
            for (int i = 0; i < WORLD_SIZE; i++)
            {   
                for (int j = 0; j < WORLD_SIZE; j++)
                {
                    world[i][j].nations_count = 0;
                    world[i][j].nations = NULL;

                    world[i][j].building.time = 0;
                    world[i][j].building.bt = NULL;
                    world[i][j].building.type = 0;
                    world[i][j].building.material[0].type = 0;
                    world[i][j].building.material[0].value = 0;
                    world[i][j].building.material[1].type = 0;
                    world[i][j].building.material[1].value = 0;
                    world[i][j].building.material[2].type = 0;
                    world[i][j].building.material[2].value = 0;
                    world[i][j].building.material[3].type = 0;
                    world[i][j].building.material[3].value = 0;

                    world[i][j].temperature = 0;
                    world[i][j].material[0].type = NON;
                    world[i][j].material[0].value = NON;
                    world[i][j].material[1].type = NON;
                    world[i][j].material[1].value = NON;
                    world[i][j].material[2].type = NON;
                    world[i][j].material[2].value = NON;
                }
            }

            temperature_layer_generate(world);
            grass_layer_generate(world);
            snow_layer_generate(world);
            sand_layer_generate(world);
            tree_layer_generate(world);
            water_layer_generate(world);
            rock_layer_generate(world);
            stick_layer_generate(world);
            stone_layer_generate(world);

            target_nation = GetRandomValue(0, nations_count - 1);
            
            ore_layer_generate(COAL, 50, 100, 50, 5, 7, world);
            ore_layer_generate(COPPER, 60, 120, 70, 5, 7, world);
            ore_layer_generate(IRON, 40, 80, 30, 3, 5, world);
            ore_layer_generate(OIL, 20, 40, 10, 3, 5, world);
            ore_layer_generate(GLD, 20, 30, 10, 3, 5, world);
            ore_layer_generate(TITANIUM, 10, 20, 20, 3, 3, world);
        }
        
        camera_update(&camera);
        world_update(camera, &target_cell, world);
        bulding_update(&mode.mode, building, world, camera, &target_building);
        workers_update(&mode, &workers, &workers_count, camera, world, target_nation, &target_workers, &target_workers_count);

        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(BLACK);

        world_render(world, camera, texture, target_cell);
        building_render(mode.mode, camera, world, target_building);
        workers_render(mode, workers, workers_count, target_workers, target_workers_count, target_nation, world, camera);

        DrawText(TextFormat("%d", GetFPS()), camera.target.x - ((WINDOW_WIDTH / 2) / camera.zoom), camera.target.y - ((WINDOW_HEIGHT / 2) / camera.zoom) + ((mode.mode != NON ? 20 : 0) / camera.zoom), 20 / camera.zoom, RED);

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    for (int i = 0; i < workers_count; i++)
    {
        free(workers[i].items);
        free(workers[i].orders);
        free(workers[i].nations);
    }

    free(workers);
    free(target_workers);

    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            for (int k = 0; k < world[i][j].nations_count; k++)
            {
/*                 if (world[i][j].nations[k].feature != NULL)
                {
                    free(world[i][j].nations[k].feature);
                }

                if (world[i][j].nations[k].influence_from != NULL)
                {
                    free(world[i][j].nations[k].influence_from);
                } */
            }

            if (world[i][j].nations != NULL)
            {
                free(world[i][j].nations);
            }

            if (world[i][j].building.bt != NULL)
            {
                free(world[i][j].building.bt);
            }
        }
    }

    UnloadTexture(texture);

    for (int i = 0; i < WORLD_SIZE; i++)
    {
        free(world[i]);
        world[i] = NULL;
    }

    free(world);
    world = NULL;

    return 0;
}
