#include "GameApplication.h"


//-------------------------------------------------------------------------------------
GameApplication::GameApplication(void): mCurrentObject(0), bLMouseDown(false), bRMouseDown(false) {

}
//-------------------------------------------------------------------------------------
GameApplication::~GameApplication(void)
{
}

//-------------------------------------------------------------------------------------
void GameApplication::createScene(void)
{
	loadEnv();
	setupEnv();
	loadObjects();
	loadCharacters();
}

void // Load the buildings or ground plane, etc
GameApplication::loadEnv()
{
	using namespace Ogre;

	// create a floor mesh resource
	MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	Plane(Vector3::UNIT_Y, 0), 100, 100, 10, 10, true, 1, 10, 10, Vector3::UNIT_Z);

	//// create a floor entity, give it a material, and place it at the origin
    Entity* floor = mSceneMgr->createEntity("Floor", "floor");
    floor->setMaterialName("Examples/StormySkyBox");
	floor->setCastShadows( false );
	floor->setVisible( false );
    mSceneMgr->getRootSceneNode()->attachObject(floor);

	// Lecture 8
	mSceneMgr->setSkyBox(true, "Examples/StormySkyBox" );//,5,, 5, 8);
 
}


void // Set up lights, shadows, etc
GameApplication::setupEnv()
{
	using namespace Ogre;

	// set shadow properties
	mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
	mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTextureSize(1024);
	mSceneMgr->setShadowTextureCount(1);

	// disable default camera control so the character can do its own
	mCameraMan->setStyle(OgreBites::CS_FREELOOK); //CS_FREELOOK, CS_ORBIT, CS_MANUAL

	// use a small amount of ambient lighting
	mSceneMgr->setAmbientLight(ColourValue(0.0f, 0.0f, 0.0f));


	//////////////////////////////////////////////////////////////////////////////////
	//but we also want to set up our raySceneQuery after everything has been initialized
	mRayScnQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	

	/********************* HOMEWORK 7 ************************/
	// Set the Shadow type to Additive
	sceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_TEXTURE_ADDITIVE );

}

void // Load other propers or objects (e.g. furniture)
GameApplication::loadObjects()
{
	using namespace Ogre;

	Ogre::Entity *ent;
	Ogre::SceneNode *node;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Lecture 7
	ent = mSceneMgr->createEntity("Gun", "38pistol.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("GunNode", Ogre::Vector3(5.0f, 1.4f,  5.0f));
	node->attachObject(ent);
	node->pitch(Degree(90));
	node->setScale(0.1f, 0.1f, 0.1);

	ent = mSceneMgr->createEntity("Gun2", "9mm.mesh");
	node = mSceneMgr->getRootSceneNode()->createChildSceneNode("GunNode2", Ogre::Vector3(5.0f, 1.4f,  5.0f));
	node->attachObject(ent);
	node->pitch(Degree(90));
	node->setScale(0.1f, 0.1f, 0.1);
	///////////////////////////////////////////////////////////////////////////////////////////////////
}

void // Load actors, agents or characters
GameApplication::loadCharacters()
{
	
}

void 
GameApplication::addTime(Ogre::Real deltaTime)
{
	/*
	// update all agents
	std::list<Boid*>::iterator iter;
	for (iter = boidsList.begin(); iter != boidsList.end(); iter++) {
		(*iter)->update(deltaTime); // same as draw_boids()
		(*iter)->move();		// same as move_to_new_position()
	}
	*/
	//updateCamera(deltaTime);
}

//-------------------------------------------------------------------------------------
bool GameApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
	else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
	/////////////////////////////////////////////////////////////////////////////////////
	else if (arg.key == OIS::KC_1) 
	{
		// Turn the light off if it is already on
		if( lights[0]->isVisible() )
			lights[0]->setVisible( false );
		else	// Turn it on
			lights[0]->setVisible( true );
	}
	else if (arg.key == OIS::KC_2)
	{
		// Set the top left corner to red
		lights[2]->setSpecularColour( 1.0, 0.0, 0.0 );
		lights[2]->setDiffuseColour( 1.0, 0.0, 0.0 );
	}
	else if (arg.key == OIS::KC_3) 
	{
		// Set the top right corner to blue
		lights[3]->setSpecularColour( 0.0, 0.0, 1.0 );
		lights[3]->setDiffuseColour( 0.0, 0.0, 1.0 );		
	}
	else if (arg.key == OIS::KC_4) 
	{
		// Set the bottom right corner to gree
		lights[4]->setSpecularColour( 0.0, 1.0, 0.0 );
		lights[4]->setDiffuseColour( 0.0, 1.0, 0.0 );
	}
	else if (arg.key == OIS::KC_5) 
	{
		// Loop through Lights
		for( int i = 0; i < lights.size(); i++ ) {
			// Get a random value for Red, Green, Blue
			float r = (rand() % 255)/255.0;
			float g = (rand() % 255)/255.0;
			float b = (rand() % 255)/255.0;
			// Set the light using a random color
			lights[i]->setSpecularColour( r, g, b );
			lights[i]->setDiffuseColour( r, g, b );
			lights[i]->setVisible(true);
		}
	}
	else if (arg.key == OIS::KC_6) // move 9mm a bit
	{
		sceneMgr->getSceneNode("GunNode2")->translate(0,0,1);
	}
///////////////////////////////////////////////////////////////////////////////////////
    mCameraMan->injectKeyDown(arg);
    return true;
}

bool GameApplication::keyReleased( const OIS::KeyEvent &arg )
{
    mCameraMan->injectKeyUp(arg);
    return true;
}

bool GameApplication::mouseMoved( const OIS::MouseEvent &arg )
{
	// Lecture 8
	//if the left mouse button is held down
	if(bLMouseDown)
	{
		//find the current mouse position
		//CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
		Ogre::Vector3 mousePos; 
		mousePos.x = arg.state.X.abs;
		mousePos.y = arg.state.Y.abs;
		mousePos.z = arg.state.Z.abs;
 
		//create a raycast straight out from the camera at the mouse's location
		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.x/float(arg.state.width), mousePos.y/float(arg.state.height));
		mRayScnQuery->setRay(mouseRay);
		mRayScnQuery->setSortByDistance(false);	//world geometry is at the end of the list if we sort it, so lets not do that
 
		Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
		Ogre::RaySceneQueryResult::iterator iter = result.begin();
 
		//check to see if the mouse is pointing at the world and put our current object at that location
		for(iter; iter != result.end(); iter++)
		{
			//if(iter->worldFragment) // Only works for terrain?
			//{
			//	mCurrentObject->setPosition(iter->worldFragment->singleIntersection);
			//	break;
			//}	
			if (iter->movable)
			{
				mCurrentObject->setPosition(iter->movable->getParentSceneNode()->getPosition());
			}
		}
	}
	else if(bRMouseDown)	//if the right mouse button is held down, be rotate the camera with the mouse
	{
		mCamera->yaw(Ogre::Degree(-arg.state.X.rel * 0.1));
		mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * 0.1));
	}
	/////////////////////////////////////////////////////////////////////////////////////////

    if (mTrayMgr->injectMouseMove(arg)) return true;
    //mCameraMan->injectMouseMove(arg);
    return true;
}

bool GameApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	//////////////////////////////////////////////////////////////////////////////////////
	// Lecture 8
	if(id == OIS::MB_Left)
	{
		//show that the current object has been deselected by removing the bounding box visual
		if(mCurrentObject)
		{
			mCurrentObject->showBoundingBox(false);
		}
 
		//find the current mouse position
		//CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
		Ogre::Vector3 mousePos;
		mousePos.x = arg.state.X.abs;
		mousePos.y = arg.state.Y.abs;
		mousePos.z = arg.state.Z.abs;
		
 
		//then send a raycast straight out from the camera at the mouse's position
		Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mousePos.x/float(arg.state.width), mousePos.y/float(arg.state.height));
		mRayScnQuery->setRay(mouseRay);
		mRayScnQuery->setSortByDistance(true);
		//mRayScnQuery->setQueryMask(bRobotMode ? ROBOT_MASK : NINJA_MASK);	//will return objects with the query mask in the results
 
		/*
		This next chunk finds the results of the raycast
		If the mouse is pointing at world geometry we spawn a robot at that position
		*/
		Ogre::RaySceneQueryResult& result = mRayScnQuery->execute();
		Ogre::RaySceneQueryResult::iterator iter = result.begin();
 
		for(iter; iter != result.end(); iter++)
		{
			//if you clicked on a robot or ninja it becomes selected
			if(iter->movable && iter->movable->getName().substr(0, 5) != "tile[")
			{
				mCurrentObject = iter->movable->getParentSceneNode();
				break;
			}
		}
 
		//now we show the bounding box so the user can see that this object is selected
		if(mCurrentObject)
		{
			mCurrentObject->showBoundingBox(true);
		}
 
		bLMouseDown = true;
	}
	else if (id == OIS::MB_Right)
		bRMouseDown = true;

	//////////////////////////////////////////////////////////////////////////////////////
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool GameApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	// Lecture 8
	if(id  == OIS::MB_Left)
	{
		bLMouseDown = false;
	}
	if(id  == OIS::MB_Right)
	{
		bRMouseDown = false;
	}
	else
		mCameraMan->injectMouseUp(arg, id);

	//////////////////////////////////////////////////////////////////////////////
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    
    return true;
}
