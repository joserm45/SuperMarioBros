#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2DynArray.h"


// ----------------------------------------------------
struct Layer
{
	Layer() {};
	p2SString name;
	uint width = 0;
	uint height = 0;
	uint* tiles = nullptr;
	uint size_tiles = 0;
	bool iscollision = false;
	float parallax_speed = 0;

	inline uint Get(uint x, uint y) const
	{
		if (x >= width || y >= height)
		{
			LOG("Invalid tile position");
			return 0;
		}
		uint position = (width * y) + x;
		return tiles[position];
	}

	~Layer()
	{
		RELEASE_ARRAY(tiles);
	};
};


struct Tile
{
	uint gid = 0;
	bool iscollision = false;

};

// ----------------------------------------------------
struct TileSet
{
	
	SDL_Rect GetTileRect(int id) const;
	//Tilset();
	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
	p2DynArray<Tile>	tile_data;
	//uint				tile_count;
	bool GetTileCollision(uint gid)const;
	~TileSet()
	{
		SDL_DestroyTexture(texture);
		//RELEASE_ARRAY(tile_data);
	}
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<Layer*>		layers;

};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	void Clear();

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y)const;

	Layer* GetCollisionLayer()const;
private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	void MapPositionPlayer();
	
	Layer* LoadLayer(pugi::xml_node& node);


public:

	MapData map_data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__