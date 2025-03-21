#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "args.h"
#include "defines.h"
#include "globals.h"
#include "input.h"
#include "object.h"
#include "physics.h"
#include "quadtree.h"
#include "renderer.h"

void print_usage() {
    printf("quadtree visualisation by tevz\n"
           "  options:\n"
           "    -t...test: spawn a large number of balls at start\n"
           "    -f...fps: set target fps (limit)\n"
           "    -h...help: print usage\n"
           "\n");
}

int parse_args(int argc, char** argv) {
    g_args.fps = 120;
    g_args.test = false;

    for (int opt; (opt = getopt(argc, argv, "htf:")) != -1;) {
        switch (opt) {
        case 'h':
            print_usage();
            return 1;
        case 't':
            printf("TestMode\n");
            g_args.test = true;
            continue;
        case 'f':
            g_args.fps = atoi(optarg);
            printf("Set FPS: %d\n", g_args.fps);
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if (parse_args(argc, argv)) {
        return 0;
    }
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "quadtree");

    g_nearby_items_is_lst = false;
    qt_build(&g_qt);
    SetTargetFPS(g_args.fps);

    if (g_args.test) {
        Vector2 spawn_location =
            (Vector2){.x = SCREEN_WIDTH / 2.0, .y = SCREEN_HEIGHT / 2.0};
        printf("Spawn test balls in location : %f,%f\n", spawn_location.x,
               spawn_location.y);
        for (int i = 0; i < 5000; ++i) {
            Ball o = (Ball){.pos = spawn_location};
            ball_add(o);
        }
    }

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        read_input();

        if (g_visualize_compare)
            simulate_compare();
        else
            simulate_physics();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        render();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    list_free_nodes(&g_collision_check_lines);
    //--------------------------------------------------------------------------------------
    return 0;
}
