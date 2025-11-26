#pragma once

/// Interface de base du composite.
/// Représente un élément de l'orthèse (point ou nuage).
class IElement {
public:
    virtual ~IElement() = default;

    /// Affichage "logique" de l’élément (sera utilisé par les afficheurs).
    virtual void afficher() const = 0;
};
