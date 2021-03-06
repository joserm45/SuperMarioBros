#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Input.h"

#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	
	//p2List_item<TileSet*>* tile_data = map_data.tilesets.start;
	p2List_item<Layer*>* layer_data = map_data.layers.start;

	while (layer_data)
	{
		for (uint x = 0; x < layer_data->data->width; x++)
		{
			for (uint y = 0; y < layer_data->data->height; y++)
			{
				uint tile_id = layer_data->data->Get(x, y);
				iPoint position = MapToWorld(x, y);
				for(uint i=0; i<map_data.tilesets.count();i++)
				App->render->Blit(map_data.tilesets[i]->texture, position.x - (App->render->camera.x * layer_data->data->parallax_speed), position.y, &map_data.tilesets[0]->GetTileRect(tile_id));
			}
		}
		layer_data = layer_data->next;
	}
		
	/*int x, y;
	App->input->GetMousePosition(x, y);
	iPoint tile = WorldToMap(x, y);
	LOG("Mouse tile pos: x - %i, y - %i", tile.x, tile.y);*/
}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * map_data.tile_width;
	ret.y = y * map_data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y)const
{
	iPoint ret;
	ret.x = x / map_data.tile_width;
	ret.y = y / map_data.tile_height;
	return ret;
}

Layer* j1Map::GetCollisionLayer()const
{
	for (p2List_item<Layer*>* it = map_data.layers.start; it; it = it->next)
	{
		if (it->data->iscollision == true)
		{
			return it->data;
		}
	}
	return nullptr;		
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}
bool TileSet::GetTileCollision(uint gid)const
{
	int relative_id = gid - firstgid;
	for (int i = 0; i < tile_data.Count(); i++)
	{
		if (tile_data[i].gid == relative_id)
		{
 		return tile_data[i].iscollision;
		}
	}
	return false;
 }
// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");
	Clear();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	Clear();

	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		map_data.tilesets.add(set);
	}

		// Load layer info ----------------------------------------------
	
	
	pugi::xml_node root_node = map_file.child("map");

	for (pugi::xml_node node_it = root_node.child("layer"); node_it; node_it = node_it.next_sibling())
	{
		map_data.layers.add(LoadLayer(node_it));

	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", map_data.width, map_data.height);
		LOG("tile_width: %d tile_height: %d", map_data.tile_width, map_data.tile_height);

		p2List_item<TileSet*>* item = map_data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

			// Adapt this vcode with your own variables
		
		p2List_item<Layer*>* item_layer = map_data.layers.start;
		while(item_layer != NULL)
		{
			Layer *l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

void j1Map::Clear()
{
	p2List_item<TileSet*>* item;
	item = map_data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	map_data.tilesets.clear();


	// Remove all layers
	p2List_item<Layer*>* item2;
	item2 = map_data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	map_data.layers.clear();

	map_file.reset();
}

void j1Map::MapPositionPlayer()
{
	pugi::xml_node node_pos;
	///node_pos = map_file.child("tilset").child("tile").child("properties").child("property");
}
// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		map_data.width = map.attribute("width").as_int();
		map_data.height = map.attribute("height").as_int();
		map_data.tile_width = map.attribute("tilewidth").as_int();
		map_data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		map_data.background_color.r = 0;
		map_data.background_color.g = 0;
		map_data.background_color.b = 0;
		map_data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) map_data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) map_data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) map_data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			map_data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			map_data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			map_data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			map_data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");
	
	int i = 0;
	for (pugi::xml_node tile_it = tileset_node.child("tile"); tile_it; tile_it = tile_it.next_sibling())
	{
		Tile tile;

		tile.gid = tile_it.attribute("id").as_uint();

		for (pugi::xml_node property_it = tile_it.child("properties").child("property"); property_it; property_it = property_it.next_sibling())
		{
			p2SString property_name = property_it.attribute("name").as_string();
			if (property_name == "Colider")
			{
				tile.iscollision = property_it.attribute("value").as_bool();
			}
			
		}
		i++;
		set->tile_data.PushBack(tile);
	}

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}


Layer* j1Map::LoadLayer(pugi::xml_node& node)
{
	Layer* layer = new Layer();
	
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
		 
	layer->size_tiles = layer->width * layer->height;
	layer->tiles = new uint[layer->size_tiles];
	memset(layer->tiles, 0, layer->size_tiles);

	pugi::xml_node tile_iteration;
	tile_iteration = node.child("data").child("tile");
	for (uint i = 0; i < layer->size_tiles; i++)
	{

		layer->tiles[i] = tile_iteration.attribute("gid").as_int();
		tile_iteration = tile_iteration.next_sibling();
	}
	// TO parallax and colisions

	for (pugi::xml_node it = node.child("properties").child("property"); it; it = it.next_sibling())
	{
		p2SString property_name = it.attribute("name").as_string();
		if (property_name == "Parallax")
		{
			layer->parallax_speed = it.attribute("value").as_float();
		}
		if (property_name == "Colliders")
		{
			layer->iscollision = it.attribute("value").as_bool();
		}

	}
	

	return layer;
}