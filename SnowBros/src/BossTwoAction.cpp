/*
 * BossTwoAction.cpp
 *
 *  Created on: 23 ago 2019
 *      Author: laolr
 */

#include "BossTwoAction.h"

BossTwoAction::BossTwoAction() {
	morenteCounter = 0;
	mortoCounter = 0;
	sparaCounter = 0;
}

BossTwoAction::~BossTwoAction() {
}

void BossTwoAction::onAction(bool* key) {
	Actor* tmp = dynamic_cast<Actor*>(entity);

	tmp->setFalling(true);

	if (tmp->isFalling())
	{
		tmp->setPosY(tmp->getPosY() + tmp->get_f_speed());
	}

	if (tmp->getLastDirection() == SPAWN)
	{
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("fermo");
	}

	if (tmp->getVita() <= 0 && morenteCounter <= 24)
	{
		if (!(tmp->getLastDirection() == MORTO))
			tmp->getSpritesheetManager()->selectCurrentSpritesheet("morente");
		tmp->setLastDirection(MORTO);
		tmp->setSpinge(true);
		morenteCounter++;
	}

	if (morenteCounter > 24)
	{
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("morto");
		mortoCounter++;
	}

	if (mortoCounter > 12)
	{
		tmp->setDestroyed(true);
		EnemyCounter* enemyCounter = EnemyCounter::getinstance();
		for (int i = 0; i < enemyCounter->getEnemiesNumber(); i++)
			enemyCounter->decCounter();
		PlayerScore* playerScore = PlayerScore::getInstance();
		playerScore->addScore(2000);
	}

	if (sparaCounter == 0)
	{
		tmp->setLastDirection(SHOOTING);
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("spara");
		EnemyCounter* enemyCounter = EnemyCounter::getinstance();
		if (enemyCounter->getEnemiesNumber() <= 1)
		{
			tmp->getSpawner()->spawnEntity("M2", tmp->getPosX() + 38, tmp->getPosY());
			tmp->getSpawner()->spawnEntity("M2", tmp->getPosX() + tmp->getDim()->x - 38, tmp->getPosY());
		}
	}

	if (sparaCounter == 10)
	{
		tmp->setLastDirection(SPAWN);
	}

	if (sparaCounter > 48)
	{
		sparaCounter = -1;
	}

	tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
	if (tmp->getLastDirection() != MORTO)
		sparaCounter++;
}

Action* BossTwoAction::clone() { return new BossTwoAction(); }
