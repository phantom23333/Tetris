#pragma once

#include <config.h>
#include <set>
#include <vector>

#include "tetromino.h"

namespace Tetris
{
	class Matrix
	{
	public:
		static Matrix& getInstance()
		{
			if (m_instance == nullptr)
			{
				Matrix* m = new Matrix();
				return *m;
			}
			return *m_instance;
		}

		~Matrix()
		{
			free(m_instance);
		}

		std::vector<std::vector<int>> matrix;

		bool init();

		void refresh();

		bool update();

		bool clearLine();

		std::vector<std::pair<sf::Vector2i, Shape>> blocks;

		std::vector<int> clearline;

		std::vector<int>& operator[](int row);

	private:
		Matrix()
		{
			m_instance = this;
			for (int i = 0; i < ROWS; ++i)
			{
				lines.insert({ i, {} });
			}
		}

		inline static Matrix* m_instance = nullptr;
		int m_column = COLUMNS;
		int m_row = ROWS;

		// ROW - std::vector 2i
		std::map<int, std::set < std::pair<int, int>>> lines;
	};
}
