#include "spline.hpp"
#include"draw.hpp"


SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

bool init() {
    bool ok = true;
    TTF_Init();
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Can't init SDL: " << SDL_GetError() << std::endl;
    }

    win = SDL_CreateWindow("Graphic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        std::cout << "Can't create window: " << SDL_GetError() << std::endl;
        ok = false;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL) {
        std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;
        ok = false;
    }
    return ok;
}

void quit() {
    SDL_DestroyWindow(win);
    win = NULL;

    SDL_DestroyRenderer(ren);
    ren = NULL;

    SDL_Quit;
}

int main (int arhc, char ** argv) {

     if (!init()) {
        quit();
        system("pause");
        return 1;
    }

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);

    DrawGraphic(ren, {1110, 1129, 1141, 1153, 1167, 1188}, {38, 49, 39, 48, 54, 72}, "B, Gs", "H, Gs", "Graphic H(B)", 1);


    SDL_RenderPresent(ren);

    SDL_Event e;
    // Флаг выхода
    bool quit1 = false;

    while (!quit1){
        while (SDL_PollEvent(&e))
        {
        // Если пользователь попытался закрыть окно
        if (e.type == SDL_QUIT)
        {
            quit1 = true;
        }
        }
    }
    quit();
    return 0;
}


/*int main()
{
    DoubleVariable v({1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    std::cout << v.GetCovariance() << '\n';
    std::cout << v.GetCorrelation() << '\n';
    std::cout << v.GetDispersionX() << '\n';
    std::cout << v.GetDispersionY() << '\n';
    std::cout << v.GetCovariance() << '\n';
    std::cout << "y = ax + b" << '\n';
    std::cout << "a = (" << v.GetA() << " +/- " << v.GetAError() << ")" << '\n';
    std::cout << "b = (" << v.GetB() << " +/- " << v.GetBError() << ")" << '\n';
    return 0;
}*/
