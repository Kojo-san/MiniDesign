#pragma once
#include "ITexture.h"
#include <string>

/// Texture contenant un symbole simple (o, #, $, etc.)
class TextureSimple : public ITexture {
public:
    explicit TextureSimple(const std::string& symbole);

    std::string appliquer(const Point& p) const override;

private:
    std::string symbole_;
};
