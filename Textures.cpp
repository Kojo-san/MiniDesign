#include "Textures.h"

using namespace std;

unique_ptr<ITexture> creerTextureDepuisString(const string& txt)
{
    if (txt.empty())
        return make_unique<TextureVide>();


    if (txt.size() == 1)
        return make_unique<TextureSimple>(txt);


    if (txt.size() == 2) {
        auto t1 = make_unique<TextureSimple>(txt.substr(0,1));
        auto t2 = make_unique<TextureSimple>(txt.substr(1,1));
        return make_unique<TextureMix>(std::move(t1), std::move(t2));
    }

    return make_unique<TextureLibre>(txt);
}
