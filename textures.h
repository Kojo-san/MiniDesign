#pragma once
#include <string>
#include <memory>

class Point;

class ITexture {
public:
    virtual ~ITexture() = default;
    virtual std::string appliquer(const Point& p) const = 0;
};

class TextureVide : public ITexture {
public:
    std::string appliquer(const Point& p) const override;
};

class TextureSimple : public ITexture {
public:
    explicit TextureSimple(std::string symbole);
    std::string appliquer(const Point& p) const override;
private:
    std::string symbole_;
};

class TextureLibre : public ITexture {
public:
    explicit TextureLibre(std::string valeur);
    std::string appliquer(const Point& p) const override;
private:
    std::string valeur_;
};

class TextureDecorateur : public ITexture {
public:
    explicit TextureDecorateur(std::shared_ptr<ITexture> interne);
protected:
    std::shared_ptr<ITexture> interne_;
};

class TextureMix : public TextureDecorateur {
public:
    TextureMix(std::shared_ptr<ITexture> base, std::shared_ptr<ITexture> extra);
    std::string appliquer(const Point& p) const override;
private:
    std::shared_ptr<ITexture> extra_;
};
