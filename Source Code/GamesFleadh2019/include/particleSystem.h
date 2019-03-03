
#pragma once

#include "Thor\Particles.hpp"
#include "Thor\Animations\FadeAnimation.hpp"
#include "Thor\Math.hpp"
#include <SFML\Graphics.hpp>

class ParticleSystem
{
public:
	ParticleSystem() = default;

	void render(sf::RenderWindow & t_window);
	bool update(sf::Time t_deltaTime);
	
	void createPuff(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition);
	void createLandingParticles(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition, int t_emissionRate);
	void createTeleportParticles(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition);
	bool particleActive();

	void clearParticles();


private:
	// particle system used
	thor::ParticleSystem m_system;

	// bool if the system is currently activate
	bool systemActive{ false };


	// how long the emitters last for
	const  double PARTICLE_TIME = 0.75f;

	
};

