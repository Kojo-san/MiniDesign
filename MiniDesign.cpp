#include <iostream>
#include <sstream>
#include <string>
#include "Domaine.h"
#include "Surfaces.h"
#include "commandes.h"

using namespace std;

static bool parsePoint(const string& token, int& x, int& y)
{
    if (token.size() < 5) return false;
    if (token.front() != '(' || token.back() != ')') return false;

    string inner = token.substr(1, token.size() - 2);
    for (char& c : inner)
        if (c == ',') c = ' ';

    istringstream iss(inner);
    if (iss >> x >> y)
        return true;
    return false;
}


int main(int argc, char* argv[])
{
    string args;
    if (argc > 1) {
        ostringstream oss;
        for (int i = 1; i < argc; ++i) oss << argv[i] << " ";
        args = oss.str();
    } else {
        cout << "Entrez les points au format (x,y) :\n> ";
        getline(cin, args);
    }

    NuageDePoints nuage("principal");

    istringstream iss(args);
    string tok;
    int id = 0;
    while (iss >> tok) {
        int x, y;
        if (parsePoint(tok, x, y)) {
            nuage.ajouter(std::make_unique<Point>(id++, x, y));
        }
    }

    AfficheurO1 aff1(nuage);
    AfficheurO2 aff2(nuage);
    GestionnaireSurface gest;
    ControleurCommandes ctrl(nuage, aff1, aff2, gest);

    string cmd;
    while (true) {
        cout << "\nCommandes:\n"
             << "a  - Afficher les points et les nuages\n"
             << "o1 - Afficher l'orthèse avec les textures des points\n"
             << "o2 - Afficher l'orthèse avec les IDs des points\n"
             << "f  - Fusionner des points/nuages dans un nouveau nuage (et appliquer texture)\n"
             << "d  - Deplacer un point (ID)\n"
             << "s  - Supprimer un point (ID)\n"
             << "u  - Annuler la dernière commande (undo)\n"
             << "r  - Réappliquer la commande annulée (redo)\n"
             << "c1 - Créer les surfaces selon l'ordre des IDs\n"
             << "c2 - Créer les surfaces selon la distance minimale\n"
             << "q  - Quitter\n> ";
        if (!getline(cin, cmd)) break;
        if (cmd == "q") break;
        if (cmd == "u") {
            ctrl.undo();
        } else if (cmd == "r") {
            ctrl.redo();
        } else {
            ctrl.executerCommande(cmd);
        }
    }

    cout << "Fin du programme MiniDesign.\n";
    return 0;
}
