#include "building.h"

/* void building_run(cell_t **world)
{

} */

void bulding_update(int *mode, building_t building, cell_t **world, Camera2D camera, building_t **target_building)
{
    if (IsKeyPressed(KEY_Q))
    {
        (*mode) = ((*mode) == BUILDING_MODE ? NON : BUILDING_MODE);
    }

    if ((*mode) == BUILDING_MODE)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            for (int i = 0; i < WORLD_SIZE; i++)
            {
                for (int j = 0; j < WORLD_SIZE; j++)
                {
                    if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), (Rectangle)
                    {
                        .x = j * CELL_PIXEL_SIZE,
                        .y = i * CELL_PIXEL_SIZE,
                        .width = CELL_PIXEL_SIZE,
                        .height = CELL_PIXEL_SIZE
                    }) &&
                    world[i][j].building.type == NON)
                    {
                        world[i][j].building = (building_t)
                        {
                            .type = building.type,
                            .material = 
                            {
                                (material_t)
                                {
                                    .type = building.material[0].type,
                                    .value = building.material[0].value,
                                },

                                (material_t)
                                {
                                    .type = building.material[1].type,
                                    .value = building.material[1].value,
                                },

                                (material_t)
                                {
                                    .type = building.material[2].type,
                                    .value = building.material[2].value,
                                },

                                (material_t)
                                {
                                    .type = building.material[3].type,
                                    .value = building.material[3].value,
                                },
                            },
                            .time = building.time,
                            .bt = building.bt,
                        };

                        switch (building.type)
                        {
                            case STOCK:
                                world[i][j].building.bt = malloc(sizeof(stock_bt));

                                ((stock_bt*)world[i][j].building.bt)->stock[0].type = NON;
                                ((stock_bt*)world[i][j].building.bt)->stock[0].value = 0;

                                ((stock_bt*)world[i][j].building.bt)->stock[1].type = NON;
                                ((stock_bt*)world[i][j].building.bt)->stock[1].value = 0;

                                ((stock_bt*)world[i][j].building.bt)->stock[2].type = NON;
                                ((stock_bt*)world[i][j].building.bt)->stock[2].value = 0;
                            break;
                        }

                        (*target_building) = &world[i][j].building;

                        goto EXIT;
                    }
                }
            }
        }

        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            for (int i = 0; i < WORLD_SIZE; i++)
            {
                for (int j = 0; j < WORLD_SIZE; j++)
                {
                    if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), (Rectangle)
                    {
                        .x = j * CELL_PIXEL_SIZE,
                        .y = i * CELL_PIXEL_SIZE,
                        .width = CELL_PIXEL_SIZE,
                        .height = CELL_PIXEL_SIZE
                    }) &&
                    world[i][j].building.type != NON)
                    {
                        (*target_building) = &world[i][j].building;

                        goto EXIT;
                    }
                }
            }
        }

        EXIT:
    }
}

void building_render(int mode, Camera2D camera, cell_t **world, building_t *target_building)
{  
    if (mode == BUILDING_MODE)
    {
        for (int y = 0; y < WORLD_SIZE; y++)
        {
            for (int x = 0; x < WORLD_SIZE; x++)
            {
                if (CheckCollisionRecs((Rectangle)
                {
                    .x = camera.target.x - ((WINDOW_WIDTH / 2) / camera.zoom),
                    .y = camera.target.y - ((WINDOW_HEIGHT / 2) / camera.zoom),
                    .width = WINDOW_WIDTH / camera.zoom,
                    .height = WINDOW_HEIGHT / camera.zoom
                }, (Rectangle)
                {
                    .x = x * CELL_PIXEL_SIZE,
                    .y = y * CELL_PIXEL_SIZE,
                    .width = CELL_PIXEL_SIZE,
                    .height = CELL_PIXEL_SIZE
                }) == false)
                {
                    continue;
                }

                DrawRectangle(x * CELL_PIXEL_SIZE, y * CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, (Color)
                {
                    .r = 81,
                    .g = 81,
                    .b = 81,
                    .a = 100
                });

                if (world[y][x].building.type != NON)
                {
                    DrawRectangle(x * CELL_PIXEL_SIZE, y * CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, world[y][x].building.time <= 0 ? GREEN : RED);
                }

                if (target_building == &world[y][x].building)
                {
                    DrawRectangle(x * CELL_PIXEL_SIZE, y * CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, BLUE);
                }
            }
        }

        DrawText("BUILDING", camera.target.x - ((WINDOW_WIDTH / 2) / camera.zoom), camera.target.y - ((WINDOW_HEIGHT / 2) / camera.zoom), 20 / camera.zoom, GRAY);
    }
}