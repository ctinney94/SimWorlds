//add some light to the scene
#include "light.h"
#include "gamedata.h"
//a simple light defined by a colour and and ambient colour and used as a point light with no drop off in the standard shader used to draw VBGOs

Light::Light(Vector3 _pos, Color _colour, Color _ambientColour)
{
	m_pos = _pos;
	m_colour = _colour;
	m_ambientColour = _ambientColour;
}

Light::~Light()
{
}

void Light::Tick(GameData* _GD)
{
	//not really needed but spins the camera around to show off the lambert default VBGO shader
	static float time = 0.0f;
	time += _GD->dt;
	m_pos.x = 100.0f * cos(time);
	m_pos.z = 100.0f * sin(time);
}