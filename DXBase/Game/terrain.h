#ifndef _TERRAIN_H_
#define _TERRAIN_H_
#include "CMOGO.h"
//a terrain CMOGO that just sits there

class Terrain : public CMOGO
{
public:
	Terrain(string _fileName, ID3D11Device* _pd3dDevice, MyEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale );
	~Terrain();

protected:

	void Tick(GameData* GD){};
};

#endif