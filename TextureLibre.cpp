#include "TextureLibre.h"

TextureLibre::TextureLibre(const std::string& val)
    : valeur_(val) {}

std::string TextureLibre::appliquer(const Point&) const {
    return valeur_;
}
