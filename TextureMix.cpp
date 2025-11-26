#include "TextureMix.h"
#include "ITexture.h"

TextureMix::TextureMix(ITexture* base, ITexture* extra)
    : TextureDecorateur(base), extra_(extra) {}

std::string TextureMix::appliquer(const Point& p) const {
    return interne_->appliquer(p) + extra_->appliquer(p);
}
