#pragma once

#include <NY_Object3D.h>
#include <memory>

#include "Head.h"

class Head;

class ProtoPlayer
{
public:
	ProtoPlayer();
	~ProtoPlayer();

	void Init();

	void Update(std::shared_ptr<Head> head);

	void Draw();

	void UIDraw();

	void Cut(std::shared_ptr<Head> head);

	void Slap(std::shared_ptr<Head> head);




private:





};

