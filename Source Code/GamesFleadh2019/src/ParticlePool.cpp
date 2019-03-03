
#include "ParticlePool.h"

void ParticlePool::createPuff(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition)
{
	if (m_poolFull)
	{
		m_nextAvailable = (m_nextAvailable + 1) % s_POOL_SIZE;
	}

	m_particles.at(m_nextAvailable).createPuff(t_particleTexture, t_particlePosition);
}

void ParticlePool::createLandingParticles(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition, int t_emissionRate) {
	if (m_poolFull)
	{
		m_nextAvailable = (m_nextAvailable + 1) % s_POOL_SIZE;
	}

	m_particles.at(m_nextAvailable).createLandingParticles(t_particleTexture, t_particlePosition, t_emissionRate);
}

void ParticlePool::createTeleportParticles(sf::Texture & t_particleTexture, sf::Vector2f & t_particlePosition)
{
	if (m_poolFull)
	{
		m_nextAvailable = (m_nextAvailable + 1) % s_POOL_SIZE;
	}

	m_particles.at(m_nextAvailable).createTeleportParticles(t_particleTexture, t_particlePosition);
}

void ParticlePool::update(sf::Time & t_time)
{

	int activeCount = 0;

	m_poolFull = false;
	for (int index = 0; index < s_POOL_SIZE; index++)
	{
		if (m_particles.at(index).update(t_time))
		{
			m_nextAvailable = index;
		}
		else
		{
			activeCount++;
		}
	}

	if (s_POOL_SIZE == activeCount)
	{
		m_poolFull = true;
	}
}

void ParticlePool::render(sf::RenderWindow & t_window)
{
	for (int index = 0; index < s_POOL_SIZE; index++)
	{
		if (m_particles.at(index).particleActive())
		{
			m_particles.at(index).render(t_window);
		}
	}
}

void ParticlePool::clearParticles()
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		m_particles.at(i).clearParticles();
	}
}
