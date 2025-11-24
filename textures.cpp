#include "textures.h"
#include "domaine.h"

TextureSimple::TextureSimple(std::string s) : symbole_(std::move(s)) {}
TextureLibre::TextureLibre(std::string v) : valeur_(std::move(v)) {}

std::string TextureVide::appliquer(const Point&) const {
    return " ";   // sera affiché comme '.' dans l'afficheur
}

std::string TextureSimple::appliquer(const Point&) const {
    return symbole_;
}

std::string TextureLibre::appliquer(const Point&) const {
    return valeur_;
}

TextureDecorateur::TextureDecorateur(std::shared_ptr<ITexture> interne)
    : interne_(std::move(interne)) {}

TextureMix::TextureMix(std::shared_ptr<ITexture> base, std::shared_ptr<ITexture> extra)
    : TextureDecorateur(std::move(base)), extra_(std::move(extra)) {}

std::string TextureMix::appliquer(const Point& p) const {
    return interne_->appliquer(p) + extra_->appliquer(p);
}
