#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../Engine.hpp"

struct SceneConfig
{
	
};

class Scene
{
public:
	Scene(Engine* parentEngine, SceneConfig sceneInitData);
	~Scene();
	
private:
	Engine* parentEngine = nullptr;
};

#endif //ENGINE_HPP