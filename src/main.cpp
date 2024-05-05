#include <SFML/Graphics.hpp> // Include the main SFML graphics header
#include <config.h>
#include <thread>
#include <iostream>
#include <queue>
#include <random>
#include <tetromino.h>

#include "matrix.h"
#include "text.h"


int main()
{
	sf::RenderWindow window(sf::VideoMode({ COLUMNS * SCREEN_SIZE * CELL_SIZE * 2, ROWS * SCREEN_SIZE * CELL_SIZE}), "Tetris", sf::Style::Close);
	
	// Set the frame rate limit to 144 frames per second
	window.setFramerateLimit(144);


	// random number
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_int_distribution<int> distribution(0, 6);

#pragma region init matrix
	Tetris::Matrix& matrix = Tetris::Matrix::getInstance();
	matrix.init();
#pragma endregion

#pragma region deltaTime
	auto lastFrame = std::chrono::high_resolution_clock::now();
	double deltaTime = 0.0;
	double loopTime = 0.0167;
	double resetTime = loopTime;
#pragma endregion

#pragma region init 7 tetrominos
	std::vector<Tetris::Tetromino> tetrominos;
	for (int i = 0; i < 7; ++i)
	{
		tetrominos.emplace_back(Tetris::Tetromino(matrix, static_cast<Tetris::Shape>(i)));
	}
	int currentShape = -1;
#pragma endregion


#pragma region flag pool
	unsigned char soft_drop_timer = 0;
	unsigned char move_timer = 0;
	unsigned char rotate = 0;
	unsigned char gameOver = 0;
	unsigned char resetable = 0;
	unsigned char currentFallSpeed = START_FALL_SPEED;
	unsigned char fallTimer = 0;
	unsigned char cleanLineEffect = 0;
	unsigned char hardDrop = 0;
	int score = 0;
	std::queue<int> nextShape;
	nextShape.push(distribution(engine));
	nextShape.push(distribution(engine));
#pragma endregion

	// Main loop
	while (window.isOpen())
	{
		// deltaTime calculation
		auto currentFrame = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> targetDuration(1.0f / FPS);

		std::chrono::duration<double> currentDuration = currentFrame - lastFrame;
		if (currentDuration < targetDuration)
		{
			std::this_thread::sleep_for(targetDuration - (currentDuration));
			deltaTime = targetDuration.count();
		} else
		{
			deltaTime = (double)(currentDuration).count();
		}
		lastFrame = currentFrame;

		if (loopTime < 0)
		{
			loopTime = resetTime;
		}
		else
		{
			loopTime -= deltaTime;
			continue;
		}


		if (currentShape == -1 || tetrominos[currentShape].isReset == true)
		{
			currentShape = nextShape.front();
			nextShape.pop();
			tetrominos[currentShape].isReset = false;
			nextShape.push(distribution(engine));
		}
		auto& T = tetrominos[currentShape];

		matrix.refresh();
		matrix.update();
		// Input
		sf::Event event;
		// Poll for events

		while (window.pollEvent(event))
		{
			// Check if the window was closed
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Key::Space:
					hardDrop = 0;
					break;
				case sf::Keyboard::Key::RControl:
				case sf::Keyboard::Key::Up:
					rotate = 0;
					break;
				case sf::Keyboard::Key::Down:
					soft_drop_timer = 0;
					break;
				case sf::Keyboard::Key::Right:
				case sf::Keyboard::Key::Left:
					move_timer = 0;
					break;
				default:;
				}
			default:;
			}

		}

		// Update
		if (cleanLineEffect == 0)
		{
			if (gameOver == 0)
			{
				if (rotate == 0)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
					{
						rotate = 1;
						T.rotation(1, matrix);
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
					{
						rotate = 1;
						T.rotation(0, matrix);
					}
				}


				if (move_timer == 0)
				{
					if (isKeyPressed(sf::Keyboard::Key::Left))
					{
						move_timer = 1;
						T.moveLeft(matrix);
					}
					if (isKeyPressed(sf::Keyboard::Key::Right))
					{
						move_timer = 1;
						T.moveRight(matrix);
					}
				}
				else
				{
					move_timer = (move_timer + 1) % MOVE_SPEED;
				}

				if (hardDrop == 0)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
					{
						hardDrop = 1;
						fallTimer = currentFallSpeed;
						T.minos = T.hard_drop(matrix);
					}
				}

				if (soft_drop_timer == 0)
				{
					if (isKeyPressed(sf::Keyboard::Key::Down))
					{
						if (T.moveDown(matrix))
						{
							fallTimer = 0;
							soft_drop_timer = 1;
						}
					}
				}
				else
				{
					soft_drop_timer = (soft_drop_timer + 1) % SOFT_DOWN_SPEED;
				}

				//std::cout << "CURRENT FALL TIMER： " << (int)currentFallSpeed << "\n";
				if (currentFallSpeed == fallTimer)
				{
					
					if (T.moveDown(matrix) == 0)
					{
						T.update(matrix);
						if (matrix.clearLine())
						{
							// clearLine effect trigger
							currentFallSpeed = std::max(currentFallSpeed - 1, SOFT_DOWN_SPEED);
							cleanLineEffect = CLEAR_EFFECT_DURATION;
							if (score > 6)
							{
								currentFallSpeed = std::min(currentFallSpeed - 2, SOFT_DOWN_SPEED);
							}
						}
						if (T.reset(static_cast<Tetris::Shape>(currentShape), matrix))
						{
							resetable = 1;
						}

					}
					fallTimer = 0;
				}
				else
				{
					fallTimer++;
				}
			} else
			{
				// if game is over

			}
		}
		else
		{
			cleanLineEffect--;
			if (cleanLineEffect == 0)
			{
				score += (int)matrix.clearline.size();
				matrix.clearline.erase(matrix.clearline.begin(), matrix.clearline.end());
				cleanLineEffect = 0;
			}
		}


		if (!matrix.update() && resetable)
		{
			resetable = 0;
			gameOver = 1;
			std::cout << "GAME OVER" << std::endl;
		}



		

		// Render
		window.clear(sf::Color::Black);

		sf::RectangleShape cell{ sf::Vector2f{(CELL_SIZE - 1) * SCREEN_SIZE, (CELL_SIZE - 1) * SCREEN_SIZE} };
		sf::RectangleShape preview_border(sf::Vector2f(5 * CELL_SIZE * SCREEN_SIZE, 5 * CELL_SIZE * SCREEN_SIZE));
		preview_border.setFillColor(sf::Color(0, 0, 0));
		preview_border.setOutlineThickness(-1);
		preview_border.setPosition({ CELL_SIZE * (1.5f * COLUMNS - 2.5f) * SCREEN_SIZE, CELL_SIZE * (0.25f * ROWS - 2.5f)* SCREEN_SIZE });
		window.draw(preview_border);
		
		// Draw the matrix and fixed minos
		for (int i = 0; i < COLUMNS; i++)
		{
			for (int j = 0; j < ROWS ; j++)
			{
				cell.setPosition(sf::Vector2f{ (float)CELL_SIZE * SCREEN_SIZE * i, (float)CELL_SIZE * SCREEN_SIZE * j });
				if (matrix[i][j])
				{
					cell.setFillColor(Tetris::colorMap[static_cast<Tetris::Shape>(matrix[i][j] - 1)]);
				} else
				{
					cell.setFillColor(sf::Color{ 10,10,10 });
				}
				window.draw(cell);
			}
		}

		
		if (gameOver == 0)
		{
			// Draw Ghost minos
			for(auto& mino : T.hard_drop(matrix))
			{
				cell.setPosition({ static_cast<float>(CELL_SIZE * mino.x) * SCREEN_SIZE, static_cast<float>(CELL_SIZE * mino.y)*SCREEN_SIZE });
				cell.setFillColor(sf::Color{(unsigned char)(T.color.r >> 3), (unsigned char)(T.color.g >> 3) , (unsigned char)(T.color.b >> 3)});
				window.draw(cell);
			}

			// Draw Next Shape
			auto center = sf::Vector2i{ static_cast<int>(1.5f * COLUMNS) , static_cast<int>(0.25f * ROWS)};
			for (auto& mino : T.getMinosPosition((Tetris::Shape)nextShape.front(), center))
			{
				//Shifting the tetromino to the center of the preview border
				float next_tetromino_x = CELL_SIZE * (mino.x - 0.5f) * SCREEN_SIZE;
				float next_tetromino_y = CELL_SIZE * mino.y * SCREEN_SIZE;

				if (Tetris::Shape::I == (Tetris::Shape)nextShape.front())
				{
					next_tetromino_y += (round(0.5f * CELL_SIZE));
				}
				else if (Tetris::Shape::O == (Tetris::Shape)nextShape.front())
				{
					next_tetromino_x -= (round(0.5f * CELL_SIZE));
				}

				cell.setPosition({ next_tetromino_x, next_tetromino_y});
				cell.setFillColor(Tetris::colorMap[(Tetris::Shape)nextShape.front()]);
				window.draw(cell);
			}

			//If the game is not over
			if (T.isReset == 0)
			{
				cell.setFillColor(T.color);
				for (auto& mino : T.minos)
				{
					cell.setPosition({ static_cast<float>(CELL_SIZE * mino.x) * SCREEN_SIZE, static_cast<float>(CELL_SIZE * mino.y) * SCREEN_SIZE });
					window.draw(cell);
				}
			}
		}



		//Drawing the effect
		if (cleanLineEffect != 0)
		{
			cell.setFillColor(sf::Color{255, 215, 0});
			cell.setSize({ static_cast<float>(CELL_SIZE - 1) * SCREEN_SIZE * cleanLineEffect / CLEAR_EFFECT_DURATION, static_cast<float>(CELL_SIZE - 1) * SCREEN_SIZE * cleanLineEffect / CLEAR_EFFECT_DURATION });
			for (unsigned char a = 0; a < COLUMNS; a++)
			{
				for (auto& i : matrix.clearline)
				{
					cell.setPosition({ (CELL_SIZE * ((float)a + (float)(CLEAR_EFFECT_DURATION - cleanLineEffect) * 0.5f / CLEAR_EFFECT_DURATION )) * SCREEN_SIZE, (CELL_SIZE * ((float)i + (float)(CLEAR_EFFECT_DURATION - cleanLineEffect) / CLEAR_EFFECT_DURATION * 0.5f) * SCREEN_SIZE)});
					window.draw(cell);
				}
			}
		}

		draw_text({ CELL_SIZE * (0.5f + COLUMNS) * SCREEN_SIZE, CELL_SIZE * 0.5 * ROWS * SCREEN_SIZE }, std::to_string(score), window);
		std::cout << "SCORE!!! :" << score << std::endl;
		window.display();
	}

	return 0; // Exit application
}
