#pragma once

#include <vector>

#include "PoPossibEngin.h"

#include "SFML/Graphics.hpp"

class PoPossibEngin;

class Scene
{
private:

protected:
	PoPossibEngin* _poPossibEngin;
	std::vector<sf::Texture> _textures;

public:
	Scene(PoPossibEngin& poPossibEngin);
	virtual ~Scene();

	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};
