/*
 * TileDescriptor.h
 *
 *  Created on: 23 apr 2019
 *      Author: francesco
 */

#ifndef TILEDESCRIPTOR_H_
#define TILEDESCRIPTOR_H_

#include "EntityDescriptor.h"
#include "Tile.h"

class TileDescriptor: public EntityDescriptor {
public:
	TileDescriptor(Dimensions* d, ALLEGRO_BITMAP* b) : EntityDescriptor(d, b, nullptr, nullptr, nullptr, nullptr,"Tile") { }
	virtual ~TileDescriptor() {}

	Entity* getDescripted(float x, float y) override
	{
		return new Tile(x, y, dim, bitmap, type);
	}
};

#endif /* TILEDESCRIPTOR_H_ */
