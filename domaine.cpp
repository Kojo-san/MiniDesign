#include "domaine.h"
#include "textures.h"
#include "observer.h"
#include <iostream>
#include <algorithm>

// ----- Point -----
Point::Point(int id, int x, int y, std::shared_ptr<ITexture> texture)
    : id_(id), x_(x), y_(y), texture_(std::move(texture)) {}

int Point::getId() const { return id_; }

void Point::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

std::shared_ptr<ITexture> Point::getTexture() const { return texture_; }

void Point::afficher() const {
    std::cout << id_ << ": (" << x_ << "," << y_ << ")\n";
}

// ----- NuageDePoints -----
NuageDePoints::NuageDePoints(const std::string& id) : id_(id) {}

int NuageDePoints::getId() const {
    // si on veut un ID numérique pour ce nuage, on peut dériver de id_ ou retourner -1
    return -1;
}

void NuageDePoints::ajouter(std::shared_ptr<IElement> element) {
    elements_.push_back(std::move(element));
    notifier();
}

void NuageDePoints::supprimer(int id) {
    auto it = std::remove_if(elements_.begin(), elements_.end(),
        [id](const std::shared_ptr<IElement>& e) { return e->getId() == id; });
    if (it != elements_.end()) {
        elements_.erase(it, elements_.end());
        notifier();
    }
}

void NuageDePoints::deplacer(int id, int nx, int ny) {
    for (auto& e : elements_) {
        auto p = std::dynamic_pointer_cast<Point>(e);
        if (p && p->getId() == id) {
            p->setPosition(nx, ny);
            notifier();
            return;
        }
    }
}

std::shared_ptr<IElement> NuageDePoints::trouver(int id) const {
    for (auto& e : elements_) {
        if (e->getId() == id)
            return e;
    }
    return nullptr;
}

const std::vector<std::shared_ptr<IElement>>& NuageDePoints::getElements() const {
    return elements_;
}

void NuageDePoints::afficher() const {
    for (auto& e : elements_) {
        e->afficher();
    }
}

void NuageDePoints::attacher(IObservateur* obs) {
    observateurs_.push_back(obs);
}

void NuageDePoints::detacher(IObservateur* obs) {
    auto it = std::remove(observateurs_.begin(), observateurs_.end(), obs);
    observateurs_.erase(it, observateurs_.end());
}

void NuageDePoints::notifier() {
    for (auto* obs : observateurs_)
        obs->actualiser();
}

// ----- Surface -----
void Surface::ajouterPoint(const Point& p) {
    points_.push_back(p);
}
