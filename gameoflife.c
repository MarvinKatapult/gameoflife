
#include <raylib.h>

#include <cdefs.h>

#include <stdio.h>

#define START_WORLD_W   1400
#define START_WORLD_H   900

#define CELL_SIZE     20

#define CELL_COUNT_W  (START_WORLD_W / CELL_SIZE)
#define CELL_COUNT_H  (START_WORLD_H / CELL_SIZE)

#define TIME_PER_STEP 0.1f

typedef struct {
    i64 x;
    i64 y;
    bool alive;
    bool next_gen;
} Cell;

static Cell world[CELL_COUNT_H][CELL_COUNT_W];
static float frames_since_step;
static bool game_running;

void takeInput(void);
void loadWorld(void);
void drawGrid(void);
void doTurn(void);

int main(void) {

    loadWorld();
    InitWindow(START_WORLD_W, START_WORLD_H, "Conways Game of Life");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        frames_since_step += GetFrameTime();
        takeInput();
        BeginDrawing();
            drawGrid();
            if (game_running && frames_since_step > TIME_PER_STEP) {
                frames_since_step = 0.f;
                doTurn();
            }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}

void takeInput(void) {
        if (IsKeyPressed(KEY_SPACE)) {
            game_running = !game_running;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Cell * c = &world[GetMouseY() / CELL_SIZE][GetMouseX() / CELL_SIZE];
            c->alive = true;
            c->next_gen = true;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Cell * c = &world[GetMouseY() / CELL_SIZE][GetMouseX() / CELL_SIZE];
            c->alive = false;
            c->next_gen = false;
        }

        if (IsKeyPressed(KEY_C)) {
            loadWorld();
        }
}

void loadWorld(void) {
    for (int y = 0; y < CELL_COUNT_H; y++) {
        for (int x = 0; x < CELL_COUNT_W; x++) {
            Cell * c = &world[y][x];
            c->x = x;
            c->y = y;
            c->alive = false;
            c->next_gen = false;
        }
    }
}

void drawGrid(void) {
    ClearBackground(BLACK);
    for (int y = 0; y < CELL_COUNT_H; y++) {
        const int cell_y = y * CELL_SIZE;
        DrawLine(0, cell_y, GetScreenWidth(), cell_y, WHITE);
    }

    for (int x = 0; x < CELL_COUNT_W; x++) {
        const int cell_x = x * CELL_SIZE;
        DrawLine(cell_x, 0, cell_x, GetScreenHeight(), WHITE);
    }

    for (int y = 0; y < CELL_COUNT_H; y++) {
        for (int x = 0; x < CELL_COUNT_W; x++) {
            Cell * c = &world[y][x];
            if (c->alive) {
                DrawRectangle(c->x * CELL_SIZE, c->y * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1, RED);
            }
        }
    }
}

int getNCount(Cell * c) {
    int ret = 0;
    if (!c) return ret;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            const int n_cell_x = c->x + x;
            const int n_cell_y = c->y + y;
            if (n_cell_x < 0 || n_cell_x >= CELL_COUNT_W) continue;
            if (n_cell_y < 0 || n_cell_y >= CELL_COUNT_H) continue;
            if (x == 0 && y == 0) continue;

            const Cell * n_cell = &world[n_cell_y][n_cell_x];
            if (n_cell->alive) ret++;
        }
    }

    return ret;
}

void doTurn(void) {
    for (int y = 0; y < CELL_COUNT_H; y++) {
        for (int x = 0; x < CELL_COUNT_W; x++) {
            Cell * c = &world[y][x];
            const int n_count = getNCount(c);
            if (n_count < 2 || n_count > 3) c->next_gen = false;
            if (n_count == 3) c->next_gen = true;
        }
    }

    for (int y = 0; y < CELL_COUNT_H; y++) {
        for (int x = 0; x < CELL_COUNT_W; x++) {
            Cell * c = &world[y][x];
            c->alive = c->next_gen;
        }
    }
}
