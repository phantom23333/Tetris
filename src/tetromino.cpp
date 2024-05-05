#include "tetromino.h"
#include <matrix.h>
#include <config.h>
#include <iostream>


namespace Tetris
{
	// test purpose only
	Tetromino::Tetromino(Matrix& matrix, Shape shape) : shape(shape), color(colorMap[shape])
	{
		minos = getMinosPosition(shape, sf::Vector2i{ COLUMNS / 2, 1 });
	}


	bool Tetromino::moveDown(Matrix& m_matrix)
	{
		for (auto& i : minos)
		{
			if (i.y == ROWS - 1)
			{
				// we use second round of render to solve this problem
				//for (auto& i : minos)
				//{
				//	m_matrix.blocks.push_back({i, shape});
				//}
				std::cout << "it hitted! and this on the end row" << std::endl;
				return false;
			}
			if (m_matrix[i.x][i.y + 1] > 0)
			{
				std::cout << "We hitted another block!" << std::endl;
				//for (auto& i : minos)
				//{
				//	m_matrix.blocks.push_back({i, shape });
				//}
				return false;
			}
		}
		for (auto& i : minos)
		{
			i.y++;
		}
		return true;
	}

	void Tetromino::moveLeft(Matrix& m_matrix)
	{
		for (auto& i : minos)
		{
			if (i.x == 0)
			{
				return;
			}
			if (i.y < 0) continue;
			if (m_matrix[i.x - 1][i.y] > 0)
			{
				return;
			}
		}
		for (auto& i: minos)
		{
			i.x--;
		}
	}

	void Tetromino::moveRight(Matrix& m_matrix)
	{
		for (auto& i : minos)
		{
			if (i.x == COLUMNS - 1)
			{
				return;
			}
			if (i.y < 0) continue;
			if (m_matrix[i.x + 1][i.y] > 0)
			{
				return;
			}
		}
		for (auto& i : minos)
		{
			i.x++;
		}
		
	}

	std::vector<sf::Vector2i> Tetromino::getMinosPosition(Shape shape, sf::Vector2i& center)
	{
		std::vector<sf::Vector2i> res(4);
		auto vec = blueprint[shape];
		for (size_t i = 0; i < res.size(); ++i)
		{
			res[i] = center + vec[i];
		}
		return res;
	}

	std::vector<sf::Vector2i> Tetromino::getWallKickData(unsigned char next)
	{
		if (shape == I)
		{
			switch (rotate)
			{
			case 0:
			case 2:
			{
				switch (next)
				{
				case 1:
				{
					return { {0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2} };
				}
				case 3:
				{
					return { {0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2} };
				}
				}
			}
			case 1:
			{
				return { {0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2} };
			}
			case 3:
			{
				return { {0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2} };
			}
			}

			return { {} };
		}
		else
		{
			switch (rotate)
			{
			case 0:
			{
				switch (next)
				{
				case 1:
				{
					return { {0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2} };
				}
				case 3:
				{
					return { {0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1} };
				}
				}
			}
			case 1:
			{
				switch (next)
				{
				case 0:
				{
					return { {0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2} };
				}
				case 2:
				{
					return { {0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1} };
				}
				}
			}
			case 2:
			{
				switch (next)
				{
				case 1:
				{
					return { {0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1} };
				}
				case 3:
				{
					return { {0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2} };
				}
				}
			}
			case 3:
			{
				switch (next)
				{
				case 0:
				{
					return { {0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1} };
				}
				case 2:
				{
					return { {0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2} };
				}
				}
			}
			}

			return { {0, 0} };

		}
	}

	void Tetromino::update(Matrix& m_matrix)
	{
		for (auto& i : minos)
		{
			if(i.y < 0) continue;
			m_matrix.blocks.emplace_back(std::pair{i, shape});
		}
	}

	bool Tetromino::reset(Shape i_shape, Matrix& matrix)
	{
		//Reset the variables
		isReset = 1;
		rotate = 0;
		shape = i_shape;
		color = colorMap[shape];
		minos = getMinosPosition(shape, sf::Vector2i{COLUMNS / 2, 1});
		for (auto& mino : minos)
		{
			if (0 < matrix[mino.x][mino.y])
			{
				//Return that we can't reset because there's a tetromino at the spawn location
				return 0;
			}
		}

		return 1;
	}

	bool Tetromino::rotation(unsigned char clockwise, Matrix& m_matrix)
	{
		if (shape != O)
		{
			unsigned char nextRotation;
			auto currentMinos = minos;

			if (clockwise)
			{
				nextRotation = (rotate + 1) % 4;
			} else
			{
				nextRotation = (rotate + 3) % 4;
			}


			if (shape != I)
			{
				if (clockwise)
				{
					for (int i = 0; i < minos.size(); i++)
					{
						auto relPos = minos[i] - minos[0];
						minos[i].x = minos[0].x - relPos.y;
						minos[i].y = minos[0].y + relPos.x;
					}
				}
				else
				{
					for (int i = 0; i < minos.size(); i++)
					{
						auto relPos = minos[i] - minos[0];
						minos[i].x = minos[0].x + relPos.y;
						minos[i].y = minos[0].y - relPos.x;
					}
				}


			}
			else
			{
				sf::Vector2f original = 0.5f * static_cast<sf::Vector2f>(minos[1] + minos[2]);
				switch (rotate)
				{
				case 0:
					original.y += 0.5f;
					break;
				case 1:
					original.x -= 0.5f;
					break;
				case 2:
					original.y -= 0.5f;
					break;
				case 3:
					original.x += 0.5f;
					break;
				}

				for (auto& i : minos)
				{
					auto relPos = static_cast<sf::Vector2f>(i) - original;
					if (clockwise)
					{
						i.x = static_cast<int>(original.x + relPos.y);
						i.y = static_cast<int>(original.y - relPos.x);
					} else
					{
						i.x = static_cast<int>(original.x - relPos.y);
						i.y = static_cast<int>(original.y + relPos.x);
					}
				}
			}

			
			for (auto wallKick : getWallKickData(nextRotation))
			{
				bool canTurn = 1;
				for (auto& i : minos)
				{
					if (i.x + wallKick.x < 0 || i.x + wallKick.x >= COLUMNS || i.y + wallKick.y >= ROWS)
					{
						canTurn = 0;
						break;
					}

					if (i.y + wallKick.y < 0) continue;
					else if (m_matrix[i.x + wallKick.x][i.y + wallKick.y] > 0)
					{
						canTurn = 0;
						break;
					}
				}

				// if we can really turn
				if (canTurn)
				{
					rotate = nextRotation;

					for (auto&i : minos)
					{
						i.x += wallKick.x;
						i.y += wallKick.y;
					}
					return true;
				}

			}

			minos = currentMinos;


		}

		return false;
	}

	std::vector<sf::Vector2i> Tetromino::hard_drop(Matrix& m_matrix)
	{
		bool falling = 1;
		unsigned char totalMovement = 0;

		std::vector<sf::Vector2i> ghost_minos = minos;

		while (falling)
		{
			totalMovement++;
			for (auto& mino : minos)
			{
				if (ROWS == totalMovement + mino.y)
				{
					falling = 0;

					break;
				}

				if (0 > totalMovement + mino.y)
				{
					continue;
				}
				else if (0 < m_matrix[mino.x][totalMovement + mino.y])
				{
					falling = 0;

					break;
				}
			}
		}

		for (auto& mino : ghost_minos)
		{
			mino.y += totalMovement - 1;
		}

		return ghost_minos;
	}
}

