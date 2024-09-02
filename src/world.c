#include "world.h"

void temperature_layer_generate(cell_t **world)
{
    int temp_temperature = 0;

    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            world[i][j].temperature = temp_temperature;
        }

        ++temp_temperature;
    }
}

void grass_layer_generate(cell_t **world)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            world[i][j].material[0].type = NON;
            world[i][j].material[0].value = NON;

            world[i][j].material[1].type = GRASS;
            world[i][j].material[1].value = GetRandomValue(40, 150);

            world[i][j].material[2].type = NON;
            world[i][j].material[2].value = NON;

            world[i][j].building = (building_t)
            {
                .type = NON,
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
                .time = 0,
                .bt = NULL,
            };
        }
    }
}

void snow_layer_generate(cell_t **world)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        int rand = GetRandomValue(2, 8);

        for (int j = 0; j < (100 + rand); j++)
        {
            world[j][i].material[1].type = SNOW;
            world[j][i].material[1].value = GetRandomValue(40, 100);
        }
    }
}

void sand_layer_generate(cell_t **world)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        int rand = GetRandomValue(2, 8);

        for (int j = (400 - rand); j < WORLD_SIZE; j++)
        {
            world[j][i].material[1].type = SAND;
            world[j][i].material[1].value = GetRandomValue(40, 100);
        }
    }
}

void tree_layer_generate(cell_t **world)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {  
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            if (GetRandomValue(1, 10000) <= (world[i][j].material[1].type == SNOW ? 20 : 
                                            (world[i][j].material[1].type == SAND ? 0 : 40)))
            {   
                int condition_k = ((i - TSPA_AVAIL_ZONE_OS) + TSPA_AVAIL_ZONE_SZ),
                    condition_l = ((j - TSPA_AVAIL_ZONE_OS) + TSPA_AVAIL_ZONE_SZ);

                for (int k = (i - TSPA_AVAIL_ZONE_OS); k < condition_k; k++)
                {
                    for (int l = (j - TSPA_AVAIL_ZONE_OS); l < condition_l; l++)
                    {
                        if  (l < 0 || 
                             k < 0 || 
                             l >= WORLD_SIZE || 
                             k >= WORLD_SIZE)
                        {
                            continue;
                        }

                        else if (world[k][l].material[2].type == TREE)
                        {
                            goto FOR_J_LABEL;
                        }
                    }
                }

                int move_count = GetRandomValue((world[i][j].material[1].type == SNOW ? 10 : 
                                                (world[i][j].material[1].type == SAND ? 0 : 10)), (world[i][j].material[1].type == SNOW ? 40 : 
                                                                                                  (world[i][j].material[1].type == SAND ? 0 : 60)));

                int target_i = i, 
                    target_j = j;

                for (int k = 0; k < move_count; k++)
                {
                    int chance = GetRandomValue(0, 3);

                    int condition_l = ((target_i - 1) + 3),
                        condition_n = ((target_j - 1) + 3);

                    for (int l = (target_i - 1); l < condition_l; l++)
                    {
                        for (int n = (target_j - 1); n < condition_n; n++)
                        {
                            if (l < 0 ||
                                n < 0 ||
                                l >= WORLD_SIZE ||
                                n >= WORLD_SIZE)
                            {
                                continue;
                            }

                            world[l][n].material[2].type = TREE;
                            world[l][n].material[2].value = GetRandomValue(50, 120);
                        }
                    }

                    switch (chance)
                    {
                        case 0:
                            --target_i;
                            --target_j;
                        break;

                        case 1:
                            --target_i;
                            ++target_j;
                        break;
                        
                        case 2:
                            ++target_i;
                            ++target_j;
                        break;

                        case 3:
                            ++target_i;
                            --target_j;
                        break;
                    }
                }
            }

            FOR_J_LABEL:
        }
    }

    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {   
            int count = 0;

            int condition_k = ((i - 1) + 3),
                condition_l = ((j - 1) + 3);

            for (int k = (i - 1); k < condition_k; k++)
            {
                for (int l = (j - 1); l < condition_l; l++)
                {
                    if (count >= 5)
                    {
                        world[i][j].material[2].type = TREE;
                        world[i][j].material[2].value = GetRandomValue(50, 120);
                    }

                    else if (k < 0 || 
                             l < 0 || 
                             k >= WORLD_SIZE || 
                             l >= WORLD_SIZE)
                    {
                        continue;
                    }

                    else if (world[k][l].material[2].type == TREE)
                    {
                        ++count;
                    }
                }
            }
        }
    }
}

void water_layer_generate(cell_t **world)
{   
    for (int i = 0; i < WORLD_SIZE; i++)
    {  
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            if (GetRandomValue(1, 10000) <= (world[i][j].material[1].type == SNOW ? 60 : 
                                            (world[i][j].material[1].type == SAND ? 20 : 20)) && 
                (world[i][j].material[1].type != WATER ||
                 world[i][j].material[2].type == NON))
            {   
                int move_count = GetRandomValue((world[i][j].material[1].type == SNOW ? 20 : 
                                                (world[i][j].material[1].type == SAND ? 20 : 20)),  (world[i][j].material[1].type == SNOW ? 70 : 
                                                                                                    (world[i][j].material[1].type == SAND ? 80 : 200)));

                int target_i = i, 
                    target_j = j;

                for (int k = 0; k < move_count; k++)
                {
                    int chance = GetRandomValue(0, 3);

                    int condition_l = ((target_i - 1) + 3),
                        condition_n = ((target_j - 1) + 3);

                    for (int l = (target_i - 1); l < condition_l; l++)
                    {
                        for (int n = (target_j - 1); n < condition_n; n++)
                        {
                            if (l < 0 ||
                                n < 0 ||
                                l >= WORLD_SIZE ||
                                n >= WORLD_SIZE)
                            {
                                continue;
                            }

                            int condition_b = ((l - 1) + 3),
                                condition_v = ((n - 1) + 3);

                            for (int b = (l - 1); b < condition_b; b++)
                            {
                                for (int v = (n - 1); v < condition_v; v++)
                                {
                                    if (b < 0 ||
                                        v < 0 ||
                                        b >= WORLD_SIZE ||
                                        v >= WORLD_SIZE)
                                    {
                                        continue;
                                    }

                                    else if (world[b][v].material[2].type == TREE)
                                    {
                                        switch (chance)
                                        {
                                            case 0:
                                                ++target_i;
                                                ++target_j;
                                            break;

                                            case 1:
                                                ++target_i;
                                                --target_j;
                                            break;

                                            case 2:
                                                --target_i;
                                                --target_j;
                                            break;

                                            case 3:
                                                --target_i;
                                                ++target_j;
                                            break;
                                        }

                                        ++k;

                                        goto FOR_K_LABEL;
                                    }
                                }
                            }

                            world[l][n].material[1].type = WATER;
                            world[l][n].material[1].value = GetRandomValue(20, 80);
                        }
                    }

                    switch (chance)
                    {
                        case 0:
                            --target_i;
                            --target_j;
                        break;

                        case 1:
                            --target_i;
                            ++target_j;
                        break;

                        case 2:
                            ++target_i;
                            ++target_j;
                        break;

                        case 3:
                            ++target_i;
                            --target_j;
                        break;
                    }

                    FOR_K_LABEL:
                }
            }
        }
    }

    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {   
            int count = 0;

            int condition_k = ((i - 1) + 3),
                condition_l = ((j - 1) + 3);

            for (int k = (i - 1); k < condition_k; k++)
            {
                for (int l = (j - 1); l < condition_l; l++)
                {
                    if (count >= 5)
                    {
                        world[i][j].material[1].type = WATER;
                        world[i][j].material[1].value = GetRandomValue(20, 80);
                    }

                    else if (k < 0 || 
                             l < 0 || 
                             k >= WORLD_SIZE || 
                             l >= WORLD_SIZE)
                    {
                        continue;
                    }

                    else if (world[k][l].material[1].type == WATER)
                    {
                        ++count;
                    }
                }
            }
        }
    }
}

void rock_layer_generate(cell_t **world)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            if (GetRandomValue(0, 10000) <= 300 &&
                world[i][j].material[1].type != WATER &&
                world[i][j].material[2].type == NON) 
            {
                world[i][j].material[2].type = ROCK;
                world[i][j].material[2].value = GetRandomValue(5, 20);
            }
        }
    }
}

void stick_layer_generate(cell_t **world)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            if (GetRandomValue(0, 10000) <= 300 &&
                world[i][j].material[1].type != WATER &&
                world[i][j].material[2].type == NON) 
            {
                world[i][j].material[2].type = STICK;
                world[i][j].material[2].value = GetRandomValue(5, 20);
            }
        }
    }
}

void stone_layer_generate(cell_t **world)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            if (GetRandomValue(1, 10000) <= 30 && 
                (world[i][j].material[1].type != WATER ||
                 world[i][j].material[2].type != TREE))
            {   
                int move_count = GetRandomValue(10, 30);

                int target_i = i, 
                    target_j = j;

                for (int k = 0; k < move_count; k++)
                {
                    int chance = GetRandomValue(0, 3);

                    int condition_l = ((target_i - 1) + 3),
                        condition_n = ((target_j - 1) + 3);

                    for (int l = (target_i - 1); l < condition_l; l++)
                    {
                        for (int n = (target_j - 1); n < condition_n; n++)
                        {
                            if (l < 0 ||
                                n < 0 ||
                                l >= WORLD_SIZE ||
                                n >= WORLD_SIZE)
                            {
                                continue;
                            }

                            int condition_b = ((l - 2) + 5),
                                condition_v = ((n - 2) + 5);

                            for (int b = (l - 2); b < condition_b; b++)
                            {
                                for (int v = (n - 2); v < condition_v; v++)
                                {
                                    if (b < 0 ||
                                        v < 0 ||
                                        b >= WORLD_SIZE ||
                                        v >= WORLD_SIZE)
                                    {
                                        continue;
                                    }

                                    else if (world[b][v].material[2].type == TREE ||
                                             world[b][v].material[1].type == WATER)
                                    {
                                        switch (chance)
                                        {
                                            case 0:
                                                ++target_i;
                                                ++target_j;
                                            break;

                                            case 1:
                                                ++target_i;
                                                --target_j;
                                            break;

                                            case 2:
                                                --target_i;
                                                --target_j;
                                            break;

                                            case 3:
                                                --target_i;
                                                ++target_j;
                                            break;
                                        }

                                        ++k;

                                        goto FOR_K_LABEL;
                                    }
                                }
                            }

                            world[l][n].material[1].type = STONE;
                            world[l][n].material[1].value = GetRandomValue(30, 100);
                        }
                    }

                    switch (chance)
                    {
                        case 0:
                            --target_i;
                            --target_j;
                        break;

                        case 1:
                            --target_i;
                            ++target_j;
                        break;

                        case 2:
                            ++target_i;
                            ++target_j;
                        break;

                        case 3:
                            ++target_i;
                            --target_j;
                        break;
                    }

                    FOR_K_LABEL:
                }
            }
        }
    }

    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {   
            int count = 0;

            int condition_k = ((i - 1) + 3),
                condition_l = ((j - 1) + 3);

            for (int k = (i - 1); k < condition_k; k++)
            {
                for (int l = (j - 1); l < condition_l; l++)
                {
                    if (count >= 5)
                    {
                        world[i][j].material[1].type = STONE;
                        world[i][j].material[1].value = GetRandomValue(30, 100);
                    }

                    else if (k < 0 || 
                             l < 0 || 
                             k >= WORLD_SIZE || 
                             l >= WORLD_SIZE)
                    {
                        continue;
                    }

                    else if (world[k][l].material[1].type == STONE)
                    {
                        ++count;
                    }
                }
            }
        }
    }
}

void ore_layer_generate(int type, int min_value, int max_value, int chance, int min_size, int max_size, cell_t **world)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {   
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            if (GetRandomValue(0, 10000) <= chance &&
                world[i][j].material[0].type == NON)
            {
                int sizes[2] = 
                {
                    min_size,
                    max_size
                };

                int size_id = GetRandomValue(0, 1);

                int condition_k = ((i - floor(sizes[size_id] / 2)) + sizes[size_id]),
                    condition_l = ((j - floor(sizes[size_id] / 2)) + sizes[size_id]);

                for (int k = (i - floor(sizes[size_id] / 2)); k < condition_k; k++)
                {
                    for (int l = (j - floor(sizes[size_id] / 2)); l < condition_l; l++)
                    {
                        if (k < 0 ||
                            l < 0 ||
                            k >= WORLD_SIZE ||
                            l >= WORLD_SIZE)
                        {
                            continue;
                        }   

                        else if (world[k][l].material[1].type == STONE)
                        {
                            world[k][l].material[2].type = type;
                            world[k][l].material[2].value = GetRandomValue(min_value, max_value);
                        }

                        else 
                        {
                            world[k][l].material[0].type = type;
                            world[k][l].material[0].value = GetRandomValue(min_value, max_value);
                        }
                    }
                }
            }
        }
    }
}

void nations_layer_generate(cell_t **world, int *nations_count)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            if (world[i][j].material[1].type != WATER)
            {
                world[i][j].nations_count = 0;
                world[i][j].nations = malloc(1);
            }

            else
            {
                world[i][j].nations_count = -1;
                world[i][j].nations = NULL;
            }
        }
    }

    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            if (world[i][j].material[1].type != WATER &&
                GetRandomValue(0, 30000) >= 29990)
            {
                ++(*nations_count);

                ++world[i][j].nations_count;
                world[i][j].nations = realloc(world[i][j].nations, sizeof(nation_t) * world[i][j].nations_count);
                world[i][j].nations[world[i][j].nations_count - 1].feature = malloc(1);
                world[i][j].nations[world[i][j].nations_count - 1].feature_count = 0;
                world[i][j].nations[world[i][j].nations_count - 1].id = (*nations_count) - 1;
                world[i][j].nations[world[i][j].nations_count - 1].people = GetRandomValue(2, 5);
                world[i][j].nations[world[i][j].nations_count - 1].influence_from = malloc(1);

/*                 for (int l = 0; l < WORLD_SIZE; l++)
                {
                    for (int n = 0; n < WORLD_SIZE; n++)
                    {
                        for (int m = 0; m < world[l][n].nations_count; m++)
                        {
                            world[l][n].nations[m].influence_from = realloc(world[l][n].nations[m].influence_from, sizeof(int) * (*nations_count));
                            world[l][n].nations[m].influence_from[(*nations_count) - 1] = 0;
                        }
                    }
                } */

                //world[i][j].nations[world[i][j].nations_count - 1].influence_from[(*nations_count) - 1] = 100;       
            }
        }
    }
}

void world_update(Camera2D camera, cell_t **target_cell, cell_t **world)
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
                }))
                {
                    (*target_cell) = &world[i][j];
                }
            }
        }
    }
}

void world_render(cell_t **world, Camera2D camera, Texture2D texture, cell_t *target_cell)
{
    for (int i = 0; i < WORLD_SIZE; i++)
    {
        for (int j = 0; j < WORLD_SIZE; j++)
        {
            if (CheckCollisionRecs((Rectangle)
            {
                .x = camera.target.x - ((WINDOW_WIDTH / 2) / camera.zoom),
                .y = camera.target.y - ((WINDOW_HEIGHT / 2) / camera.zoom),
                .width = WINDOW_WIDTH / camera.zoom,
                .height = WINDOW_HEIGHT / camera.zoom
            }, (Rectangle)
            {
                .x = j * CELL_PIXEL_SIZE,
                .y = i * CELL_PIXEL_SIZE,
                .width = CELL_PIXEL_SIZE,
                .height = CELL_PIXEL_SIZE
            }) == false)
            {
                continue;
            }

            DrawTexturePro(texture, (Rectangle)
            {
                .x = (world[i][j].material[1].type == GRASS ? 0 : 
                     (world[i][j].material[1].type == STONE ? 16 : 
                     (world[i][j].material[1].type == SAND ? 32 : 
                     (world[i][j].material[1].type == WATER ? 48 : 
                     (world[i][j].material[1].type == SNOW ? 64 : -16))))),

                .y = 0,
                .width = SOURCE_TEXTURE_SIZE,
                .height = SOURCE_TEXTURE_SIZE
            }, (Rectangle)
            {
                .x = j * CELL_PIXEL_SIZE,
                .y = i * CELL_PIXEL_SIZE,
                .width = CELL_PIXEL_SIZE,
                .height = CELL_PIXEL_SIZE
            }, (Vector2)
            {
                .x = 0,
                .y = 0
            }, 0.0f, WHITE);

            DrawTexturePro(texture, (Rectangle)
            {
                .x = (world[i][j].material[2].type == TREE ? 0 :
                     (world[i][j].material[2].type == STICK ? 0 :
                     (world[i][j].material[2].type == NON ? -16 : 16))),

                .y = (world[i][j].material[2].type == TREE ? 16 :
                     (world[i][j].material[2].type == STICK ? 32 :
                     (world[i][j].material[2].type == ROCK ? 112 : 
                     (world[i][j].material[2].type == TITANIUM ? 16 : 
                     (world[i][j].material[2].type == COAL ? 32 : 
                     (world[i][j].material[2].type == COPPER ? 48 : 
                     (world[i][j].material[2].type == IRON ? 64 : 
                     (world[i][j].material[2].type == GLD ? 80 :
                     (world[i][j].material[2].type == OIL ? 96 : -16))))))))),

                .width = SOURCE_TEXTURE_SIZE,
                .height = SOURCE_TEXTURE_SIZE
            }, (Rectangle)
            {
                .x = j * CELL_PIXEL_SIZE,
                .y = i * CELL_PIXEL_SIZE,
                .width = CELL_PIXEL_SIZE,
                .height = CELL_PIXEL_SIZE
            }, (Vector2)
            {
                .x = 0,
                .y = 0
            }, 0.0f, WHITE);

            if (target_cell == &world[i][j])
            {
                DrawRectangleLines(j * CELL_PIXEL_SIZE, i * CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, YELLOW);
            }
        }
    }
}
