#include "particleSystem.h"

/// <summary>
/// @brief render function to draw the particles on the screen
/// </summary>
/// <param name="t_window"></param>
void ParticleSystem::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_system);
}

/// <summary>
/// @brief update function for the particles 
/// updates the particles system and checks to see if the system has finished
/// return  true if finsihed else false
/// </summary>
/// <param name="t_deltaTime"></param>
/// <returns></returns>
bool ParticleSystem::update(sf::Time t_deltaTime)
{
	m_system.update(t_deltaTime);	
	if (t_deltaTime.asSeconds() > PARTICLE_TIME)
	{
		systemActive = false;
		return true;
	}
	else
	{
		return false;
	}
}

void ParticleSystem::createPuff(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition)
{
	systemActive = true;
	// clears all values from the system 
	m_system.clearAffectors();
	m_system.clearEmitters();
	m_system.clearParticles();

	m_system.setTexture(t_particleTexture);
	
	thor::FadeAnimation fader(0.0f, 1.0f);
	m_system.addAffector(thor::AnimationAffector(fader));

	thor::UniversalEmitter emitter;		// emitter for the system - what causes stuff to happen in the particle system
	emitter.setEmissionRate(10);
	emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.1), sf::seconds(0.5)));
	emitter.setParticlePosition(thor::Distributions::circle(t_particlePosition, 5));
	emitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(25.0f, 0.0f), 360.0));
	emitter.setParticleRotation(thor::Distributions::uniform(0.0f, 360.0f));
	m_system.addEmitter(emitter, sf::seconds(0.2));
}

/// <summary>
/// creates big puff of puffs upon landing impact
/// </summary>
/// <param name="t_particleTexture">texture used on particle</param>
/// <param name="t_particlePosition">where to emmit from</param>
/// <param name="t_emissionRate">value scaled off player's impact velocity</param>
void ParticleSystem::createLandingParticles(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition, int t_emissionRate)
{
	systemActive = true;

	// clears all values from the system 
	m_system.clearAffectors();
	m_system.clearEmitters();
	m_system.clearParticles();

	m_system.setTexture(t_particleTexture);

	thor::FadeAnimation fader(0.0f, 0.2f);
	m_system.addAffector(thor::AnimationAffector(fader));

	thor::UniversalEmitter emitter;		// emitter for the system - what causes stuff to happen in the particle system
	emitter.setEmissionRate(t_emissionRate);
	emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.1), sf::seconds(0.5)));
	emitter.setParticlePosition(thor::Distributions::circle(t_particlePosition, 5));	
	emitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(0.0, -100.0f), 90.0f));
	emitter.setParticleRotation(thor::Distributions::uniform(0.0f, 360.0f));
	m_system.addEmitter(emitter, sf::seconds(0.05f));
}

void ParticleSystem::createTeleportParticles(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition)
{
	systemActive = true;

	// clears all values from the system 
	m_system.clearAffectors();
	m_system.clearEmitters();
	m_system.clearParticles();

	m_system.setTexture(t_particleTexture);

	thor::FadeAnimation fader(0.0f, 1.0f);
	m_system.addAffector(thor::AnimationAffector(fader));

	thor::UniversalEmitter emitter;		// emitter for the system - what causes stuff to happen in the particle system
	emitter.setEmissionRate(500);
	emitter.setParticleLifetime(thor::Distributions::uniform(sf::seconds(0.3), sf::seconds(0.5)));
	emitter.setParticlePosition(thor::Distributions::circle(t_particlePosition, 5));
	emitter.setParticleVelocity(thor::Distributions::deflect(sf::Vector2f(100.0, 0.0f), 360.0));
	emitter.setParticleRotation(thor::Distributions::uniform(0.0f, 360.0f));
	m_system.addEmitter(emitter, sf::seconds(0.05f));
}

/// <summary>
/// @brief returns the current state of the system active bool
/// </summary>
/// <returns></returns>
bool ParticleSystem::particleActive()
{
	return systemActive;
}

void ParticleSystem::clearParticles()
{
	m_system.clearAffectors();
	m_system.clearEmitters();
	m_system.clearParticles();
}
