#pragma once
#include "ITexture.h"
#include <string>

/// Texture totalement libre (ex: "st", "++", "ID", etc.)
class TextureLibre : public ITexture {
public:
    explicit TextureLibre(const std::string& val);

    std::string appliquer(const Point& p) const override;

private:
    std::string valeur_;
};
