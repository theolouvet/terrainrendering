//#include <irrlicht.h>
#include<irrlicht/irrlicht.h>
#include <iostream>

#include "terrain.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

 

ISceneManager* smgr ;
IrrlichtDevice *device;
irr::SKeyMap keyMap[5];
bool UseHighLevelShaders = true;
bool UseCgShaders = false;

class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:

    virtual void OnSetConstants(video::IMaterialRendererServices* services,
                s32 userData)
        {
            video::IVideoDriver* driver = services->getVideoDriver();

            // set inverted world matrix
            // if we are using highlevel shaders (the user can select this when
            // starting the program), we must set the constants by name.

            core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
            invWorld.makeInverse();

            if (UseHighLevelShaders)
                services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
            else
                services->setVertexShaderConstant(invWorld.pointer(), 0, 4);

            // set clip matrix

            core::matrix4 worldViewProj;
            worldViewProj = driver->getTransform(video::ETS_PROJECTION);
            worldViewProj *= driver->getTransform(video::ETS_VIEW);
            worldViewProj *= driver->getTransform(video::ETS_WORLD);

            if (UseHighLevelShaders)
                services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
            else
                services->setVertexShaderConstant(worldViewProj.pointer(), 4, 4);

            // set camera position

            core::vector3df pos = device->getSceneManager()->
                getActiveCamera()->getAbsolutePosition();

            if (UseHighLevelShaders)
                services->setVertexShaderConstant("mLightPos", reinterpret_cast<f32*>(&pos), 3);
            else
                services->setVertexShaderConstant(reinterpret_cast<f32*>(&pos), 8, 1);

            // set light color

            video::SColorf col(0.0f,1.0f,1.0f,0.0f);

            if (UseHighLevelShaders)
                services->setVertexShaderConstant("mLightColor",
                        reinterpret_cast<f32*>(&col), 4);
            else
                services->setVertexShaderConstant(reinterpret_cast<f32*>(&col), 9, 1);

            // set transposed world matrix

            core::matrix4 world = driver->getTransform(video::ETS_WORLD);
            world = world.getTransposed();

            if (UseHighLevelShaders)
            {
                services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

                // set texture, for textures you can use both an int and a float setPixelShaderConstant interfaces (You need it only for an OpenGL driver).
                s32 TextureLayerID = 0;
                if (UseHighLevelShaders)
                    services->setPixelShaderConstant("myTexture", &TextureLayerID, 1);
            }
            else
                services->setVertexShaderConstant(world.pointer(), 10, 4);
        }
};



int main(){


    video::E_DRIVER_TYPE driverType= video::EDT_OPENGL ;
    if (driverType==video::EDT_COUNT)
        return 1;
    device = createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
                           false, false, false, 0);
    if (!device)
        return 1;
    device->setWindowCaption(L"Game");

    

    IVideoDriver* driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();
    video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
   
  
    const video::E_GPU_SHADING_LANGUAGE shadingLanguage = video::EGSL_DEFAULT;
    device->getCursorControl()->setVisible(false);
 
    keyMap[0].Action = irr::EKA_MOVE_FORWARD;  // avancer
            keyMap[0].KeyCode = irr::KEY_KEY_Z;        // Z
            keyMap[1].Action = irr::EKA_MOVE_BACKWARD; // reculer
            keyMap[1].KeyCode = irr::KEY_KEY_S;        // s
            keyMap[2].Action = irr::EKA_STRAFE_LEFT;   // a gauche
            keyMap[2].KeyCode = irr::KEY_KEY_A;        // a
            keyMap[3].Action = irr::EKA_STRAFE_RIGHT;  // a droite
            keyMap[3].KeyCode = irr::KEY_KEY_D;        // d
            keyMap[4].Action = irr::EKA_JUMP_UP;       // saut
            keyMap[4].KeyCode = irr::KEY_SPACE;        // barre espace
            ICameraSceneNode* camfree =  smgr->addCameraSceneNodeFPS(
                    0,
            100,
            10.0f,
            -1,
            0,//keymap
            5
            );
            camfree->setFarValue(30000.0f);

    //edit shader test
    io::path vsFileName = "media/terraindemo/opengl.vert"; // filename for the vertex shader
    io::path psFileName = "media/terraindemo/opengl.frag"; // filename for the pixel shader
    io::path vsQuad = "media/terraindemo/quad.vert"; // filename for the vertex shader
    io::path psQuad = "media/terraindemo/quad.frag"; // filename for the pixel shader

    if (!driver->queryFeature(video::EVDF_PIXEL_SHADER_1_1) &&
            !driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1))
        {
            device->getLogger()->log("WARNING: Pixel shaders disabled "\
                "because of missing driver/hardware support.");
            psFileName = "";
        }

        if (!driver->queryFeature(video::EVDF_VERTEX_SHADER_1_1) &&
            !driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1))
        {
            device->getLogger()->log("WARNING: Vertex shaders disabled "\
                "because of missing driver/hardware support.");
            vsFileName = "";
        }


   

    MyShaderCallBack* mc = new MyShaderCallBack();
    s32 newMaterialType1 = 0;

    // create material from high level shaders (hlsl, glsl or cg)

    newMaterialType1 = gpu->addHighLevelShaderMaterialFromFiles(
        vsFileName, "vertexMain", video::EVST_VS_1_1,
        psFileName, "pixelMain", video::EPST_PS_1_1,
        mc, video::EMT_SOLID, 0, shadingLanguage);
    //mc->drop();

    // add terrain scene node
    
       const io::path& heightMapFileName = "media/terraindemo/terrain-heightmap.bmp";
       const io::path& TextureFileName = "media/terraindemo/terrain-texture.jpg";
       terrain ter = terrain();
       ter.terrainHM(smgr, heightMapFileName);
       ter.setMaterialFlag(video::EMF_LIGHTING, false);
       ter.setMaterialTexture(0, driver->getTexture(TextureFileName));
       //ter.setMaterialTexture(1, driver->getTexture("media/terraindemo/detailmap3.jpg"));
       ter.setMaterialType((video::E_MATERIAL_TYPE)newMaterialType1);


       scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
           driver->getTexture("media/terraindemo/irrlicht2_up.jpg"),
           driver->getTexture("media/terraindemo/irrlicht2_dn.jpg"),
           driver->getTexture("media/terraindemo/irrlicht2_lf.jpg"),
           driver->getTexture("media/terraindemo/irrlicht2_rt.jpg"),
           driver->getTexture("media/terraindemo/irrlicht2_ft.jpg"),
           driver->getTexture("media/terraindemo/irrlicht2_bk.jpg"));
       scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("media/terraindemo/skydome.jpg"),16,8,0.95f,2.0f);

       driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

       //add quad
       const irr::scene::IGeometryCreator *geomentryCreator = smgr->getGeometryCreator();
       s32 newMaterialType2 = 0;

       newMaterialType2 = gpu->addHighLevelShaderMaterialFromFiles(
           vsQuad, "vertexMain", video::EVST_VS_1_1,
           psQuad, "pixelMain", video::EPST_PS_1_1,
           mc, video::EMT_TRANSPARENT_ALPHA_CHANNEL, 0, shadingLanguage);
       irr::scene::IMesh* plane = geomentryCreator->createPlaneMesh(
                   irr::core::dimension2d<irr::f32>(300, 300),
                   irr::core::dimension2d<irr::u32>(40, 40));

       irr::scene::ISceneNode* ground = smgr->addMeshSceneNode(plane);
       ground->setMaterialTexture(0, driver->getTexture("media/terraindemo/water.jpg"));
       ground->setPosition(irr::core::vector3df(5500.f, 0.f, 4500.f));
       ground->setMaterialType((video::E_MATERIAL_TYPE)newMaterialType2);
       plane->setMaterialFlag(irr::video::EMF_LIGHTING, false);


    int lastFPS = -1;
    
    while(device->run())
    {
        driver->beginScene(true, true, video::SColor(255,200,200,200));
        smgr->drawAll();
        driver->endScene();
        int fps = driver->getFPS();
       
                if (lastFPS != fps)
                {
                    core::stringw tmp(L"GAME - Irrlicht Engine [");
                    tmp += driver->getName();
                    tmp += L"] fps: ";
                    tmp += fps;
                    tmp += L"] X=";
                    tmp += smgr->getActiveCamera()->getPosition().X;
                    tmp += L"] Y=";
                    tmp += smgr->getActiveCamera()->getPosition().Y;
                    tmp += L"] Z=";
                    tmp += smgr->getActiveCamera()->getPosition().Z;
                    device->setWindowCaption(tmp.c_str());
                    lastFPS = fps;
                }

    }


    device->drop();

  return 0;
}



