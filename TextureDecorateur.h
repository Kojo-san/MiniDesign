#pragma once
#include "ITexture.h"

/// Classe abstraite décorateur
class TextureDecorateur : public ITexture {
public:
    explicit TextureDecorateur(ITexture* interne);
    virtual ~TextureDecorateur();

protected:
    ITexture* interne_;   // décoré
};
