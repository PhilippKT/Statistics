#include <SDL2/SDL.h>
#include"statistics.hpp"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

void DrawCoord(SDL_Renderer* ren, int x0, int y0);
void DrawFatLine(SDL_Renderer* ren, int x1, int y1, int x2, int y2);
void DrawFatPoint(SDL_Renderer* ren, int x, int y);


void DrawFatLine(SDL_Renderer* ren, int x1, int y1, int x2, int y2){
    SDL_RenderDrawLine(ren, x1, y1, x2, y2);
    SDL_RenderDrawLine(ren, x1 + 1, y1, x2 + 1, y2);
    SDL_RenderDrawLine(ren, x1 - 1, y1, x2 - 1, y2);
    SDL_RenderDrawLine(ren, x1, y1 + 1, x2, y2 + 1);
    SDL_RenderDrawLine(ren, x1, y1 - 1, x2, y2 - 1);
}

void DrawFatPoint(SDL_Renderer* ren, int x, int y){
    SDL_Rect rect1 = {x, y, 5, 5};
    SDL_RenderFillRect(ren, &rect1);
}

void DrawCoord(SDL_Renderer* ren, int x0, int y0){
    DrawFatLine(ren, x0, 20, x0, SCREEN_HEIGHT - 20);
    DrawFatLine(ren, 20, y0, SCREEN_WIDTH - 20, y0);

    DrawFatLine(ren, x0, 20, x0 - 10, 35);
    DrawFatLine(ren, x0, 20, x0 + 10, 35);
    DrawFatLine(ren, x0 - 20, 40, x0 - 20, 25);
    DrawFatLine(ren, x0 - 20, 25, x0 - 30, 15);
    DrawFatLine(ren, x0 - 20, 25, x0 - 10, 15);

    DrawFatLine(ren, SCREEN_WIDTH - 20, y0, SCREEN_WIDTH - 35, y0 + 10);
    DrawFatLine(ren, SCREEN_WIDTH - 20, y0, SCREEN_WIDTH - 35, y0 - 10);
    DrawFatLine(ren, SCREEN_WIDTH - 40, y0 + 40, SCREEN_WIDTH - 25, y0 + 10);
    DrawFatLine(ren, SCREEN_WIDTH - 40, y0 + 10, SCREEN_WIDTH - 25, y0 + 40);

    DrawFatLine(ren, x0 - 20, y0 + 25, x0 - 5, y0 + 25);
    DrawFatLine(ren, x0 - 5, y0 + 25, x0 - 5, y0 + 5);
    DrawFatLine(ren, x0 - 5, y0 + 5, x0 - 20, y0 + 5);
    DrawFatLine(ren, x0 - 20, y0 + 5, x0 - 20, y0 + 25);

    for (int i = 0; i <= SCREEN_HEIGHT; i +=5 ) {
        SDL_RenderDrawLine(ren, 0, i, SCREEN_WIDTH, i);
    }

    for (int i = 0; i <= SCREEN_WIDTH; i +=5 ) {
        SDL_RenderDrawLine(ren, i, 0, i, SCREEN_HEIGHT);
    }
}

void DrawGraphic(SDL_Renderer* ren, std::vector<double> data_x, std::vector<double> data_y){
    DoubleVariable v(data_x, data_y);

    double xmin = v.GetMinimumX();
    double xmax = v.GetMaximumX();
    double ymax = v.GetMaximumY();
    double ymin = v.GetMinimumY();

    float scale_factor_x = (SCREEN_WIDTH - 40) / (xmax - xmin);
    float scale_factor_y = (SCREEN_HEIGHT - 40) / (ymax - ymin);

    int x0 = (int)(xmin + 40);
    int y0 = (int)(SCREEN_HEIGHT - 50 - ymin);

    DrawCoord(ren, x0, y0);

    size_t sz = v.GetSize();

    for(size_t i = 0; i < sz; i++){
        DrawFatPoint(ren, x0 + (int)(data_x[i] * scale_factor_x), y0 - (int)(scale_factor_y * data_y[i]));
    }

    double a = v.GetA();
    double b = v.GetB();

    int   y1 = y0 - (int)((a * (10 - x0) / scale_factor_x + b)* scale_factor_y);
    int   y2 = y0 - (int)((a * (SCREEN_WIDTH - 10 - x0) / scale_factor_x + b)* scale_factor_y);

    DrawFatLine(ren, 10, y1, SCREEN_WIDTH - 10, y2);
}
