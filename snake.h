#pragma once

class Snake_Part : public Game_Object
{
private:
	MoveList next_status;
	MoveList status;
	Background* bkd;
	int speed;
public:
	Snake_Part(SDL_Renderer* ren, SDL_Texture* texture, MoveList status, Background* bkd, int x, int y) : Game_Object(ren, texture)
	{
		resize(bkd->Get_length(), bkd->Get_length());
		move(x, y);
		this->bkd = bkd;
		this->status = status;
		next_status = status;
		speed = 4; //только степени двойки
	}

	void Set_Next_Status(MoveList move)
	{
		next_status = move;
	}

	MoveList Get_Next_Status()
	{
		return next_status;
	}

	void Set_Status(MoveList move)
	{
		status = move;
	}

	MoveList Get_Status()
	{
		return status;
	}

	void Go()
	{
		switch (status)
		{
		case UP: move(0, -speed); break;
		case DOWN: move(0, speed); break;
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
	SDL_Texture** texture;
	Background* bkd;
	Apple* apl;
	int* score;
public:
	Snake(SDL_Renderer* ren, SDL_Texture** texture, Background* bkd, Apple* apl, int* score)
	{
		this->score = score;
		this->apl = apl;
		this->bkd = bkd;
		this->ren = ren;
		this->texture = texture;
		snake.push_front(
			new Snake_Part(ren, texture[0], UP, bkd,
			(bkd->Get_number() /2 )* bkd->Get_length(), 
			(bkd->Get_number() / 2)*bkd->Get_length()));
		resize_snake();
		
	}

	~Snake()
	{
		for (auto tmp : snake)
			delete tmp;
		snake.clear();
	}

	void resize_snake()
	{
		int x, y;
		switch (snake.front()->Get_Status())
		{
		case UP: x = snake.front()->get_x(); y = snake.front()->get_y() + bkd->Get_length(); break;
		case DOWN: x = snake.front()->get_x(); y = snake.front()->get_y() - bkd->Get_length(); break;
		case LEFT:  x = snake.front()->get_x() + bkd->Get_length(); y = snake.front()->get_y(); break;
		case RIGHT:  x = snake.front()->get_x() - bkd->Get_length(); y = snake.front()->get_y(); break;
		default: x = snake.front()->get_x(); y = snake.front()->get_y() + bkd->Get_length();
		}
		snake.push_front(new Snake_Part(ren, texture[1], snake.front()->Get_Status(), bkd, x,y ));

		
		Section* section;
		do
		{
			x = (rand() % (bkd->Get_number() - 2) + 1) * bkd->Get_length();
			y = (rand() % (bkd->Get_number() - 2) + 1) * bkd->Get_length();
			section = bkd->Search_Section(x, y, bkd->Get_length(), bkd->Get_length());

		} while (section->get_busy() != 0);
		apl->moveTo(x, y);
		section->set_busy(2);
		(*score)++;
	}

	void Controll(SDL_Event event)
	{
		//изменяем движение головы
		switch (event.key.keysym.sym)
		{
		case SDLK_w: 
			if (snake.back()->Get_Status() != DOWN)
				snake.back()->Set_Next_Status(UP);
			break;
		case SDLK_s:
			if (snake.back()->Get_Status() != UP)
				snake.back()->Set_Next_Status(DOWN);
			break;
		case SDLK_a:
			if (snake.back()->Get_Status() != RIGHT)
				snake.back()->Set_Next_Status(LEFT); 
			break;
		case SDLK_d:
			if (snake.back()->Get_Status() != LEFT)
				snake.back()->Set_Next_Status(RIGHT);
			break;
		}

	}

	bool Go()
	{	
		if ((snake.front()->Get_Status() == NONE))
			return true;
		Section* head;
		head = bkd->Search_Section(snake.back()->get_x(), snake.back()->get_y(), snake.back()->get_w(), snake.back()->get_h());
		//пережаем поординаты головы
		if(head)
		{
			if (head->get_busy() == 1)
				return false;
			if (head->get_busy() == 2)
				resize_snake();
			head->set_busy(1);
			bkd->Search_Section(snake.front()->get_x(), snake.front()->get_y(), snake.front()->get_w(), snake.front()->get_h())->set_busy(0);

			MoveList oldMove = snake.back()->Get_Status();
			snake.back()->Set_Status(snake.back()->Get_Next_Status());

			for (auto tmp = ++snake.rbegin(); tmp != snake.rend(); tmp++)
			{
				(*tmp)->Set_Next_Status(oldMove);
				oldMove = (*tmp)->Get_Status();
				(*tmp)->Set_Status((*tmp)->Get_Next_Status());
			}
		}

		for (auto tmp = snake.begin(); tmp != snake.end(); tmp++)
			(*tmp)->Go();

		return true;
	}

	void render()
	{
		for (auto tmp : snake)
			switch (tmp->Get_Status())
			{
			case UP: tmp->render(0); break;
			case DOWN: tmp->render(180); break;
			case LEFT: tmp->render(270); break;
			case RIGHT: tmp->render(90); break;
			}
			
	}
};

