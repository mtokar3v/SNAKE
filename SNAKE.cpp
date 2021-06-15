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
#include "apple.h"
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

	//подключаем sdl и задаем настройки окна
	if (!init(ren, win, displayMode))	return -1;

	//подгружаем текстурки
	SDL_Texture* background_texture = IMG_LoadTexture(ren, "image//background.png");
	Background* background;
	SDL_Texture* apple_texture = IMG_LoadTexture(ren, "image//apple.png");
	Apple* apple;
	int score;
	SDL_Texture* head_texture[2];
	head_texture[0] = IMG_LoadTexture(ren, "image//head.png");
	head_texture[1] = IMG_LoadTexture(ren, "image//body.png");
	Snake* snake;


	bool run = true;
	bool run_game = true;
	SDL_Event event;
	Timer t;
	int raiting[11];
	while (run_game)
	{
		t.reset();
		score = -1;
		background = new Background(ren, background_texture, 16);
		background->resize(displayMode, displayMode);
		apple = new Apple(ren, apple_texture, background);
		snake = new Snake(ren, head_texture, background, apple, &score);
		
			while (run)
			{
				SDL_Delay(10);
				if (SDL_PollEvent(&event))
				{
					snake->Controll(event);
					if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
						run = false;
				}
				if (!snake->Go())
					run = false;

				background->render();
				add_text(ren, "score: ", 40, 0, 0);
				add_text(ren, std::to_string(score), 40, 70, 0);
				add_text(ren, "time: ", 40, 100, 0);
				add_text(ren, std::to_string((int)t.elapsed()), 40, 150, 0);

				apple->render();
				snake->render();
				SDL_RenderPresent(ren);
			}
		make_raiting(ren, raiting, score);
		delete apple;
		delete background;
		delete snake;
			while (!run)
			{
				SDL_Delay(10);
				if (SDL_PollEvent(&event))
				{
					if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
						run = true;

					if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q)
					{
						run = true;
						run_game = false;
					}
				}
			}
	}
	
	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(apple_texture);
	SDL_DestroyTexture(head_texture[0]);
	SDL_DestroyTexture(head_texture[1]);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}