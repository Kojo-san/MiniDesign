#include "TextureSimple.h"

TextureSimple::TextureSimple(const std::string& symbole)
    : symbole_(symbole) {}

std::string TextureSimple::appliquer(const Point&) const {
    return symbole_;
}
