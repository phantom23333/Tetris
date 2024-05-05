
#include <execution>
#include <iostream>
#include <matrix.h>

bool Tetris::Matrix::init()
{
	matrix = std::vector(m_column, std::vector<int>(m_row));
	return true;
}

std::vector<int>& Tetris::Matrix::operator[](int column)
{
	if (column < m_column && column >= 0)
	{
		return matrix[column];
	}
	std::cerr << "invalid row number" << std::endl;
	std::vector<int> res{};
	return res;
}

void Tetris::Matrix::refresh()
{
	for (auto& i : matrix)
	{
		for (auto& j : i)
		{
			j = 0;
		}
	}
}

bool Tetris::Matrix::update()
{
	// blocks that already fixed
	for (auto& i : blocks)
	{
		if (i.first.y <= 0)
		{
			return false;
		}
		matrix[i.first.x][i.first.y] = 1 + i.second;
	}
	return true;
}

bool Tetris::Matrix::clearLine()
{

	std::queue<int> marked;
	for (const auto& [i, j] : blocks)
	{
		lines[i.y].insert({i.x, i.y});
	}

	for (auto& [i, j] : lines)
	{
		if (j.size() == COLUMNS)
		{
			marked.push(i);
			clearline.emplace_back(i);
		}
	}

	if (marked.empty())
	{
		return false;
	}

	for (auto& i : lines)
	{
		i.second.erase(i.second.begin(), i.second.end());
	}

	while (!marked.empty())
	{
		int i = marked.front();
		blocks.erase(std::remove_if(blocks.begin(), blocks.end(), [i](const std::pair<sf::Vector2i, Shape>& pair)
			{
				return pair.first.y == i;
			}), blocks.end());
		for (int j = i - 1; j > 0; --j)
		{
			for (auto& p : blocks)
			{
				if (p.first.y == j)
				{
					p.first.y++;
				}
			}
		}
		marked.pop();

		
	}
	return true;
}
