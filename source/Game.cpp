//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include <algorithm>
#include <fstream>
#include "Game.h"
#include "Spawner.h"
#include "Math.h"
#include "Random.h"

Game::Game()
    :mRunning(true)
    ,mPlayChannel(0)
    ,mWindowSize({600, 448})
    ,mCameraPos(Vector2::Zero)
{

}

bool Game::Initialize() {
    int initialized = SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO);
    if (initialized != 0) {
        return false;
    }
    mWindow = SDL_CreateWindow("Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowSize.x, mWindowSize.y, 0);
    if (mWindow == NULL) {
        return false;
    }
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    int imgInitialized = IMG_Init(IMG_INIT_PNG);
    if ((imgInitialized & IMG_INIT_PNG) != IMG_INIT_PNG) {
        return false;
    }
    if (mRenderer == NULL) {
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        return false;
    }
    LoadData();
    mPreviousTime = SDL_GetTicks();
    Random::Init();
    
    return true;
};

void Game::Shutdown() {
    UnloadData();
    IMG_Quit();
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    Mix_CloseAudio();
    SDL_Quit();
};

void Game::Runloop() {
    while (mRunning) {
        ProcessUpdate();
        UpdateGame();
        GenerateOutput();
    }
};

void Game::ProcessUpdate() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mRunning = false;
                break;
                
            default:
                break;
        }
    }
    
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    if (keyboardState[SDL_SCANCODE_ESCAPE]) {
        mRunning = false;
    }
    
    std::vector<Actor*> copyActors = mActors;
    for (Actor* a : copyActors) {
        a->ProcessInput(keyboardState);
    }
}

void Game::UpdateGame() {
    while ((int)SDL_GetTicks() < mPreviousTime + 16) {}
    int currentTime = SDL_GetTicks();
    float deltaTime = (float)(currentTime - mPreviousTime) / 1000;
    if (deltaTime > 0.033f) {
        deltaTime = 0.033f;
    }
    mPreviousTime = currentTime;
    
    std::vector<Actor*> actors = mActors;
    for (Actor* a : actors) {
        a->Update(deltaTime);
    }
    
    std::vector<Actor*> tempActors;
    for (Actor* a : mActors) {
        if (a->GetState() == ActorState::Destroy) {
            tempActors.emplace_back(a);
        }
    }
    
    if (!tempActors.empty()) {
        for (Actor* tempActor : tempActors) {
            delete tempActor;
        }
    }
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0);
    SDL_RenderClear(mRenderer);
    
    for (SpriteComponent* sprite : mSprites) {
        if (sprite->IsVisible()) {
            sprite->Draw(mRenderer);
        }
    }
    
    SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor* actor) {
    mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
    std::vector<Actor*>::iterator it;
    it = std::find(mActors.begin(), mActors.end(), actor);
    
    if (it != mActors.end()) {
        mActors.erase(it);
    }
}

void Game::LoadData() {
    Vector2 tempPosition;
    
    Actor* background = new Actor(this);
    tempPosition.Set(3392, 224);
    background->SetPosition(tempPosition);
    SpriteComponent* backgroundSprite = new SpriteComponent(background, 1);
    backgroundSprite->SetTexture(GetTexture("Assets/Background.png"));
    
    std::ifstream levelFile("Assets/Level1.txt", std::ifstream::in);
    std::string line;
    Vector2 blockPosition(16, 16);
    while(std::getline(levelFile, line)) {
        blockPosition.x = 16;
        for (char c : line) {
            if (c == 'A') {
                Block* block = new Block(this);
                block->SetPosition(blockPosition);
                block->SetTexture("Assets/BlockA.png");
            } else if (c == 'B') {
                Block* block = new Block(this);
                block->SetPosition(blockPosition);
                block->SetTexture("Assets/BlockB.png");
            } else if (c == 'C') {
                Block* block = new Block(this);
                block->SetPosition(blockPosition);
                block->SetTexture("Assets/BlockC.png");
            } else if (c == 'D') {
                Block* block = new Block(this);
                block->SetPosition(blockPosition);
                block->SetTexture("Assets/BlockD.png");
            } else if (c == 'E') {
                Block* block = new Block(this);
                block->SetPosition(blockPosition);
                block->SetTexture("Assets/BlockE.png");
            } else if (c == 'F') {
                Block* block = new Block(this);
                block->SetPosition(blockPosition);
                block->SetTexture("Assets/BlockF.png");
            } else if (c == 'G') {
                Block* block = new Block(this);
                block->SetPosition(blockPosition);
                block->SetTexture("Assets/BlockG.png");
            } else if (c == 'H') {
                Block* block = new Block(this);
                block->SetPosition(blockPosition);
                block->SetTexture("Assets/BlockH.png");
            } else if (c == 'I') {
                Block* block = new Block(this);
                block->SetPosition(blockPosition);
                block->SetTexture("Assets/BlockI.png");
            } else if (c == 'P') {
                mPlayer = new Player(this);
                mPlayer->SetPosition(blockPosition);
            } else if (c == 'Y') {
                Spawner* spawner = new Spawner(this);
                spawner->SetPosition(blockPosition);
            }
            
            blockPosition.x += 32;
        }
        
        blockPosition.y += 32;
    }
    
    mPlayChannel = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
}

void Game::UnloadData() {
    if (!mActors.empty()) {
        mActors.erase(mActors.begin(), mActors.end());
    }

    for (auto it = mTextureCache.begin(); it != mTextureCache.end(); it++) {
        SDL_DestroyTexture(it->second);
    }
    
    for (auto it = mSoundCache.begin(); it != mSoundCache.end(); it++) {
        Mix_FreeChunk(it->second);
    }
    
    mTextureCache.clear();
}

SDL_Texture* Game::GetTexture(const char* fileName) {
    std::unordered_map<std::string, SDL_Texture*>::const_iterator got = mTextureCache.find(fileName);
    if (got != mTextureCache.end())
        return got->second;
    else {
        SDL_Surface* tempSurface = IMG_Load(fileName);
        SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(mRenderer, tempSurface);

        SDL_FreeSurface(tempSurface);
        mTextureCache.emplace(fileName, tempTexture);
        return tempTexture;
    }
}

Mix_Chunk* Game::GetSound(const char* fileName) {
    std::unordered_map<std::string, Mix_Chunk*>::const_iterator got = mSoundCache.find(fileName);
    if (got != mSoundCache.end())
        return got->second;
    else {
        Mix_Chunk* tempSound = Mix_LoadWAV(fileName);
        mSoundCache.emplace(fileName, tempSound);
        return tempSound;
    }
}

void Game::AddSprite(SpriteComponent* sprite) {
    
    mSprites.emplace_back(sprite);
    std::sort(mSprites.begin(), mSprites.end(),
              [](SpriteComponent* a, SpriteComponent* b) {
                  return a->GetDrawOrder() < b->GetDrawOrder();
              });
}

void Game::RemoveSprite(SpriteComponent* sprite) {
    std::vector<SpriteComponent*>::iterator it;
    it = std::find(mSprites.begin(), mSprites.end(), sprite);
    
    if (it != mSprites.end()) {
        mSprites.erase(it);
    }
}

void Game::AddBlock(Block* block) {
    mBlocks.emplace_back(block);
}

void Game::RemoveBlock(Block* block) {
    std::vector<Block*>::iterator it;
    it = std::find(mBlocks.begin(), mBlocks.end(), block);
    
    if (it != mBlocks.end()) {
        mBlocks.erase(it);
    }
}

void Game::AddEnemy(Goomba* goomba) {
    mEnemies.emplace_back(goomba);
}

void Game::RemoveEnemy(Goomba* goomba) {
    std::vector<Goomba*>::iterator it;
    it = std::find(mEnemies.begin(), mEnemies.end(), goomba);
    
    if (it != mEnemies.end()) {
        mEnemies.erase(it);
    }
}

void Game::SetCameraPos(Vector2 position) {
    mCameraPos = position;
}
