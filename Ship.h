#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#ifndef SpriteMove_Ship_h
#define SpriteMove_Ship_h

class Ship {
	public:
		Ship(const int);
		bool alive;
		int posx;
		int posy;
		int posxcorner;
		int posycorner;
		
		
		void setLoc(int,int);

		void draw(sf::RenderWindow &win);

	private:
		sf::Sprite sprite;
		sf::Texture texture;

};
		
#endif	
