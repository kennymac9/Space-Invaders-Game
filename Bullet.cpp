#include "Bullet.h"
#include <iostream>
#include <stdexcept>
using namespace std;




Bullet::Bullet(const int id){

    if(!texture.loadFromFile("bullet.png"))
        throw invalid_argument("Bullet not loaded!");
    

    bulletId = id;
    origin = true;
    alive = true;
    bulletSpeed = 6;
    

}

void Bullet::setLoc(float xpos, float ypos){
	sprite.setPosition(xpos,ypos);
    bullet_posx = xpos;
    bullet_posy = ypos;

}



int Bullet::getid(){
    return bulletId;
};


void Bullet::draw(sf::RenderWindow &win){
    sprite.setTexture(texture);
    win.draw(sprite);
};