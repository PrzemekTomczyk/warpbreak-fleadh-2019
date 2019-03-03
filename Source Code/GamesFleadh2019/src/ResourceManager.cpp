/// <summary>
/// @authors: Przemek Tomczyk - C00218004
/// </summary>

#include "ResourceManager.h"

ResourceManager::ResourceManager() // default constructor
{
	// load all assets into holders
	loadTextures();
	loadFonts();
	loadBuffers();
}

ResourceManager::~ResourceManager() // default destructor
{
}

sf::Texture & ResourceManager::getTexture(TextureID t_textureID)
{
	return m_textureHolder[t_textureID];
}

sf::Font & ResourceManager::getFont(FontID t_fontID)
{
	return m_fontHolder[t_fontID];
}

sf::SoundBuffer & ResourceManager::getBuffer(SoundBufferID t_bufferID)
{
	return m_bufferHolder[t_bufferID];
}

void ResourceManager::loadTextures()
{
	try
	{
		//eg below
		//m_textureHolder.acquire(TextureID::PLAYER_TEXTURE, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\Player.png"));
		m_textureHolder.acquire(TextureID::SPARK, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\Spark.png"));
		m_textureHolder.acquire(TextureID::SMOKE, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\Smoke.png"));
		m_textureHolder.acquire(TextureID::TELEPORT_PARTICLE, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\TelepostParticle.png"));

		m_textureHolder.acquire(TextureID::PLAYER_IDLE, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\PLAYER\\idle.png"));
		m_textureHolder.acquire(TextureID::PLAYER_RUN, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\PLAYER\\run.png"));
		m_textureHolder.acquire(TextureID::PLAYER_JUMP, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\PLAYER\\jump.png"));
		m_textureHolder.acquire(TextureID::PLAYER_PUNCH, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\PLAYER\\punch.png"));
		m_textureHolder.acquire(TextureID::SHIELDER, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\ENEMIES\\shielder.png"));
		m_textureHolder.acquire(TextureID::CHARGER, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\ENEMIES\\charger.png"));
		m_textureHolder.acquire(TextureID::WIN, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\win.png"));
		m_textureHolder.acquire(TextureID::LOSE, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\lose.png"));
		m_textureHolder.acquire(TextureID::ORB, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\PLAYER\\orb.png"));
		m_textureHolder.acquire(TextureID::BULLET, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\ENEMIES\\bullet.png"));
		m_textureHolder.acquire(TextureID::TUTORIAL_MOVE, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\TUTORIALS\\thumbstick.png"));
		m_textureHolder.acquire(TextureID::TUTORIAL_JUMP, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\TUTORIALS\\Jump.png"));
		m_textureHolder.acquire(TextureID::TUTORIAL_PUNCH, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\TUTORIALS\\Punch_Button.png"));
		m_textureHolder.acquire(TextureID::TUTORIAL_PUNCH_FAIL, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\TUTORIALS\\Punch_Fail.png"));
		m_textureHolder.acquire(TextureID::TUTORIAL_PUNCH_SUCCESS, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\TUTORIALS\\Punch_Success.png"));
		m_textureHolder.acquire(TextureID::TUTORIAL_ORB_THROW, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\TUTORIALS\\Orb_Throw.png"));
		m_textureHolder.acquire(TextureID::TUTORIAL_ORB_TELEPORT, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\TUTORIALS\\Orb_Teleport.png"));
		m_textureHolder.acquire(TextureID::TUTORIAL_ORB_RECALL, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\TUTORIALS\\Orb_Recall.png"));
		m_textureHolder.acquire(TextureID::PEDASTAL_ORB, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\PLAYER\\pedastal_with_orb.png"));
		m_textureHolder.acquire(TextureID::PEDASTAL_EMPTY, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\PLAYER\\pedastal_without_orb.png"));
		m_textureHolder.acquire(TextureID::GOAL, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\goal.png"));
		m_textureHolder.acquire(TextureID::CHECK, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\check.png"));
		m_textureHolder.acquire(TextureID::SHIELDER_IDLE, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\ENEMIES\\shielder_Idle.png"));
		m_textureHolder.acquire(TextureID::SHIELDER_WALK, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\ENEMIES\\shielder_walk.png"));
		m_textureHolder.acquire(TextureID::CHARGER_IDLE, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\ENEMIES\\charger_Idle.png"));
		m_textureHolder.acquire(TextureID::CHARGER_WALK, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\ENEMIES\\charger_walk.png"));
		m_textureHolder.acquire(TextureID::CHARGER_RUN, thor::Resources::fromFile<sf::Texture>("ASSETS\\IMAGES\\ENEMIES\\charger_run.png"));
	}
	catch (thor::ResourceLoadingException& e)
	{
		std::cout << "Error with texture: " << e.what() << std::endl;
	}
}

void ResourceManager::loadFonts()
{
	try
	{
		m_fontHolder.acquire(FontID::ARIBLK, thor::Resources::fromFile<sf::Font>("ASSETS\\fonts\\ariblk.ttf"));
		m_fontHolder.acquire(FontID::ORDINARY, thor::Resources::fromFile<sf::Font>("ASSETS\\fonts\\Ordinary.ttf"));

	}
	catch (thor::ResourceLoadingException& e)
	{
		std::cout << "Error with font: " << e.what() << std::endl;
	}
}

void ResourceManager::loadBuffers()
{
	try
	{
		//eg below
		//m_bufferHolder.acquire(SoundBufferID::BUTTON_CLICK, thor::Resources::fromFile<sf::SoundBuffer>("ASSETS\\SOUNDS\\buttonClick.wav"));
		m_bufferHolder.acquire(SoundBufferID::JUMP_LAND, thor::Resources::fromFile<sf::SoundBuffer>("ASSETS\\SOUNDS\\landSound.wav"));
		m_bufferHolder.acquire(SoundBufferID::STEP, thor::Resources::fromFile<sf::SoundBuffer>("ASSETS\\SOUNDS\\step.wav"));
		m_bufferHolder.acquire(SoundBufferID::WARP, thor::Resources::fromFile<sf::SoundBuffer>("ASSETS\\SOUNDS\\warp.wav"));
		m_bufferHolder.acquire(SoundBufferID::ALERT, thor::Resources::fromFile<sf::SoundBuffer>("ASSETS\\SOUNDS\\alert.wav"));
		m_bufferHolder.acquire(SoundBufferID::ORB_PICKUP, thor::Resources::fromFile<sf::SoundBuffer>("ASSETS\\SOUNDS\\orb_pickup.wav"));
		m_bufferHolder.acquire(SoundBufferID::ORB_BOUNCE, thor::Resources::fromFile<sf::SoundBuffer>("ASSETS\\SOUNDS\\bouncehit.wav"));
	}
	catch (thor::ResourceLoadingException& e)
	{
		std::cout << "Error with buffer: " << e.what() << std::endl;
	}
}

