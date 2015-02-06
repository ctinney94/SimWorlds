#ifndef _SNAIL_H_
#define _SNAIL_H_
#include "VBGO.h"
#include <string>

struct GameData;

class Snail : public VBGO
{
public:
	Snail( ID3D11Device* _GD, std::string _filename, int _sections, float _scale, float _rot, float _step, Color _col1, Color _col2 );
	virtual ~Snail(){};

	virtual void Tick(GameData* _GD);

protected:
};

#endif