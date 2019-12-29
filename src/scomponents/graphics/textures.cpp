#include "textures.h"

#include "graphics/render-command.h"
#include "gui/icons.h"

void Textures::init(RenderCommand& rcommand) {
}

void Textures::destroy(RenderCommand& rcommand) {
    for (auto& texture : m_txts) {
        rcommand.deleteTexture(texture);
    }
}
