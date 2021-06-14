#pragma once

class Section
{
private:
	MoveList status;
	int x;
	int y;
public:
	Section() : x(0), y(0), status(NONE) {}
	
	void set_cords(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void change_status(MoveList move)
	{
		status = move;
	}

	MoveList get_status()
	{
		return status;
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
	int section_number;		//���������� ������
	int section_length;		//������ ������
public:
	Background(SDL_Renderer* ren, SDL_Texture* texture, int size) : Game_Object(ren, texture)
	{
		this->section_number = size;
		//����������� ������� ������� ������ �� ������� ����� ������ ������
		matrix = new Section* [size];
		for (int i = 0; i < size; i++)
			matrix[i] = new Section[size];

		section_length = displayMode / size;
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				matrix[i][j].set_cords(i * section_length, j * section_length);
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

	Section* Search_Section(int x, int y, int w, int h, MoveList move)
	{
		int i = 0;
		Section temp[2]; //��� ��� ���� ������ ����� ��������� ����� �� ���� ������� ������������, �� ������ ����������� ����� �� ��� ��� �����
		for (int i = 0; i < section_number;i++)
			for (int j = 0; j < section_number; j++)
			{
				//���� ���� ��������� ������ ������, �� ����� �� ����������
				if (matrix[i][j].get_x() <= x && x + w <= matrix[i][j].get_x() + section_length &&
					matrix[i][j].get_y() <= y && y + h <= matrix[i][j].get_y() + section_length)
					return matrix[i] + j;

				//���� ����������� ������ �� � ����������
				if (matrix[i][j].get_y() <= y && y + h <= matrix[i][j].get_y() + section_length &&
					(matrix[i][j].get_x() > x + w && x + w < matrix[i][j].get_x() + section_length ||
						matrix[i][j].get_x() < x && x < matrix[i][j].get_x() + section_length))
				{
					temp[i] = matrix[i][j];
					i++;
					continue;
				}

				//���� ����������� ������ �� y ����������
				if (matrix[i][j].get_x() <= x && x + w <= matrix[i][j].get_x() + section_length &&
					(matrix[i][j].get_y() > y + h && y + h < matrix[i][j].get_y() + section_length ||
						matrix[i][j].get_y() < y && y < matrix[i][j].get_y() + section_length))
				{
					temp[i] = matrix[i][j];
					i++;
				}
			}

		//���� ���� �������� ���� ��� ������� � ���� ����� �����, ������� ���� ��� �����, ����� ������� ��, ����� ������� ������ 
		if (temp[0].get_x() <= temp[1].get_x() && temp[0].get_y() <= temp[1].get_y() && (move == DOWN || move == RIGHT))
			return temp + 1;
		else
			return temp;

		std::cout << "������ �� �������";
	}

};