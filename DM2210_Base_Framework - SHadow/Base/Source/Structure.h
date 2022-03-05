#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Entity.h"

class CStructure : public CEntity
{
public:
	virtual void Init();

	virtual void Update(double dt);

	CStructure();
	~CStructure();
};

#endif