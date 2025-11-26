#pragma once
#include <string>

class Point;

/// Interface du patron Décorateur.
class ITexture {
public:
    virtual ~ITexture() = default;

    /// Retourne le caractère (ou la chaîne) à afficher pour un point.
    virtual std::string appliquer(const Point& p) const = 0;
};
