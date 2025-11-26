#pragma once
#include <string>
#include <memory>

// ======== Patron Décorateur pour les textures ========

class ITexture {
public:
    virtual ~ITexture() = default;
    virtual std::string valeur() const = 0;   // ex: "", "o", "#", "o#"
};

// Texture "vide" -> représente le '.'
class TextureVide : public ITexture {
public:
    std::string valeur() const override { return ""; }
};

// Texture simple (un symbole, ex: "o" ou "#")
class TextureSimple : public ITexture {
public:
    explicit TextureSimple(const std::string& s) : symbole_(s) {}
    std::string valeur() const override { return symbole_; }
private:
    std::string symbole_;
};

// Texture libre (arbitraire, ex: "st", "l")
class TextureLibre : public ITexture {
public:
    explicit TextureLibre(const std::string& v) : valeur_(v) {}
    std::string valeur() const override { return valeur_; }
private:
    std::string valeur_;
};

// Décorateur de texture (pour combiner)
class TextureDecorateur : public ITexture {
public:
    explicit TextureDecorateur(std::unique_ptr<ITexture> interne)
        : interne_(std::move(interne)) {}
protected:
    std::unique_ptr<ITexture> interne_;
};

// Exemple de combiné : texture mixte (ajoute une autre texture)
class TextureMix : public TextureDecorateur {
public:
    TextureMix(std::unique_ptr<ITexture> interne,
               std::unique_ptr<ITexture> extra)
        : TextureDecorateur(std::move(interne)),
          extra_(std::move(extra)) {}

    std::string valeur() const override {
        return interne_->valeur() + extra_->valeur();
    }
private:
    std::unique_ptr<ITexture> extra_;
};

// Fabrique utilitaire pour créer la bonne texture en fonction d’une chaîne
std::unique_ptr<ITexture> creerTextureDepuisString(const std::string& txt);
