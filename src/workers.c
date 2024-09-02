#include "workers.h"

void workers_run(workers_t *workers, int workers_count)
{
    for (int i = 0; i < workers_count; i++)
    {
        if (workers[i].orders_count != 0)
        {
            switch (workers[i].orders[0].type)
            {
                case MOVE_ORDER:
                {
                    if ((workers[i].position.x - ((((move_ot*)workers[i].orders->order)->end_point.x) - CELL_PIXEL_SIZE / 2)) != 0 ||
                        (workers[i].position.y - ((((move_ot*)workers[i].orders->order)->end_point.y) - CELL_PIXEL_SIZE / 2)) != 0)
                    {
                        workers[i].position = Vector2MoveTowards(workers[i].position, (Vector2)
                        {
                            .x = ((move_ot*)workers[i].orders[0].order)->end_point.x - CELL_PIXEL_SIZE / 2,
                            .y = ((move_ot*)workers[i].orders[0].order)->end_point.y - CELL_PIXEL_SIZE / 2,
                        }, 10);

                        continue;
                    }

                    goto FREE;
                }
                break;

                case PICK_UP_ORDER:
                {
                    if (CheckCollisionRecs((Rectangle)
                    {
                        .x = workers[i].position.x,
                        .y = workers[i].position.y,
                        .width = WORKERS_HITBOX_SIZE,
                        .height = WORKERS_HITBOX_SIZE
                    }, (Rectangle)
                    {
                        .x = ((pick_up_ot*)workers[i].orders[0].order)->end_point.x,
                        .y = ((pick_up_ot*)workers[i].orders[0].order)->end_point.y,
                        .width = CELL_PIXEL_SIZE,
                        .height = CELL_PIXEL_SIZE
                    }) == false)
                    {
                        workers[i].position = Vector2MoveTowards(workers[i].position, (Vector2)
                        {
                            .x = ((pick_up_ot*)workers[i].orders[0].order)->end_point.x + (CELL_PIXEL_SIZE - 40),
                            .y = ((pick_up_ot*)workers[i].orders[0].order)->end_point.y + (CELL_PIXEL_SIZE - 40),
                        }, 10);

                        continue;
                    }

                    for (int j = 0; j < workers[i].items_count; j++)
                    {
                        if (workers[i].items[j].type == ((pick_up_ot*)workers[i].orders[0].order)->cell->material[2].type)
                        {
                            workers[i].items[j].value += ((pick_up_ot*)workers[i].orders[0].order)->cell->material[2].value;

                            ((pick_up_ot*)workers[i].orders[0].order)->cell->material[2].value = 0;

                            goto FREE;
                        }
                    }

                    item_new(((pick_up_ot*)workers[i].orders[0].order)->cell->material[2].type, ((pick_up_ot*)workers[i].orders[0].order)->cell->material[2].value, i);

                    ((pick_up_ot*)workers[i].orders[0].order)->cell->material[2].value = 0;

                    goto FREE;
                }
                break;
            }

            FREE:

            free(workers[i].orders[0].order);

            int k = 0;

            while (++k < workers[i].orders_count)
            {
                workers[i].orders[k - 1] = workers[i].orders[k];
            }

            --workers[i].orders_count;

            workers[i].orders = (workers[i].orders_count <= 0 ? realloc(workers[i].orders, 1) :
                                                                realloc(workers[i].orders, sizeof(order_t) * workers[i].orders_count));
        }
    }
}

void workers_update(mode_t *mode, workers_t * *workers, int * workers_count, Camera2D camera, cell_t **world, int target_nation, workers_t * **target_workers, int * target_workers_count)
{
    if (IsKeyPressed(KEY_W))
    {
        mode->mode = (mode->mode == WORKERS_MODE ? NON : WORKERS_MODE);
    }

    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        for (int i = 0; i < (*workers_count); i++)
        {
            if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), (Rectangle)
            {
                .x = (*workers)[i].position.x,
                .y = (*workers)[i].position.y,
                .width = WORKERS_HITBOX_SIZE,
                .height = WORKERS_HITBOX_SIZE,
            }))
            {
                (*target_workers_count) = 1;
                (*target_workers) = realloc((*target_workers), sizeof(workers_t*));
                (*target_workers)[(*target_workers_count) - 1] = workers[i];

                break;
            }
        }
    }

    else if (mode->mode == WORKERS_MODE)
    {
        switch (GetKeyPressed())
        {
            case KEY_ONE: { mode->sub_mode = (mode->sub_mode == MINE_OPDER ? NON : MINE_OPDER); } break;

            case KEY_TWO: { mode->sub_mode = (mode->sub_mode == CRAFT_ORDER ? NON : CRAFT_ORDER); } break;

            case KEY_THREE: { mode->sub_mode = (mode->sub_mode == PICK_UP_ORDER ? NON : PICK_UP_ORDER); } break;

            case KEY_FOUR: { mode->sub_mode = (mode->sub_mode == BUILDING_ORDER ? NON : BUILDING_ORDER); } break;

            case KEY_FIVE: { mode->sub_mode = (mode->sub_mode == MOVE_ORDER ? NON : MOVE_ORDER); } break;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            switch (mode->sub_mode)
            {
                case MOVE_ORDER:
                {
                    for (int i = 0; i < (*target_workers_count); i++)
                    {
                        Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), camera);

                        order_new_p(MOVE_ORDER, move_ot, i)

                        ((move_ot*)(*target_workers)[i]->orders[(*target_workers)[i]->orders_count - 1].order)->end_point = (Vector2)
                        {
                            .x = mouse_position.x,
                            .y = mouse_position.y
                        };
                    }
                }
                break;

                case PICK_UP_ORDER:
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
                            (world[i][j].material[2].type == STICK ||
                             world[i][j].material[2].type == ROCK))
                            {
                                for (int l = 0; l < (*target_workers_count); l++)
                                {
                                    order_new_p(PICK_UP_ORDER, pick_up_ot, l);

                                    ((pick_up_ot*)(*target_workers)[l]->orders[(*target_workers)[l]->orders_count - 1].order)->cell = &world[i][j];
                                    ((pick_up_ot*)(*target_workers)[l]->orders[(*target_workers)[l]->orders_count - 1].order)->end_point = (Vector2)
                                    {
                                        .x = j * CELL_PIXEL_SIZE,
                                        .y = i * CELL_PIXEL_SIZE
                                    };
                                }

                                goto EXIT;
                            }
                        }
                    }
                }
                break;

                default:
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
                            world[i][j].material[1].type != WATER)
                            {
                                for (int k = 0; k < world[i][j].nations_count; k++)
                                {
                                    if (world[i][j].nations[k].people > 0)
                                    {
                                        goto NEXT;
                                    }
                                }

                                goto EXIT;

                                NEXT:

                                ++(*workers_count);
                                (*workers) = realloc((*workers), sizeof(workers_t) * (*workers_count));
                                (*workers)[*workers_count - 1].position = (Vector2)
                                { 
                                    .x = j * CELL_PIXEL_SIZE + (CELL_PIXEL_SIZE - 40),
                                    .y = i * CELL_PIXEL_SIZE + (CELL_PIXEL_SIZE - 40),
                                };
                                (*workers)[*workers_count - 1].items_count = 0;
                                (*workers)[*workers_count - 1].items = malloc(1);
                                (*workers)[*workers_count - 1].orders_count = 0;
                                (*workers)[*workers_count - 1].orders = malloc(1);
                                (*workers)[*workers_count - 1].nations_count = 0;
                                (*workers)[*workers_count - 1].nations = malloc(1);

                                for (int k = 0; k < world[i][j].nations_count; k++)
                                {
                                    if (world[i][j].nations[k].people > 0)
                                    {
                                        ++(*workers)[*workers_count - 1].nations_count;
                                        (*workers)[*workers_count - 1].nations = realloc((*workers)[*workers_count - 1].nations, sizeof(nation_t) * (*workers)[*workers_count - 1].nations_count);
                                        (*workers)[*workers_count - 1].nations[(*workers)[*workers_count - 1].nations_count - 1] = world[i][j].nations[k];
                                        (*workers)[*workers_count - 1].nations[(*workers)[*workers_count - 1].nations_count - 1].people = world[i][j].nations[k].people;
                                        world[i][j].nations[k].people = 0;
                                    }
                                }

                                goto EXIT;
                            }
                        }
                    }
                }
                break;
            }

            EXIT:
        }
    }
}

void workers_render(mode_t mode, workers_t *workers, int workers_count, workers_t **target_workers, int target_workers_count, int target_nation, cell_t **world, Camera2D camera)
{
    if (mode.mode == WORKERS_MODE)
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

                DrawRectangle(j * CELL_PIXEL_SIZE, i * CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, CELL_PIXEL_SIZE, (Color)
                {
                    .r = 81,
                    .g = 81,
                    .b = 81,
                    .a = 100
                });

/*                 for (int l = 0; l < world[i][j].nations_count; l++)
                {
                    if (world[i][j].nations[l].id == target_nation)
                    {
                        DrawRectangle(j * CELL_PIXEL_SIZE, i * CELL_PIXEL_SIZE, CELL_PIXEL_SIZE * 10, CELL_PIXEL_SIZE * 10, GREEN);
                    }
                } */
            }
        }
    }

    for (int i = 0; i < workers_count; i++)
    {
        DrawRectangle(workers[i].position.x, workers[i].position.y, WORKERS_HITBOX_SIZE, WORKERS_HITBOX_SIZE, RED);
    }

    for (int i = 0; i < target_workers_count; i++)
    {
        DrawRectangleLines(target_workers[i]->position.x - 1, target_workers[i]->position.y - 1, WORKERS_HITBOX_SIZE + 2, WORKERS_HITBOX_SIZE + 2, YELLOW);
    }
}
