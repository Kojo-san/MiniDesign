#pragma once
#include "TextureDecorateur.h"

/// Combine deux textures librement :
/// Exemple : base="o" extra="#" → "o#"
class TextureMix : public TextureDecorateur {
public:
    TextureMix(ITexture* base, ITexture* extra);

    std::string appliquer(const Point& p) const override;

private:
    ITexture* extra_;
};
