#include "NuageDePoints.h"
#include "Point.h"
#include "IObservateur.h"
#include <algorithm>
#include <iostream>

NuageDePoints::NuageDePoints(const std::string& id)
    : id_(id) {}

NuageDePoints::~NuageDePoints() {
    // Le nuage est propriétaire de ses éléments.
    for (IElement* e : elements_) {
        delete e;
    }
}

const std::string& NuageDePoints::getId() const {
    return id_;
}

void NuageDePoints::ajouter(IElement* element) {
    if (element) {
        elements_.push_back(element);
        notifier();
    }
}

void NuageDePoints::supprimer(int pointId) {
    auto it = std::find_if(elements_.begin(), elements_.end(),
        [pointId](IElement* elem) {
            Point* p = dynamic_cast<Point*>(elem);
            return p && p->getId() == pointId;
        });

    if (it != elements_.end()) {
        delete *it;
        elements_.erase(it);
        notifier();
    }
}

void NuageDePoints::deplacer(int pointId, const Point& nouveau) {
    Point* p = trouverPointParId(pointId);
    if (p) {
        p->setPosition(nouveau.getX(), nouveau.getY());
        notifier();
    }
}

std::vector<IElement*> NuageDePoints::getElements() const {
    return elements_;
}

void NuageDePoints::afficher() const {
    std::cout << "Nuage \"" << id_ << "\" contenant " << elements_.size()
              << " élément(s)\n";
    for (IElement* e : elements_) {
        if (e) e->afficher();
    }
}

// ===================== Observer (ISujet) =====================

void NuageDePoints::attacher(IObservateur* obs) {
    if (!obs) return;
    if (std::find(observateurs_.begin(), observateurs_.end(), obs)
        == observateurs_.end()) {
        observateurs_.push_back(obs);
    }
}

void NuageDePoints::detacher(IObservateur* obs) {
    auto it = std::remove(observateurs_.begin(), observateurs_.end(), obs);
    observateurs_.erase(it, observateurs_.end());
}

void NuageDePoints::notifier() {
    for (IObservateur* obs : observateurs_) {
        if (obs) obs->actualiser();
    }
}

// ===================== Helpers =====================

Point* NuageDePoints::trouverPointParId(int pointId) const {
    for (IElement* elem : elements_) {
        Point* p = dynamic_cast<Point*>(elem);
        if (p && p->getId() == pointId) {
            return p;
        }
        // Si un élément est lui-même un Nuage, on pourrait y descendre
        NuageDePoints* sousNuage = dynamic_cast<NuageDePoints*>(elem);
        if (sousNuage) {
            Point* res = sousNuage->trouverPointParId(pointId);
            if (res) return res;
        }
    }
    return nullptr;
}
