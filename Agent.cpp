#include "Agent.h"

Agent::Agent(std::string name, std::string filename, float height, float angleOffset, float scale) 
{
	using namespace Ogre;

	if (sceneMgr == NULL)
		return;

	this->height = height;
	this->angleOffset = angleOffset;
	mBodyNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * height);
	mBodyEntity = sceneMgr->createEntity(name, filename);
	mBodyNode->attachObject(mBodyEntity);

	mBodyNode->scale(Ogre::Vector3(scale, scale, scale));
	mBodyNode->yaw(Ogre::Degree(0));

	///////////////////////////////////////////////////////////////////////////////
	// Lecture 7
	// create swords and attach to sheath
	mSword1 = sceneMgr->createEntity("SinbadSword1", "Sword.mesh");   // load the models
	mSword2 = sceneMgr->createEntity("SinbadSword2", "Sword.mesh");
	mBodyEntity->attachObjectToBone("Sheath.L", mSword1);			  // attachment to the agent's sheath bones
	mBodyEntity->attachObjectToBone("Sheath.R", mSword2);

	// Discover the bones in this figure
	Skeleton *skel = mBodyEntity->getSkeleton();

	skel->getBone("Head")->scale(4,4,4);

	Skeleton::BoneIterator bi = skel->getBoneIterator();
	while (bi.hasMoreElements())
	{
		Bone *b = bi.getNext();
		std::string name = b->getName();

		// While we're at it, let's see what is going on with the joints
		// Oops there aren't any: Like a donut hole
		//b->;
	}
		
	this->procedural = false; // Not performing a procedural animation 
	////////////////////////////////////////////////////////////////////////////////
	// Lecture 8
	// Have the character's orientation track another node's position.
	// 3rd parameter is the vector of the character to use. 
	// 4th parameter is an offset. In this case it is the height because the gun is on the floor
	//this->mBodyNode->setAutoTracking(true, sceneMgr->getSceneNode("GunNode2"), Vector3(0,0,1), Vector3(0,4,0));

	/*Vector3 temp = sceneMgr->getSceneNode("GunNode2")->getPosition();
	temp.y+=4;
	this->mBodyNode->lookAt(temp, Node::TS_WORLD, Vector3(0,0,1));*/
	/////////////////////////////////////////////////////////////////////

	setupAnimations();
	setBaseAnimation("IdleBase", true);
	setTopAnimation("IdleTop", true);
	idleBaseID = "";
	idleTopID = "";
	locoBaseID = "";
	locoTopID = "";
	mWalkSpeed = 35.0f;
	mDirection = Ogre::Vector3::ZERO;
}

void 
Agent::setPosition(float x, float y, float z)
{
	this->mBodyNode->setPosition(x, y + height, z);
}

void 
Agent::setupAnimations()
{
	using namespace Ogre;

	this->mTimer = 0;
	this->mVerticalVelocity = 0;

	// this is very important due to the nature of the exported animations
	mBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	AnimationStateSet* aSet = mBodyEntity->getAllAnimationStates();
	AnimationStateIterator iter = mBodyEntity->getAllAnimationStates()->getAnimationStateIterator();
	while (iter.hasMoreElements())
	{
		AnimationState *a = iter.getNext();
		mAnims[a->getAnimationName()] = a;
		a->setLoop(true);
		mFadingIn[a->getAnimationName()] = false;
		mFadingOut[a->getAnimationName()] = false;
	}

	// start off in the idle state (top and bottom together)
	if (this->idleTopID != "")
	{
		setBaseAnimation(idleBaseID);
		setTopAnimation(idleTopID);
	}
}

void 
Agent::setBaseAnimation(std::string id, bool reset)
{
	//if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
	if (mFadingIn.count(mBaseAnimID) > 0)
	{
		// if we have an old animation, fade it out
		
		mFadingIn[mBaseAnimID] = false;
		mFadingOut[mBaseAnimID] = true;
	}

	mBaseAnimID = id;

	//if (id != ANIM_NONE)
	if (mAnims.count(id) > 0)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}
	
void Agent::setTopAnimation(std::string id, bool reset)
{
	//if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS)
	if (mFadingIn.count(mTopAnimID) > 0)
	{
		// if we have an old animation, fade it out
		mFadingIn[mTopAnimID] = false;
		mFadingOut[mTopAnimID] = true;
	}

	mTopAnimID = id;

	//if (id != ANIM_NONE)
	if (mAnims.count(id) > 0)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}

void 
Agent::updateAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	Real baseAnimSpeed = 1;
	Real topAnimSpeed = 1;

	mTimer += deltaTime;
/////////////////////////////////////////////////////////////////////////////////
	// Lecture 7
	// Drawing Swords
	// half-way through the animation is when the hand grasps the handles...
	if (mTopAnimID == "DrawSwords")
	if (mTimer >= mAnims[mTopAnimID]->getLength() / 2 &&
		mTimer - deltaTime < mAnims[mTopAnimID]->getLength() / 2)
	{
		// so transfer the swords from the sheaths to the hands
		mBodyEntity->detachAllObjectsFromBone();
		mBodyEntity->attachObjectToBone("Handle.L", mSword1);
		mBodyEntity->attachObjectToBone("Handle.R", mSword2);
		// change the hand state to grab or let go
		mAnims["HandsClosed"]->setEnabled(true);
		mAnims["HandsRelaxed"]->setEnabled(false);
	}
//////////////////////////////////////////////////////////////////////////////////////

	if (mTopAnimID != "IdleTop")
	if (mAnims.count(mTopAnimID) > 0)
	if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			setTopAnimation("IdleTop", true);
			setBaseAnimation("IdleBase", true);
			mTimer = 0;
		}
	
	// increment the current base and top animation times
	if (mAnims.count(mBaseAnimID) > 0)
		mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);
	if (mAnims.count(mTopAnimID) > 0)
		mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);

	// apply smooth transitioning between our animations
	fadeAnimations(deltaTime);
}

void 
Agent::fadeAnimations(Ogre::Real deltaTime)
{
	using namespace Ogre;

	std::map<std::string, AnimationState*>::iterator iter;
	iter = mAnims.begin();
	for (int i = 0; i < mAnims.size(); i++)
	{
		std::string key = iter->first;
		iter++;
		
		if (mFadingIn[key])
		{
			// slowly fade this animation in until it has full weight
			Real newWeight = mAnims[key]->getWeight() + deltaTime * 7.5f; //ANIM_FADE_SPEED;
			mAnims[key]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight >= 1) mFadingIn[key] = false;
		}
		else if (mFadingOut[key])
		{
			// slowly fade this animation out until it has no weight, and then disable it
			Real newWeight = mAnims[key]->getWeight() - deltaTime * 7.5f; //ANIM_FADE_SPEED;
			mAnims[key]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				mAnims[key]->setEnabled(false);
				mFadingOut[key] = false;
			}
		}
	}
}

bool 
Agent::nextLocation()
{
	if (mWalkList.empty())
		return false;

	
	return true;
}

void 
Agent::updateLocomote(Ogre::Real deltaTime)
{
	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (this->nextLocation()) // a check to see if there is another destination
		{
			// Set running animation
			this->setBaseAnimation(this->locoBaseID, true);
			this->setTopAnimation(this->locoTopID, true);
		}
	}
	else
	{
		Ogre::Real move = mWalkSpeed * deltaTime;
		mDistance -= move;

		if (mDistance <= 0.0f) // over shooting target
		{
			this->mBodyNode->setPosition(mDestination);
			mDirection = Ogre::Vector3::ZERO;
			 if (!this->nextLocation()) // a check to see if there is another destination
			 {
				 this->setBaseAnimation(this->idleBaseID, true);
				 this->setTopAnimation(this->idleTopID, true);
			 }
			 else
			 {
				 Ogre::Vector3 src = this->mBodyNode->getOrientation() * Ogre::Vector3::UNIT_X;
				 if ((1.0f + src.dotProduct(mDirection)) < 0.0001f)
				 {
					 this->mBodyNode->yaw(Ogre::Degree(180));
				 }
				 else
				 {
					Ogre::Quaternion quat = src.getRotationTo(mDirection);
					this->mBodyNode->rotate(quat);
					this->mBodyNode->yaw(Ogre::Degree(angleOffset));  // To correct for models facing different directions
				}
			 }
		}
		else
		{
			this->mBodyNode->translate(mDirection * move);
		}
	}
}

void 
Agent::update(Ogre::Real deltaTime)
{
	this->updateLocomote(deltaTime);
	this->updateAnimations(deltaTime);

	/////////////////////////////////////////////////////
	// Lecture 7:
	if (procedural)  // For now, just look around
		this->updateLookAround(deltaTime);
	/////////////////////////////////////////////////////
}


void 
Agent::setIdleBase(std::string id)
{
	this->idleBaseID = id;
}

void 
Agent::setIdleTop(std::string id)
{
	this->idleTopID = id;
}

void 
Agent::setLocoTop(std::string id)
{
	this->locoTopID = id;
}

void 
Agent::setLocoBase(std::string id)
{
	this->locoBaseID = id;
}

//////////////////////////////////////////////////////////////////////////////////////
// Lecture 7
void
Agent::drawSwords()
{
	// take swords out (or put them back, since it's the same animation but reversed)
	setTopAnimation("DrawSwords", true);
	mTimer = 0;	
}

void
Agent::holdGun()
{
	// There is not animation for this. It just pops
	using namespace Ogre;

	Entity* ent = sceneMgr->getEntity("Gun");			// bad hard coding of weapon

	if (ent == NULL) return;

	ent->detachFromParent();			// must first detach from current spot
	mBodyEntity->attachObjectToBone("Handle.R", ent);	// now attach it to the agent
	ent->getParentNode()->scale(0.1, 0.1, 0.1);			// scale it again
	ent->getParentNode()->yaw(Degree(-90));				// rotate it into position
	ent->getParentNode()->pitch(Degree(180));	
	ent->getParentNode()->translate(0, -1.0, 0);			// translate a little
	//
	// change the hand state to grab
	mAnims["HandsClosed"]->setEnabled(true);
	mAnims["HandsRelaxed"]->setEnabled(false);	
}

void
Agent::lookAround()
{
	using namespace Ogre;
	
	// Stop animation playbacks
	mAnims[this->idleTopID]->setEnabled(false);
	//mAnims[this->idleBaseID]->setEnabled(false);

	this->procedural = true; // indicate that updateLookAround should be called.

	Bone *b = mBodyEntity->getSkeleton()->getBone("Head"); // get the Head bone
	b->setManuallyControlled(true);  // Son of a $&*@: Needed for rotation to have any effect		 
}

void 
Agent::updateLookAround(Ogre::Real deltaTime)
{
	using namespace Ogre;

	static int count = 0;  // To know where we are in the animation

	Bone *b = mBodyEntity->getSkeleton()->getBone("Head");	// get the Head bone
	b->setInheritOrientation(false);
	
	if (count > 480)				// Stop after a while
	{
		count = 0;					// reset the count for next time (remember it's static)
		
		// restart idle animation
		procedural = false;
		b->setManuallyControlled(false);
		this->setBaseAnimation("IdleBase");
		this->setTopAnimation("IdleTop");
	}
	
	if (b != NULL)  // if this bone exists
	{
		float angle;
		if (count < 60)			 // turn the head back and forth
			angle = -1;
		else if (count < 180)
			angle = 1;
		else if (count < 300)
			angle = -1;
		else if (count < 420)
			angle = 1;
		else
			angle = -1;

		b->yaw(Degree(angle)); // rotate head: 
		//b->scale(2, 2, 2);
		count++;
	}
	else
		procedural = false;  // stop doing a procedural animation
}
//////////////////////////////////////////////////////////////////////////////////////