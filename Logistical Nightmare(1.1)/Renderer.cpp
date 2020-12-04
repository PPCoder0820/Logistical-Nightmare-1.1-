#include "pch.h"
#include "Renderer.h"

//constants initialization
const float Renderer::m_TIME_DILATION = 10000.f;
const float Renderer::m_ZOOM_SENSITIVITY = 0.1f;
const std::string Renderer::m_monthStrings[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

Renderer::Renderer()
{
	//resolution 
	m_resolution.x = sf::VideoMode::getDesktopMode().width;
	m_resolution.y = sf::VideoMode::getDesktopMode().height;
	//hud background
	sf::Color brown(150, 75, 0, 255);
	m_hudBackground.setFillColor(brown);
	m_hudBackground.setPosition(0, 0);
	m_hudBackground.setSize(sf::Vector2f(m_resolution.x, m_resolution.y * 0.15f));
	//research background
	m_researchBackground.setFillColor(brown);
	m_researchBackground.setPosition(0, 0);
	m_researchBackground.setSize(sf::Vector2f(600, 600));
	//map background
	m_mapBackground.setFillColor(sf::Color::Green);
	m_mapBackground.setPosition(0, 0);
	m_mapBackground.setSize(m_mapSize);
	//font set
	m_font.loadFromFile("font/Stanberry.ttf");
	//Allegiance names
	m_allegiance1.setFont(m_font);
	m_allegiance2.setFont(m_font);
	m_allegiance1.setCharacterSize(30);
	m_allegiance2.setCharacterSize(30);
	m_allegiance1.setPosition(20, 20);
	m_allegiance2.setPosition(m_resolution.x * 0.85f, 20);
	//Tabs
	m_tabs[0].setString("Map");
	m_tabs[1].setString("Research");
	m_tabs[2].setString("Tile");
	m_tabs[3].setString("Build");
	m_tabs[4].setString("Storage");
	m_tabs[5].setString("Units");
	//Construction points display
	m_allegianceCP1.setFont(m_font);
	m_allegianceCP2.setFont(m_font);
	m_allegianceCP1.setCharacterSize(15);
	m_allegianceCP2.setCharacterSize(15);
	m_allegianceCP1.setPosition(200, 20);
	m_allegianceCP2.setPosition(m_resolution.x * 0.8f, 20);
	for (int i = 0; i < 6; i++) {
		m_tabs[i].setFillColor(sf::Color::Black);
		m_tabs[i].setCharacterSize(25);
		m_tabs[i].setPosition((m_resolution.x / 6) * i + m_resolution.x / 12, m_resolution.y * 0.125);
		m_tabs[i].setFont(m_font);
		m_tabs[i].setOrigin(m_tabs[i].getLocalBounds().width / 2, m_tabs[i].getLocalBounds().height / 2);
	}
	for (int i = 0; i < 6; i++) {
		m_tabButtons[i].setTexture(TextureHolder::getTexture("graphics/tab_button.png"));
		m_tabButtons[i].setPosition((m_resolution.x / 6) * i + m_resolution.x / 12, m_resolution.y * 0.125);
		m_tabButtons[i].setOrigin(m_tabButtons[i].getLocalBounds().width / 2, m_tabButtons[i].getLocalBounds().height / 2);
	}
	//runTime
	m_time.setCharacterSize(30);
	m_time.setFont(m_font);
	m_time.setPosition(m_resolution.x * 0.45f, 0);
}

void Renderer::drawToWindow(sf::RenderWindow& window, sf::View& hudView, sf::View& uiView, sf::View& mapView, e_tab& tabs, std::vector <std::vector <Tile> >& tiles)
{
	switch (tabs) {
	case e_tab::UNITS:
		drawMapToWindow(window, mapView, tiles);
		break;
	case e_tab::RESEARCH:
		drawResearchToWindow(window, uiView);
		break;
	case e_tab::PRODUCTION:
		//drawTileToWindow(window, uiView);
		break;
	case e_tab::LOGISTICS:
		//drawFactoryToWindow(window, uiView);
		break;
	case e_tab::BUILDING:
		break;
	case e_tab::OPTIONS:
		break;
	}
	drawHudToWindow(window, hudView);
}

void Renderer::drawHudToWindow(sf::RenderWindow& window, sf::View& hudView)
{
	window.setView(hudView);
	window.draw(m_hudBackground);
	window.draw(m_allegiance1);
	window.draw(m_allegiance2);
	window.draw(m_allegianceCP1);
	window.draw(m_allegianceCP2);
	for (int i = 0; i < 6; i++) {
		window.draw(m_tabButtons[i]);
	}
	for (int i = 0; i < 6; i++) {
		window.draw(m_tabs[i]);
	}
	m_time.setString(Renderer::secondsToDateAndTime(runTime));
	window.draw(m_time);
}

void Renderer::drawMapToWindow(sf::RenderWindow& window, sf::View& mapView, std::vector <std::vector <Tile> >& tiles)
{
	window.setView(mapView);
	for (int i = 0; i < int(m_tilesNums.x); i++) {
		for (int j = 0; j < int(m_tilesNums.y); j++) {
			window.draw(tiles[i][j].getTileSprite());
			if (tiles[i][j].hasFactory())window.draw(tiles[i][j].getFactorySprite());;
			window.draw(tiles[i][j].getTerrainSprite());
		}
	}
}

void Renderer::drawResearchToWindow(sf::RenderWindow& window, sf::View& uiView)
{
	window.setView(uiView);
	window.draw(m_researchBackground);
	for (auto& res : infantryRes) {
		window.draw(res.getEquipmentBackground());
		window.draw(res.getEquipment().getEquipmentSprite());
	}
	for (auto& res : tankRes) {
		window.draw(res.getEquipmentBackground());
		window.draw(res.getEquipment().getEquipmentSprite());
	}
	for (auto& res : antiTankRes) {
		window.draw(res.getEquipmentBackground());
		window.draw(res.getEquipment().getEquipmentSprite());
	}
}

std::string Renderer::secondsToDateAndTime(float sec)
{
	e_tab res = e_tab(1);
	sec = sec * m_TIME_DILATION;
	float days = (sec / 86400.f);
	int year = 1;
	std::stringstream ss;

	while ((year % 4 != 0 && days >= 365) || (year % 4 == 0 && days >= 366)) {
		days -= 365;
		days = (year % 4 == 0) ? (days - 1) : days;
		year++;
	}

	int month = 1;
	bool addedOne = true;
	while (addedOne) {
		switch (month) {
		case 1:
			addedOne = (days > 31.f);
			days = addedOne ? (days - 31.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		case 2:
			if (year % 4 == 0) {
				addedOne = (days > 29.f);
				days = addedOne ? (days - 29.f) : days;
				month = addedOne ? (month + 1) : month;
				break;
			}
			else {
				addedOne = (days > 28.f);
				days = addedOne ? (days - 28.f) : days;
				month = addedOne ? (month + 1) : month;
				break;
			}
		case 3:
			addedOne = (days > 31.f);
			days = addedOne ? (days - 31.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		case 4:
			addedOne = (days > 30.f);
			days = addedOne ? (days - 30.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		case 5:
			addedOne = (days > 31.f);
			days = addedOne ? (days - 31.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		case 6:
			addedOne = (days > 30.f);
			days = addedOne ? (days - 30.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		case 7:
			addedOne = (days > 31.f);
			days = addedOne ? (days - 31.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		case 8:
			addedOne = (days > 31.f);
			days = addedOne ? (days - 31.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		case 9:
			addedOne = (days > 30.f);
			days = addedOne ? (days - 30.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		case 10:
			addedOne = (days > 31.f);
			days = addedOne ? (days - 31.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		case 11:
			addedOne = (days > 30.f);
			days = addedOne ? (days - 30.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		case 12:
			addedOne = (days > 31.f);
			days = addedOne ? (days - 31.f) : days;
			month = addedOne ? (month + 1) : month;
			break;
		}
	}
	int hours = (days - float(int(days))) / (1.f / 24.f);
	ss << m_monthStrings[month - 1] << " " << int(days) + 1 << " " << (year + 1940) << "\n    " << hours << ":00";
	return ss.str();
}