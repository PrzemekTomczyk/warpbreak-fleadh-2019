#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\System\Clock.hpp>
#include <vector>
//#include <Debug.h>

using namespace std;
using namespace sf;

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(const Texture&);
	AnimatedSprite(const Texture&, const IntRect&);
	~AnimatedSprite();

	const Clock& getClock();
	const Time& getTime();
	void setTime(Time t);
	void reset();
	const vector<IntRect>& getFrames();
	const IntRect& getFrame(int);
	void addFrame(IntRect&);
	const int getCurrentFrame();
	void setOffset(sf::Vector2f v);
	void setLooping(bool looping);
	const sf::Vector2f getOffset();
	void update();

private:
	Clock m_clock;
	Time m_time;
	vector<IntRect> m_frames;
	int m_current_frame;
	sf::Vector2f m_offset;
	bool m_looping;
};

#endif // !ANIMATED_SPRITE_H
