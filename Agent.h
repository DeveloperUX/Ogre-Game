#ifndef AGENT_H
#define AGENT_H

#include "BaseApplication.h"
#include <deque>

extern Ogre::SceneManager* sceneMgr; // Defined in main.cpp

class Agent 
{
private:

	Ogre::SceneNode* mBodyNode; 
	Ogre::Entity* mBodyEntity; 
	float height;						// Characters are different heights
	float angleOffset;					// Some times they are created facing different directions

	std::map<std::string, Ogre::AnimationState*> mAnims;		// master animation list
	std::string mBaseAnimID;                   // current base (full- or lower-body) animation
	std::string mTopAnimID;                    // current top (upper-body) animation
	std::string idleBaseID;					   // what is the idle base animation called
	std::string idleTopID;						// what is the idle top animation called
	std::string locoBaseID;						// what is the walk/run base animation called
	std::string locoTopID;						// what is the walk/run top animation called. 
	
	std::map<std::string, bool> mFadingIn;
	std::map<std::string, bool> mFadingOut;
	Ogre::Real mTimer;					  // general timer to see how long animations have been playing
	Ogre::Real mVerticalVelocity;		  // for jumping

	Ogre::Real mDistance;					// The distance the agent has left to travel
	Ogre::Vector3 mDirection;				// The direction the object is moving
	Ogre::Vector3 mDestination;				// The destination the object is moving towards
	std::deque<Ogre::Vector3> mWalkList;	// The list of points we are walking to
	Ogre::Real mWalkSpeed;					// The speed at which the object is moving
	bool nextLocation();					// Is there another destination?
	void updateLocomote(Ogre::Real deltaTime);			// update the character's walking

	void setupAnimations();
	void setBaseAnimation(std::string id, bool reset = false);
	void setTopAnimation(std::string id, bool reset = false);
	void fadeAnimations(Ogre::Real deltaTime);
	void updateAnimations(Ogre::Real deltaTime);

	/////////////////////////////////////////////////////////////////////////////
	// Lecture 7:
	bool procedural;		// Is this character performing a procedural animation?
	///////////////////////////////////////////////////////////////////////////////
public:
	Agent(std::string name, std::string filename, float height = 5, float angleOffset = 90, float scale = 1); // takes a mesh file
	void setPosition(float x, float y, float z); 

	void setIdleBase(std::string id);
	void setIdleTop(std::string id);
	void setLocoTop(std::string id);
	void setLocoBase(std::string id);

	void update(Ogre::Real deltaTime);		// update the agent
	
	//////////////////////////////////////////////////////////////////////////
	// Lecture 7
	Ogre::Entity* mSword1; // keep pointers to the swords
	Ogre::Entity* mSword2;
	void drawSwords();	   // draw the swords
	void holdGun();			// Perhaps should pass in an object model?!!
	void lookAround();		// procedurally move the character's head
	void updateLookAround(Ogre::Real deltaTime);
	//////////////////////////////////////////////////////////////////////////
	// Lecture 8
	Ogre::Entity* getEntity(){return this->mBodyEntity; }
	////////////////////////////////////////////////////////////////////////
};

#endif