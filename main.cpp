#include <SFML/Graphics.hpp>
#include <iostream>
#include <tuple>
#include <vector>
#include <list>
#include <stdexcept>
#include <cmath>
#include "Ship.h"
#include "Alien.h"
#include "Bullet.h"


#define WIDTH 800
#define HEIGHT 600
#define NUMBER_OF_ALIENS 35
#define ALIEN_PNG_SIZE 120
#define SPACING_BTW_ALIENS 50
#define CHANCE_OF_ALIEN_SHOT 10
#define SHIP_HIT_BUFFER 30
#define SHIP_BUFFER_FROM_SCREEN_BOTTOM 80
using namespace std;



std::tuple<float, float> calculate_Pos(int posx, int posy){
    //Tuple that will be passed the size of the PNG image files so the size of the pixel image and area it occupies can be determined 
    Alien alien(0);
    float scale;
    float imageSize;
    float newPosX;
    float newPosY;
    scale = alien.getScaling();

    imageSize = scale * ALIEN_PNG_SIZE;

    newPosX = imageSize + posx;
    newPosY = imageSize + posy;

    return std::make_tuple(newPosX,newPosY);
    

}

int CollisionDetection(int xpos_bullet, int ypos_bullet, int xpos_alien, int ypos_alien, int xposcorner_alien, int yposcorner_alien, int xpos_ship, int ypos_ship,int xposcorner_ship, int yposcorner_ship, bool shooter, int alienID, int bulletID){
    //Function to test the collision of a bullet and either alien or player
    if (shooter){ // Player is shooter
        //cout << xpos_alien << " : " << xpos_bullet << " : " << xposcorner_alien << endl;
        if(xpos_bullet > xpos_alien && xpos_bullet < xposcorner_alien){
            //cout << "bullet on track" << endl;
            if (ypos_bullet < yposcorner_alien && ypos_bullet > ypos_alien){
                //cout<< "hit" << endl;
                //cout << xpos_bullet << " " << ypos_bullet << " " << endl;
                return 1;
            }
        }
    } else if (!shooter){
        if(xpos_bullet > xpos_ship && xpos_bullet < xposcorner_ship-SHIP_HIT_BUFFER){
            if (ypos_bullet < yposcorner_ship && ypos_bullet > ypos_ship){
                return 2;
            }
        }
        
        
    } 
    return 0;
}

bool randomAlienShot(){
    //Function to randomly determine when an alien shoots
    int random;
    random = rand() %  CHANCE_OF_ALIEN_SHOT + 1;
    if (random == CHANCE_OF_ALIEN_SHOT){
        return true;

    }
}

int randomAlien(){
    // function to randomly determine which alien will shoot
    int random;
    random = rand() %  NUMBER_OF_ALIENS;
    return random;
}




int main(){

    //Bunch of initializations
    srand (time(NULL));
    int NUMBER_OF_BULLETS = 0;
    bool spacePressed = false;
    bool spaceReleased = false;
    bool returnReleased = false;
    bool play = true;
    bool playerBulletAlive = false;
    bool movement_direction = true;
    int movement_vector;
    int detector = 0;
    bool win = false;
    bool lose = false;
    int initial_spacing_top = 20;
    auto Alien_PNG_Size = calculate_Pos(0,0);
    int one_side_alien_PNG = get<0>(Alien_PNG_Size);
    int total_pixel_len = NUMBER_OF_ALIENS*SPACING_BTW_ALIENS + one_side_alien_PNG;
    int number_of_alien_rows = ceil((float)total_pixel_len/(3*HEIGHT/4));
    int aliens_per_row = floor(NUMBER_OF_ALIENS/(float)number_of_alien_rows);
    int spacing_between_aliens_col = SPACING_BTW_ALIENS;
    int spacing_between_aliens_rows = SPACING_BTW_ALIENS;
    int alien_increment = 0;
    int playerShotBulletID = 0;
    //int aliens_per_row_remainder = NUMBER_OF_ALIENS%number_of_alien_rows;
    //cout << "number_of_alien_rows " << number_of_alien_rows << endl;
    //cout << "aliens_per_row " << aliens_per_row << endl;
    //cout << "aliens_per_row_remainder " << aliens_per_row_remainder << endl;

    //The vectors that store aliens and bullets
    vector <Bullet> vector_of_bullets;
    vector <Alien> vector_of_aliens;

    
 

    //Setting up the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT),"Space Invaders");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    sf::Sprite back;
    sf::Texture space;
    if(!space.loadFromFile("space_image.png"))
        throw invalid_argument("Background not loaded!");
    
    back.setTexture(space);
    window.draw(back);



   
    

    //Initial objects, importantly the ship is placed in center screen and size determined
    Alien alien(0);
    Ship myShip(0);
    myShip.posx = WIDTH/2;
    myShip.posy = HEIGHT-SHIP_BUFFER_FROM_SCREEN_BOTTOM;
    myShip.setLoc(myShip.posx,myShip.posy);
    auto shipCornerCoords = calculate_Pos(myShip.posx,myShip.posy);
    myShip.posxcorner = get<0>(shipCornerCoords);
    myShip.posycorner = get<1>(shipCornerCoords);

    //cout << "NO ERROR 155" << endl;

    //Setting up initial positions so creating rows of alien matrix is easier
    //Also setting some characteristics to each alien i.e. alive
    for(int i = 0; i<NUMBER_OF_ALIENS; i++){

        Alien alien(i);
        vector_of_aliens.push_back(alien);

        //cout << vector_of_aliens.size() << endl;
        //cout << vector_of_aliens[i].getid() << endl;
        
        vector_of_aliens[i].alive = true;
        vector_of_aliens[i].alien_posx = 0;
        vector_of_aliens[i].alien_posy = 0;

        

        auto cornerCoords = calculate_Pos(vector_of_aliens[i].alien_posx,vector_of_aliens[i].alien_posy);

        vector_of_aliens[i].alien_posx_corner = get<0>(cornerCoords);
        vector_of_aliens[i].alien_posy_corner = get<1>(cornerCoords);
 
    }

    

    //cout << "NO ERROR 180" << endl;
    


    //Setting spacing, rows, and columns of matrix
    for(int i = 0; i < number_of_alien_rows; i++){
        for(int j = 0; j <= aliens_per_row; j++){
            
            vector_of_aliens[alien_increment].setLoc(spacing_between_aliens_col * j, initial_spacing_top+spacing_between_aliens_rows*i);
            vector_of_aliens[alien_increment].alien_posx_setup = vector_of_aliens[alien_increment].alien_posx;
            vector_of_aliens[alien_increment].alien_posy_setup = vector_of_aliens[alien_increment].alien_posy;
            
            //cout << vector_of_aliens[alien_increment].alien_posx_setup << endl;
            //cout << vector_of_aliens[alien_increment].alien_posy_setup << endl;
            
            alien_increment++;

            
        }
    }

    //cout << "NO ERROR 190" << endl;

    //Start of game loop
    sf::Event event;

    while (play == true) {
        

        //Detecting keys for exit and fire
        while (window.pollEvent(event)) {
            //Start Command

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return) {
                returnReleased = true;
                
            }

            //Exit Commands
            if (event.type == sf::Event::Closed) {
                play = false;
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
                play = false;
            }

            //Fire bullet commands
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                spacePressed = true;
                
            }

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                spaceReleased = true;
            }

        }

        
        //cout << "NO ERROR 228" << endl;
        

        //reseting key press so bullets only fire on release
        if (spacePressed == true) {
            //std::cout << "Key has been pressed\n";
            spacePressed = false;
        }
        
        //Bullet fire function
        if (spaceReleased == true) {
            spaceReleased = false;
            //Detect is there is already a bullet on screen and player is alive to play
            if(myShip.alive == true && playerBulletAlive == false){
                Bullet bullet(NUMBER_OF_BULLETS);
                vector_of_bullets.push_back(bullet);

                playerShotBulletID = vector_of_bullets[NUMBER_OF_BULLETS].getid();
                //std::cout << "NUMBER OF BULLETS "<<NUMBER_OF_BULLETS << std::endl;

                vector_of_bullets[NUMBER_OF_BULLETS].setLoc(myShip.posx+15,myShip.posy);
                
                

                //std::cout << vector_of_bullets[NUMBER_OF_BULLETS].bullet_posx << " : " << vector_of_bullets[NUMBER_OF_BULLETS].bullet_posy << std::endl;
                
                vector_of_bullets[NUMBER_OF_BULLETS].draw(window);

                NUMBER_OF_BULLETS++;  
            }       
        }

        //cout << "NO ERROR 260" << endl;
        
        //Aliens random ability to fire back, also assigns alien bullets special characteristics like speed and origin
        if(randomAlienShot() == true){
            //cout << "RANDOM SHOT LOOP GO " << endl;
            int alienShot = randomAlien();

            //cout <<"ALIEN TO SHOOT " <<alienShot << endl;
            //cout << "LIFE STATUS "<< vector_of_aliens[alienShot].alive << endl;
            if(vector_of_aliens[alienShot].alive == true){
                int xcoordShot;
                int ycoordShot;

                xcoordShot = floor((vector_of_aliens[alienShot].alien_posx + vector_of_aliens[alienShot].alien_posx_corner)/2);
                ycoordShot = vector_of_aliens[alienShot].alien_posy_corner;

                //std::cout << "NUMBER OF BULLETS: "<<NUMBER_OF_BULLETS << std::endl;
                Bullet bullet(NUMBER_OF_BULLETS);
                vector_of_bullets.push_back(bullet);

                vector_of_bullets[NUMBER_OF_BULLETS].setLoc(xcoordShot,ycoordShot);    
                vector_of_bullets[NUMBER_OF_BULLETS].origin = false;
                vector_of_bullets[NUMBER_OF_BULLETS].bulletSpeed = -6;
            
                vector_of_bullets[NUMBER_OF_BULLETS].draw(window);

                NUMBER_OF_BULLETS++;     
             }
        }


        //cout << "NO ERROR 298" << endl;
        // key detect for return. never used
        if (returnReleased == true) {
            //std::cout << "Return Released \n";
            returnReleased = false;
        }

        //Ships movement key detect
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            if(myShip.posxcorner < WIDTH){
               myShip.posx += 2; 
            }
            
            
            myShip.setLoc(myShip.posx,myShip.posy);
            auto shipCornerCoords = calculate_Pos(myShip.posx,myShip.posy);
            myShip.posxcorner = get<0>(shipCornerCoords);
            
            //cout << myShip.posxcorner << " : " << myShip.posycorner << endl;
            
        }
        // more movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            if(myShip.posx > 0){
               myShip.posx -= 2; 
            }
            
            myShip.setLoc(myShip.posx,myShip.posy);
            auto shipCornerCoords = calculate_Pos(myShip.posx,myShip.posy);
            myShip.posxcorner = get<0>(shipCornerCoords);

        }

        if(win || lose){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
                win = false;
                lose = false;
                for(int i = 0; i < NUMBER_OF_ALIENS; i++){
                    vector_of_aliens[i].alive = true;
                    vector_of_aliens[i].alien_posx = vector_of_aliens[i].alien_posx_setup;
                    vector_of_aliens[i].alien_posy = vector_of_aliens[i].alien_posy_setup;
                    vector_of_aliens[i].draw(window);
                    

                }
                for(int i = 0; i<NUMBER_OF_BULLETS; i++){
                    vector_of_bullets[i].alive = false;
                }
                myShip.posx = WIDTH/2;
                myShip.posy = HEIGHT-SHIP_BUFFER_FROM_SCREEN_BOTTOM;
                myShip.setLoc(myShip.posx,myShip.posy);
                playerBulletAlive = false;
                myShip.alive = true;
                myShip.draw(window);
                window.display();
            }
        }

        //cout << "NO ERROR 298" << endl;
        
        //Clearing screen from last draw and drawing back ground
        window.clear();
        window.draw(back);

        
        //Passing aliens, bullets, ship to collision detection
        for(int j = 0; j < NUMBER_OF_ALIENS; j++){
            for(int i = 0; i < NUMBER_OF_BULLETS; i++){
             
                detector = CollisionDetection(vector_of_bullets[i].bullet_posx,vector_of_bullets[i].bullet_posy,vector_of_aliens[j].alien_posx,vector_of_aliens[j].alien_posy,vector_of_aliens[j].alien_posx_corner,vector_of_aliens[j].alien_posy_corner,myShip.posx,myShip.posy,myShip.posxcorner,myShip.posycorner,vector_of_bullets[i].origin, vector_of_aliens[i].getid(), vector_of_bullets[i].getid());

                //cout << detector << endl;
                //cout << vector_of_bullets[i].bullet_posy << endl;
                //cout << vector_of_aliens[j].alive << endl;

                if (detector == 1 && vector_of_bullets[i].alive && vector_of_aliens[j].alive){
                    //cout << "hit" << endl;
                    vector_of_aliens[j].alive = false;
                    vector_of_bullets[i].alive = false;
                    playerBulletAlive = false;
                    
                    detector = 0;

                }

                if (detector == 2 && vector_of_bullets[i].alive && myShip.alive){
                    //cout << "dead" << endl;
                    vector_of_bullets[i].alive = false;
                    myShip.alive = false;

                }


            }
        }


        //cout << "NO ERROR 365" << endl;


        
        //Printing bullets and determining if they shouold be alive. Dead if hit alien or off screen so not re drawn
        for(int i = 0; i < NUMBER_OF_BULLETS; i++){
            if(vector_of_bullets[i].alive == true){
                vector_of_bullets[i].setLoc(vector_of_bullets[i].bullet_posx,vector_of_bullets[i].bullet_posy-vector_of_bullets[i].bulletSpeed);
                vector_of_bullets[i].draw(window);
            }
            if(vector_of_bullets[i].bullet_posy < 0 || vector_of_bullets[i].bullet_posy > HEIGHT){
                vector_of_bullets[i].alive = false;
                if(vector_of_bullets[i].origin == true){
                    //Making sure one player bullet is shot at a time
                    playerBulletAlive = false;
                }
            }     
        }

        //cout << "POS " << vector_of_bullets[0].bullet_posy << endl;
        //cout << "BULLET LIFE " << vector_of_bullets[0].alive << endl;
        //cout << "SIZE OF BULLET " << vector_of_bullets.size() << endl;
        //Finding the player bullets to make sure no more than one is alive
        if(vector_of_bullets.size() > 0){
            for(int i = 0; i < NUMBER_OF_BULLETS; i++){
                if(vector_of_bullets[i].origin == true && vector_of_bullets[i].alive == true){
                    playerBulletAlive = true;
                }
            }
        }


        //cout << "NO ERROR 386" << endl;
        //Setting up alien direction and whether aliens are alive and should be drawn
        for(int i = 0; i < NUMBER_OF_ALIENS; i++){
            movement_direction ? movement_vector = 1 : movement_vector = -1;

            if(vector_of_aliens[i].alive == true){
                vector_of_aliens[i].setLoc(vector_of_aliens[i].alien_posx+movement_vector,vector_of_aliens[i].alien_posy);
                
                if(vector_of_aliens[i].alien_posx > WIDTH-one_side_alien_PNG){
                    for(int i = 0; i < NUMBER_OF_ALIENS; i++){
                        vector_of_aliens[i].alien_posy = vector_of_aliens[i].alien_posy + spacing_between_aliens_rows;
                        movement_direction = false;
                    }
                } else if (vector_of_aliens[i].alien_posx < 0){
                    for(int i = 0; i < NUMBER_OF_ALIENS; i++){
                        vector_of_aliens[i].alien_posy = vector_of_aliens[i].alien_posy + spacing_between_aliens_rows;
                        movement_direction = true;
                    }
                }   

                auto cornerCoords = calculate_Pos(vector_of_aliens[i].alien_posx,vector_of_aliens[i].alien_posy);
                vector_of_aliens[i].alien_posx_corner = get<0>(cornerCoords);
                vector_of_aliens[i].alien_posy_corner = get<1>(cornerCoords);

                if(vector_of_aliens[i].alien_posy_corner > HEIGHT){
                    for(int i = 0; i < NUMBER_OF_ALIENS; i++){
                        vector_of_aliens[i].alien_posy = vector_of_aliens[i].alien_posy_setup;
                        vector_of_aliens[i].alien_posx = vector_of_aliens[i].alien_posx_setup;
                    }  
                } 
                vector_of_aliens[i].draw(window);    
            } 
        }


        //cout << "NO ERROR 427" << endl;

        
        
        
        

        //Checking if ship is alive. if dead you lose
        if(myShip.alive == true){
            myShip.draw(window);
        }else if(myShip.alive == false){
            lose = true;
            win = false;
            sf::Sprite loser;
            sf::Texture loserScreen;
            if(!loserScreen.loadFromFile("loseScreen.png"))
                throw invalid_argument("Background not loaded!");
            
            loser.setTexture(loserScreen);
            window.draw(loser);
        }
         
        
        //checking if all the aliens are dead. if so you win
        int aliveAlienCounter = 0;
        for(int i=0; i<NUMBER_OF_ALIENS; i++){
            if(vector_of_aliens[i].alive == true){
                aliveAlienCounter++;
            }
        }
        //cout <<"ALIVE ALIENS "<< aliveAlienCounter << endl;
        //cout <<"SIZE OF ALIENS VECTOR "<< vector_of_aliens.size() << endl; 
        
        if(aliveAlienCounter == 0){
            win = true;
            lose = false;
            sf::Sprite winner;
            sf::Texture winnerScreen;
            if(!winnerScreen.loadFromFile("winScreen.png"))
                throw invalid_argument("Background not loaded!");
            
            winner.setTexture(winnerScreen);
            window.draw(winner);
            
        }
        
        
        
        
        //updating display
        window.display();

        

        //cout << "NO ERROR 479" << endl;


    }



    window.close();
    return 0;


}


