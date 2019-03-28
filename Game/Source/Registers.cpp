//==================================================================-
/*
/file   Registers.cpp
/author Jakob McFarland
/date   1/20/19
/brief

This is the implementation file for all master register functions,
as specified in specification file Registers.h.

*/
//==================================================================-

#include <stdafx.h>
#include "Registers.h"
#include <GameObjectFactory.h>

#include "ColorChange.h"
#include "MonkeyMovement.h"
#include "PlayerShip.h"
#include "ScreenWrap.h"
#include "TimedDeath.h"
#include "PlayerProjectile.h"
#include "CameraFollow.h"


//master component register
void Registers::RegisterComponents()
{
	GameObjectFactory::GetInstance().RegisterComponent<Behaviors::ColorChange>();
	GameObjectFactory::GetInstance().RegisterComponent<Behaviors::MonkeyMovement>();
	GameObjectFactory::GetInstance().RegisterComponent<Behaviors::PlayerShip>();
	GameObjectFactory::GetInstance().RegisterComponent<Behaviors::ScreenWrap>();
	GameObjectFactory::GetInstance().RegisterComponent<Behaviors::TimedDeath>();
	GameObjectFactory::GetInstance().RegisterComponent<Behaviors::PlayerProjectile>();
	GameObjectFactory::GetInstance().RegisterComponent<Behaviors::CameraFollow>();
}
