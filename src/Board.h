#pragma once

#include <graphx.h>
#include <compression.h>

class Board
{
public:
	Board():tiles(nullptr),tilemap(){}
	Board(gfx_sprite_t* tiles, uint8_t* map, gfx_sprite_t** tilesP, uint8_t width, uint8_t height, uint8_t tileSize, uint8_t drawWidth, uint8_t drawHeight, uint8_t mapWidth, uint8_t mapHeight, uint32_t x, uint8_t y, bool compressed, unsigned char* compressedTiles, unsigned char* compressedMap);
	void debugDraw();
	void draw(bool clipping);
	void setOffset(int x, int y);
	uint32_t getOffset();
private:
	gfx_tilemap_t tilemap;
	gfx_sprite_t* tiles;
	uint16_t x, y;
};

