#pragma once
#include <TL-Engine.h>

#define Going_Up 0
#define Going_Down 1

using namespace tle;
class CHover
{
protected:
	IModel* m_hover;
	float m_speed = 0;
	float m_dt;
	float m_floatSpeed = 0;
	int m_floatState = Going_Up;
	int m_radius = 4;
	I3DEngine* m_myEngine;
public:
	//Hover acting like a hover (slowly moves up and down)
	void Wobble(float m_dt);
	IModel* GetModel();
	int GetRadius();
	
	CHover();
	~CHover();
};