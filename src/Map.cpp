#include <fstream>
#include "./Game.h"
#include "./Map.h"
#include "./EntityManager.h"
#include "./Components/TileComponent.h"

extern EntityManager entityManager;

Map::Map(std::string textureID, int scale, int tileSize) {
    this->textureID = textureID;
    this->scale = scale;
    this->tileSize = tileSize;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY) {
    std::fstream mapFile;
    mapFile.open(filePath);

    for(int y = 0; y < mapSizeY; y++) {
        for(int x = 0; x < mapSizeX; x++) {
            char ch;
            mapFile.get(ch);
            int sourceRectY = atoi(&ch) * tileSize;
            mapFile.get(ch);
            int sourceRectX = atoi(&ch) * tileSize;
            Addtile(sourceRectX, sourceRectY, x * (scale * tileSize), y * (scale * tileSize));
            mapFile.ignore();
        }
    }
    mapFile.close();
}

void Map::Addtile(int sourceRectX, int sourceRectY, int posX, int posY) {
    Entity& tile(entityManager.AddEntity("Tile", EntityType::GROUND, tileLayer));
    tile.AddComponent<TileComponent>(sourceRectX, sourceRectY, posX, posY, tileSize, scale, textureID);
}