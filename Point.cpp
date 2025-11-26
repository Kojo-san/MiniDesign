#include "Point.h"
#include <iostream>

Point::Point(int id, int x, int y, ITexture* texture)
    : id_(id), x_(x), y_(y), texture_(texture) {}

int Point::getId() const {
    return id_;
}

int Point::getX() const {
    return x_;
}

int Point::getY() const {
    return y_;
}

void Point::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

ITexture* Point::getTexture() const {
    return texture_;
}

void Point::setTexture(ITexture* texture) {
    texture_ = texture;
}

void Point::afficher() const {
    std::cout << "Point #" << id_ << " (" << x_ << ", " << y_ << ")\n";
}
