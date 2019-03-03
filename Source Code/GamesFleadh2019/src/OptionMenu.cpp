#include "OptionMenu.h"



OptionMenu::OptionMenu(sf::Font & t_font, sf::RenderWindow & t_window, Menu & state, Options & options) :
	m_title("Options", t_font),
	m_optionMenu(t_window),
	m_state(state),
	m_options(options),
	m_arialBlackFont(t_font)
{
	initGui();
	m_transitionTotalTime = sf::seconds(2);

	sf::Transform temp;
	temp.translate(0, -1200);
	m_renderState.transform = temp;

}


OptionMenu::~OptionMenu()
{
}

void OptionMenu::processEvents(Xbox360Controller & pad)
{
	if (!m_transitionIn && !m_transitionOut)
	{
		if (pad.m_currentState.DpadUp && !pad.m_previousState.DpadUp)
		{
			messageHandler(m_selected->getUp()); //returns a string 
		}
		if (pad.m_currentState.DpadDown && !pad.m_previousState.DpadDown)
		{
			messageHandler(m_selected->getDown());
		}
		if (pad.m_currentState.DpadLeft && !pad.m_previousState.DpadLeft)
		{
			messageHandler(m_selected->getLeft());
		}
		if (pad.m_currentState.DpadRight && !pad.m_previousState.DpadRight)
		{
			messageHandler(m_selected->getRight());
		}
		if (pad.m_currentState.A && !pad.m_previousState.A)
		{
			messageHandler(m_selected->getA());
		}

		if (pad.m_currentState.B && !pad.m_previousState.B ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
		{
			transitionOut(Menu::StateMainMenu);
		}
	}
}

void OptionMenu::update(sf::Time t_deltaTime)
{
	m_musicVol->setValue(m_options.musicVol);
	m_soundVol->setValue(m_options.soundVol);
	m_mute->setChecked(m_options.mute);
	m_animationSpeed->setValue(100 - m_options.animSpeed);
	m_fontSize->setChecked(m_options.fontSize);


	m_musicVol->setCharacterSize(m_options.fontSize * 5 + 10);
	m_soundVol->setCharacterSize(m_options.fontSize * 5 + 10);
	m_mute->setCharacterSize(m_options.fontSize * 5 + 10);
	m_animationSpeed->setCharacterSize(m_options.fontSize * 5 + 10);
	m_fontSize->setCharacterSize(m_options.fontSize * 5 + 10);
	m_back->setCharacterSize(m_options.fontSize * 5 + 10);

	double scaler;
	if (m_transitionIn || m_transitionOut)
	{
		m_transitionTime -= t_deltaTime;
		scaler = t_deltaTime.asSeconds() / (m_transitionTotalTime.asSeconds() / 100 * m_options.animSpeed);
	}
	if (m_transitionIn)
	{
		m_renderState.transform.translate(sf::Vector2f(0, scaler * 1200));

		if (m_transitionTime.asSeconds() < 0)
		{
			m_transitionIn = false;

			m_renderState = sf::Transform();
		}
	}
	if (m_transitionOut)
	{
		m_renderState.transform.translate(sf::Vector2f(0, scaler * -1200));
		if (m_transitionTime.asSeconds() < 0)
		{
			m_transitionOut = false;
			m_state = m_nextState;
			m_changeMenu = true;
		}
	}
}

void OptionMenu::render(sf::RenderWindow & t_window)
{
	t_window.draw(m_optionMenu, m_renderState);
	t_window.draw(m_title, m_renderState);
}

void OptionMenu::messageHandler(std::string s)
{
	if (s == "music")
	{
		m_selected->deselect(); //changes colour back to default
		m_selected = m_musicVol; //changes the currently selected widget to this
		m_selected->select(); //changes the colour so that it looks like it's selected
		std::cout << "music" << std::endl;
	}

	if (s == "sound")
	{
		m_selected->deselect();
		m_selected = m_soundVol;
		m_selected->select();
		std::cout << "sound" << std::endl;
	}

	if (s == "mute")
	{
		m_selected->deselect();
		m_selected = m_mute;
		m_selected->select();
		std::cout << "mute" << std::endl;
	}

	if (s == "animation")
	{
		m_selected->deselect();
		m_selected = m_animationSpeed;
		m_selected->select();
		std::cout << "animation" << std::endl;
	}

	if (s == "font")
	{
		m_selected->deselect();
		m_selected = m_fontSize;
		m_selected->select();
		std::cout << "font" << std::endl;
	}

	if (s == "back")
	{
		m_selected->deselect();
		m_selected = m_back;
		m_selected->select();
		std::cout << "back" << std::endl;
	}

	if (s == "increasemusic")
	{
		if (m_options.musicVol < 100)
		{
			m_options.musicVol += 5;
		}
	}
	
	if (s == "decreasemusic")
	{
		if (m_options.musicVol > 0)
		{
			m_options.musicVol -= 5;
		}
	}

	if (s == "increasesound")
	{
		if (m_options.soundVol < 100)
		{
			m_options.soundVol += 5;
		}
	}

	if (s == "decreasesound")
	{
		if (m_options.soundVol > 0)
		{
			m_options.soundVol -= 5;
		}
	}

	if (s == "toggle")
	{
		m_options.mute = !m_options.mute;
	}

	if (s == "radioleft")
	{
		if (m_options.fontSize > 1)
		{
			m_options.fontSize--;
		}
	}
	
	if (s == "radioright")
	{
		if (m_options.fontSize < 3)
		{
			m_options.fontSize++;
		}
	}

	if (s == "increaseanim")
	{
		m_options.animSpeed -= 5;
		if (m_options.animSpeed < 0)
		{
			m_options.animSpeed = 0;
		}
	}

	if (s == "decreaseanim")
	{
		m_options.animSpeed += 5;
		if (m_options.animSpeed > 100)
		{
			m_options.animSpeed = 100;
		}
	}

	if (s == "mainmenu")
	{
		transitionOut(Menu::StateMainMenu);
		//m_state = Menu::StateMainMenu;
	}

}

void OptionMenu::transitionIn()
{
	m_selected->deselect();
	m_selected = m_musicVol;
	m_selected->select();

	sf::Transform temp;
	temp.translate(0, -1200);
	m_renderState.transform = temp;

	m_transitionIn = true;

	m_transitionTime = sf::seconds(m_transitionTotalTime.asSeconds() / 100 * m_options.animSpeed);
}

void OptionMenu::transitionOut(Menu s)
{
	m_transitionOut = true;
	m_transitionTime = sf::seconds(m_transitionTotalTime.asSeconds() / 100 * m_options.animSpeed);

	m_nextState = s;
}

bool OptionMenu::getChangeMenu()
{
	return m_changeMenu;
}

void OptionMenu::resetChangeMenu()
{
	m_changeMenu = false;
}

void OptionMenu::initGui()
{
	VLayout* layout = new VLayout;
	layout->setSpace(25);
	
	m_musicVol = new Slider("Music Volume", m_arialBlackFont, 80, 450);
	m_musicVol->setValue(m_options.musicVol);
	m_musicVol->setCharacterSize(CHARACTER_SIZE);
	layout->add(m_musicVol);
	
	m_soundVol = new Slider("Sound Volume", m_arialBlackFont, 75, 450);
	m_soundVol->setValue(m_options.soundVol);
	m_soundVol->setCharacterSize(CHARACTER_SIZE);
	layout->add(m_soundVol);

	m_mute = new CheckButton("Mute", m_arialBlackFont, 205, m_options.mute, 450);
	m_mute->setCharacterSize(CHARACTER_SIZE);
	layout->add(m_mute);
	
	m_animationSpeed = new Slider("Animation Speed", m_arialBlackFont, 50, 450);
	m_animationSpeed->setValue(m_options.animSpeed);
	m_animationSpeed->setCharacterSize(CHARACTER_SIZE);
	layout->add(m_animationSpeed);
	
	m_fontSize = new RadioButton("Font Size", m_arialBlackFont, 125, 450);
	m_fontSize->setCharacterSize(CHARACTER_SIZE);
	m_fontSize->setChecked(m_options.fontSize);
	layout->add(m_fontSize);
	
	m_back = new TextButton("Back", m_arialBlackFont, 450);
	m_back->setCharacterSize(CHARACTER_SIZE);
	layout->add(m_back);

	//setting the strings that will be returned on a button input
	m_musicVol->setDown("sound");
	m_musicVol->setLeft("decreasemusic");
	m_musicVol->setRight("increasemusic");

	m_soundVol->setUp("music");
	m_soundVol->setDown("mute");
	m_soundVol->setLeft("decreasesound");
	m_soundVol->setRight("increasesound");

	m_mute->setUp("sound");
	m_mute->setDown("animation");
	m_mute->setA("toggle");

	m_animationSpeed->setUp("mute");
	m_animationSpeed->setDown("font");
	m_animationSpeed->setLeft("decreaseanim");
	m_animationSpeed->setRight("increaseanim");

	m_fontSize->setUp("animation");
	m_fontSize->setDown("back");
	m_fontSize->setLeft("radioleft");
	m_fontSize->setRight("radioright");

	m_back->setUp("font");
	m_back->setA("mainmenu");

	m_optionMenu.setLayout(layout);

	m_selected = m_musicVol;
	m_selected->select();
}