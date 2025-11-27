#pragma once
#include <vector>
#include <string>

// Taille de la grille (même que l’énoncé de base)
constexpr int LARGEUR = 30;
constexpr int HAUTEUR = 20;

// Point à afficher dans la grille (chaîne : '.', 'o', '#', 'o#', '0'..)
struct Pixel {
    int x;
    int y;
    std::string c;
};

// Segment entre deux points (pour tracer les surfaces)
struct Segment {
    int x0, y0;
    int x1, y1;
};

// Dessine la scène : points + segments
void dessinerGrille(const std::vector<Pixel>& points,
                    const std::vector<Segment>& segments);
