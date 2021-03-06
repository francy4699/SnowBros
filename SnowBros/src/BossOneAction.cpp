/*
 * BossOneAction.cpp
 *
 *  Created on: 21 ago 2019
 *      Author: laolr
 */

#include "BossOneAction.h"

BossOneAction::BossOneAction() {
	saltaCounter = 0;
	pocoMortoCounter = 0;
	quasiMortoCounter = 0;
	mortoCounter = 0;
}

BossOneAction::~BossOneAction() {
}

void BossOneAction::onAction(bool* key) {
	Actor* tmp = dynamic_cast<Actor*>(entity);

	tmp->setMaxHeight(0);

	if (saltaCounter == 0)
	{
		tmp->setImmobile(true);
		tmp->setFalling(false);
		tmp->setJumping(true);
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("saltaL");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		EnemyCounter* enemyCounter = EnemyCounter::getinstance();
		if (enemyCounter->getEnemiesNumber() <= 1)
		{
			tmp->getSpawner()->spawnEntity("M1", tmp->getPosX(), tmp->getPosY() + 100);
			tmp->getSpawner()->spawnEntity("M1", tmp->getPosX(), tmp->getPosY());
		}
	}

	if (tmp->getPosY() >= 102)
	{
		tmp->setImmobile(false);
	}

	if (saltaCounter == 50)
	{
		tmp->setFalling(false);
		tmp->setJumping(true);
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("saltaL");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		EnemyCounter* enemyCounter = EnemyCounter::getinstance();
		if (enemyCounter->getEnemiesNumber() <= 1)
		{
			tmp->getSpawner()->spawnEntity("M1", tmp->getPosX(), tmp->getPosY() - 100);
			tmp->getSpawner()->spawnEntity("M1", tmp->getPosX(), tmp->getPosY());
		}
	}

	if (tmp->isFalling() && saltaCounter >= 0)
	{
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("fermoL");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
	}


	if (tmp->isFalling())
	{
		tmp->setPosY(tmp->getPosY() + tmp->get_f_speed());
	}

	if (tmp->isJumping())
	{
		tmp->setPosY(tmp->getPosY() - tmp->get_j_speed());
	}

	if (tmp->getPosY() <= tmp->getMaxHeight())
	{
		tmp->setJumping(false);
		tmp->setCanJump(false);
		tmp->setFalling(true);
	}

	if (tmp->getVita() <= 0 && pocoMortoCounter <= 12)
	{
		saltaCounter = -1;
		tmp->setPosY(102);
		tmp->setSpinge(true);
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("pocoMortoL");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		pocoMortoCounter++;
	}

	if (pocoMortoCounter > 12 && quasiMortoCounter <= 12)
	{
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("quasiMortoL");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		quasiMortoCounter++;
	}

	if (quasiMortoCounter > 12 && mortoCounter <= 12)
	{
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("mortoL");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
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

	if (saltaCounter >= 0)
		saltaCounter++;

	if (saltaCounter > 100)
		saltaCounter = 0;
}

Action* BossOneAction::clone() { return new BossOneAction(); }
