#ifndef _TURRET_BASE_H_
#define _TURRET_BASE_H_
#include "CMOGO.h"
//I suppose we shoudl really call this the main palyer character

class Turret_Base : public CMOGO
{
public:
	Turret_Base(string _fileName, ID3D11Device* _pd3dDevice, MyEffectFactory* _EF);
	~Turret_Base();

	void Tick(GameData* GD);

protected:

};

#endif