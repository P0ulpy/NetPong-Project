#ifndef SCENE_CPP
#define SCENE_CPP

#include "Scene.hpp"

Scene::Scene(PoPossibEngin& parentEngine, const SceneConfig& sceneInitData) : _poPossibEngin(&parentEngine)
{

}

Scene::~Scene()
{

}

PoPossibEngin& Scene::getParentEngine() const
{
	return *_poPossibEngin;
}

#endif //SCENE_CPP