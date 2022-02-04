/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "uno/Game.h"
#include "uno/ResourceManager.h"
#include "uno/SpriteRenderer.h"
#include "uno/GameObject.h"

// Game-related State data
SpriteRenderer* Renderer;
GameObject* Player;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../../../../unoLib/rsrc/default.vert", "../../../../unoLib/rsrc/default.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("../../../../unoLib/rsrc/background.jpg", false, "background");
    ResourceManager::LoadTexture("../../../../unoLib/rsrc/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../../../../unoLib/rsrc/block.png", false, "block");
    ResourceManager::LoadTexture("../../../../unoLib/rsrc/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../../../../unoLib/rsrc/paddle.png", true, "paddle");
    // load levels
    GameLevel one; one.Load("../../../../unoLib/rsrc/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("../../../../unoLib/rsrc/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("../../../../unoLib/rsrc/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("../../../../unoLib/rsrc/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        this->Levels[this->Level].Draw(*Renderer);
        // draw player
        Player->Draw(*Renderer);
    }
}