#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#ifndef SpriteMove_Alien_h
#define SpriteMove_Alien_h

class Alien {
	public:
		Alien(const int);
		
		int alien_posx;
		int alien_posy;
		int alien_posx_corner;
		int alien_posy_corner;
		int alien_posx_setup;
		int alien_posy_setup;
		bool alive;
		float alien_scaling;	
		
		int getid();
		void setLoc(float,float);
		void draw(sf::RenderWindow &win);
		float getScaling();
	
	private:
		int alienId;
		int speed;
		sf::Sprite sprite;
		sf::Texture texture;

};

#endif
