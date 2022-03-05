#include "Entity.h"
#include "SingletonTemplate.h"
#include <iostream>
#include <list>

class CEntityManager : public CSingletonTemplate<CEntityManager>
{
	friend class CSingletonTemplate<CEntityManager>;
public:
	virtual void Init(void);

	// Add a CEntity3D* to this class instance
	virtual void Add(CEntity* cEntity);
	// Remove a CEntity3D* from this class instance
	virtual bool Erase(CEntity* cEntity);

	// Collision Check for a CEntity3D*
	virtual bool CollisionCheck(CEntity* cEntity);

	// Update this class instance
	virtual void Update(const double dElapsedTime);

	// CleanUp all CEntity3Ds which are marked for deletion
	virtual void CleanUp(void);

protected:
	CEntityManager(void);
	virtual ~CEntityManager();

	std::list<CEntity*> lEntity;
};
