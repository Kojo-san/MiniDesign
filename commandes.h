#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>

#include "ICommand.h"
#include "NuageDePoints.h"
#include "AfficheurO1.h"
#include "AfficheurO2.h"
#include "GestionnaireSurface.h"

class IElement;  
class NuageDePoints;
class AfficheurO1;
class AfficheurO2;
class GestionnaireSurface;
class ITexture;
class Surface;

/// Commande : afficher la liste des points et nuages
class CmdAfficherListe : public ICommand {
public:
    explicit CmdAfficherListe(NuageDePoints& nuage);

    void executer() override;
    void annuler() override {}  // rien à annuler

private:
    NuageDePoints& nuage_;
};

/// Commande : afficher l’orthèse avec les textures (o1)
class CmdAfficherO1 : public ICommand {
public:
    explicit CmdAfficherO1(AfficheurO1& aff);

    void executer() override;
    void annuler() override {}  // rien à annuler

private:
    AfficheurO1& aff_;
};

/// Commande : afficher l’orthèse avec les IDs (o2)
class CmdAfficherO2 : public ICommand {
public:
    explicit CmdAfficherO2(AfficheurO2& aff);

    void executer() override;
    void annuler() override {}  // rien à annuler

private:
    AfficheurO2& aff_;
};

/// Commande : fusionner des points dans un nuage (changer leur texture)
class CmdFusion : public ICommand {
public:
    explicit CmdFusion(NuageDePoints& nuage);

    void executer() override;
    void annuler() override;

private:
    NuageDePoints& nuage_;
    std::vector<int> ids_;
    std::vector<ITexture*> anciennesTextures_;
    ITexture* nouvelleTexture_ = nullptr;
    bool aEteExecute_ = false;
};

/// Commande : déplacer un point (undo = revenir à l’ancienne position)
class CmdDeplacer : public ICommand {
public:
    explicit CmdDeplacer(NuageDePoints& nuage);

    void executer() override;
    void annuler() override;

private:
    NuageDePoints& nuage_;
    int id_ = -1;
    int ancienX_ = 0, ancienY_ = 0;
    int nouveauX_ = 0, nouveauY_ = 0;
    bool aEteExecute_ = false;
};

/// Commande : supprimer un point (undo = le réinsérer)
class CmdSupprimer : public ICommand {
public:
    explicit CmdSupprimer(NuageDePoints& nuage);

    void executer() override;
    void annuler() override;

private:
    NuageDePoints& nuage_;
    int id_ = -1;
    IElement* sauvegarde_ = nullptr;
    std::size_t index_ = 0;
    bool aEteExecute_ = false;
};

/// Commande : générer surface C1 (ordre IDs)
class CmdSurfaceC1 : public ICommand {
public:
    CmdSurfaceC1(GestionnaireSurface& gest, NuageDePoints& nuage);

    void executer() override;
    void annuler() override {}  // rien à annuler

private:
    GestionnaireSurface& gest_;
    NuageDePoints& nuage_;
};

/// Commande : générer surface C2 (distance minimale)
class CmdSurfaceC2 : public ICommand {
public:
    CmdSurfaceC2(GestionnaireSurface& gest, NuageDePoints& nuage);

    void executer() override;
    void annuler() override {}  // rien à annuler

private:
    GestionnaireSurface& gest_;
    NuageDePoints& nuage_;
};
