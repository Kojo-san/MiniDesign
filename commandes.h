#pragma once
#include <map>
#include <stack>
#include <memory>
#include <string>
#include <vector>
#include "Domaine.h"
#include "Surfaces.h"

// ===== Commande =====

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void executer() = 0;
    virtual void annuler() = 0;
};

class Historique {
public:
    void pushExec(std::unique_ptr<ICommand> cmd);
    void undo();
    void redo();
private:
    std::stack<std::unique_ptr<ICommand>> pileUndo_;
    std::stack<std::unique_ptr<ICommand>> pileRedo_;
};

class ControleurCommandes {
public:
    ControleurCommandes(NuageDePoints& nuage,
                        AfficheurO1& aff1,
                        AfficheurO2& aff2,
                        GestionnaireSurface& gest);

    void executerCommande(const std::string& nom);
    void undo();
    void redo();

private:
    NuageDePoints& nuage_;
    AfficheurO1& aff1_;
    AfficheurO2& aff2_;
    GestionnaireSurface& gest_;
    Historique hist_;
    void creerEtExecuter(const std::string& nom);
};

// a
class CmdAfficherListe : public ICommand {
public:
    CmdAfficherListe(NuageDePoints& n) : nuage_(n) {}
    void executer() override;
    void annuler() override {}
private:
    NuageDePoints& nuage_;
};

// o1
class CmdAfficherO1 : public ICommand {
public:
    CmdAfficherO1(AfficheurO1& a, const NuageDePoints& n, const GestionnaireSurface& g)
        : aff_(a), nuage_(n), gest_(g) {}
    void executer() override;
    void annuler() override {}
private:
    AfficheurO1& aff_;
    const NuageDePoints& nuage_;
    const GestionnaireSurface& gest_;
};

// o2
class CmdAfficherO2 : public ICommand {
public:
    CmdAfficherO2(AfficheurO2& a, const NuageDePoints& n, const GestionnaireSurface& g)
        : aff_(a), nuage_(n), gest_(g) {}
    void executer() override;
    void annuler() override {}
private:
    AfficheurO2& aff_;
    const NuageDePoints& nuage_;
    const GestionnaireSurface& gest_;
};

// f
class CmdFusion : public ICommand {
public:
    CmdFusion(NuageDePoints& n, AfficheurO1& a1, AfficheurO2& a2);
    void executer() override;
    void annuler() override {}
private:
    NuageDePoints& nuage_;
    AfficheurO1& a1_;
    AfficheurO2& a2_;
    std::vector<int> ids_;
    std::string texture_;
};

// d
class CmdDeplacer : public ICommand {
public:
    CmdDeplacer(NuageDePoints& n, AfficheurO1& a1, AfficheurO2& a2);
    void executer() override;
    void annuler() override;
private:
    NuageDePoints& nuage_;
    AfficheurO1& a1_;
    AfficheurO2& a2_;
    int id_ = -1;
    int ancienX_, ancienY_;
    int nouvX_, nouvY_;
};

// s
class CmdSupprimer : public ICommand {
public:
    CmdSupprimer(NuageDePoints& n, int id);

    void executer() override;
    void annuler() override;

private:
    NuageDePoints& nuage_;
    int id_;

    std::unique_ptr<IElement> sauvegarde_;
    int index_ = -1;
    bool aEteExecute_ = false;
};

// c1 / c2
class CmdSurfaceC1 : public ICommand {
public:
    CmdSurfaceC1(GestionnaireSurface& g, const NuageDePoints& n)
        : gest_(g), nuage_(n) {}
    void executer() override;
    void annuler() override {}
private:
    GestionnaireSurface& gest_;
    const NuageDePoints& nuage_;
};

class CmdSurfaceC2 : public ICommand {
public:
    CmdSurfaceC2(GestionnaireSurface& g, const NuageDePoints& n)
        : gest_(g), nuage_(n) {}
    void executer() override;
    void annuler() override {}
private:
    GestionnaireSurface& gest_;
    const NuageDePoints& nuage_;
};
