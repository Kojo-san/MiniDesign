#include "Textures.h"

using namespace std;

unique_ptr<ITexture> creerTextureDepuisString(const string& txt)
{
    if (txt.empty())
        return make_unique<TextureVide>();

    // si c'est une seule lettre ou symbole simple -> TextureSimple
    if (txt.size() == 1)
        return make_unique<TextureSimple>(txt);

    // s'il y a 2 caractères, on les combine via TextureMix
    if (txt.size() == 2) {
        auto t1 = make_unique<TextureSimple>(txt.substr(0,1));
        auto t2 = make_unique<TextureSimple>(txt.substr(1,1));
        return make_unique<TextureMix>(std::move(t1), std::move(t2));
    }

    // sinon TextureLibre
    return make_unique<TextureLibre>(txt);
}
