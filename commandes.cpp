#include "Commandes.h"
#include <iostream>
#include <sstream>
using namespace std;

// ================= Historique =================

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

// ================= Controleur =================

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

void ControleurCommandes::creerEtExecuter(const string& nom) {
    if (nom == "a") hist_.pushExec(make_unique<CmdAfficherListe>(nuage_));
    else if (nom == "o1") hist_.pushExec(make_unique<CmdAfficherO1>(aff1_, nuage_, gest_));
    else if (nom == "o2") hist_.pushExec(make_unique<CmdAfficherO2>(aff2_, nuage_, gest_));
    else if (nom == "f") hist_.pushExec(make_unique<CmdFusion>(nuage_, aff1_, aff2_));
    else if (nom == "d") hist_.pushExec(make_unique<CmdDeplacer>(nuage_, aff1_, aff2_));
    else if (nom == "s") hist_.pushExec(make_unique<CmdSupprimer>(nuage_, 0)); // corrigé ci-dessous
    else if (nom == "c1") hist_.pushExec(make_unique<CmdSurfaceC1>(gest_, nuage_));
    else if (nom == "c2") hist_.pushExec(make_unique<CmdSurfaceC2>(gest_, nuage_));
}

// ================= Commandes =================

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
        char c = (p->getId() < 10 ? '0' + p->getId() : '*');
        px.push_back({ p->x(), p->y(), c });
    }

    cout << "\n\n";
    dessinerGrille(px, gest_.segmentsCourants());
}

// ===== Fusion =====

CmdFusion::CmdFusion(NuageDePoints& n, AfficheurO1& a1, AfficheurO2& a2)
    : nuage_(n), a1_(a1), a2_(a2) {}

void CmdFusion::executer() {
    cout << "IDs des points a fusionner : ";
    string l;
    getline(cin, l);
    ids_.clear();

    int id;
    istringstream iss(l);
    while (iss >> id) ids_.push_back(id);

    cout << "Texture : ";
    getline(cin, texture_);

    for (int pid : ids_)
        if (auto* p = nuage_.trouverPointParId(pid))
            p->setTexture(creerTextureDepuisString(texture_));

    cout << "\n";
    a1_.afficher();
    a2_.afficher();
}

// ===== Déplacer =====

CmdDeplacer::CmdDeplacer(NuageDePoints& n, AfficheurO1& a1, AfficheurO2& a2)
    : nuage_(n), a1_(a1), a2_(a2) {}

void CmdDeplacer::executer() {
    cout << "ID du point : ";
    cin >> id_;
    cout << "Nouvelle position (x y) : ";
    cin >> nouvX_ >> nouvY_;
    string dummy; getline(cin, dummy);

    if (auto* p = nuage_.trouverPointParId(id_)) {
        ancienX_ = p->x();
        ancienY_ = p->y();
        p->setPos(nouvX_, nouvY_);
    }

    a1_.afficher();
    a2_.afficher();
}

void CmdDeplacer::annuler() {
    if (auto* p = nuage_.trouverPointParId(id_))
        p->setPos(ancienX_, ancienY_);
}

// ===== Supprimer =====

CmdSupprimer::CmdSupprimer(NuageDePoints& n, int id)
    : nuage_(n), id_(id) {}

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

// ===== Surfaces =====

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
