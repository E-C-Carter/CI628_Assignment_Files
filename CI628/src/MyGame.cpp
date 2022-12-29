#include "MyGame.h"
#include "SDL_image.h"
#include <SDL_ttf.h>

// Resets the particle counter and enables rendering of the particle system
//Has a string arugment which is check to see what the ball has just hit and updates the pHold variable
void MyGame::pReset(std::string hitBy) {
    
    if (hitBy == "bat1") {
        pTimer = pTimerConst;
        SDL_Delay(15);
        pHold = hitBy;
        renderP = true;
    } else if (hitBy == "bat2") {
        pTimer = pTimerConst;
        SDL_Delay(15);
        pHold = hitBy;
        renderP = true;
    }
    else if (hitBy == "score") {
        pTimer = pTimerConst;
        SDL_Delay(15);
        pHold = hitBy;
        renderP = true;
    }

}

// Loads the font and checks it
void MyGame::initFont() {
    //std::cout << "Font Load function called" << std::endl;

    font = TTF_OpenFont("roamer.ttf", 32);
    if (font == nullptr) {
        std::cout << "Font not loaded" << std::endl;
    }
    else {
        std::cout << "Font loaded" << std::endl;
    }
    
        
}

// Generates the new texture each 
// Needs to be updated to not run every update
void MyGame::genScoreTex(SDL_Renderer* renderer) {
    // Updates the score text variable - doing so by converting the stored player socre to a string
    p1ScoreText = std::to_string(p1Score);
    p2ScoreText = std::to_string(p2Score);

    // Sets the color
    SDL_Color White = { 255, 255, 255 };
    // Creates the new surface texture
    SDL_Surface* score1 = TTF_RenderText_Blended(font, p1ScoreText.c_str(), White);
    // Converts the surface to the texture 
    scorePlayer1T = SDL_CreateTextureFromSurface(renderer, score1);

    SDL_Surface* score2 = TTF_RenderText_Blended(font, p2ScoreText.c_str(), White);

    scorePlayer2T = SDL_CreateTextureFromSurface(renderer, score2);
}


//Intialization of Audio
void MyGame::init_audio() {
    // open 44.1KHz, 
    // signed 16bit
    // system byte order, 
    // stereo audio,
    // using 1024 byte chunks
    //Checks for an qudio mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        return;
    }

    // Loads the indualvidual sound files for each and ensures they are loaded
    sound = Mix_LoadWAV("drop.wav");
    if (sound == nullptr) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
    else {
        std::cout << "Sound effect loaded" << std::endl;
    }
    //Loads the paddle and ball hit sound
    paddleSound = Mix_LoadWAV("batBounce.wav");
    if (sound == nullptr) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
    else {
        std::cout << "Paddle ball hit Sound effect loaded" << std::endl;
    }
    //Loads the ball bouncing on wall sound
    ballBounceSound = Mix_LoadWAV("wallBounce.wav");
    if (sound == nullptr) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
    else {
        std::cout << "Paddle ball hit Sound effect loaded" << std::endl;
    }
    //Loads the score sound
    scoreSound = Mix_LoadWAV("score.wav");
    if (sound == nullptr) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
    else {
        std::cout << "Score Sound effect loaded" << std::endl;
    }


}

// Intialization of Textures
void MyGame::init(SDL_Renderer* renderer) {
    // BALL TEXTURE LOAD
    SDL_Surface* surface = IMG_Load("ball.png");

    //Checks the ball texture has been loaded
    if (surface != nullptr) {
        std::cout << "Ball Loaded" << std::endl;
        ballLoaded = true;
    }
    else {
        std::cout << "Ball Not Loaded" << std::endl;
        ballLoaded = false;
    }

    //Assigns the texture to the ball1 variable
    ball1 = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_FreeSurface(surface);
    

    //BAT TEXTURE LOAD
    //BAT 1
    surface = IMG_Load("bat.png");
    //Checks the ball texture has been loaded
    if (surface != nullptr) {
        std::cout << "Bat Loaded" << std::endl;
        bat1Loaded = true;
        
    }
    else {
        std::cout << "Bat Not Loaded" << std::endl;
        bat1Loaded = false;
        
    }

    bat1 = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_FreeSurface(surface);

    //BAT 2
    surface = IMG_Load("bat.png");
    //Checks the ball texture has been loaded
    if (surface != nullptr) {
        std::cout << "Bat Loaded" << std::endl;
        bat2loaded = true;

    }
    else {
        std::cout << "Bat Not Loaded" << std::endl;
        bat2loaded = false;
    }

    bat2 = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    //Generates particles
    for (int i = 0; i < 1000; i++) {
        Particle* p = new Particle();
        p->pX = 20;
        p->pY = 30;

        particles.push_back(p);
    }
}


//Gets infomation from the sever
void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        // Changed to handle 6 args so that it can handle player X Coords
        // Gets the game data from the sever
        if (args.size() == 6) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player2Y = stoi(args.at(1));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));
            game_data.player1X = stoi(args.at(4));
            game_data.player2X = stoi(args.at(5));
        }
    } else {
        std::cout << "Received: " << cmd << std::endl;
    }
    //SCORE UPDATE
    //Updates the score from the sever message
    if (cmd == "SCORES") {
        if (args.size() == 2) {
            p1Score = stoi(args.at(0));
            p2Score = stoi(args.at(1));
            std::cout << "Player 1 Score: " << p1Score << std::endl;
            std::cout << "Player 2 Score: " << p2Score << std::endl;
        }
        else {}
    }

    //SOUND
    //This checks for a server message being sent and then plays a sound
    //Swapping this to a case would be better
    //The particle system makes use of this as well to reset the particle systme
    if (cmd == "BALL_HIT_BAT1") {
        play_paddleSound();
        
        pReset("bat1");
    }
    else if (cmd == "BALL_HIT_BAT2") {
        play_paddleSound();

        pReset("bat2");
    }

    if (cmd == "HIT_WALL_LEFTGAME_DATA") {
        play_ballBounceSound();
    }
    else if (cmd == "HIT_WALL_RIGHTGAME_DATA") {
        play_ballBounceSound();
    }
    else if (cmd == "HIT_WALL_UP") {
        play_ballBounceSound();
    }
    else if (cmd == "HIT_WALL_DOWN") {
        play_ballBounceSound();
    }
    if (cmd == "SCORES") {
        play_scoreSound();
        pReset("score");
    }
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        //switch statement that controls both player movement
        // WASD used to control player 1
        // IJKL used to control player 2
        // F does nothing relavent
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        case SDLK_i:
            send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
            break;
        case SDLK_k:
            send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
            break;
        case SDLK_a:
            send(event.type == SDL_KEYDOWN ? "A_DOWN" : "A_UP");
            break;
        case SDLK_d:
            send(event.type == SDL_KEYDOWN ? "D_DOWN" : "D_UP");
            break;
        case SDLK_j:
            send(event.type == SDL_KEYDOWN ? "J_DOWN" : "J_UP");
            break;
        case SDLK_l:
            send(event.type == SDL_KEYDOWN ? "L_DOWN" : "L_UP");
            break;
        case SDLK_f:
            play_sound();
            break;

    }
}
//PLAY SOUNDS EFFECTS fUNCTIONS
//Find a Way to make this into one function?
void MyGame::play_sound() {
    if (Mix_PlayChannel(-1, sound, 0) == -1) {
        printf("Error playing sound. Mix_PlayChannel: %s\n", Mix_GetError());
    }
}

void MyGame::play_paddleSound() {
    if (Mix_PlayChannel(-1, paddleSound, 0) == -1) {
        printf("Error playing sound. Mix_PlayChannel: %s\n", Mix_GetError());
    }
}

void MyGame::play_ballBounceSound() {
    if (Mix_PlayChannel(-1, ballBounceSound, 0) == -1) {
        printf("Error playing sound. Mix_PlayChannel: %s\n", Mix_GetError());
    }
}
void MyGame::play_scoreSound() {
    if (Mix_PlayChannel(-1, scoreSound, 0) == -1) {
        printf("Error playing sound. Mix_PlayChannel: %s\n", Mix_GetError());
    }
}

//The update loop - It will update the screen on the client side
void MyGame::update() {
    player1.y = game_data.player1Y;
    player2.y = game_data.player2Y;
    ball.x = game_data.ballX;
    ball.y = game_data.ballY;
    player1.x = game_data.player1X;
    player2.x = game_data.player2X;

    //The particle update loop
    //Edits to create particles

    for (auto p : particles) {
        p->t += 0.016;

        // [0..1]
        //Determines the particle's values on spawn
        double randomValue = rand() * 1.1 / RAND_MAX - 0.5;

        if (randomValue > 0) {
            p->t = 0.0;
            randomValue = rand() * 1.0 / RAND_MAX - 0.5;

            p->pX = game_data.ballX + randomValue * 1;
            p->pY = game_data.ballY + randomValue * 3;

        }
    }
    //Simple timer to control if the paritcle system should render or not
    if (pTimer >= 1) {
        pTimer--;
        //std::cout << pTimer << std::endl;

    }
    else if(pTimer < 1) {
       renderP = false;
        
    }
    
}


//Renders the game on the client side.
void MyGame::render(SDL_Renderer* renderer) {
    

    // Checks to see if all the textures have been laoded correctly and uses them if they have or uses default rendering if they havn't
    if (ballLoaded == true && bat1Loaded == true && bat2loaded == true) {
        //Renders the textures to each induvidual componet
        SDL_RenderCopy(renderer, ball1, NULL, &ball);
        SDL_RenderCopy(renderer, bat1, NULL, &player1);
        SDL_RenderCopy(renderer, bat2, NULL, &player2);

        

    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &player1);
        SDL_RenderDrawRect(renderer, &player2);
        SDL_RenderDrawRect(renderer, &ball);
        SDL_RenderFillRect(renderer, &player1);
        SDL_RenderFillRect(renderer, &player2);
        SDL_SetRenderDrawColor(renderer, 255, 5, 5, 255);
        SDL_RenderFillRect(renderer, &ball);
    }

    SDL_SetRenderDrawColor(renderer, 244, 244, 244, 255);
    
    //Checks if particles should be rendered and then renders the particles to the screen
    if (renderP == true) {

        for (auto p : particles) {
            
            if (p1Score >= 21 || p2Score >= 21) {
                //Renders the particles red when a players score equals 21 or more, primative game over
                SDL_SetRenderDrawColor(renderer, 226, 33, 33, 255);
                pTimer = 250;
            }
            else {
                if (pHold == "bat1") {
                    //Renders the particles orange when hitting player 1 bat
                    SDL_SetRenderDrawColor(renderer, 238, 82, 54, 255);
                }
                else if (pHold == "bat2") {
                    //Renders the particles purple when hitting player 2 bat
                    SDL_SetRenderDrawColor(renderer, 52, 12, 251, 255);
                }
                else if (pHold == "score") {
                    //Renders the particles Green when scoring is achived
                    SDL_SetRenderDrawColor(renderer, 52, 173, 84, 255);
                }
            }
            
            SDL_Rect rect = { p->pX, p->pY, 18, 18 };

            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    // Calls the generate the score texture
    genScoreTex(renderer);
    // Renders the new texture to the rect that holds it
    SDL_RenderCopy(renderer, scorePlayer1T, NULL, &p1ScoreRect);
    SDL_RenderCopy(renderer, scorePlayer2T, NULL, &p2ScoreRect);
    

    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //SDL_RenderDrawRect(renderer, &player1);
    //SDL_RenderDrawRect(renderer, &player2);
    //SDL_RenderDrawRect(renderer, &ball);
    //SDL_RenderFillRect(renderer, &player1);
    //SDL_RenderFillRect(renderer, &player2);
    //SDL_SetRenderDrawColor(renderer, 255, 5, 5, 255);
    //SDL_RenderFillRect(renderer, &ball);
}
// The destory function which empties all stored sounds 
void MyGame::destroy() {
    Mix_FreeChunk(sound);
    sound = nullptr;
    Mix_FreeChunk(paddleSound);
    paddleSound = nullptr;
    Mix_FreeChunk(ballBounceSound);
    ballBounceSound = nullptr;
    Mix_FreeChunk(scoreSound);
    scoreSound = nullptr;

    Mix_CloseAudio();
}