#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "SDL2/SDL.h"
#include "math.h"
#include "spritecomponent.h"

class Game {

public:
    Game();
    bool Initialize(void);
    void Shutdown(void);
    void Runloop(void);
    void ProcessUpdate(void);
    void UpdateGame(void);
    void GenerateOutput(void);
    void LoadData(void);
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    std::vector<class Actor*> GetActors() { return mActors; }
    void AddSprite(SpriteComponent* sprite);
    void RemoveSprite(SpriteComponent* sprite);
    class Player* GetPlayer() { return mPlayer; }
    SDL_Texture* GetTexture(const char* fileName);
    SDL_Point GetWindowSize() { return mWindowSize; }
    Vector2 GetCameraPosition() { return mCameraPos; }
    void SetCameraPosition(Vector2 position) { mCameraPos = position; }

private:
    void UnloadData(void);

protected:
    Vector2 mCameraPos;
    bool mRunning;
    int mPreviousTime;
    SDL_Point mWindowSize;
    Vector2 mWindowSizeVector;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    std::vector<class Actor*> mActors;
    std::unordered_map<std::string, SDL_Texture*> mTextureCache;
    std::vector<SpriteComponent*> mSprites;
    class Player* mPlayer;
};