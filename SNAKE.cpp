#include <iostream>
#include <time.h>
#include <random>
#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <windows.h>
#include <vector>
#include <list>
#include <fstream>

#include "function.h"
#include "timer.h"
#include "GameObject.h"
#include "Background.h"
#include "snake.h"

const int displayMode = 512;

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	srand(time(0));

	SDL_Renderer* ren = nullptr;
	SDL_Window* win = nullptr;
	TTF_Font* font;
	SDL_Color color;

	//подключаем sdl и задаем настройки окна
	if (!init(ren, win, displayMode))	return -1;

	//подгружаем текстурки
	SDL_Texture* background_texture = IMG_LoadTexture(ren, "image//background.png");
	
	Background background(ren, background_texture, 16);
	background.resize(displayMode, displayMode);
	
	SDL_Texture* head_texture = IMG_LoadTexture(ren, "image//head.png");
	Snake snake(ren, head_texture, &background);

	bool run = true;
	SDL_Event event;
	while (run)
	{
		if (SDL_PollEvent(&event))
		{
			snake.Controll(event);
			background.render();
			snake.render();
			SDL_RenderPresent(ren);
		}
	}

	
	return 0;
}