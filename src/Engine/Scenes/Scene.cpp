#include "Scene.hpp"

Scene::Scene(PoPossibEngin& parentEngine, const SceneConfig& sceneInitData) : _engine(&parentEngine)
{

}

Scene::~Scene()
{

}

PoPossibEngin& Scene::getParentEngine() const
{
	return *_engine;
}