#include "./TextManager.h"
#include "./FontManager.h"
#include "./Components/TextComponent.h"

extern EntityManager entityManager;

Entity& TextManager::DisplayText(std::string textID, int posX, int posY, std::string text, int fontSize, std::string fontFamily, SDL_Color fontColor) {
    Entity& textEntity = entityManager.AddEntity(LOSS_TEXT_ID, EntityType::UI, LayerType::UI);
    FontManager::ChangeFontSize(fontFamily.c_str(), fontSize);
    textEntity.AddComponent<TextComponent>(
        posX,
        posY,
        text,
        fontFamily,
        fontColor
     );
}