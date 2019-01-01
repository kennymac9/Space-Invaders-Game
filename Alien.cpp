#include "Alien.h"
#include <iostream>
#include <stdexcept>

using namespace std;



Alien::Alien(const int id) {
	
	alienId = id;
	speed = 5;

	alive = true;
	alien_scaling = 0.5;

	texture.loadFromFile("alien.png");

	sprite.setTexture(texture);
	sprite.scale(sf::Vector2f(alien_scaling,alien_scaling));
}


void Alien::draw(sf::RenderWindow &win) {
	sprite.setTexture(texture);
	win.draw(sprite);
}

int Alien::getid(){
    return alienId;
}

void Alien::setLoc(float xpos, float ypos){
	sprite.setPosition(xpos,ypos);
    alien_posx = xpos;
    alien_posy = ypos;

}

float Alien::getScaling(){
	return alien_scaling;
}

