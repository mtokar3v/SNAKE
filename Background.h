#pragma once

class Section
{
private:
	char busy; // 0 - ничего, 1 - зме€, 2 - €блоко
	int x;
	int y;
public:
	Section() : x(0), y(0), busy(0) {}
	
	void set_cords(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void set_busy(char b)
	{
		busy = b;
	}

	char get_busy()
	{
		return busy;
	}

	int get_x()
	{
		return x;
	}
	
	int get_y()
	{  
		return y;
	}
};

class Background : public Game_Object
{
private:
	Section** matrix;
	int section_number;		//количество секций
	int section_length;		//размер секции
public:
	Background(SDL_Renderer* ren, SDL_Texture* texture, int size) : Game_Object(ren, texture)
	{
		this->section_number = size;
		//динамически создаем матрицу секций по которым будет бегать змейка
		matrix = new Section* [size];
		for (int i = 0; i < size; i++)
			matrix[i] = new Section[size];

		section_length = displayMode / size;
		
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				matrix[i][j].set_cords(i * section_length, j * section_length);
				if (i == 0 || i == size - 1 || j == 0 || j == size -1)
					matrix[i][j].set_busy(1);	// в стенки врезатьс€ - верна€ смерть
			}
		}
	}

	~Background()
	{
		for (int i = 0; i < section_number; i++)
				delete[] matrix[i];
		delete matrix;
	}

	int Get_length()
	{
		return section_length;
	}

	int Get_number()
	{
		return section_number;
	}

	Section* Search_Section(int x, int y, int w, int h)
	{
		for (int i = 0; i < section_number; i++)
			for (int j = 0; j < section_number; j++)
			{
				if (matrix[i][j].get_x() <= x && x + w <= matrix[i][j].get_x() + section_length &&
					matrix[i][j].get_y() <= y && y + h <= matrix[i][j].get_y() + section_length)
					return matrix[i]+j;
			}
		return nullptr;
	}

	void render()
	{
	
		SDL_RenderCopy(ren, texture, NULL, rect);
		SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0xff, 0xFF);
		for (int i = 0; i < section_number; i++)
		{
			SDL_RenderDrawLine(ren, i * section_length, 0, i * section_length, displayMode);
			SDL_RenderDrawLine(ren, 0, i * section_length, displayMode, i * section_length);
		}
		
	}
};