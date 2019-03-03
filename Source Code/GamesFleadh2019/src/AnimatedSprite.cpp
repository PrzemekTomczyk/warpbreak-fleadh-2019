#include <AnimatedSprite.h>

AnimatedSprite::AnimatedSprite()
{
	m_current_frame = 0;
}

AnimatedSprite::AnimatedSprite(const sf::Texture& t) : Sprite(t), m_current_frame(0), m_time(seconds(0.5f)), m_offset(0, 0), m_looping(true) {}

AnimatedSprite::AnimatedSprite(const sf::Texture& t, const sf::IntRect& rect) : Sprite(t), m_current_frame(0), m_time(seconds(0.5f)), m_offset(0, 0), m_looping(true) {
	m_frames.push_back(rect);
}

AnimatedSprite::~AnimatedSprite() {}

const sf::Clock& AnimatedSprite::getClock() {
	return m_clock;
}

const sf::Time& AnimatedSprite::getTime() {
	return m_time;
}

void AnimatedSprite::setTime(Time t)
{
	m_time = t;
}

void AnimatedSprite::reset()
{
	m_current_frame = 0;
}

const vector<IntRect>& AnimatedSprite::getFrames() {
	return m_frames;
}

const IntRect& AnimatedSprite::getFrame(int n) {
	return m_frames[n];
}

void AnimatedSprite::addFrame(IntRect& frame) {
	m_frames.push_back(frame);
}

const int AnimatedSprite::getCurrentFrame() {
	return m_current_frame;
}

void AnimatedSprite::setOffset(sf::Vector2f v)
{
	m_offset = v;
}

void AnimatedSprite::setLooping(bool looping)
{
	m_looping = looping;
}

const sf::Vector2f AnimatedSprite::getOffset()
{
	return m_offset;
}

void AnimatedSprite::update() {
	if (m_clock.getElapsedTime() > m_time) {
		if (m_frames.size() > m_current_frame + 1)
		{
			m_current_frame++;
		}
		else {
			if (m_looping)
			{
				m_current_frame = 0;
			}
		}
		m_clock.restart();
	}
}

