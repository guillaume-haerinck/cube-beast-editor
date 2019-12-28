#include "textures.h"

#include "graphics/render-command.h"
#include "gui/icons.h"

void Textures::init(RenderCommand& rcommand) {
    m_txts.at(static_cast<unsigned int>(TextureIndex::UI)) = rcommand.createTexture(0, iconsData::eraser, iconsData::eraser_size);
}

void Textures::destroy(RenderCommand& rcommand) {
    for (auto& texture : m_txts) {
        rcommand.deleteTexture(texture);
    }
}
