#pragma once
#include <vector>

class NuageDePoints;
class Point;
class ITexture;

class IObservateur {
public:
    virtual ~IObservateur() = default;
    virtual void actualiser() = 0;
};

class ISujet {
public:
    virtual ~ISujet() = default;
    virtual void attacher(IObservateur*) = 0;
    virtual void detacher(IObservateur*) = 0;
    virtual void notifier() = 0;
};

// ----- Afficheur (Template Method) -----
class Afficheur {
public:
    virtual ~Afficheur() = default;
    void afficher();
protected:
    virtual void preparer();
    virtual void afficherContenu() = 0;
    virtual void terminer();
};

// O1 : textures
class AfficheurO1 : public Afficheur, public IObservateur {
public:
    AfficheurO1(NuageDePoints& nuage);
    void actualiser() override;
protected:
    void afficherContenu() override;
private:
    NuageDePoints& nuage_;
};

// O2 : IDs
class AfficheurO2 : public Afficheur, public IObservateur {
public:
    AfficheurO2(NuageDePoints& nuage);
    void actualiser() override;
protected:
    void afficherContenu() override;
private:
    NuageDePoints& nuage_;
};
