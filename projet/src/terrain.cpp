#include"terrain.h"


terrain::terrain(/* args */)
{
}

terrain::~terrain()
{
}


void terrain::terrainHM(ISceneManager* smgr,
const io::path& heightMapFileName){
    monterrain = smgr->addTerrainSceneNode(
        heightMapFileName,
        0,                  // parent node
        -1,                 // node id
        core::vector3df(0.f,-550.f, 0.f),     // position
        core::vector3df(0.f, 0.f, 0.f),     // rotation
        core::vector3df(40.f, 4.4f, 40.f),  // scale
        video::SColor ( 255, 255, 255, 255 ),   // vertexColor
        5,                  // maxLOD
        scene::ETPS_17,             // patchSize
        4                   // smoothFactor
        );
     monterrain->scaleTexture(1.0f, 20.0f);
}

void terrain::setMaterialTexture(u32 i, video::ITexture* t){
    monterrain->setMaterialTexture(i,t);
}
void terrain::setMaterialFlag(video::E_MATERIAL_FLAG ef, bool b){
    monterrain->setMaterialFlag(ef, b);
}
void terrain::setMaterialType(video::E_MATERIAL_TYPE et){
      monterrain->setMaterialType(et);
}