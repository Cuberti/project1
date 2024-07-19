#ifndef DEFINES_H
#define DEFINES_H

#include "raylib.h"
#include "raymath.h"

#define NON 0

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define CHUNK_SIZE 10
#define WORLD_SIZE 500

#define TSPA_AVAIL_ZONE_SZ 15
#define TSPA_AVAIL_ZONE_OS 7

#define RIVER_DEFAULT_SZ 3
#define RIVER_DEFAULT_OS 1

#define CELL_PIXEL_SIZE 50
#define WORKERS_HITBOX_SIZE 30

#define DEFUALT_CAMERA_ZOOM 1.0f
#define DEFAULT_CAMERA_ROTATION 0.0f
#define ZOOM_INCREMENT 0.25f

#define ZOOM_MAX 2.0f
#define ZOOM_MIN 0.025f

#define MATERIAL_ARRAY_SIZE 3

#define TSPA_DEFAULT_CHANCE 20

#define SOURCE_TEXTURE_SIZE 16

typedef enum material_types
{   
    TREE = 1,
    GRASS = 2,
    STONE = 3,
    WATER = 4,
    SAND = 5,
    SNOW = 6,
    ROCK = 7,
    STICK = 8,

    TITANIUM = 9,
    COAL = 10,
    COPPER = 11,
    IRON = 12,
    GLD = 13,
    URANIUM = 14,
    OIL = 15
} material_types;

typedef enum building_types
{
    WOODEN_HOUSE = 1,
    STOCK = 2
} building_types;

typedef enum equipment_types
{
    PICK = 1,
} equipment_types;

typedef enum mode_types
{
    BUILDING_MODE = 1,
    WORKERS_MODE = 2,
} mode_types;

typedef enum nation_feature_types
{
    UNPREPARED = 1,
} workers_buffs_types;

typedef enum workers_orders_types
{
    MINE_OPDER = 1,
    PICK_UP_ORDER = 2,
    CRAFT_ORDER = 3,
    BUILDING_ORDER = 4,
    MOVE_ORDER = 5,
} workers_orders_types;

typedef struct nation_t
{
    //int id;
    long people;
    //int *influence_from;
    int *feature;
    int feature_count;
} nation_t;

typedef struct material_t
{
    int type;
    float value;
} material_t;

typedef struct item_t
{
    int type;
    float value;
} item_t;

typedef struct building_t building_t;

struct building_t
{
    int type;
    material_t material[4];
    int time;
    void *bt;
};

typedef struct cell_t
{
    int temperature;
    material_t material[MATERIAL_ARRAY_SIZE];
    building_t building;
    int nations_count;
    nation_t *nations;
} cell_t;

typedef struct order_t
{
    int type;
    void *order;
} order_t;

typedef struct workers_t
{
    Vector2 position;
    int nations_count;
    nation_t *nations;
    int items_count;
    item_t *items;
    int orders_count;
    order_t *orders;
} workers_t; 

typedef struct move_ot
{
    Vector2 end_point;
} move_ot;

typedef struct pick_up_ot
{
    Vector2 end_point;
    cell_t *cell;
} pick_up_ot;

typedef struct stock_bt
{
    material_t stock[3];
} stock_bt;

typedef struct mode_t
{
    int mode, sub_mode;
} mode_t;

#define order_new_p(_type, _order, _i) \
    ++(*target_workers)[_i]->orders_count; \
    (*target_workers)[_i]->orders = realloc((*target_workers)[_i]->orders, sizeof(order_t) * (*target_workers)[_i]->orders_count); \
    (*target_workers)[_i]->orders[(*target_workers)[_i]->orders_count - 1].type = _type; \
    (*target_workers)[_i]->orders[(*target_workers)[_i]->orders_count - 1].order = malloc(sizeof(_order));

#define item_new(_type, _value, _i) \
    ++workers[_i].items_count; \
    workers[_i].items = realloc(workers[_i].items, sizeof(item_t) * workers[_i].items_count); \
    workers[_i].items[workers[_i].items_count - 1].type = _type; \
    workers[_i].items[workers[_i].items_count - 1].value = _value;

#endif