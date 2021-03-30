#include <iostream>
#include "./Game.h"
#include "./TextManager.h"
#include "./InputManager.h"
#include "./AssetManager.h"
#include "./Map.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/InputComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Components/TextComponent.h"
#include "./Components/ProjectileComponent.h"
#include "../lib/glm/glm.hpp"
#include "./Constants.h"
#include "./Customs.h"

InputManager inputManager;
EntityManager entityManager;
bool Game::isRespawning;
bool Game::isLevelComplete;
bool Game::isRunning;
bool Game::debugMode;
SDL_Renderer* Game::renderer;
AssetManager* Game::assetManager = new AssetManager(&entityManager); 
SDL_Event Game::sdlEvent;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Entity* player = NULL;
Map* map;

Game::Game() {
    if(!isRunning) {        
        this->isRunning = false;
        this->debugMode = false;
    }
    if(!isLevelComplete) {
        this->isLevelComplete = false;
    }
    if(!isRespawning) {
        this->isRespawning = false;
    }
}

Game::~Game() {
}

void Game::GameWon() {
    if(this->isLevelComplete)
        return;

    this->isLevelComplete = true;

    if(entityManager.GetEntityByName(WIN_TEXT_ID) != nullptr) {
        std::cout << "win text already created \n";
        return;
    }

    TextManager::DisplayText(WIN_TEXT_ID, static_cast<int>(WINDOW_WIDTH / 2), static_cast<int>(WINDOW_HEIGHT / 2), missionCompleteText, 500, missionCompleteFontFamily, missionCompleteColor);
}

void Game::GameOver() {   
    this->isRespawning = true;
    player->SetActive(false);

    if(this->isLevelComplete)
        return;

    if(entityManager.GetEntityByName(LOSS_TEXT_ID) != nullptr) {
        std::cout << "loss text already created \n";
        return;
    }

    TextManager::DisplayText(LOSS_TEXT_ID, static_cast<int>(WINDOW_WIDTH / 2), static_cast<int>(WINDOW_HEIGHT / 2), gameOverText, 500, gameOverFontFamily, gameOverColor);
}

void Game::RespawnPlayer() {    
    if(!player->IsActive()) {
        entityManager.DestroyEntity(LOSS_TEXT_ID);

        player->SetActive(true);
        TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
        MovementComponent* playerMovement = player->GetComponent<MovementComponent>();
        playerTransform->position = playerTransform->spawnPos; 
        playerMovement->Reset();

        this->isRespawning = false;       
    }
}

bool Game::IsRunning() const {
    return this->isRunning;
}

void Game::Init(int width, int height) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initialising SDL" << std::endl;
        return;
    }
    if(TTF_Init() != 0) {
        std::cerr << "Error initialising TTF" << std::endl;
        return;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );
    if(!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    LoadLevel(1);

    isRunning = true;
    return;
}   

void Game::LoadLevel(int levelID) {
    /*****************/
    /* open lua file */
    /*****************/
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string levelName = "Level" + std::to_string(levelID);
    lua.script_file("./assets/scripts/" + levelName + ".lua");

    sol::table levelData = lua[levelName];

    /*****************************/
    /* LOAD config from lua file*/
    /*****************************/

    sol::table config = lua["config"];

    sol::table missionComplete = config["missionCompleteMessage"];

    missionCompleteText = missionComplete["text"];
    missionCompleteFontFamily = missionComplete["family"];
    missionCompleteColor = {
        static_cast<int>(missionComplete["color"]["r"]),
        static_cast<int>(missionComplete["color"]["g"]),
        static_cast<int>(missionComplete["color"]["b"]),
        static_cast<int>(missionComplete["color"]["a"])
    };

    sol::table gameOver = config["gameOverMessage"];

    gameOverText = gameOver["text"];
    gameOverFontFamily = gameOver["family"];
    gameOverColor = {
        static_cast<int>(gameOver["color"]["r"]),
        static_cast<int>(gameOver["color"]["g"]),
        static_cast<int>(gameOver["color"]["b"]),
        static_cast<int>(gameOver["color"]["a"])
    };

    /*****************************/
    /* LOAD assets from lua file */
    /*****************************/
    sol::table levelAssets = levelData["assets"];

    unsigned int assetIndex = 0;
    while(true) {
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
        if(existsAssetIndexNode == sol::nullopt) {
            break;
        } else {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];
            if(assetType.compare("texture") == 0) {
                std::string assetID = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->AddTexture(assetID, assetFile.c_str());
            } else if(assetType.compare("font") == 0) {
                std::string assetID = asset["id"];
                std::string assetFile = asset["file"];
                int fontSize = asset["fontSize"];
                assetManager->AddFont(assetID, assetFile.c_str(), fontSize);
            }
        }
        assetIndex++;
    }

    /**************************/
    /* LOAD map from lua file */
    /**************************/
    sol::table levelMap = levelData["map"];
    std::string mapTextureID = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    map = new Map(
        mapTextureID,
        static_cast<int>(levelMap["scale"]),
        static_cast<int>(levelMap["tileSize"])
    );

    map->LoadMap(
        mapFile,
        static_cast<int>(levelMap["mapSizeX"]),
        static_cast<int>(levelMap["mapSizeY"])
    );
    
    /*******************************/
    /* LOAD entities from lua file */
    /*******************************/
    sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;

    while(true) {
        sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
        if(existsEntityIndexNode == sol::nullopt) {
            break;
        } else {
            sol::table entity = levelEntities[entityIndex];
            std::string entityName = entity["name"];
            EntityType entityType = static_cast<EntityType>(static_cast<int>(entity["type"]));
            LayerType entityLayer = static_cast<LayerType>(static_cast<int>(entity["layer"]));

            // add new entity
            auto& newEntity = entityManager.AddEntity(entityName, entityType, entityLayer);
            
            // add transform component
            sol::optional<sol::table> existsTransformComponent = entity["components"]["transform"];
            if(existsTransformComponent != sol::nullopt) {
                newEntity.AddComponent<TransformComponent>(
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["velocity"]["y"]),
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"]),
                    static_cast<int>(entity["components"]["transform"]["scale"])
                );
            }

            // add sprite component
            sol::optional<sol::table> existsSpriteComponent = entity["components"]["sprite"];
            if(existsSpriteComponent != sol::nullopt) {
                std::string textureId = entity["components"]["sprite"]["textureAssetId"];
                bool isAnimated = entity["components"]["sprite"]["animated"];
                if(isAnimated) {
                    newEntity.AddComponent<SpriteComponent>(
                        textureId,
                        static_cast<int>(entity["components"]["sprite"]["frameCount"]),
                        static_cast<int>(entity["components"]["sprite"]["animationSpeed"]),
                        static_cast<bool>(entity["components"]["sprite"]["hasDirections"]),
                        static_cast<bool>(entity["components"]["sprite"]["fixed"])
                    );
                } else {
                    newEntity.AddComponent<SpriteComponent>(textureId);
                }
            }             

            // add movement component
            sol::optional<sol::table> existsMovementComponent = entity["components"]["movement"];
            if(existsMovementComponent != sol::nullopt) {
                newEntity.AddComponent<MovementComponent>(
                    static_cast<int>(entity["components"]["movement"]["speed"])
                );
            }   

            // add input component
            sol::optional<sol::table> existsInputComponent = entity["components"]["input"];
            if(existsInputComponent != sol::nullopt) {
                newEntity.AddComponent<InputComponent>();
            }                    

            // add collider component
            sol::optional<sol::table> existsColliderComponent = entity["components"]["collider"];
            if(existsColliderComponent != sol::nullopt) {
                newEntity.AddComponent<ColliderComponent>(
                    entity["components"]["collider"]["tag"],
                    static_cast<int>(entity["components"]["transform"]["position"]["x"]),
                    static_cast<int>(entity["components"]["transform"]["position"]["y"]),  
                    static_cast<int>(entity["components"]["transform"]["width"]),
                    static_cast<int>(entity["components"]["transform"]["height"])                  
                );
            }

            // add projectile component
            sol::optional<sol::table> existsProjectileEmitterComponent = entity["components"]["projectile"];
            if (existsProjectileEmitterComponent != sol::nullopt) {
                int parentEntityXPos = entity["components"]["transform"]["position"]["x"];
                int parentEntityYPos = entity["components"]["transform"]["position"]["y"];
                int parentEntityWidth = entity["components"]["transform"]["width"];
                int parentEntityHeight = entity["components"]["transform"]["height"];
                int projectileWidth = entity["components"]["projectile"]["width"];
                int projectileHeight = entity["components"]["projectile"]["height"];
                int projectileSpeed = entity["components"]["projectile"]["speed"];
                int projectileRange = entity["components"]["projectile"]["range"];
                int projectileAngle = entity["components"]["projectile"]["angle"];
                bool projectileShouldLoop = entity["components"]["projectile"]["shouldLoop"];
                std::string textureAssetId = entity["components"]["projectile"]["textureAssetId"];
                Entity& projectile(entityManager.AddEntity("projectile", EntityType::FLYING, LayerType::PROJECTILE));
                projectile.AddComponent<TransformComponent>(
                    parentEntityXPos + (parentEntityWidth / 2),
                    parentEntityYPos + (parentEntityHeight / 2),
                    0,
                    0,
                    projectileWidth,
                    projectileHeight,
                    1
                );
                projectile.AddComponent<SpriteComponent>(textureAssetId);
                projectile.AddComponent<ProjectileComponent>(
                    &newEntity,
                    projectileSpeed,
                    projectileAngle,
                    projectileRange,
                    projectileShouldLoop
                );
                projectile.AddComponent<ColliderComponent>(
                    "PROJECTILE",
                    parentEntityXPos,
                    parentEntityYPos,
                    projectileWidth,
                    projectileHeight
                );
            }

            // add text component
            sol::optional<sol::table> existsTextComponent = entity["components"]["text"];
            if(existsTextComponent != sol::nullopt) {
                SDL_Color textColor = {
                    static_cast<int>(entity["components"]["text"]["color"]["r"]),
                    static_cast<int>(entity["components"]["text"]["color"]["g"]),
                    static_cast<int>(entity["components"]["text"]["color"]["b"]),
                    static_cast<int>(entity["components"]["text"]["color"]["a"])
                };
                newEntity.AddComponent<TextComponent>(
                    static_cast<int>(entity["components"]["text"]["x"]),
                    static_cast<int>(entity["components"]["text"]["y"]),
                    entity["components"]["text"]["text"],
                    entity["components"]["text"]["family"],
                    textColor
                );                
            }
        }
        entityIndex++;
    }

    SetPlayer();
}

void Game::SetPlayer() {
    std::string playerName = "player";
    entityManager.SetPlayer(playerName);
    player = entityManager.GetPlayer();
    player->SetHeight(Height::GROUND);
    SpriteComponent* playerSprite = player->GetComponent<SpriteComponent>();
    playerSprite->ToggleAnimation();
}

void Game::ProcessInput() {
    SDL_PollEvent(&sdlEvent);
    switch(sdlEvent.type) {
        case SDL_QUIT: {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            switch(sdlEvent.key.keysym.sym) {
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                case DEBUG_MODE:
                    if(!inputManager.CheckKeyPressed(DEBUG_MODE)) {
                        debugMode = !debugMode;
                    }
                    break;
                case RESPAWN:
                    if(isRespawning) {
                        RespawnPlayer();
                    }
                default:
                    break;
            }
            break;
        }
        case SDL_KEYUP: {
            switch(sdlEvent.key.keysym.sym) {
                case SDLK_ESCAPE:
                    isRunning = false;
                    break;
                case DEBUG_MODE:
                    inputManager.SetKeyReleased(DEBUG_MODE);
                    break;
                default:
                    break;
            }
            break;            
        }
        default: {
            break;
        }
    }
}

void Game::Update() {
    // deltatime is the diff in ticks from last frame converted to secs
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.f;
    // clamp deltatime
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    ticksLastFrame = SDL_GetTicks();

    // call manager.update to update all entities
    entityManager.Update(deltaTime);

    HandleCameraMovement();
    CheckCollisions(); 
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // if there are no entities, return
    if(entityManager.HasNoEntities())
        return;
    // call manager.render to render all entities
    entityManager.Render();

    SDL_RenderPresent(renderer);
}

void Game::Terminate() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::HandleCameraMovement() {
    if(!player)
        return;
    TransformComponent* playerTransform = player->GetComponent<TransformComponent>();
    camera.x = playerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = playerTransform->position.y - (WINDOW_HEIGHT / 2);
    
    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
    entityManager.CheckCollisions();
}