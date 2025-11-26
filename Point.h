#pragma once

#include "IElement.h"

class ITexture;   // sera défini plus tard (patron Décorateur)

/// Point élémentaire de l'orthèse.
class Point : public IElement {
public:
    Point(int id, int x, int y, ITexture* texture = nullptr);

    int  getId() const;
    int  getX() const;
    int  getY() const;
    void setPosition(int x, int y);

    ITexture* getTexture() const;
    void      setTexture(ITexture* texture);

    /// Affichage logique (pour debug / tests).
    void afficher() const override;

private:
    int id_;
    int x_;
    int y_;
    ITexture* texture_;  // non possédé; géré ailleurs (Décorateur)
};
