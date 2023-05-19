#include "Head.h"

std::weak_ptr<ProtoPlayer> Head::playerPtr = std::make_shared<ProtoPlayer>();

void Head::setPlayerPtr(std::shared_ptr<ProtoPlayer> ptr)
{
	playerPtr = ptr;
}
