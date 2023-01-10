#include "MyGame.h"

SDL_Rect player1 = { 0, 0, 20, 60 };
SDL_Rect player2 = { 0, 0, 20, 60 };
//plays any sound parsed here
void playSound(Mix_Chunk* sound)
{
    Mix_PlayChannel(-1, sound, 0);
}

//sets the x position of the bats to be consistent with the server
void setBatX()
{
    player1.x = game_data.player1X;
    player2.x = game_data.player2X;
}
void MyGame::init(SDL_Renderer* renderer)
{
    //loads image files
    SDL_Surface* batSurface = IMG_Load("bat.png");
    SDL_Surface* ballSurface = IMG_Load("ball.png");
    SDL_Surface* particleSurface = IMG_Load("particle.png");
    if (batSurface != nullptr) {
        std::cout << "Loaded" << std::endl;
    }
    else {
        std::cout << "Not Loaded" << std::endl;
    }
    bat = SDL_CreateTextureFromSurface(renderer, batSurface);
    SDL_FreeSurface(batSurface);

    //tells console if the texture has loaded properly or not
    if (ballSurface != nullptr) {
        std::cout << "Loaded" << std::endl;
    }
    else {
        std::cout << "Not Loaded" << std::endl;
    }
    //creating texture for the ball
    ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface);

    //loading sound file
    batHit = Mix_LoadWAV("bat_hit.wav");
    //tells console if the sound has loaded properly or not
    if (batHit != nullptr)
    {
        std::cout << "loaded sound" << std::endl;
    }
    else
    {
        std::cout << "failed to load sound" << std::endl;
    }
    //creating texture for particle effects
    if (particleSurface != nullptr) {
        std::cout << "Loaded" << std::endl;
    }
    else {
        std::cout << "Not Loaded" << std::endl;
    }
    particleTexture = SDL_CreateTextureFromSurface(renderer, particleSurface);
    SDL_FreeSurface(particleSurface);
}
//recieves data from the server
void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        //Data relating to positions of game objects, bats and ball
        if (args.size() == 6) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player1X = stoi(args.at(1));
            game_data.player2Y = stoi(args.at(2));
            game_data.player2X = stoi(args.at(3));
            game_data.ballX = stoi(args.at(4));
            game_data.ballY = stoi(args.at(5));
            setBatX();
        }
        
    }
    else if (cmd == "SCORES") {
        //player score data
        if (args.size() == 2) {
            //recieving score value and playing a sound whenever a new value is recieved
            score.p1Score = stoi(args.at(0));
            score.p2Score = stoi(args.at(1));
            playSound(batHit);
        }
    }//plays sound when recieving a message from the server
    else if(cmd == "PLAY_BAT_HIT_SOUNDGAME_DATA")
        //tells game to play a sound whenever the "PLAY_BAT_HIT_SOUNDGAME_DATA" command is recieved from the server
    {
        playSound(batHit);
        particlePlay = true;
    }
    else {
        std::cout << "Received: " << cmd << std::endl;
    }
}
//sends data to server
void MyGame::send(std::string message) {
    messages.push_back(message);
}
//takes player input
void MyGame::input(SDL_Event& event) {
    //keyboard input events, controls player movements
    switch (event.key.keysym.sym) {
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
        case SDLK_SPACE:
            send(event.type == SDL_KEYDOWN ? "SPACE_DOWN" : "SPACE_UP");
            break;
    }
}

void MyGame::update() {
    //keeps bat and ball positions consistent with the server
    player1.y = game_data.player1Y;
    player2.y = game_data.player2Y;
    ball.x = game_data.ballX;
    ball.y = game_data.ballY;
   
    //removes value number value from scoreStrm
    scoreStrmP1.clear();
    //blanks string value of scoreStrm
    scoreStrmP1.str(std::string());
    //sets scoreStrm to be equal to the score in the server
    scoreStrmP1 << score.p1Score;

    scoreStrmP2.clear();
    scoreStrmP2.str(std::string());
    scoreStrmP2 << score.p2Score;
    
}
//freeing surface and texture to prevent memory leaks
void freeTexture(SDL_Surface* surface, SDL_Texture* texture)
{
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
//rendering for particle effects
void MyGame::renderParticles(SDL_Renderer* renderer)
{

    if (particlePlay)
    {
        particle.x = ball.x;
        particle.y = ball.y;
            SDL_RenderCopy(renderer, particleTexture, NULL, &particle);
        particlePlay = false;
    }
}
//rendering for bat and ball
void MyGame::render(SDL_Renderer* renderer) {
    //render ball and bat textures
    SDL_RenderCopy(renderer, bat, NULL, &player1);
    SDL_RenderCopy(renderer, bat, NULL, &player2);
    SDL_RenderCopy(renderer, ballTexture, NULL, &ball);
}
//renderering for text
void MyGame::renderText(SDL_Renderer* renderer, TTF_Font* sans)
{
    //sets the texture for player ones score text
    sLeftScore = TTF_RenderText_Solid(sans, scoreStrmP1.str().c_str(), white);
    leftScore = SDL_CreateTextureFromSurface(renderer, sLeftScore);
    //resizes texture box to properly fit and display the texture, then renders
    SDL_QueryTexture(leftScore, NULL, NULL, &lScoreRect.w, &lScoreRect.h);
    SDL_RenderCopy(renderer, leftScore, NULL, &lScoreRect);

    sRightScore = TTF_RenderText_Solid(sans, scoreStrmP2.str().c_str(), white);
    rightScore = SDL_CreateTextureFromSurface(renderer, sRightScore);
    SDL_QueryTexture(rightScore, NULL, NULL, &rScoreRect.w, &rScoreRect.h);
    SDL_RenderCopy(renderer, rightScore, NULL, &rScoreRect);
    //frees text textures and surfaces
    freeTexture(sLeftScore, leftScore);
    freeTexture(sRightScore, rightScore);
}

