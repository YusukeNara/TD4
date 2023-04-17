#pragma once
#include <string>

#include "RVector.h"

class BaseCollider;
class Object3d;
class GameObject;

class ColliderInfo
{
public:
	ColliderInfo(RVector3 inter,std::string tag);

	//è’ìÀì_
	RVector3 inter;
	//É^ÉOñº
	std::string tag;

};

