#include "Domaine.h"
#include <algorithm>
#include <iomanip>

using namespace std;


Point::Point(int id, int x, int y)
    : id_(id), x_(x), y_(y), texture_(make_unique<TextureVide>()) {}

std::string Point::symbolePourAffichage() const {
    std::string v = texture_->valeur();
    return v.empty() ? std::string(".") : v;
}

void Point::afficherListe(ostream& os, int indent) const {
    string tabs(indent, ' ');
    os << tabs << id_ << ": (" << x_ << "," << y_ << ")  textures: '"
       << texture_->valeur() << "'\n";
}


NuageDePoints::NuageDePoints(const string& nom)
    : nom_(nom) {}

void NuageDePoints::ajouter(unique_ptr<IElement> el) {
    elements_.push_back(std::move(el));
}

Point* NuageDePoints::trouverPointParId(int id) {
    for (auto& e : elements_) {
        if (auto* p = dynamic_cast<Point*>(e.get()))
            if (p->getId() == id) return p;

        if (auto* n = dynamic_cast<NuageDePoints*>(e.get()))
            if (auto* p2 = n->trouverPointParId(id)) return p2;
    }
    return nullptr;
}

const Point* NuageDePoints::trouverPointParId(int id) const {
    return const_cast<NuageDePoints*>(this)->trouverPointParId(id);
}

void NuageDePoints::afficherListe(ostream& os, int indent) const {
    string tabs(indent, ' ');
    os << tabs << "Liste:\n";
    for (const auto& e : elements_)
        e->afficherListe(os, indent);

    for (const auto& e : elements_) {
        if (auto* n = dynamic_cast<NuageDePoints*>(e.get())) {
            vector<Point*> pts;
            n->collecterPoints(pts);

            os << tabs << "Nuage '" << n->nom() << "' contient les points: ";
            for (size_t i = 0; i < pts.size(); ++i) {
                os << pts[i]->getId();
                if (i + 1 < pts.size()) os << ", ";
            }
            os << "\n";
        }
    }
}

void NuageDePoints::collecterPoints(vector<Point*>& res) {
    for (auto& e : elements_)
        e->collecterPoints(res);
}

void NuageDePoints::attacher(IObservateur* obs) {
    observateurs_.push_back(obs);
}

void NuageDePoints::detacher(IObservateur* obs) {
    observateurs_.erase(
        remove(observateurs_.begin(), observateurs_.end(), obs),
        observateurs_.end());
}

void NuageDePoints::notifier() {
    for (auto* o : observateurs_)
        o->actualiser();
}


Afficheur::Afficheur(NuageDePoints& nuage)
    : nuage_(nuage) {
    nuage_.attacher(this);
}

void Afficheur::afficher() {
    vector<Point*> pts;
    nuage_.collecterPoints(pts);

    vector<Pixel> pixels;
    for (auto* p : pts)
        pixels.push_back({ p->x(), p->y(), symbolePourPoint(*p) });

    vector<Segment> segs; 

    afficherEntete(cout);
    dessinerGrille(pixels, segs);
}

std::string AfficheurO1::symbolePourPoint(const Point& p) const {
    return p.symbolePourAffichage();
}

void AfficheurO1::afficherEntete(ostream& os) const {
    os << "\nAffichage textures des points :\n";
}

std::string AfficheurO2::symbolePourPoint(const Point& p) const {
    int id = p.getId();
    if (id >= 0 && id <= 9) return std::string(1, char('0' + id));
    return std::string("*");
}

void AfficheurO2::afficherEntete(ostream& os) const {
    os << "\nAffichage IDs des points :\n";
}
