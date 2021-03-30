#pragma once

#include <string>

class Map {
    private:
        std::string textureID;
        int scale;
        int tileSize;
        LayerType tileLayer = LayerType::TILE;
    public:
        Map(std::string textureID, int scale, int tileSize);
        ~Map();
        void LoadMap(std::string filePath, int mapSizeX, int mapSizeY);
        void Addtile(int sourceX, int sourceY, int posX, int posY);
};