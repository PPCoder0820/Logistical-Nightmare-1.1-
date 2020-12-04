#include"pch.h"
#include"Tile.h"
#include"Renderer.h"
#include<iostream>
Tile::Tile(int terrain, sf::Vector2i pos)
{
	m_terrainSprite.setPosition(Renderer::m_TILE_SIZE * pos.x, Renderer::m_TILE_SIZE * pos.y + Renderer::m_TILE_SIZE / 2);
	m_terrainSprite.setTexture(TextureHolder::getTexture("graphics/" + terrainNumToString(terrain) + ".png"));
	m_terrainSprite.setScale(0.5f, 0.5f);

	m_factorySprite.setPosition(Renderer::m_TILE_SIZE * pos.x, Renderer::m_TILE_SIZE * pos.y);
	m_factorySprite.setTexture(TextureHolder::getTexture("graphics/factory.png"));
	m_factorySprite.setScale(0.5f, 0.5f);

	m_tileSprite.setPosition(Renderer::m_TILE_SIZE * pos.x, Renderer::m_TILE_SIZE * pos.y);
	m_tileSprite.setTexture(TextureHolder::getTexture("graphics/map_background.png"));
	m_tileSprite.setScale(0.5f, 0.5f);

	m_terrain = terrain;
	m_hasFactory = false;
	m_inProduction = "ConstructionPoints";
}

bool Tile::hasFactory() const
{
	return m_hasFactory;
}

void Tile::addFactory()
{
	m_hasFactory = true;
}

void Tile::setEquipmentInProduction(std::string equipmentName, float productionCost)
{
	m_productionCost = productionCost;
	m_inProduction = equipmentName;
}

std::pair <std::string, int> Tile::update(float time)
{
	m_produced += time * (m_factoryOutput / m_productionCost);
	int latestBatch = int(m_produced) - m_producedBefore;
	m_producedBefore = int(m_produced);
	std::pair<std::string, int> result = make_pair(m_inProduction, latestBatch);
	return result;
}

std::string Tile::terrainNumToString(int terrain)
{
	if (terrain == 0) {
		return "plains";
	}
	else if (terrain == 1) {
		return "mountain";
	}
}

sf::Sprite& Tile::getTileSprite()
{
	return m_tileSprite;
}

sf::Sprite& Tile::getFactorySprite()
{
	return m_factorySprite;
}

sf::Sprite& Tile::getTerrainSprite()
{
	return m_terrainSprite;
}
