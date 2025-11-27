#pragma once
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "affichage.h"
#include "Textures.h"

class IObservateur;

// ======== IElement (Composite) ========

class IElement {
public:
    virtual ~IElement() = default;
    virtual void afficherListe(std::ostream& os, int indent = 0) const = 0;
    virtual void collecterPoints(std::vector<class Point*>& res) = 0;
    virtual int getId() const = 0;
};

// ======== Point ========

class Point : public IElement {
public:
    Point(int id, int x, int y);

    int getId() const override { return id_; }
    int x() const { return x_; }
    int y() const { return y_; }

    void setPos(int nx, int ny) { x_ = nx; y_ = ny; }

    void setTexture(std::unique_ptr<ITexture> t) { texture_ = std::move(t); }
    const ITexture& texture() const { return *texture_; }

    // Retourne la chaîne complète pour affichage (ex: "", "o", "#", "o#")
    std::string symbolePourAffichage() const;

    void afficherListe(std::ostream& os, int indent = 0) const override;
    void collecterPoints(std::vector<Point*>& res) override { res.push_back(this); }

private:
    int id_;
    int x_;
    int y_;
    std::unique_ptr<ITexture> texture_;
};

// ======== Observer ========

class ISujet {
public:
    virtual ~ISujet() = default;
    virtual void attacher(IObservateur* obs) = 0;
    virtual void detacher(IObservateur* obs) = 0;
    virtual void notifier() = 0;
};

class IObservateur {
public:
    virtual ~IObservateur() = default;
    virtual void actualiser() = 0;
};

// ======== NuageDePoints (Composite + Sujet) ========

class NuageDePoints : public IElement, public ISujet {
public:
    explicit NuageDePoints(const std::string& nom = "principal");

    int getId() const override { return -1; }
    const std::string& nom() const { return nom_; }

    void ajouter(std::unique_ptr<IElement> el);

    Point* trouverPointParId(int id);
    const Point* trouverPointParId(int id) const;

    void afficherListe(std::ostream& os, int indent = 0) const override;
    void collecterPoints(std::vector<Point*>& res) override;

    // Observer
    void attacher(IObservateur* obs) override;
    void detacher(IObservateur* obs) override;
    void notifier() override;

    // accès sécurisé
    auto& elements() { return elements_; }
    const auto& elements() const { return elements_; }

private:
    std::string nom_;
    std::vector<std::unique_ptr<IElement>> elements_;
    std::vector<IObservateur*> observateurs_;
};

// ======== Afficheurs ========

class Afficheur : public IObservateur {
public:
    explicit Afficheur(NuageDePoints& nuage);
    virtual ~Afficheur() = default;

    void afficher();
    void actualiser() override {}

protected:
    virtual std::string symbolePourPoint(const Point& p) const = 0;
    virtual void afficherEntete(std::ostream& os) const = 0;

private:
    NuageDePoints& nuage_;
};

class AfficheurO1 : public Afficheur {
public:
    using Afficheur::Afficheur;
protected:
    std::string symbolePourPoint(const Point& p) const override;
    void afficherEntete(std::ostream& os) const override;
};

class AfficheurO2 : public Afficheur {
public:
    using Afficheur::Afficheur;
protected:
    std::string symbolePourPoint(const Point& p) const override;
    void afficherEntete(std::ostream& os) const override;
};
