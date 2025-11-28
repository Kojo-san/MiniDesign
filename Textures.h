#pragma once
#include <string>
#include <memory>


class ITexture {
public:
    virtual ~ITexture() = default;
    virtual std::string valeur() const = 0;   
};

class TextureVide : public ITexture {
public:
    std::string valeur() const override { return ""; }
};


class TextureSimple : public ITexture {
public:
    explicit TextureSimple(const std::string& s) : symbole_(s) {}
    std::string valeur() const override { return symbole_; }
private:
    std::string symbole_;
};

class TextureLibre : public ITexture {
public:
    explicit TextureLibre(const std::string& v) : valeur_(v) {}
    std::string valeur() const override { return valeur_; }
private:
    std::string valeur_;
};

class TextureDecorateur : public ITexture {
public:
    explicit TextureDecorateur(std::unique_ptr<ITexture> interne)
        : interne_(std::move(interne)) {}
protected:
    std::unique_ptr<ITexture> interne_;
};


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

std::unique_ptr<ITexture> creerTextureDepuisString(const std::string& txt);
