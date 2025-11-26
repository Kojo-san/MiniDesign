#include "Commandes.h"
#include "NuageDePoints.h"
#include "Point.h"
#include "IElement.h"
#include "AfficheurO1.h"
#include "AfficheurO2.h"
#include "TextureSimple.h"
#include "TextureLibre.h"
#include "TextureVide.h"
#include "GestionnaireSurface.h"
#include "CreateurSurfaceC1.h"
#include "CreateurSurfaceC2.h"
#include <iostream>
#include <sstream>

// ------------------------------------------------------------
// CmdAfficherListe
// ------------------------------------------------------------

CmdAfficherListe::CmdAfficherListe(NuageDePoints& nuage)
    : nuage_(nuage) {}

void CmdAfficherListe::executer() {
    nuage_.afficher();
}

// ------------------------------------------------------------
// CmdAfficherO1
// ------------------------------------------------------------

CmdAfficherO1::CmdAfficherO1(AfficheurO1& aff)
    : aff_(aff) {}

void CmdAfficherO1::executer() {
    aff_.actualiser();
}

// ------------------------------------------------------------
// CmdAfficherO2
// ------------------------------------------------------------

CmdAfficherO2::CmdAfficherO2(AfficheurO2& aff)
    : aff_(aff) {}

void CmdAfficherO2::executer() {
    aff_.actualiser();
}

// ------------------------------------------------------------
// CmdFusion
// ------------------------------------------------------------

CmdFusion::CmdFusion(NuageDePoints& nuage)
    : nuage_(nuage) {}

void CmdFusion::executer() {
    ids_.clear();
    anciennesTextures_.clear();
    nouvelleTexture_ = nullptr;
    aEteExecute_ = false;

    std::cout << "IDs des points à fusionner (ex: 0 2 4) : ";
    std::string ligne;
    std::getline(std::cin, ligne);
    std::istringstream iss(ligne);
    int id;
    while (iss >> id) {
        ids_.push_back(id);
    }

    if (ids_.empty()) {
        std::cout << "Aucun ID fourni, annulation de la fusion.\n";
        return;
    }

    std::cout << "Texture à appliquer (ex: o, #, o#, l, st, ...) : ";
    std::string t;
    std::getline(std::cin, t);

    if (t.empty()) {
        nouvelleTexture_ = new TextureVide();
    } else if (t.size() == 1) {
        nouvelleTexture_ = new TextureSimple(t);
    } else {
        nouvelleTexture_ = new TextureLibre(t);
    }

    for (int pid : ids_) {
        Point* p = nuage_.trouverPointParId(pid);  // friend
        if (p) {
            anciennesTextures_.push_back(p->getTexture());
            p->setTexture(nouvelleTexture_);
        } else {
            anciennesTextures_.push_back(nullptr);
        }
    }

    aEteExecute_ = true;
    nuage_.notifier();
}

void CmdFusion::annuler() {
    if (!aEteExecute_) return;

    for (std::size_t i = 0; i < ids_.size(); ++i) {
        int pid = ids_[i];
        Point* p = nuage_.trouverPointParId(pid);
        if (p) {
            p->setTexture(anciennesTextures_[i]);
        }
    }
    nuage_.notifier();
    // On ne delete pas nouvelleTexture_ ici pour éviter d'invalider
    // les pointeurs éventuellement utilisés; petite fuite acceptable.
}

// ------------------------------------------------------------
// CmdDeplacer
// ------------------------------------------------------------

CmdDeplacer::CmdDeplacer(NuageDePoints& nuage)
    : nuage_(nuage) {}

void CmdDeplacer::executer() {
    std::cout << "ID du point à déplacer : ";
    if (!(std::cin >> id_)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Entrée invalide.\n";
        return;
    }
    std::cout << "Nouvelle position x y : ";
    if (!(std::cin >> nouveauX_ >> nouveauY_)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Entrée invalide.\n";
        return;
    }
    std::cin.ignore(10000, '\n'); // consommer le \n

    Point* p = nuage_.trouverPointParId(id_);
    if (!p) {
        std::cout << "Point introuvable.\n";
        return;
    }

    ancienX_ = p->getX();
    ancienY_ = p->getY();

    p->setPosition(nouveauX_, nouveauY_);
    aEteExecute_ = true;
    nuage_.notifier();
}

void CmdDeplacer::annuler() {
    if (!aEteExecute_) return;

    Point* p = nuage_.trouverPointParId(id_);
    if (!p) return;

    p->setPosition(ancienX_, ancienY_);
    nuage_.notifier();
}

// ------------------------------------------------------------
// CmdSupprimer
// ------------------------------------------------------------

CmdSupprimer::CmdSupprimer(NuageDePoints& nuage)
    : nuage_(nuage) {}

void CmdSupprimer::executer() {
    std::cout << "ID du point à supprimer : ";
    if (!(std::cin >> id_)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Entrée invalide.\n";
        return;
    }
    std::cin.ignore(10000, '\n');

    sauvegarde_ = nullptr;
    index_ = 0;
    aEteExecute_ = false;

    auto& elems = nuage_.elements_;  // accès via friend

    for (std::size_t i = 0; i < elems.size(); ++i) {
        Point* p = dynamic_cast<Point*>(elems[i]);
        if (p && p->getId() == id_) {
            sauvegarde_ = elems[i];
            index_ = i;
            elems.erase(elems.begin() + i);
            aEteExecute_ = true;
            nuage_.notifier();
            std::cout << "Point #" << id_ << " supprimé.\n";
            return;
        }
    }

    std::cout << "Point introuvable.\n";
}

void CmdSupprimer::annuler() {
    if (!aEteExecute_ || !sauvegarde_) return;

    auto& elems = nuage_.elements_;
    if (index_ > elems.size())
        index_ = elems.size();

    elems.insert(elems.begin() + index_, sauvegarde_);
    nuage_.notifier();
}

// ------------------------------------------------------------
// CmdSurfaceC1
// ------------------------------------------------------------

CmdSurfaceC1::CmdSurfaceC1(GestionnaireSurface& gest, NuageDePoints& nuage)
    : gest_(gest), nuage_(nuage) {}

void CmdSurfaceC1::executer() {
    gest_.setStrategie(std::make_shared<CreateurSurfaceC1>());
    Surface s = gest_.generer(nuage_);

    std::cout << "Surface C1 (ordre IDs) :\n";
    for (const auto& p : s.points) {
        std::cout << "ID " << p.getId()
                  << " (" << p.getX() << "," << p.getY() << ")\n";
    }
}

// ------------------------------------------------------------
// CmdSurfaceC2
// ------------------------------------------------------------

CmdSurfaceC2::CmdSurfaceC2(GestionnaireSurface& gest, NuageDePoints& nuage)
    : gest_(gest), nuage_(nuage) {}

void CmdSurfaceC2::executer() {
    gest_.setStrategie(std::make_shared<CreateurSurfaceC2>());
    Surface s = gest_.generer(nuage_);

    std::cout << "Surface C2 (distance minimale) :\n";
    for (const auto& p : s.points) {
        std::cout << "ID " << p.getId()
                  << " (" << p.getX() << "," << p.getY() << ")\n";
    }
}
