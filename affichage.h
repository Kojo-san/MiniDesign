#pragma once
#include <vector>
#include <string>

constexpr int LARGEUR = 30;
constexpr int HAUTEUR = 20;

struct Pixel {
    int x;
    int y;
    std::string c;
};

struct Segment {
    int x0, y0;
    int x1, y1;
};

void dessinerGrille(const std::vector<Pixel>& points,
                    const std::vector<Segment>& segments);
