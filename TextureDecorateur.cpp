#include "TextureDecorateur.h"

TextureDecorateur::TextureDecorateur(ITexture* interne)
    : interne_(interne) {}

TextureDecorateur::~TextureDecorateur() {
    delete interne_;
}
