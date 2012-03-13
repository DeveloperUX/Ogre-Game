#ifndef __GameApplication_h_
#define __GameApplication_h_

#include "BaseApplication.h"
#include <list>
#include "Agent.h"

class Agent; // forward declaration

class GameApplication : public BaseApplication
{
	
private:

	std::list<Agent *> agentList;
	std::vector<Ogre::Light*> lights;

	void loadEnv();			// Load the buildings or ground plane, etc
	void setupEnv();		// Set up lights, shadows, etc	
	void loadObjects();		// Load other propers or objects (e.g. furniture)
	void loadCharacters();	// Load actors, agents or characters
	
	void addTime(Ogre::Real deltaTime);
public:
    GameApplication(void);
    virtual ~GameApplication(void);

	//////////////////////////////////////////////////////////////////////////
	// Lecture 7: moved from base application
	// OIS::KeyListener
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    bool mouseMoved( const OIS::MouseEvent &arg );
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	//////////////////////////////////////////////////////////////////////////////////
	// Lecture 8
	bool bLMouseDown, bRMouseDown;	//true if mouse buttons are held down
	Ogre::SceneNode *mCurrentObject;	//pointer to our currently selected object
	//int mCount;						//number of objects created
	/////////////////////////////////////////////////////////////////////////////////

protected:
    virtual void createScene(void);
};

#endif // #ifndef __GameApplication_h_
