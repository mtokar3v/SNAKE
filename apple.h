#pragma once

class Apple : public Game_Object
{
private:
	Background* bkd;
public:
	Apple(SDL_Renderer* ren, SDL_Texture* texture, Background* bkd) : Game_Object(ren, texture)
	{
		this->bkd = bkd;
		resize(bkd->Get_length() * 0.8, bkd->Get_length());
	}
};