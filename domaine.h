#pragma once
#include <vector>
#include <memory>
#include <string>

class ITexture;  // forward

// ---- IElement ----
class IElement {
public:
    virtual ~IElement() = default;
    virtual void afficher() const = 0;        // pour debug / listing
    virtual int  getId() const = 0;
};

// ---- Point ----
class Point : public IElement {
public:
    Point(int id, int x, int y, std::shared_ptr<ITexture> texture);

    int getId() const override;
    int getX() const { return x_; }
    int getY() const { return y_; }
    void setPosition(int x, int y);
    std::shared_ptr<ITexture> getTexture() const;

    void afficher() const override;

private:
    int id_;
    int x_;
    int y_;
    std::shared_ptr<ITexture> texture_;
};

// ---- Observer forward ----
class IObservateur;
class ISujet;

// ---- NuageDePoints ----
class NuageDePoints : public IElement {
public:
    explicit NuageDePoints(const std::string& id);

    int getId() const override;          // hash/indice interne si besoin
    const std::string& getNom() const { return id_; }

    void ajouter(std::shared_ptr<IElement> element);
    void supprimer(int id);
    void deplacer(int id, int nx, int ny);

    std::shared_ptr<IElement> trouver(int id) const;
    const std::vector<std::shared_ptr<IElement>>& getElements() const;

    void afficher() const override;

    // Observer
    void attacher(IObservateur* obs);
    void detacher(IObservateur* obs);
    void notifier();

private:
    std::string id_;
    std::vector<std::shared_ptr<IElement>> elements_;
    std::vector<IObservateur*> observateurs_;
};

// ---- Surface ----
class Surface {
public:
    void ajouterPoint(const Point& p);
    const std::vector<Point>& getPoints() const { return points_; }

private:
    std::vector<Point> points_;
};
