
#pragma once
#include "Entity.h"

#include "Vector3.h"

#include "Player.h"

#include "CVirus.h"

class CPickUp : public CEntity
{
public:
	enum PICKUP_TYPE
	{
		MUTATION_PTS = 0,
		VIRUS_LAUNCHER,
		AIR_FRESHENER,
		NUM_PICKUP,
	};

	// Default Constructor
	CPickUp(void);

	// Constructor with vectors
	CPickUp(	const Vector3 vec3Position,
				const Vector3 vec3Front = Vector3(0.0f, 0.0f, -1.0f));

	// Destructor
	virtual ~CPickUp(void);

	// Initialise this class instance
	bool Init(void);

	// Set pickup type
	virtual void SetPickUpType(PICKUP_TYPE PUType);

	// Get pickup type
	virtual CPickUp::PICKUP_TYPE GetPickUpType(void) const;

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// Activate the CCollider for this class instance
	virtual void ActivateCollider(void);

	virtual void Effect(void) const;


protected:
	CPickUp::PICKUP_TYPE PUType;
};
