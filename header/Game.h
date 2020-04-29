#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Block.h"
#include "Goomba.h"
#include "Player.h"
#include "Math.h"

class Game {
    
public:
    Game();
    bool Initialize(void);
    void Shutdown(void);
    void Runloop(void);
    void ProcessUpdate(void);
    void UpdateGame(void);
    void GenerateOutput(void);
    
    int GetPlayChannel() const { return mPlayChannel; }

    SDL_Point GetWindowSize(){ return mWindowSize; };
    
    void AddActor(Actor* actor);
    void RemoveActor(Actor* actor);
    
    SDL_Texture* GetTexture(const char* fileName);
    Mix_Chunk* GetSound(const char* fileName);
    
    void AddSprite(SpriteComponent* sprite);
    void RemoveSprite(SpriteComponent* sprite);
    
    void AddBlock(Block* block);
    void RemoveBlock(Block* block);
    std::vector<Block*> GetBlocks() { return mBlocks; }
    
    void AddEnemy(Goomba* goomba);
    void RemoveEnemy(Goomba* goomba);
    std::vector<Goomba*> GetEnemies() { return mEnemies; }
    
    Player* GetPlayer() { return mPlayer; }
    
    Vector2 GetCameraPos() { return mCameraPos; }
    void SetCameraPos(Vector2 position);
    
private:
    void LoadData(void);
    void UnloadData(void);
    
protected:
    bool mRunning;
    int mPreviousTime;
    int mPlayChannel;
    SDL_Point mWindowSize;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    std::vector<Actor*> mActors;
    std::unordered_map<std::string, SDL_Texture*> mTextureCache;
    std::unordered_map<std::string, Mix_Chunk*> mSoundCache;
    std::vector<SpriteComponent*> mSprites;
    std::vector<Block*> mBlocks;
    std::vector<Goomba*> mEnemies;
    Player* mPlayer;
    Vector2 mCameraPos;
    Game* game;
};
