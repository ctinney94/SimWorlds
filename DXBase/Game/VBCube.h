#ifndef _VB_CUBE_H_
#define _VB_CUBE_H_
#include "VBGO.h"
#include "vertex.h"
//procedurally generate a VBGO Cube
//each side be divided in to _size * _size squares (2 triangles per square)

class VBCube : public VBGO
{
public:
	VBCube(){};
	virtual ~VBCube(){};

	//initialise the Veretx and Index buffers for the cube
	void init(int _size, ID3D11Device* _GD);

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see Spirla, SpikedVB and Pillow
	virtual void Transform(){};

	int m_size;
	myVertex* m_vertices;
};

#endif