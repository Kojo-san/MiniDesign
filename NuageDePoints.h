#pragma once

#include "IElement.h"
#include "ISujet.h"
#include <string>
#include <vector>


class Point;
class IObservateur;

/// Composite récursif : un nuage peut contenir des Points et d'autres Nuages.
class NuageDePoints : public IElement, public ISujet {
public:
    explicit NuageDePoints(const std::string& id);
    ~NuageDePoints();

    /// Identifiant "logique" du nuage (ex. texture ou nom).
    const std::string& getId() const;

    // --- Composite ---
    void ajouter(IElement* element);
    void supprimer(int pointId);
    void deplacer(int pointId, const Point& nouveau);
    std::vector<IElement*> getElements() const;

    // --- Affichage logique ---
    void afficher() const override;

    // --- Observer (ISujet) ---
    void attacher(IObservateur* obs) override;
    void detacher(IObservateur* obs) override;
    void notifier() override;

private:
    std::string id_;
    std::vector<IElement*> elements_;        // Nuage possède les éléments
    std::vector<IObservateur*> observateurs_;

    Point* trouverPointParId(int pointId) const;

    friend class CmdFusion;
    friend class CmdDeplacer;
    friend class CmdSupprimer;

};


