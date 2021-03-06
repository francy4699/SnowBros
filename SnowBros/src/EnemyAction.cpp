/*
 * EnemyAction.cpp
 *
 *  Created on: 17 ago 2019
 *      Author: laolr
 */

#include "EnemyAction.h"
#include <iostream>

EnemyAction::EnemyAction() {
	mortoCounter = 0;
	sparaCounter = 0;
	soundBank = SoundBank::getInstance();
}

EnemyAction::~EnemyAction() {
}

void EnemyAction::onAction(bool* key)
{
	Actor* tmp = dynamic_cast<Actor*>(entity);

	if (tmp->getType() == "MinionOne" && tmp->isSpawning() && !tmp->getImmobile())
	{
		if (tmp->getPosX() > 20)
			tmp->setPosX(tmp->getPosX() - 10);
		else
			tmp->setSpawning(false);
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("volaL");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		tmp->setImmobile(false);
		return;
	}

	if (tmp->getType() == "MinionTwo" && tmp->isSpawning() && !tmp->getImmobile() && tmp->getPosX() < screenWidth / 2)
	{
		if (tmp->getPosX() > 20)
		{
			tmp->setPosX(tmp->getPosX() - 10);
			tmp->setPosY(tmp->getPosY() - 5);
		}
		else
			tmp->setSpawning(false);
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("volaL");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		tmp->setImmobile(false);
		return;
	}

	else if (tmp->getType() == "MinionTwo" && !tmp->getImmobile() && tmp->isSpawning() && tmp->getPosX() > screenWidth / 2)
	{
		if (tmp->getPosX() < screenWidth - 20)
		{
			tmp->setPosX(tmp->getPosX() + 10);
			tmp->setPosY(tmp->getPosY() - 5);
		}
		else
			tmp->setSpawning(false);
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("volaR");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		tmp->setImmobile(false);
		return;
	}

	if (tmp->getImmobile() && (tmp->getLastDirection() == NO_DIRECTION || tmp->getLastDirection() == NO_DIRECTION_LEFT || tmp->getLastDirection() == NO_DIRECTION_RIGHT) && tmp->isSpawning())
	{
		tmp->setFalling(true);

		if (tmp->isFalling()) {
			tmp->setPosY(tmp->getPosY() + tmp->getSpeed());
		}

		if(mortoCounter > 10)
		{
			tmp->setDestroyed(true);
			// int x = rand() % 10;
			// if (x == 0)
				tmp->getSpawner()->spawnEntity("G", tmp->getPosX(), tmp->getPosY());
			PlayerScore* p = PlayerScore::getInstance();
			p->addScore(500);
		}
		if(mortoCounter >= 0 && mortoCounter != 10)
		{
			if(tmp->getLastDirection() == NO_DIRECTION_LEFT)
			{
				tmp->getSpritesheetManager()->selectCurrentSpritesheet("mortoL");
				tmp->setLastDirection(NO_DIRECTION);
			}
			else if (tmp->getLastDirection() == NO_DIRECTION_RIGHT)
			{
				tmp->getSpritesheetManager()->selectCurrentSpritesheet("mortoR");
				tmp->setLastDirection(NO_DIRECTION);
			}
			tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		}
		mortoCounter++;
		return;
	}

	randAction action;
	if (!tmp->getImmobile())
	{
		cooldown = 0;
		int x = rand() % 4;
		switch (x) {
			case 0:
				action = left;
				break;
			case 1:
				action = right;
				break;
			case 2:
				if (tmp->getType() == "MinionOne")
					action = fermo;
				else
					action = up;
				break;
			case 3:
				if (tmp->getType() == "Enemy2" && sparaCounter > 12)
				{
					sparaCounter = 0;
					action = shoot;
				}
				else
					action = fermo;
				break;
		}
		sparaCounter++;
	}
	else
	{
		if (cooldown > 24)
		{
			if (tmp->getLivelloPalla() == POCA)
			{
				tmp->setLivelloPalla(NULLA);
				tmp->setImmobile(false);
			}
			else if (tmp->getLivelloPalla() == TANTA)
			{
				tmp->setLivelloPalla(POCA);
			}
			else if (tmp->getLivelloPalla() == PIENA)
			{
				tmp->setLivelloPalla(TANTA);
			}
			cooldown = 0;
		}
		cooldown++;
		action = fermo;
	}

	tmp->setSpawning(false);
	tmp->setShooting(false);

	if (!tmp->isJumping())
	{
		tmp->setMaxHeight(tmp->getPosY() - 32);
	}

	if(action == shoot)
	{
		if((!tmp->isShooting() && tmp->getLastDirection() == RIGHT) || (!tmp->isShooting() && tmp->getLastDirection() == NO_DIRECTION_RIGHT) || (!tmp->isShooting() && tmp->getLastDirection() == JUMPING_RIGHT) || (!tmp->isShooting() && tmp->getLastDirection() == FALLING_RIGHT))
		{
			tmp->getSpritesheetManager()->selectCurrentSpritesheet("sparaR");
			tmp->setLastDirection(SHOOTING_RIGHT);
		}
		else if((!tmp->isShooting() && tmp->getLastDirection() == LEFT) || (!tmp->isShooting() && tmp->getLastDirection() == NO_DIRECTION_LEFT) || (!tmp->isShooting() && tmp->getLastDirection() == JUMPING_LEFT) || (!tmp->isShooting() && tmp->getLastDirection() == FALLING_LEFT))
		{
			tmp->getSpritesheetManager()->selectCurrentSpritesheet("sparaL");
			tmp->setLastDirection(SHOOTING_LEFT);
		}

		if(!tmp->isJumping() && !tmp->isFalling())
		{
			tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
			tmp->setShooting(true);
			if (tmp->getLastDirection() == SHOOTING_LEFT)
				tmp->getSpawner()->spawnEntity("FL", tmp->getPosX(), tmp->getPosY());
			if (tmp->getLastDirection() == SHOOTING_RIGHT)
				tmp->getSpawner()->spawnEntity("FR", tmp->getPosX() + tmp->getDim()->x, tmp->getPosY());
		}
	}

	if(!(action == shoot))
	{
		tmp->setShooting(false);
	}

	if (action == left && tmp->getPosX() >= tmp->getSpeed())
	{
		if(!tmp->isShooting() && !tmp->isJumping())
		{
			if(!(tmp->getLastDirection() == LEFT))
				tmp->getSpritesheetManager()->selectCurrentSpritesheet("camminaL");
			tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
			tmp->setLastDirection(LEFT);
		}
		else if(tmp->isShooting() && tmp->getLastDirection() == SHOOTING_RIGHT)
		{
			tmp->getSpritesheetManager()->selectCurrentSpritesheet("sparaL");
			tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
			tmp->setLastDirection(SHOOTING_LEFT);
		}
		else
		{
			tmp->setLastDirection(SHOOTING_LEFT);
		}

		tmp->setPosX(tmp->getPosX() - tmp->getSpeed());
		tmp->setMoving(true);
	}

	if (action == right && tmp->getPosX() <= screenWidth - tmp->getDim()->x - tmp->getSpeed()) {
		if(!tmp->isShooting() && !tmp->isJumping())
		{
			if(!(tmp->getLastDirection() == RIGHT))
				tmp->getSpritesheetManager()->selectCurrentSpritesheet("camminaR");
			tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
			tmp->setLastDirection(RIGHT);
		}
		else if(tmp->isShooting() && tmp->getLastDirection() == SHOOTING_LEFT)
		{
			tmp->getSpritesheetManager()->selectCurrentSpritesheet("sparaR");
			tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
			tmp->setLastDirection(SHOOTING_RIGHT);
		}
		else
		{
			tmp->setLastDirection(SHOOTING_RIGHT);
		}

		tmp->setPosX(tmp->getPosX() + tmp->getSpeed());
		tmp->setMoving(true);
	}

	if (action == up && tmp->isCanJump()) {
		tmp->setJumping(true);
		tmp->setFalling(false);
		tmp->setCanJump(false);
		if(tmp->getLastDirection() == RIGHT || tmp->getLastDirection() == NO_DIRECTION_RIGHT || tmp->getLastDirection() == SHOOTING_RIGHT)
		{
			tmp->getSpritesheetManager()->selectCurrentSpritesheet("saltaR");
			tmp->setLastDirection(JUMPING_RIGHT);
		}
		else if(tmp->getLastDirection() == LEFT || tmp->getLastDirection() == NO_DIRECTION_LEFT || tmp->getLastDirection() == SHOOTING_LEFT)
		{
			tmp->getSpritesheetManager()->selectCurrentSpritesheet("saltaL");
			tmp->setLastDirection(JUMPING_LEFT);
		}
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		tmp->setShooting(false);
		tmp->setMoving(true);
	}

	if(!tmp->isJumping())
		tmp->setFalling(true);

	if (tmp->getPosY() <= tmp->getMaxHeight())
	{
		tmp->setJumping(false);
		tmp->setCanJump(false);
		tmp->setFalling(true);
	}

	if (tmp->isFalling() && tmp->getPosY() <= screenHeight - tmp->getDim()->y - tmp->get_f_speed()) {
		tmp->setPosY(tmp->getPosY() + tmp->get_f_speed());
		tmp->setCanFall(false);
		tmp->setCanJump(false);
	}

	if (tmp->isJumping() && !tmp->isFalling()) {
		tmp->setPosY(tmp->getPosY() - tmp->get_j_speed());
		tmp->setMoving(true);
	}

	if((!tmp->isMoving() && tmp->getLastDirection() == RIGHT) || tmp->getLastDirection() == NO_DIRECTION || (!tmp->isMoving() && !tmp->isShooting() && tmp->getLastDirection() == NO_DIRECTION_RIGHT) || (!tmp->isMoving() && !tmp->isShooting() && tmp->getLastDirection() == SHOOTING_RIGHT) || (!tmp->isMoving() && !tmp->isShooting() && tmp->getLastDirection() == JUMPING_RIGHT) || (!tmp->isMoving() && !tmp->isShooting() && tmp->getLastDirection() == FALLING_RIGHT))
	{
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("fermoR");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		tmp->setLastDirection(NO_DIRECTION_RIGHT);
	}
	else if ((!tmp->isMoving() && tmp->getLastDirection() == LEFT) || (!tmp->isMoving() && !tmp->isShooting() && tmp->getLastDirection() == NO_DIRECTION_LEFT) || (!tmp->isMoving() && !tmp->isShooting() && tmp->getLastDirection() == SHOOTING_LEFT) || (!tmp->isMoving() && !tmp->isShooting() && tmp->getLastDirection() == JUMPING_LEFT) || (!tmp->isMoving() && !tmp->isShooting() && tmp->getLastDirection() == FALLING_LEFT))
	{
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("fermoL");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
		tmp->setLastDirection(NO_DIRECTION_LEFT);
	}

	if (tmp->getLastDirection() == ROTOLA_LEFT)
	{
		if (tmp->getPosX() - tmp->getSpeed() >= 0)
			tmp->setPosX(tmp->getPosX() - tmp->getSpeed());
		else
		{
			tmp->setLastDirection(ROTOLA_RIGHT);
			tmp->setPosX(1);
		}
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
	}
	else if (tmp->getLastDirection() == ROTOLA_RIGHT)
	{
		if (tmp->getPosX() + tmp->getDim()->x + tmp->getSpeed() <= screenWidth)
			tmp->setPosX(tmp->getPosX() + tmp->getSpeed());
		else
		{
			tmp->setLastDirection(ROTOLA_LEFT);
			tmp->setPosX(screenWidth - tmp->getDim()->x - 1);
		}
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
	}

	if (tmp->getLivelloPalla() == ROTOLA && tmp->getPosY() >= 176 && (tmp->getPosX() - tmp->getSpeed() <= 0 || tmp->getPosX() + tmp->getDim()->x + tmp->getSpeed() >= screenWidth))
	{
		soundBank->playSample("delball");
		tmp->setDestroyed(true);
		PlayerScore* p = PlayerScore::getInstance();
		p->addScore(400);
	}

	tmp->setMoving(false);

	if (tmp->getLivelloPalla() == POCA)
	{
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("poca");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
	}
	if (tmp->getLivelloPalla() == TANTA)
	{
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("tanta");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
	}
	if (tmp->getLivelloPalla() == PIENA)
	{
		tmp->getSpritesheetManager()->selectCurrentSpritesheet("piena");
		tmp->getSpritesheetManager()->nextSprite(tmp->getBitmap());
	}
}

Action* EnemyAction::clone() { return new EnemyAction(); }
