#ifndef _DRAW_STATE_H_
#define _DRAW_STATE_H_
#include "MyEffectsFactory.h"
#include "CommonStates.h"

class Camera;
class Light;

struct DrawData
{
	ID3D11DeviceContext* pd3dImmediateContext;
	CommonStates* states; 
	Camera* cam;
	Light* light;

};

#endif
