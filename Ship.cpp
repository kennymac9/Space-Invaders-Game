#include "Ship.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Ship::Ship(const int id) {
	

	if(!texture.loadFromFile("spacecraft.png"))
       throw invalid_argument("Ship not loaded!");

	sprite.setTexture(texture);
	sprite.scale(sf::Vector2f(0.4,0.4));
	sprite.setPosition(posx,posy);
	alive = true;
}



void Ship::draw(sf::RenderWindow &win) {
	sprite.setTexture(texture);
	win.draw(sprite);
}

void Ship::setLoc(int xpos, int ypos){
	sprite.setPosition(xpos,ypos);
}
