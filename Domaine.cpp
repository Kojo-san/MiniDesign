#include "Domaine.h"
#include <algorithm>
#include <iomanip>

using namespace std;

int NuageDePoints::prochainId_ = 4;

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


NuageDePoints::NuageDePoints(const string& texture)
    : texture_(texture), id_(prochainId_++) {}

void NuageDePoints::ajouter(unique_ptr<IElement> el) {
    elements_.push_back(std::move(el));
    notifier();
}

IElement* NuageDePoints::trouverElementParId(int id){

    if(id_ == id){
        return this;
    }

    for(auto& e : elements_){

        if(e->getId()== id){
            return e.get();
        }

        if(auto* n = dynamic_cast<NuageDePoints*>(e.get())){

            if(auto* elem = n->trouverElementParId(id)){
                return elem;
            }        
        }
    }
    return nullptr;
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

    for (const auto& e : elements_){

        if(auto* p = dynamic_cast<Point*>(e.get())){
            os << tabs << p->getId() << ": (" << p->x() << "," << p->y() << ")  textures: '"
               << p->texture().valeur() << "'\n";
        }
        else if (auto* n = dynamic_cast<NuageDePoints*>(e.get())) {
            os << tabs << n->getId() << ": Nuage '" << n->texture() << "' contient les elements: ";

            bool premier = true;
            for (const auto& sousElem : n->elements()) {
                if (!premier) os << ", ";
                premier = false;
                os << sousElem->getId();
            }
            os << "\n";
        }
    }
}

void NuageDePoints::collecterPoints(vector<Point*>& res) {
    for (auto& e : elements_)

        if (auto* p = dynamic_cast<Point*>(e.get())){
            res.push_back(p);
        }
        else if (auto* n = dynamic_cast<NuageDePoints*>(e.get())) {
            n->collecterPoints(res);  
        }
        
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
