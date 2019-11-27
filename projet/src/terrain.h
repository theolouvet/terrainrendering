#ifndef TERRAIN_H
#define TERRAIN_H
#include<irrlicht/irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class terrain
{
private:
    /* data */
public:
    terrain(/* args */);
    ~terrain();
    scene::ITerrainSceneNode* monterrain;
    void terrainHM(ISceneManager*,const io::path& );
    void setMaterialTexture(u32, video::ITexture*);
    void setMaterialFlag(video::E_MATERIAL_FLAG, bool);
    void setMaterialType(video::E_MATERIAL_TYPE);
};

#endif