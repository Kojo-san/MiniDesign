#include "Commandes.h"
#include <iostream>
#include <sstream>
using namespace std;


void Historique::pushExec(unique_ptr<ICommand> cmd) {
    cmd->executer();
    pileUndo_.push(std::move(cmd));
    while (!pileRedo_.empty()) pileRedo_.pop();
}

void Historique::undo() {
    if (pileUndo_.empty()) return;
    auto cmd = std::move(pileUndo_.top());
    pileUndo_.pop();
    cmd->annuler();
    pileRedo_.push(std::move(cmd));
}

void Historique::redo() {
    if (pileRedo_.empty()) return;
    auto cmd = std::move(pileRedo_.top());
    pileRedo_.pop();
    cmd->executer();
    pileUndo_.push(std::move(cmd));
}


ControleurCommandes::ControleurCommandes(NuageDePoints& n,
                                         AfficheurO1& a1,
                                         AfficheurO2& a2,
                                         GestionnaireSurface& g)
    : nuage_(n), aff1_(a1), aff2_(a2), gest_(g) {}

void ControleurCommandes::executerCommande(const string& nom) {
    creerEtExecuter(nom);
}

void ControleurCommandes::undo() { hist_.undo(); }
void ControleurCommandes::redo() { hist_.redo(); }

void ControleurCommandes::creerEtExecuter(const std::string& nom)
{
    if (nom == "a") {
        
        CmdAfficherListe cmd(nuage_);
        cmd.executer();
    }
    else if (nom == "o1") {
        CmdAfficherO1 cmd(aff1_, nuage_, gest_);
        cmd.executer();
    }
    else if (nom == "o2") {
        CmdAfficherO2 cmd(aff2_, nuage_, gest_);
        cmd.executer();
    }
    else if (nom == "f") {
        hist_.pushExec(std::make_unique<CmdFusion>(nuage_, aff1_, aff2_));
    }
    else if (nom == "d") {
        hist_.pushExec(std::make_unique<CmdDeplacer>(nuage_, aff1_, aff2_));
    }
    else if (nom == "s") {
        hist_.pushExec(std::make_unique<CmdSupprimer>(nuage_, aff1_, aff2_));
    }
    else if (nom == "c1") {
        hist_.pushExec(std::make_unique<CmdSurfaceC1>(gest_, nuage_));
    }
    else if (nom == "c2") {
        hist_.pushExec(std::make_unique<CmdSurfaceC2>(gest_, nuage_));
    }
}



void CmdAfficherListe::executer() {
    nuage_.afficherListe(cout);
}

void CmdAfficherO1::executer() {
    vector<Point*> pts;
    const_cast<NuageDePoints&>(nuage_).collecterPoints(pts);

    vector<Pixel> px;
    for (auto* p : pts)
        px.push_back({ p->x(), p->y(), p->symbolePourAffichage() });

    cout << "\n\n";
    dessinerGrille(px, gest_.segmentsCourants());
}

void CmdAfficherO2::executer() {
    vector<Point*> pts;
    const_cast<NuageDePoints&>(nuage_).collecterPoints(pts);

    vector<Pixel> px;
    for (auto* p : pts) {
        std::string s = (p->getId() < 10 ? std::string(1, char('0' + p->getId())) : std::string("*"));
        px.push_back({ p->x(), p->y(), s });
    }

    cout << "\n\n";
    dessinerGrille(px, gest_.segmentsCourants());
}


CmdFusion::CmdFusion(NuageDePoints& n, AfficheurO1& a1, AfficheurO2& a2)
    : nuage_(n), a1_(a1), a2_(a2) {}

void CmdFusion::executer() {
    cout << "IDs des points/nuages a fusionner : ";
    string l;
    getline(cin, l);
    ids_.clear();

    int id;
    istringstream iss(l);
    while (iss >> id) ids_.push_back(id);

    static int compteurFusion = 0;
    std::string textureCourante = (compteurFusion == 0) ? std::string("o") : std::string("#");
    ++compteurFusion;

    for (int pid : ids_)
        if (auto* p = nuage_.trouverPointParId(pid)) {
            std::string ancienne = p->texture().valeur();
            std::string nouvelle = ancienne + textureCourante;
            p->setTexture(creerTextureDepuisString(nouvelle));
        }

    cout << "\n";
    a1_.afficher();
    a2_.afficher();
}


CmdDeplacer::CmdDeplacer(NuageDePoints& n, AfficheurO1& a1, AfficheurO2& a2)
    : nuage_(n), aff1_(a1), aff2_(a2) {}

void CmdDeplacer::executer()
{
    if (!configure_) {
        std::cout << "ID du point : ";
        std::cin >> id_;
        std::cout << "Nouvelle position (x y) : ";
        std::cin >> nouvX_ >> nouvY_;
        std::string dummy; std::getline(std::cin, dummy); // vider la fin de ligne

        Point* p = nuage_.trouverPointParId(id_);
        if (!p) {
            std::cout << "ID introuvable.\n";
            id_ = -1;
            return;
        }

        ancienX_ = p->x();
        ancienY_ = p->y();
        configure_ = true;
    }

    if (id_ >= 0) {
        if (auto* p = nuage_.trouverPointParId(id_)) {
            p->setPos(nouvX_, nouvY_);
        }
    }

    std::cout << "\n";
    aff1_.afficher();
    aff2_.afficher();
}


void CmdDeplacer::annuler()
{
    if (!configure_ || id_ < 0) return;

    if (auto* p = nuage_.trouverPointParId(id_)) {
        p->setPos(ancienX_, ancienY_);
    }

    std::cout << "\n";
    aff1_.afficher();
    aff2_.afficher();
}



CmdSupprimer::CmdSupprimer(NuageDePoints& n, AfficheurO1& a1, AfficheurO2& a2)
    : nuage_(n), aff1_(a1), aff2_(a2) {}


void CmdSupprimer::executer() {
    cout << "ID du point a supprimer : ";
    cin >> id_;
    string dummy; getline(cin, dummy);

    auto& elems = nuage_.elements();

    index_ = -1;
    for (size_t i = 0; i < elems.size(); ++i)
        if (auto* p = dynamic_cast<Point*>(elems[i].get()))
            if (p->getId() == id_) { index_ = i; break; }

    if (index_ == -1) {
        cout << "Introuvable\n";
        return;
    }

    sauvegarde_ = std::move(elems[index_]);
    elems.erase(elems.begin() + index_);
    aEteExecute_ = true;
}

void CmdSupprimer::annuler() {
    if (!aEteExecute_) return;
    auto& elems = nuage_.elements();
    elems.insert(elems.begin() + index_, std::move(sauvegarde_));
    aEteExecute_ = false;
}


void CmdSurfaceC1::executer() {
    gest_.setStrategie(make_unique<CreateurSurfaceC1>());
    gest_.generer(nuage_);
    cout << "Surfaces C1 generees.\n";
}

void CmdSurfaceC2::executer() {
    gest_.setStrategie(make_unique<CreateurSurfaceC2>());
    gest_.generer(nuage_);
    cout << "Surfaces C2 generees.\n";
}
