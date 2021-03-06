#pragma once

#include <iostream>
#include <SFML\Graphics.hpp>






class Item {
public:
	Item()
	{
		item.setSize(sf::Vector2f(50.0,50.0));
		item.setFillColor(sf::Color::Yellow);
		texture.loadFromFile("item.png");
		item.setTexture(&texture);
	}
	void setPOs(sf::Vector2f pos)
	{
		item.setPosition(pos);
	}
	sf::FloatRect getGlobalBounds()
	{
		return item.getGlobalBounds();
	}
	void toDraw(sf::RenderWindow& window)
	{
		window.draw(item);
	}



private:
	sf::RectangleShape item;
	sf::Texture texture;

};

class Solid {
public:
	Solid(sf::Vector2f pos);
	
	
	void toDraw(sf::RenderWindow& window)
	{
		window.draw(solid[0]);
		window.draw(solid[1]);
		window.draw(solid[2]);
		window.draw(solid[3]);
		window.draw(solid[4]);
	}

private:
	sf::RectangleShape solid[5];

};
