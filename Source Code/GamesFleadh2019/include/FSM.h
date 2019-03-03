#pragma once
#include <tinyfsm.hpp>
#include <iostream>
#include <AnimatedSprite.h>

//--------------------------------------------------------------FINITE STATE MACHINE SETUP------------------------------------------------------
struct Idling; // forward declaration
struct Running;
struct Jumping;
struct Falling;
struct Punching;

// Events
struct Idle : tinyfsm::Event { };
struct Run : tinyfsm::Event { };
struct Jump : tinyfsm::Event { };
struct Fall : tinyfsm::Event { };
struct Punch : tinyfsm::Event { };

//Class
class FSM : public tinyfsm::Fsm<FSM>
{
	friend class tinyfsm::Fsm<FSM>;

	virtual void react(Idle const &) { };
	virtual void react(Run const &) { };
	virtual void react(Jump const &) { };
	virtual void react(Fall const &) { };
	virtual void react(Punch const &) { };

	virtual void entry(void) { };
	void exit(void) { };

public:
	static AnimatedSprite* currentAnim;
	static AnimatedSprite idleAnim;
	static AnimatedSprite runAnim;
	static AnimatedSprite jumpAnim;
	static AnimatedSprite fallAnim;
	static AnimatedSprite punchAnim;
	int currState; //0=Idle, 1=Running, 2=Jumping, 3=Punching, 4=dying

	//static int getCurrState() { return currState; }
	static AnimatedSprite* getAnim()		{ return currentAnim; }
	static AnimatedSprite* getIdleRef()		{ return &idleAnim; }
	static AnimatedSprite* getRunRef()		{ return &runAnim; }
	static AnimatedSprite* getJumpRef()		{ return &jumpAnim; }
	static AnimatedSprite* getFallRef()		{ return &fallAnim; }
	static AnimatedSprite* getPunchRef()	{ return &punchAnim; }
	static void updateAnim() { currentAnim->update(); }
};

