#pragma once

#include "SingletonTemplate.h"


class CSettings : public CSingletonTemplate<CSettings>
{
	friend CSingletonTemplate<CSettings>;
public:
	// File information
	char* logl_root;	// Root directory for loading digital assets

protected:
	// Constructor
	CSettings(void);

	// Destructor
	virtual ~CSettings(void);
};

