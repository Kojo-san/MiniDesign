#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Point.h"
#include "NuageDePoints.h"

#include "ITexture.h"
#include "TextureVide.h"

#include "AfficheurO1.h"
#include "AfficheurO2.h"

#include "Surface.h"
#include "GestionnaireSurface.h"
#include "CreateurSurfaceC1.h"
#include "CreateurSurfaceC2.h"

#include "Commandes.h"
#include "ControleurCommandes.h"

using namespace std;

// Parse une ligne "(x,y) (x,y) ..." en liste de (x,y)
vector<pair<int,int>> lirePointsDepuisChaine(const string& ligne) {
    vector<pair<int,int>> coords;
    istringstream iss(ligne);
    string token;

    while (iss >> token) {
        if (token.size() >= 3 && token.front() == '(' && token.back() == ')') {
            string inner = token.substr(1, token.size() - 2);
            replace(inner.begin(), inner.end(), ',', ' ');
            istringstream pairStream(inner);
            int x,y;
            if (pairStream >> x >> y) {
                coords.emplace_back(x,y);
            }
        }
    }
    return coords;
}

int main(int argc, char* argv[]) {
    string args;

    // Lecture comme dans le squelette fourni
    if (argc > 1) {
        ostringstream oss;
        for (int i = 1; i < argc; ++i)
            oss << argv[i] << " ";
        args = oss.str();
    } else {
        cout << "Entrez les points au format (x,y) :\n> ";
        getline(cin, args);
    }

    // Parse des coordonnées
    vector<pair<int,int>> coords = lirePointsDepuisChaine(args);
    if (coords.empty()) {
        cout << "Aucun point valide fourni. Fin du programme.\n";
        return 0;
    }

    // Création du nuage principal et des points
    NuageDePoints nuage{"principal"};

    int id = 0;
    for (auto& c : coords) {
        int x = c.first;
        int y = c.second;
        // Texture initiale : vide (affichée comme ".")
        ITexture* tex = new TextureVide();
        Point* p = new Point(id++, x, y, tex);
        nuage.ajouter(p);
    }

    // Afficheurs / Observateurs
    AfficheurO1 afficheurO1{nuage};
    AfficheurO2 afficheurO2{nuage};
    nuage.attacher(&afficheurO1);
    nuage.attacher(&afficheurO2);

    // Gestionnaire de surfaces (Stratégie)
    GestionnaireSurface gestSurface;

    // Contrôleur de commandes
    ControleurCommandes ctrl;

    // Enregistrement des commandes : chaque lambda fabrique une NOUVELLE commande
    ctrl.enregistrer("a", [&nuage]() {
        return std::make_shared<CmdAfficherListe>(nuage);
    });

    ctrl.enregistrer("o1", [&afficheurO1]() {
        return std::make_shared<CmdAfficherO1>(afficheurO1);
    });

    ctrl.enregistrer("o2", [&afficheurO2]() {
        return std::make_shared<CmdAfficherO2>(afficheurO2);
    });

    ctrl.enregistrer("f", [&nuage]() {
        return std::make_shared<CmdFusion>(nuage);
    });

    ctrl.enregistrer("d", [&nuage]() {
        return std::make_shared<CmdDeplacer>(nuage);
    });

    ctrl.enregistrer("s", [&nuage]() {
        return std::make_shared<CmdSupprimer>(nuage);
    });

    ctrl.enregistrer("c1", [&gestSurface, &nuage]() {
        return std::make_shared<CmdSurfaceC1>(gestSurface, nuage);
    });

    ctrl.enregistrer("c2", [&gestSurface, &nuage]() {
        return std::make_shared<CmdSurfaceC2>(gestSurface, nuage);
    });

    // Boucle de commandes
    string cmd;
    while (true) {
        cout << "\nCommandes:\n"
             << "a  - Afficher les points et les nuages\n"
             << "o1 - Afficher l'orthèse avec les textures des points\n"
             << "o2 - Afficher l'orthèse avec les IDs des points\n"
             << "f  - Fusionner des points dans un nuage (et appliquer texture)\n"
             << "d  - Deplacer un point (ID)\n"
             << "s  - Supprimer un point (ID)\n"
             << "c1 - Créer les surfaces selon l'ordre des IDs\n"
             << "c2 - Créer les surfaces selon la distance minimale\n"
             << "u  - Undo (annuler la dernière commande modifiante)\n"
             << "r  - Redo (rétablir la commande annulée)\n"
             << "q  - Quitter\n> ";

        if (!getline(cin, cmd))
            break;

        if (cmd == "q")
            break;
        else if (cmd == "u")
            ctrl.undo();
        else if (cmd == "r")
            ctrl.redo();
        else
            ctrl.executerCommande(cmd);
    }

    cout << "Fin du programme MiniDesign.\n";
    return 0;
}
