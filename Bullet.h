#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>


#ifndef SpriteMove_Bullet_h
#define SpriteMove_Bullet_h


class Bullet {
    public:
        Bullet(const int);
        
        int bullet_posx;
        int bullet_posy;
        bool origin; //true is player
        bool alive;

        void setLoc(float,float);
        int getid();
        void draw(sf::RenderWindow &win);
    
        int bulletSpeed;
    private:
        int bulletId;
        
        sf::Sprite sprite;
        sf::Texture texture;

};



#endif
