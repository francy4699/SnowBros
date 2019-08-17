/*
 * EnemyCollisionHandler.h
 *
 *  Created on: 17 ago 2019
 *      Author: laolr
 */

#ifndef ENEMYCOLLISIONHANDLER_H_
#define ENEMYCOLLISIONHANDLER_H_

#include "CollisionHandler.h"
#include "Actor.h"

class EnemyCollisionHandler: public CollisionHandler {
public:
	EnemyCollisionHandler();
	virtual ~EnemyCollisionHandler();
	virtual bool handleCollision(Entity* other) override;
	virtual CollisionHandler* clone() override;
};

#endif /* ENEMYCOLLISIONHANDLER_H_ */
