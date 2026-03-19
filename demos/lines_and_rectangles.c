#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_LINE_COUNT 100
#define PALETTE_SIZE 3

typedef struct {
    Uint8 r, g, b;
} Color;

typedef struct {
    int x1, y1, x2, y2;
    Color color;
} Line;

Line lines[MAX_LINE_COUNT];
int lineCount = 0;

Color palette[PALETTE_SIZE] = {
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255}
};

Color currentColor = {255, 255, 255};

int clickState = 0;
int startX, startY;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Rajzprogram",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }

            if (event.type == SDL_MOUSEMOTION) {
                printf("Egér: (%d, %d)\n", event.motion.x, event.motion.y);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                for (int i = 0; i < PALETTE_SIZE; i++) {
                    SDL_Rect rect = {10, 10 + i * 40, 30, 30};

                    if (mx >= rect.x && mx <= rect.x + rect.w &&
                        my >= rect.y && my <= rect.y + rect.h) {
                        currentColor = palette[i];
                    }
                }

                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (clickState == 0) {
                        startX = mx;
                        startY = my;
                        clickState = 1;
                    } else {
                        if (lineCount < MAX_LINE_COUNT) {
                            lines[lineCount].x1 = startX;
                            lines[lineCount].y1 = startY;
                            lines[lineCount].x2 = mx;
                            lines[lineCount].y2 = my;
                            lines[lineCount].color = currentColor;
                            lineCount++;
                        }
                        clickState = 0;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < PALETTE_SIZE; i++) {
            SDL_Rect rect = {10, 10 + i * 40, 30, 30};

            SDL_SetRenderDrawColor(renderer,
                palette[i].r,
                palette[i].g,
                palette[i].b,
                255);

            SDL_RenderFillRect(renderer, &rect);
        }

        for (int i = 0; i < lineCount; i++) {
            SDL_SetRenderDrawColor(renderer,
                lines[i].color.r,
                lines[i].color.g,
                lines[i].color.b,
                255);

            SDL_RenderDrawLine(renderer,
                lines[i].x1, lines[i].y1,
                lines[i].x2, lines[i].y2);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}