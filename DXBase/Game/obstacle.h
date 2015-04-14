#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_
#include "VBGO.h"
#include "vertex.h"
#include "drawdata.h"
#include <dinput.h>
#include "gamedata.h"


class obstacle : public VBGO
{
public:
	obstacle(){};
	virtual ~obstacle(){};
	//initialise the Veretx and Index buffers for the cube
	void init(int _size, ID3D11Device* _GD);

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see Spirla, SpikedVB and Pillow
	//void Draw(DrawData* _DD);
	void Transform(GameData* _GD);
	void Tick(GameData* _GD);
	void input(GameData* _GD);

	float changeX = 0.0f;
	float changeY = 0.0f;
	int m_size;
	myVertex* m_vertices;
	int m_numVertices;
};

#endif