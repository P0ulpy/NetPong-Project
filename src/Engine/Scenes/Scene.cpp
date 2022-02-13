#include "Scene.hpp"

Scene::Scene(PoPossibEngin& parentEngine, const SceneConfig& sceneInitData)
    : _poPossibEngin(&parentEngine)
{

}

Scene::~Scene() = default;

PoPossibEngin& Scene::getParentEngine() const
{
	return *_poPossibEngin;
}