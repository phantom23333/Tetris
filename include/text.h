#pragma once
#include <string>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics.hpp"
#include <filesystem>

void draw_text(sf::Vector2f pos, const std::string& i_text, sf::RenderWindow& i_window);
