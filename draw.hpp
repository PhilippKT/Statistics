#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include"statistics.hpp"
#include<string>


int SCREEN_WIDTH = 720;
int SCREEN_HEIGHT = 480;

void DrawCoord(SDL_Renderer* ren, int x0, int y0, const std::string & x0_str, const std::string & y0_str, const std::string & x_name,
                const std::string & y_name, std::pair<std::pair<double, double>, double> x1, std::pair<std::pair<double, double>, double> y1);
void DrawFatLine(SDL_Renderer* ren, int x1, int y1, int x2, int y2);
void DrawFatPoint(SDL_Renderer* ren, int x, int y);
std::pair<std::pair<double, double>, double> Scale(double xmin, double xmax); // функция возвращает минимальную величину на оси и шаг для отметок на оси
void RenderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr); // рисует текстуру в Renderer в Rect
void RenderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr); // рисует текстуру в Renderer в коор. x и y
SDL_Texture* RenderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer); // делает текстуру текста
void DrawGraphic(SDL_Renderer* ren, const std::vector<double>& data_x, const std::vector<double>& data_y,
                  const std::string & x_name, const std::string & y_name, const std::string &name = "", bool spln = 0);
void DrawLine(SDL_Renderer* ren, const std::vector<double>& data_x, const std::vector<double>& data_y, double scale_factor_x,
               double scale_factor_y, double x1, double y1); // рисует сглаживающую кривую
bool init();
void quit();


SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

bool init(){
    bool ok = true;
    TTF_Init();
    win = SDL_CreateWindow("Graphic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    return ok;
}

void quit(){
    SDL_DestroyWindow(win);
    win = NULL;

    SDL_DestroyRenderer(ren);
    ren = NULL;

    SDL_Quit;
}

void RenderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip){
	SDL_RenderCopy(ren, tex, clip, &dst);
}

void RenderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	RenderTexture(tex, ren, dst, clip);
}


SDL_Texture* RenderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer){
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
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

void DrawCoord(SDL_Renderer* ren, int x0, int y0, int x_div, int y_div, const std::string & x0_str, const std::string & y0_str,
               const std::string & x_name, const std::string & y_name, std::pair<std::pair<double, double>, double> x1, std::pair<std::pair<double, double>, double> y1){
    DrawFatLine(ren, x0, 20, x0, SCREEN_HEIGHT - 20);
    DrawFatLine(ren, 20, y0, SCREEN_WIDTH - 80, y0);

    SDL_Color color = {0, 0, 0, 0};

    DrawFatLine(ren, x0, 20, x0 - 7, 32); // стрелка оси у
    DrawFatLine(ren, x0, 20, x0 + 7, 32);
    RenderTexture(RenderText(y_name, "BaskervilleBoldItalic.ttf", color, 24, ren), ren, x0 + 15, 15);

    DrawFatLine(ren, SCREEN_WIDTH - 80, y0, SCREEN_WIDTH - 92, y0 + 7); // стрелка оси х
    DrawFatLine(ren, SCREEN_WIDTH - 80, y0, SCREEN_WIDTH - 92, y0 - 7);
    RenderTexture(RenderText(x_name, "BaskervilleBoldItalic.ttf", color, 24, ren), ren, SCREEN_WIDTH - 120, y0 + 15);

    if(x0_str == y0_str) RenderTexture(RenderText(x0_str, "BaskervilleBoldItalic.ttf", color, 24, ren), ren, x0 - 15, y0);
    else{
        RenderTexture(RenderText(x0_str, "BaskervilleBoldItalic.ttf", color, 24, ren), ren, x0 + 5, y0); // первое значение на оси х
        RenderTexture(RenderText(y0_str, "BaskervilleBoldItalic.ttf", color, 24, ren), ren, x0 - 15, y0 - 25); // первое значение на оси y
    }

    int i = x0 + x_div; // данный блок (до конца цикла while) рисует деления оси х с подписями
    double div = x1.second;
    double r = x1.first.first * std::pow(10, x1.first.second);
    r+=div;
    while(i < SCREEN_WIDTH - 80){
        SDL_SetRenderDrawColor(ren, 200, 200, 200, 0);
        SDL_RenderDrawLine(ren, i, 20, i, SCREEN_HEIGHT - 20);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        DrawFatLine(ren, i, y0 - 5, i, y0 + 5);
        std::string str = std::to_string(r / std::pow(10, x1.first.second - 1));
        bool fl = 1;
        while(fl){
            if(str.size() == 1 && r == 0.0) break;
            if(str.back() == '0') str.pop_back();
            else fl = 0;
        }
        if(str.back() == '.') str.pop_back();
        RenderTexture(RenderText(str, "BaskervilleBoldItalic.ttf", color, 24, ren), ren, i-10, y0);
        r+=div;
        i += x_div;
    }

    i = y0;  // данный блок (до конца цикла while) рисует деления оси y с подписями
    SDL_SetRenderDrawColor(ren, 200, 200, 200, 0);
    SDL_RenderDrawLine(ren, 20, i + y_div, SCREEN_WIDTH - 80, i + y_div);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    DrawFatLine(ren, x0 - 5, i + y_div, x0 + 5, i + y_div);
    i -= y_div;
    div = y1.second;
    r = y1.first.first * std::pow(10, y1.first.second);
    r+=div;
    while(i > 40){
        SDL_SetRenderDrawColor(ren, 200, 200, 200, 0);
        SDL_RenderDrawLine(ren, 20, i, SCREEN_WIDTH - 80, i);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        DrawFatLine(ren, x0 - 5, i, x0 + 5, i);
        std::string str = std::to_string(r / std::pow(10, y1.first.second - 1));
        bool fl = 1;
        while(fl){
            if(str.size() == 1 && r == 0.0) break;
            if(str.back() == '0') str.pop_back();
            else fl = 0;
        }
        if(str.back() == '.') str.pop_back();
        RenderTexture(RenderText(str,  "BaskervilleBoldItalic.ttf", color, 24, ren), ren, x0 - 20, i-10);
        i -= y_div;
        r+=div;
    }

}

void DrawGraphic(SDL_Renderer* ren, const std::vector<double>& data_x, const std::vector<double>& data_y, const std::string & x_name,
                  const std::string & y_name, const std::string &name, bool spln){
    DoubleVariable v(data_x, data_y);

    double xmin = v.GetMinimumX();
    double xmax = v.GetMaximumX();
    double ymax = v.GetMaximumY();
    double ymin = v.GetMinimumY();

    std::pair<std::pair<double, double>, double> x_p1 = Scale(xmin, xmax);
    std::pair<std::pair<double, double>, double> y_p1 = Scale(ymin, ymax);

    std::pair<double, double> x1 = x_p1.first;
    double x_div = x_p1.second;
    std::pair<double, double> y1 = y_p1.first;
    double y_div = y_p1.second;
    double x2 = x1.first * std::pow(10, x1.second);
    double y2 = y1.first * std::pow(10, y1.second);

    std::string xname, yname;

    if((int)x1.second != 1) xname = x_name + "*10^" + std::to_string((int)x1.second - 1);
    if((int)y1.second != 1) yname = y_name + "*10^" + std::to_string((int)y1.second - 1);

    while(x2 <= xmax){
        x2 = x2 + x_div;
    }
    while(y2 <= ymax){
        y2 = y2 + y_div;
    }


    float scale_factor_x = (SCREEN_WIDTH - 160) / (x2 - x1.first*std::pow(10, x1.second));
    float scale_factor_y = (SCREEN_HEIGHT - 100) / (y2 - y1.first*std::pow(10, y1.second));

    DrawCoord(ren, 40, SCREEN_HEIGHT - 80, (int)(x_div * scale_factor_x), (int)(y_div * scale_factor_y),
              std::to_string((int)x1.first * 10), std::to_string((int)y1.first * 10), xname, yname, x_p1, y_p1);

    size_t sz = v.GetSize();

    if(spln) DrawLine(ren, data_x, data_y, scale_factor_x, scale_factor_y, x1.first*std::pow(10, x1.second), y1.first*std::pow(10, y1.second));

    for(size_t i = 0; i < sz; i++){
        DrawFatPoint(ren, 40 + (int)((data_x[i] - x1.first*std::pow(10, x1.second)) * scale_factor_x),
                    SCREEN_HEIGHT - 80 - (int)(scale_factor_y * (data_y[i] - y1.first*std::pow(10, y1.second)))); // точки графика
    }

    double a = v.GetA();
    double b = v.GetB();

    int   y11 = SCREEN_HEIGHT - 80 - (int)(scale_factor_y * ((a * (x1.first*std::pow(10, x1.second) - 20 / scale_factor_x) + b) - y1.first*std::pow(10, y1.second)));
    int   y22 = SCREEN_HEIGHT - 80 - (int)(scale_factor_y * ((a * x2 + b) - y1.first*std::pow(10, y1.second)));

    DrawFatLine(ren, 20, y11, 40 + (int)((x2 - x1.first*std::pow(10, x1.second)) * scale_factor_x), y22); // прямая по мнк
    SDL_SetRenderDrawColor(ren, 250, 250, 250, 0);;
    SDL_Rect rect1 = {0, 0, SCREEN_WIDTH, 15};
    SDL_RenderFillRect(ren, &rect1);
    rect1 = {0, SCREEN_HEIGHT - 15, SCREEN_WIDTH, 15};
    SDL_RenderFillRect(ren, &rect1);
    SDL_Color color = {0, 0, 0, 0};
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    if(name != "") RenderTexture(RenderText(name,  "BaskervilleBoldItalic.ttf", color, 24, ren), ren, SCREEN_WIDTH / 2, 10);

}

void DrawLine(SDL_Renderer* ren, const std::vector<double>& data_x, const std::vector<double>& data_y, double scale_factor_x,
               double scale_factor_y, double x1, double y1){
    double u[SCREEN_WIDTH - 160];
	double s[SCREEN_WIDTH - 160];
	int i;
	double i0 = x1;

	for (i = 0; i < SCREEN_WIDTH - 160; i++)
	{
		u[i] = x1 + ((double)i) / scale_factor_x;
	}

	double x[data_x.size()];
	double y[data_y.size()];
	for(size_t k = 0; k < data_x.size(); k++){
        x[k] = data_x[k];
        y[k] = data_y[k];
	}

	SPL(data_x.size(), x, y, SCREEN_WIDTH - 160, u, s);

	for (i = 0; i < SCREEN_WIDTH - 160; i++)
	{
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 200);
		SDL_RenderDrawPoint(ren, 40 + (int)((u[i] - x1)*scale_factor_x),
                    SCREEN_HEIGHT - 80 - (int)(scale_factor_y * (s[i] - y1)));
	}
}

std::pair<std::pair<double, double>, double> Scale(double xmin, double xmax){ // функция возвращает минимальную величину на оси и шаг для отметок на оси
    double delta = xmax - xmin;
    double ord = std::log10(delta);
    double div = std::round(ord);
    double divx;
    if(div <= ord) divx = std::pow(10, div - 1) * 2;
    else divx = std::pow(10, div - 1);
    xmin /= std::pow(10, div);
    xmin = std::floor(xmin);
    return std::make_pair(std::make_pair(xmin, div), divx);
}
