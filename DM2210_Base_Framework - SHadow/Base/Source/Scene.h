#ifndef SCENE_H
#define SCENE_H

#include <String>
#include <iostream>

class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual bool Init() = 0;
	virtual bool InitSections(double& dValue, double dMaxValue, std::string& sRenderMessage) = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	virtual void DeleteInstances() = 0;

	virtual void RenderNextProgressBar(double dt, double& dValue, double dMaxValue, std::string sRenderMessage) = 0;
};

#endif