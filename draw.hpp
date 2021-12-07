#include <SDL2/SDL.h>
#include"statistics.hpp"

// масштаб:работаем логарифмом и rand() - округлением, для минимума и разности минимума и максимума

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

void DrawCoord(SDL_Renderer* ren, int x0, int y0);
void DrawFatLine(SDL_Renderer* ren, int x1, int y1, int x2, int y2);
void DrawFatPoint(SDL_Renderer* ren, int x, int y);
std::pair<double, double> Scale(double xmin, double xmax){ // функция возвращает минимальную величину на оси и шаг для отметок на оси
    double delta = xmax - xmin;
    double ord = std::log10(delta);
    double div = std::round(ord);
    double divx;
    if(div <= ord) divx = std::pow(10, div - 1) * 2;
    else divx = std::pow(10, div - 1);
    xmin /= std::pow(10, div);
    xmin = std::floor(xmin);
    xmin *= std::pow(10, div);
    return std::make_pair(xmin, divx);
}


void DrawFatLine(SDL_Renderer* ren, int x1, int y1, int x2, int y2){
    SDL_RenderDrawLine(ren, x1, y1, x2, y2);
    SDL_RenderDrawLine(ren, x1 + 1, y1, x2 + 1, y2);
    SDL_RenderDrawLine(ren, x1 - 1, y1, x2 - 1, y2);
    SDL_RenderDrawLine(ren, x1, y1 + 1, x2, y2 + 1);
    SDL_RenderDrawLine(ren, x1, y1 - 1, x2, y2 - 1);
}

void DrawFatPoint(SDL_Renderer* ren, int x, int y){
    SDL_Rect rect1 = {x - 2, y - 2, 5, 5};
    SDL_RenderFillRect(ren, &rect1);
}

void DrawCoord(SDL_Renderer* ren, int x0, int y0, int x_div, int x1, int y_div, int y1){
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

    int i = x0;
    while(i < SCREEN_WIDTH - 60){
        DrawFatLine(ren, i, y0 - 5, i, y0 + 5);
        SDL_RenderDrawLine(ren, i, 20, i, SCREEN_HEIGHT - 20);
        i += x_div;
    }

    i = y0;
    DrawFatLine(ren, x0 - 5, i + y_div, x0 + 5, i + y_div);
    SDL_RenderDrawLine(ren, 20, i + y_div, SCREEN_WIDTH - 20, i + y_div);
    while(i > 40){
        DrawFatLine(ren, x0 - 5, i, x0 + 5, i);
        SDL_RenderDrawLine(ren, 20, i, SCREEN_WIDTH - 20, i);
        i -= y_div;
    }

    /*for (int i = 0; i <= SCREEN_HEIGHT; i +=5 ) {
        SDL_RenderDrawLine(ren, 0, i, SCREEN_WIDTH, i);
    }

    for (int i = 0; i <= SCREEN_WIDTH; i +=5 ) {
        SDL_RenderDrawLine(ren, i, 0, i, SCREEN_HEIGHT);
    }*/
}

void DrawGraphic(SDL_Renderer* ren, const std::vector<double>& data_x, const std::vector<double>& data_y){
    DoubleVariable v(data_x, data_y);

    double xmin = v.GetMinimumX();
    double xmax = v.GetMaximumX();
    double ymax = v.GetMaximumY();
    double ymin = v.GetMinimumY();

    double x1 = Scale(xmin, xmax).first;
    double x_div = Scale(xmin, xmax).second;
    double y1 = Scale(ymin, ymax).first;
    double y_div = Scale(ymin, ymax).second;
    double x2 = x1;
    double y2 = y1;

    while(x2 <= xmax){
        x2 = x2 + x_div;
    }
    while(y2 <= ymax){
        y2 = y2 + y_div;
    }


    float scale_factor_x = (SCREEN_WIDTH - 100) / (x2 - x1);
    float scale_factor_y = (SCREEN_HEIGHT - 100) / (y2 - y1);

    DrawCoord(ren, 40, SCREEN_HEIGHT - 80, (int)(x_div * scale_factor_x), (int)(x1), (int)(y_div * scale_factor_y), (int)(y1));

    size_t sz = v.GetSize();

    for(size_t i = 0; i < sz; i++){
        DrawFatPoint(ren, 40 + (int)((data_x[i] - x1) * scale_factor_x), SCREEN_HEIGHT - 80 - (int)(scale_factor_y * (data_y[i] - y1)));
    }

    double a = v.GetA();
    double b = v.GetB();

    int   y11 = SCREEN_HEIGHT - 80 - (int)(scale_factor_y * ((a * (x1 - 20 / scale_factor_x) + b) - y1));
    int   y22 = SCREEN_HEIGHT - 80 - (int)(scale_factor_y * ((a * x2 + b) - y1));

    DrawFatLine(ren, 20, y11, 40 + (int)((x2 - x1) * scale_factor_x), y22);
    //DrawFatLine(ren, 40, SCREEN_HEIGHT - 80, 40 + scale_factor_x*10, SCREEN_HEIGHT - 80 - scale_factor_y*10);
}
