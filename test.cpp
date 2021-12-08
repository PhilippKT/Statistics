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
