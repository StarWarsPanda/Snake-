#include "Board.h"

Board::Board(gfx_sprite_t* tiles, uint8_t* map, gfx_sprite_t** tilesP, uint8_t width, uint8_t height, uint8_t tileSize, uint8_t drawWidth, uint8_t drawHeight, uint8_t mapWidth, uint8_t mapHeight, uint32_t x, uint8_t y, bool compressed, unsigned char* compressedTiles, unsigned char* compressedMap)
	:tiles(tiles),x(0),y(0)
{
	if (compressed)
	{
		for (unsigned int i = 0; i < sizeof(tilesP) / sizeof(gfx_sprite_t*); i++)
		{
			tiles = gfx_MallocSprite(width, height);
			zx7_Decompress(tiles, compressedTiles);
			tilesP[i] = tiles;
		}

		zx7_Decompress(map, compressedMap);
	}

	tilemap.map = map;
	tilemap.tiles = tilesP;
	tilemap.type_width = tileSize;
	tilemap.type_height = tileSize;
	tilemap.tile_width = width;
	tilemap.tile_height = height;
	tilemap.draw_width = drawWidth;
	tilemap.draw_height = drawHeight;
	tilemap.width = mapWidth;
	tilemap.height = mapHeight;
}

void Board::debugDraw()
{
	for (unsigned int i = 0; i < tilemap.tile_width * tilemap.tile_height; i+=tilemap.type_width)
	{
		if (tilemap.map[i])
		{
			gfx_SetColor(tilemap.map[i]);
			gfx_FillRectangle((i % tilemap.tile_width) * tilemap.type_width, (i / tilemap.tile_width) * tilemap.type_height, tilemap.type_width, tilemap.type_height);
		}
	}
}

void Board::draw(bool clipping)
{
	if (clipping)
	{
		gfx_Tilemap(&tilemap, x, y);
	}
	else
	{
		gfx_Tilemap_NoClip(&tilemap, x, y);
	}
}

void Board::setOffset(int x, int y)
{
	this->x = x;
	this->y = y;
}

uint32_t Board::getOffset()
{
	return (uint32_t)(x << 16) + (uint32_t)y;
}