/*
 * PlayerAction.h
 *
 *  Created on: 19 apr 2019
 *      Author: francesco
 */

#ifndef PLAYERACTION_H_
#define PLAYERACTION_H_

#include "Action.h"
#include "Player.h"

class PlayerAction: public Action {
public:
	PlayerAction();
	PlayerAction(Player*);
	virtual ~PlayerAction();
	virtual void onAction(bool*) override;
};

#endif /* PLAYERACTION_H_ */