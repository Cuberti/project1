#include "camera.h"

void camera_init(Camera2D *camera)
{
    camera->zoom = DEFUALT_CAMERA_ZOOM;
    camera->rotation = DEFAULT_CAMERA_ROTATION;
    camera->offset = (Vector2)
    {
        .x = WINDOW_WIDTH / 2,
        .y = WINDOW_HEIGHT / 2
    };
    camera->target = (Vector2)
    {
        .x = 200,
        .y = 200
    };
}

void camera_update(Camera2D *camera)
{
    camera_target_control(&camera->target, camera->zoom);
    camera_zoom_control(&camera->zoom);
}

void camera_zoom_control(float *zoom)
{
    float mouse_wheel = GetMouseWheelMove();

    if (mouse_wheel != 0.0f)
    {   
        (*zoom) += (mouse_wheel * ZOOM_INCREMENT);

        if ((*zoom) >= ZOOM_MAX)
        {
            (*zoom) = ZOOM_MAX;
        }

        else if ((*zoom) <= ZOOM_MIN)
        {   
            (*zoom) = ZOOM_MIN;
        }

        //printf("ZOOM: %.1f\n", (*zoom));
    }
}

void camera_target_control(Vector2 *target, float zoom)
{
    Vector2 mouse_delta = GetMouseDelta();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) == true)
    {
        (*target).x += (-mouse_delta.x) / zoom;
        (*target).y += (-mouse_delta.y) / zoom;
    }
}