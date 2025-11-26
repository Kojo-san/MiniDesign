#include "affichage.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

// Algorithme de tracé de ligne (Bresenham simplifié)
static void tracerLigne(vector<vector<char>>& grille,
                        int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    while (true) {
        if (x0 >= 0 && x0 < LARGEUR && y0 >= 0 && y0 < HAUTEUR)
            grille[y0][x0] = '/';

        if (x0 == x1 && y0 == y1)
            break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void dessinerGrille(const vector<Pixel>& points,
                    const vector<Segment>& segments)
{
    // y = 0 en bas, comme dans l’énoncé
    vector<vector<char>> grille(HAUTEUR, vector<char>(LARGEUR, ' '));

    // tracer les lignes des surfaces
    for (const auto& s : segments) {
        tracerLigne(grille, s.x0, s.y0, s.x1, s.y1);
    }

    // placer les points (les points écrasent les '/')
    for (const auto& p : points) {
        if (p.x >= 0 && p.x < LARGEUR && p.y >= 0 && p.y < HAUTEUR)
            grille[p.y][p.x] = p.c;
    }

    // impression de haut en bas
    for (int y = HAUTEUR - 1; y >= 0; --y) {
        for (int x = 0; x < LARGEUR; ++x)
            cout << grille[y][x];
        cout << '\n';
    }
}
