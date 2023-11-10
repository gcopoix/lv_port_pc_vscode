
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <unistd.h>
#if LV_USE_SDL
  #define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
  #include <SDL2/SDL.h>
#elif LV_USE_X11
#endif
#include <stdio.h>
#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"

/*********************
 *      DEFINES
 *********************/

/*On OSX SDL needs different handling*/
#if defined(__APPLE__) && defined(TARGET_OS_MAC)
# if __APPLE__ && TARGET_OS_MAC
#define SDL_APPLE
# endif
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

static bool terminate = false;

/**********************
 *      MACROS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void on_close_cb(void * disp)
{
    LV_UNUSED(disp);
    terminate = true;
}

/*
 * Target dependent initialization of the Hardware Abstraction Layer (HAL)
 * for the Light and Versatile Graphics Library (LVGL)
 */
static lv_disp_t* hal_init(int32_t monitor_hor_res, int32_t monitor_ver_res)
{
    lv_disp_t* disp;
#if LV_USE_SDL
    disp = lv_sdl_window_create(monitor_hor_res, monitor_ver_res);

    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);

    lv_indev_t * mouse = lv_sdl_mouse_create();
    lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
    lv_indev_set_group(mousewheel, lv_group_get_default());

    lv_indev_t * keyboard = lv_sdl_keyboard_create();
    lv_indev_set_group(keyboard, lv_group_get_default());

    LV_IMAGE_DECLARE(mouse_cursor_icon);
    lv_obj_t* mouse_cursor = lv_image_create(lv_screen_active());
    lv_image_set_src(mouse_cursor, &mouse_cursor_icon);
    lv_indev_set_cursor(mouse, mouse_cursor);
#elif LV_USE_X11
    disp = lv_x11_window_create("LVGL X11 Simulation", monitor_hor_res, monitor_ver_res);
    extern lv_img_dsc_t mouse_cursor_icon;
    lv_x11_inputs_create(disp, &mouse_cursor_icon);
    lv_x11_window_set_close_cb(disp, on_close_cb, disp);

#endif // LV_USE_xxx
    return disp;
}

/*
 * Periodically called method (e.g. from main loop or thread)
 */
void do_loop(void *arg)
{
    LV_UNUSED(arg);

    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    lv_task_handler();
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int main(int argc, char ** argv)
{
    LV_UNUSED(argc);
    LV_UNUSED(argv);

    int32_t monitor_hor_res = 800;
    int32_t monitor_ver_res = 480;
    printf("Starting with screen resolution of %dx%d px\n", monitor_hor_res, monitor_ver_res);

    /*Initialize LittlevGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LittlevGL*/
    lv_disp_t* disp = hal_init(monitor_hor_res, monitor_ver_res);

    /*call demo function (defined in CMakeLists.txt)*/
    CHOSEN_DEMO();

	while(!terminate)
	{
		do_loop(NULL);
		usleep(5000);
	}
    lv_display_remove(disp);
}
