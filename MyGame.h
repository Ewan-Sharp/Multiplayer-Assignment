#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include "SDL.h"
#include <sstream>
#include <SDL_image.h>
#include <SDL_mixer.h>
static struct GameData {
    int player1Y = 0;
    int player1X = 0;
    int player2Y = 0;
    int player2X = 0;
    int ballX = 0;
    int ballY = 0;
} game_data;

static struct Scores {
    int p1Score = 0;
    int p2Score = 0;
} score;

class MyGame {

    private:
        SDL_Rect ball = { 0, 0, 20, 20};

        //Score text variables
        //text colour
        SDL_Color white = { 255, 255, 255 };
        SDL_Color black = { 0, 0, 0 };
        //determines size and location of text
        SDL_Rect lScoreRect = {50, 100, 100, 100 };
        SDL_Rect rScoreRect = { 650, 100, 100, 100 };
        SDL_Rect particle = { 0, 0, 32, 32 };
        bool particlePlay = false;
        //stores the score value in a renderable format
        std::stringstream scoreStrmP1;
        std::stringstream scoreStrmP2;
        //
        SDL_Surface* sLeftScore;
        SDL_Texture* leftScore;
        SDL_Surface* sRightScore;
        SDL_Texture* rightScore;

        SDL_Texture* bat;
        SDL_Texture* ballTexture;
        SDL_Texture* particleTexture;

        Mix_Chunk* batHit;
    public:
        std::vector<std::string> messages;

        void init(SDL_Renderer* renderer);
        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);
        void renderParticles(SDL_Renderer* renderer);
        void renderText(SDL_Renderer* renderer, TTF_Font* sans);
};

#endif