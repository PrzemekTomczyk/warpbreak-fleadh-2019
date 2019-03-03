#pragma once

#include <SFML\Graphics.hpp>
#include <array>
#include "particleSystem.h"

/// <summary>
/// @brief  basically it's the same as a projectile pool but it has particles instead
/// </summary>
class ParticlePool
{
public:
	ParticlePool() = default;

	void createPuff(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition);
	void createLandingParticles(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition, int t_emissionRate);
	void createTeleportParticles(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition);
	void update(sf::Time & t_time);
	void render(sf::RenderWindow & t_window);
	void clearParticles();
private:

	static const int s_POOL_SIZE = 2500;

	std::array<ParticleSystem, s_POOL_SIZE> m_particles;

	int m_nextAvailable{ 0 };

	bool m_poolFull{ false };

	
};

