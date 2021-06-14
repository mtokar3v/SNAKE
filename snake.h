#pragma once

enum Part
{
	UNKNOWN,
	HEAD,
	BODY,
	TAIL
};

class Snake_Part : public Game_Object
{
private:
	//Part part;
	MoveList status;
	Background* bkd;
	int speed;

public:
	Snake_Part(SDL_Renderer* ren, SDL_Texture* texture, MoveList status, Part part, Background* bkd) : Game_Object(ren, texture)
	{
		resize(bkd->Get_length() - 2, bkd->Get_length() - 2);
		//это говно нужно исправить. Тут же все относительно головы.
		move(bkd->Get_length() + 2, bkd->Get_length() + 2);
		this->bkd = bkd;
		this->status = status;
		//this->part = part;
		speed = 1;
	}

	void Set_Status(MoveList move)
	{
		status = move;
	}

	MoveList Get_Status()
	{
		return status;
	}

	void  push_status_in_section(MoveList status)
	{
		Section* section = bkd->Search_Section(get_x(), get_y(), get_w(), get_h(), status);
		if (section->get_status() == NONE)
			section->change_status(status);
	}

	void Go()
	{
		Section* section = bkd->Search_Section(get_x(), get_y(), get_w(), get_h(), status);
		if (section->get_status() != status && section->get_status() != NONE)
			status = section->get_status();

		switch (status)
		{
		case UP: move(0, speed); break;
		case DOWN: move(0, -speed); break;
		case LEFT: move(-speed, 0); break;
		case RIGHT: move(speed, 0); break;
		}
	}


};

class Snake
{
private:
	std::list<Snake_Part*> snake;
	SDL_Renderer* ren;
	SDL_Texture* texture;
	Background* bkd;

	void Go(MoveList move)
	{
		for (auto tmp = snake.begin(); tmp != snake.end(); tmp++)
		{
			if (tmp == snake.begin())
				(*tmp)->push_status_in_section(move);
			else if(tmp == snake.end())
				(*tmp)->push_status_in_section(NONE);

			(*tmp)->Go();
		}
	}
public:
	Snake(SDL_Renderer* ren, SDL_Texture* texture, Background* bkd)
	{
		this->bkd = bkd;
		this->ren = ren;
		this->texture = texture;
		snake.push_front(new Snake_Part(ren, texture, NONE, HEAD, bkd));
	}

	~Snake()
	{
		for (auto tmp : snake)
			delete tmp;
		snake.clear();
	}

	void Controll(SDL_Event event)
	{
		//изменяем движение головы
		switch (event.key.keysym.sym)
		{
		case SDLK_w: 
			if (snake.back()->Get_Status() != DOWN)
				snake.back()->Set_Status(UP);
			Go(UP); break;
		case SDLK_s:
			if (snake.back()->Get_Status() != UP)
				snake.back()->Set_Status(DOWN);
			Go(DOWN); break;
		case SDLK_a:
			if (snake.back()->Get_Status() != RIGHT)
				snake.back()->Set_Status(LEFT); 
			Go(LEFT); break;
		case SDLK_d:
			if (snake.back()->Get_Status() != LEFT)
				snake.back()->Set_Status(RIGHT);
			Go(RIGHT); break;
		}

	}

	void render()
	{
		for (auto tmp : snake)
			tmp->render();
		
	}
};

