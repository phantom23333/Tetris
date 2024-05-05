#pragma once
#include <map>
#include <vector>


#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"


namespace Tetris
{
	class Matrix;

	enum Shape
	{
		I = 0, J, L, O, S, T, Z
	};

	static std::map<Shape, std::vector<sf::Vector2i>> blueprint{
		{Shape::O, {{0,0}, {-1, 0}, {0, -1 }, {-1, -1}}},
		{Shape::J, {{0,0}, {-1, 0}, {-1, -1}, {1, 0}}},
		{Shape::L, {{0,0}, {-1, 0}, {1, 0}, {1, -1}}},
		{Shape::S, {{0,0},{-1, 0}, {0, -1}, {1, -1}}},
		{Shape::T, {{0,0}, {-1, 0}, {0, -1}, {1, 0}}},
		{Shape::Z, {{0,0}, {0, -1}, {-1, -1},{1,0}}},
		{Shape::I, { {1, -1},{0,-1}, {-1, -1} ,{-2, -1}}}

	};

	static std::map<Shape, sf::Color> colorMap{
		{Z,sf::Color::Red},
		{S,sf::Color::Green},
		{J,sf::Color::Blue},
		{I,sf::Color::Cyan},
		{O,sf::Color::Yellow},
		{T,sf::Color::Magenta},
		{L,sf::Color::White}
	};


	struct Tetromino
	{
		unsigned char rotate = 0;
		Shape shape;
		sf::Color color;

		std::vector<sf::Vector2i> minos;

		bool isReset = 0;

		Tetromino(Matrix& matrix, Shape shape);

		bool moveDown(Matrix& m_matrix);

		void moveLeft(Matrix& m_matrix);
		void moveRight(Matrix& m_matrix);

		void update(Matrix& m_matrix);
		bool reset(Shape i_shape, Matrix& matrix);

		bool rotation(unsigned char clockwise, Matrix& m_matrix);

		std::vector<sf::Vector2i> hard_drop(Matrix& m_matrix);

		std::vector<sf::Vector2i> getMinosPosition(Shape shape, sf::Vector2i& center);

		std::vector<sf::Vector2i> getWallKickData(unsigned char next);
	};
}

