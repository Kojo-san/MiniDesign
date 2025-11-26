#pragma once
#include "ITexture.h"

/// Texture par défaut : équivalente à "."
class TextureVide : public ITexture {
public:
    std::string appliquer(const Point& p) const override;
};
