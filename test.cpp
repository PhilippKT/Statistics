#include "spline.hpp"
#include"draw.hpp"

int main (int arhc, char ** argv) {

     if (!init()) {
        quit();
        system("pause");
        return 1;
    }

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);

    DoubleVariable v({1110, 1129, 1141, 1153, 1167, 1188}, {38, 49, 39, 48, 54, 72});
    std::cout << "Covariance = " << v.GetCovariance() << '\n';
    std::cout << "Correlation = " << v.GetCorrelation() << '\n';
    std::cout << "DispersionX = " << v.GetDispersionX() << '\n';
    std::cout << "DispersionY = " << v.GetDispersionY() << '\n';
    std::cout << "y = ax + b" << '\n';
    std::cout << "a = (" << v.GetA() << " +/- " << v.GetAError() << ")" << '\n';
    std::cout << "b = (" << v.GetB() << " +/- " << v.GetBError() << ")" << '\n';

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

