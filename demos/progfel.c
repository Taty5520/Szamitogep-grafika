#include <SDL2/SDL.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_LINE_COUNT 100

typedef struct {
    Uint8 r,g,b;
} Color;

typedef struct {
    int x1,y1;
    int x2,y2;
    Color color;
} Line;

Line lines[MAX_LINE_COUNT];
int line_count = 0;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Line drawer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    SDL_Event event;

    int running = 1;
    int click = 0;
    int x1,y1;

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                running = 0;

            if(event.type == SDL_MOUSEMOTION)
            {
                int mx = event.motion.x;
                int my = event.motion.y;

                printf("Mouse: %d %d\n", mx, my);
            }

            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                if(click == 0)
                {
                    x1 = x;
                    y1 = y;
                    click = 1;
                }
                else
                {
                    if(line_count < MAX_LINE_COUNT)
                    {
                        lines[line_count].x1 = x1;
                        lines[line_count].y1 = y1;
                        lines[line_count].x2 = x;
                        lines[line_count].y2 = y;

                        lines[line_count].color.r = 255;
                        lines[line_count].color.g = 255;
                        lines[line_count].color.b = 255;

                        line_count++;
                    }

                    click = 0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        for(int i=0;i<line_count;i++)
        {
            SDL_SetRenderDrawColor(renderer,
                lines[i].color.r,
                lines[i].color.g,
                lines[i].color.b,
                255);

            SDL_RenderDrawLine(renderer,
                lines[i].x1,
                lines[i].y1,
                lines[i].x2,
                lines[i].y2);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}