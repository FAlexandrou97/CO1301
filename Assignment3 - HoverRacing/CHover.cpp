//Floris Alexandrou
#include "CHover.h"
#include <TL-Engine.h>
#include <iostream>


//hover floating FSM
void CHover::Wobble(float m_dt) {
	float wobbleRate = ((float)rand() / (float)(RAND_MAX)) * 0.02;
	m_hover->MoveLocalY(m_floatSpeed*m_dt);
	switch (m_floatState)
	{
	case Going_Up: {
		m_floatSpeed += wobbleRate;
		if (m_floatSpeed >  3)
			m_floatState = Going_Down;
	}
				   break;
	case Going_Down: {
		m_floatSpeed -= wobbleRate;
		if (m_floatSpeed < -3)
			m_floatState = Going_Up;
	}
					 break;
	}
}


IModel* CHover::GetModel() {
	return m_hover;
}

int CHover::GetRadius() {
	return m_radius;
}


CHover::CHover()
{
}


CHover::~CHover()
{
}