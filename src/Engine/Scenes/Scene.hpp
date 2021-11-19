#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Graphics.hpp>
#include "../../Engine/Engine.hpp"

class PoPossibEngin;

struct SceneConfig
{
	 
};

class Scene
{
public:
	Scene(PoPossibEngin& parentEngine, const SceneConfig& sceneInitData);
	virtual ~Scene();

	virtual void start() = 0;
	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget* renderTarget = nullptr) = 0;

	[[nodiscard]] PoPossibEngin& getParentEngine() const;

protected:
	PoPossibEngin* _poPossibEngin;
	std::vector<sf::Texture> _textures;
};

#endif //ENGINE_HPP