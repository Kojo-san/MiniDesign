#include "affichage.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

static void tracerLigne(vector<vector<string>>& grille,
                        int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    std::string symbole = "/";
    if (x0 == x1) {
        symbole = "|";
    } else if (y0 == y1) {
        symbole = "-";
    } else {
        
        int dxs = x1 - x0;
        int dys = y1 - y0;
        if (dxs * dys > 0)
            symbole = "/";
        else
            symbole = "\\";
    }

    
    int err = (dx > dy ? dx : -dy) / 2;
    int curx = x0;
    int cury = y0;

    while (true) {
        if (curx >= 0 && curx < LARGEUR && cury >= 0 && cury < HAUTEUR)
            grille[cury][curx] = symbole;

        if (curx == x1 && cury == y1)
            break;
        int e2 = err;
        if (e2 > -dx) { err -= dy; curx += sx; }
        if (e2 < dy)  { err += dx; cury += sy; }
    }
}

void dessinerGrille(const vector<Pixel>& points,
                    const vector<Segment>& segments)
{
    vector<vector<string>> grille(HAUTEUR, vector<string>(LARGEUR, " "));

    for (const auto& s : segments) {
        tracerLigne(grille, s.x0, s.y0, s.x1, s.y1);
    }

    for (const auto& p : points) {
        if (p.x >= 0 && p.x < LARGEUR && p.y >= 0 && p.y < HAUTEUR)
            grille[p.y][p.x] = p.c;
    }

    for (int y = HAUTEUR - 1; y >= 0; --y) {
        for (int x = 0; x < LARGEUR; ++x)
            cout << grille[y][x];
        cout << '\n';
    }
}
