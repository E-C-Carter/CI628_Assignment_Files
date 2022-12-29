#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <SDL_ttf.h>

// Basic data structure that holds key game infomation
// Player X Coords added
static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int ballX = 0;
    int ballY = 0;
    int player1X = 0;
    int player2X = 0;
    
} game_data;

// A class that creates a particle
class Particle {

public:
    int pX;
    int pY;
    int velX;
    int velY;
    double t = 0.0;
    
    

    bool needsUpdate() {
        return t >= 0.07;
    }

    void update() {
        pX += velX;
        pY += velY;
    }

    
};


class MyGame {

    private:
        int gameWidth = 800;
        int gameHeight = 600;
        //SDL_Rect player1 = { 0, 0, 20, 60 };
        //SDL_Rect player2 = { 550, 0, 20, 60 };
        SDL_Rect player1 = { gameWidth / 4, gameHeight / 2 -30, 28, 80 };
        SDL_Rect player2 = { 3 * gameWidth / 4 - 20, gameHeight / 2 - 30, 28, 80 };
        SDL_Rect ball = { gameWidth / 2 -5, gameHeight / 2 - 5, 20, 20 };

        //Player Score Texture Location
        SDL_Rect p1ScoreRect = { 100, 100, 50, 50 };
        SDL_Rect p2ScoreRect = { 630, 100, 50, 50 };
        
        //TEXTURER Variables
        //bat
        SDL_Texture* bat1;
        SDL_Texture* bat2;


        //Ball
        SDL_Texture* ball1;

        //Score
        // Holds the textures generated for the scores 
        SDL_Texture* scorePlayer1T;
        SDL_Texture* scorePlayer2T;

        //

        //TEXTURE LOAD CHECKS - Used to check that the textures have been loaded 
        
        bool ballLoaded = false;
        bool bat1Loaded = false;
        bool bat2loaded = false;

        //FONT Variable
        
        TTF_Font* font;


        //SOUND Variables
        Mix_Chunk* sound;
        //SFX
        Mix_Chunk* paddleSound;
        Mix_Chunk* ballBounceSound;
        Mix_Chunk* scoreSound;

        //SCORES
        //Values
        int p1Score = 0;
        int p2Score = 0;

        std::string p1ScoreText;
        std::string p2ScoreText;


        // PARTICLE
        std::vector<Particle*> particles;
        
        //Particle timer - Controls the particle system
        // Timer for paticle spawning
        int pTimer = 0;
        // Hold the particle reset value
        int pTimerConst = 35;
        // Bool that controls particle rendering
        bool renderP = false;
        //A string used to hold what the ball just hit
        std::string pHold;



        


    public:
        std::vector<std::string> messages;

        //Creates the init function
        void init(SDL_Renderer* renderer);

        //The font loading function
        void initFont();
        void genScoreTex(SDL_Renderer* renderer);
        //static SDL_Texture* createTextureFromString(const std::string&, TTF_Font*, SDL_Color);


        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);

        //Public sound functions
        void init_audio();
        void play_sound();
        void play_paddleSound();
        void play_ballBounceSound();
        void play_scoreSound();
        void destroy();

        //Function to enable the Particle system
        void pReset(std::string hitby);

};

#endif