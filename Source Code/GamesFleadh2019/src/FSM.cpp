#include "FSM.h"

AnimatedSprite FSM::idleAnim;
AnimatedSprite FSM::runAnim;
AnimatedSprite FSM::jumpAnim;
AnimatedSprite FSM::fallAnim;
AnimatedSprite FSM::punchAnim;
AnimatedSprite* FSM::currentAnim;

//States
struct Idling : FSM
{
	void entry() override
	{
		currentAnim = &idleAnim;
		currentAnim->reset();
		currState = 0;
		std::cout << "* Idling" << std::endl;
	};
	void react(Run const &) override { transit<Running>(); };
	void react(Jump const &) override { transit<Jumping>(); };
	void react(Punch const &) override { transit<Punching>(); };
	void react(Fall const &) override { transit<Falling>(); };
};

struct Running : FSM
{
	void entry() override
	{
		currentAnim = &runAnim;
		currentAnim->reset();
		currState = 1;
		std::cout << "* Running" << std::endl;
	};
	void react(Idle const &) override { transit<Idling>(); };
	void react(Jump const &) override { transit<Jumping>(); };
	void react(Punch const &) override { transit<Punching>(); };
	void react(Fall const &) override { transit<Falling>(); };
};

struct Jumping : FSM
{
	void entry() override
	{
		currentAnim = &jumpAnim;
		currentAnim->reset();
		currState = 2;
		std::cout << "* Jumping" << std::endl;
	};
	void react(Idle const &) override { transit<Idling>(); };
	void react(Run const &) override { transit<Running>(); };
	void react(Punch const &) override { transit<Punching>(); };
	void react(Fall const &) override { transit<Falling>(); };
};

struct Falling : FSM
{
	void entry() override
	{
		currentAnim = &fallAnim;
		currentAnim->reset();
		currState = 2;
		std::cout << "* Falling" << std::endl;
	};
	void react(Idle const &) override { transit<Idling>(); };
	void react(Run const &) override { transit<Running>(); };
	void react(Punch const &) override { transit<Punching>(); };
	void react(Jump const &) override { transit<Jumping>(); };
};

struct Punching : FSM
{
	void entry() override
	{
		currentAnim = &punchAnim;
		currentAnim->reset();
		currState = 3;
		std::cout << "* Punching" << std::endl;
	};
	void react(Idle const &) override { transit<Idling>(); };
	void react(Run const &) override { transit<Running>(); };
	void react(Jump const &) override { transit<Jumping>(); };
	void react(Fall const &) override { transit<Falling>(); };
};

FSM_INITIAL_STATE(FSM, Idling)