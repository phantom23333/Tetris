#include "text.h"



void draw_text(sf::Vector2f pos, const std::string& i_text, sf::RenderWindow& i_window)
{
	//We're gonna align the text ot the left top
	float character_x = pos.x;
	float character_y = pos.y;

	unsigned char character_width;
	sf::Texture font_texture;
	sf::Sprite character_sprite{ font_texture };
	font_texture.loadFromFile(std::filesystem::path{ RESOURCES_PATH R"(Font.png)" });

	//We're gonna calculate the width of the character based on the font image size
	//96 because there are 96 character in the image
	character_width = font_texture.getSize().x / 96;

	character_sprite.setTexture(font_texture);

	for (const char a : i_text)
	{
		if ('\n' == a)
		{
			//After every newline we put increase the y-coordinate and reset the x-coordinate
			character_x = pos.x;
			character_y += font_texture.getSize().y;

			continue;
		}

		//Change the position of the next character
		character_sprite.setPosition({character_x, character_y});
		//Pick the character from the font image
		character_sprite.setTextureRect(sf::IntRect({ character_width * (a - 32), 0 }, { character_width, (int)font_texture.getSize().y}));

		//Increase the x-coordinate
		character_x += character_width;

		//Draw the character
		i_window.draw(character_sprite);
	}
}
