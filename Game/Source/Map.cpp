
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Collisions.h"
#include "Defs.h"
#include "PQueue.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name.Create("map");
}

// Destructor
Map::~Map()
{}
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;


	pugi::xml_node property_ = node.child("property");

	Properties::Property* Prop = new Properties::Property();

	for (property_; property_ && ret; property_ = property_.next_sibling("property"))
	{
		Prop->name = property_.attribute("name").as_string("");
		Prop->value = property_.attribute("value").as_int(0);
		properties.list.add(Prop);
	}

	return ret;
}

int Properties::GetProperty(const char* value, int defaultValue) const
{
	for (int i = 0; i < list.Count(); i++)
	{
		if (strcmp(list.At(i)->data->name.GetString(), value) == 0)
		{
			if (list.At(i)->data->value != defaultValue) return list.At(i)->data->value;
			else return defaultValue;
		}
	}

	return defaultValue;
}
// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.Create(config.child("folder").child_value());

	return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	MapLayer* layer = data.layer.start->data;

	// L06: TODO 4: Make sure we draw all the layers and not just the first one

	fPoint point;

	for (int y = 0; y < data.height; ++y)
	{
		for (int x = 0; x < data.width; ++x)
		{
			for (ListItem<MapLayer*>* layer = data.layer.start; layer; layer = layer->next)
			{
				int tileId = layer->data->Get(x, y);
				if (tileId > 0)
				{
					// L04: TODO 9: Complete the draw function       
					fPoint vec = MapToWorld(x, y);
					for (int i = 0; i < data.tilesets.Count() && data.layer.At(i) != nullptr; i++)
					{
						if (data.layer.At(i)->data->properties.GetProperty("Draw", 0) == 1 && layer->data->name != "colliders" && layer->data->name != "win" && layer->data->name != "Dead" && layer->data->name != "win2" && layer->data->name != "win3")
							app->render->DrawTexture(data.tilesets.At(i)->data->texture, vec.x, vec.y, &data.tilesets.At(i)->data->GetTileRect(tileId));
						//app->render->DrawTexture(GetTilesetFromTileId(tileId)->texture, vec.x, vec.y, &GetTilesetFromTileId(tileId)->GetTileRect(tileId));
					}
				}
			}
		}
	}
}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
fPoint Map::MapToWorld(int x, int y) const
{
	fPoint ret;

	ret.x = x * data.tileWidth;
	ret.y = y * data.tileHeight;

	return ret;
}
fPoint Map::WorldToMap(int x, int y) const
{
	fPoint ret(0, 0);

	ret.x = x / data.tileWidth;
	ret.y = y / data.tileHeight;
	// L05: TODO 3: Add the case for isometric maps to WorldToMap

	return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item->data != nullptr)
	{
		if (item->next == nullptr)
		{
			set = item->data;
			break;
		}
		if ((item->data->firstgid < id) && item->next->data->firstgid > id)
		{
			set = item->data;
			break;
		}
		item = item->next;
	}

	return set;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// L04: TODO 7: Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// L04: TODO 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* itemLayer;
	itemLayer = data.layer.start;

	while (itemLayer != NULL)
	{
		RELEASE(itemLayer->data);
		itemLayer = itemLayer->next;
	}
	data.layer.clear();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	if (ret == true)
	{
		// L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

	// L03: DONE 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTiles(tileset, set);
		if (ret == true) ret = LoadImg(tileset, set);

		data.tilesets.add(set);
	}
	// L04: TODO 4: Iterate all layers and load each of them
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layer.add(lay);

		pugi::xml_node propertiesNode;
		for (propertiesNode = layer.child("properties"); propertiesNode && ret; propertiesNode = propertiesNode.next_sibling("properties"))
		{
			Properties* property = new Properties();

			ret = LoadProperties(propertiesNode, *property);

			//data.layers.At(0)->data->properties.list.add();
			lay->properties = *property;
		}
	}

	if (ret == true)
	{
		// L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
		LOG("Successfully parsed map XML file: %s", filename);
		LOG("width: %d  height: %d", data.width, data.height);

		for (int i = 0; i < data.tilesets.Count(); i++)
		{
			LOG("Tileset %d ----", i + 1);
			LOG("name: %s firstgid: %i", data.tilesets.At(i)->data->name.GetString(), data.tilesets.At(i)->data->firstgid);
			LOG("tile width: %d tile height: %d", data.tilesets.At(i)->data->tileWidth, data.tilesets.At(i)->data->tileHeight);
			LOG("spacing: %d margin: %d", data.tilesets.At(i)->data->spacing, data.tilesets.At(i)->data->margin);
		}


		// L04: TODO 4: LOG the info for each loaded layer
	}

	mapLoaded = ret;

	return ret;
}

// L03: TODO: Load map general properties
// L03: TODO: Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load map general properties
		data.width = map.attribute("width").as_int(0);
		data.height = map.attribute("height").as_int(0);
		data.tileWidth = map.attribute("tilewidth").as_int(0);
		data.tileHeight = map.attribute("tileheight").as_int(0);
		data.type = MAPTYPE_ORTHOGONAL;
	}

	return ret;
}

// L03: TODO: Load Tileset attributes
bool Map::LoadTiles(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// L03: TODO: Load Tileset attributes
	set->firstgid = tileset_node.attribute("firstgid").as_int(0);
	set->name = tileset_node.attribute("name").as_string();
	set->margin = tileset_node.attribute("margin").as_int(0);
	set->spacing = tileset_node.attribute("spacing").as_int(0);
	set->tileHeight = tileset_node.attribute("tileheight").as_int(0);
	set->tileWidth = tileset_node.attribute("tilewidth").as_int(0);
	set->numTilesWidth = tileset_node.attribute("columns").as_int(0);
	set->numTilesHeight = tileset_node.attribute("numtiles").as_int(0) / set->numTilesWidth;

	return ret;
}

// L03: TODO: Load Tileset image
bool Map::LoadImg(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: TODO: Load Tileset image
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
	}

	return ret;
}

// L04: TODO 3: Create the definition for a function that loads a single layer

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	// L04: TODO 3: Load a single layer
	layer->name = node.attribute("name").as_string("");
	layer->width = node.attribute("width").as_int(0);
	layer->height= node.attribute("height").as_int(0);


	int size = layer->height * layer->width;
	layer->data = new uint[size];

	pugi::xml_node tile = node.child("data").child("tile");
	for (int i = 0; i < size; i++)
	{
		layer->data[i] = tile.attribute("gid").as_int(0);
		tile = tile.next_sibling("tile");
	}

	return ret;
}

void Map::LoadColliders()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* L = data.layer.start;
	ListItem<TileSet*>* T = data.tilesets.start;
	TileSet* tileSet = data.tilesets.start->data;
	uint num = 401;

	while (L != nullptr)
	{
		MapLayer* layer = L->data;
		if (layer->properties.GetProperty("Draw", 1) == 1)
		{
			L = L->next;
			continue;
		}

		for (int y = 0; y < layer->height; y++)
		{
			for (int x = 0; x < layer->width; x++)
			{
				int u = layer->Get(x, y);
				fPoint pos = MapToWorld(x, y);

				if (u != 0)
				{
					SDL_Rect rectCol = { pos.x,pos.y, data.tileWidth, data.tileHeight/3 };

					if (layer->name == "win")
						app->collisions->AddCollider(rectCol, Collider::Type::WIN, this);

					else if (layer->name == "Dead")
						app->collisions->AddCollider(rectCol, Collider::Type::DEATH, this);

					else if (layer->name == "win2")
						app->collisions->AddCollider(rectCol, Collider::Type::WIN2, this);

					else if (layer->name == "win3")
						app->collisions->AddCollider(rectCol, Collider::Type::WIN3, this);

					else if (layer->properties.GetProperty("Draw", 1) == 0)
						app->collisions->AddCollider(rectCol, Collider::Type::COLL, this);
				}
			}
		}
		L = L->next;
	}
}


void Map::ResetPath(fPoint start)
{
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();

	frontier.Push(start, 0);
	visited.add(start);
	breadcrumbs.add(start);

	memset(costSoFar, 0, sizeof(uint) * COST_OF_MAP_SIZE * COST_OF_MAP_SIZE);
}

void Map::DrawPath()
{
	fPoint pointV;
	fPoint pointF;
	fPoint pointPath;

	// Draw visited
	ListItem<fPoint>* itemVisited = visited.start;
	PQueueItem<fPoint>* itemFrontier = frontier.start;


	while (itemVisited)
	{
		pointV = itemVisited->data;

		TileSet* tileset = GetTilesetFromTileId(1);

		SDL_Rect rec = tileset->GetTileRect(1);
		fPoint pos = MapToWorld(pointV.x, pointV.y);

		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
		itemVisited = itemVisited->next;

	}
	while (itemFrontier)
	{
		TileSet* tileset = GetTilesetFromTileId(1);

		SDL_Rect rec = tileset->GetTileRect(1);

		pointF = itemFrontier->data;
		tileset = GetTilesetFromTileId(259);
		fPoint pos = MapToWorld(pointF.x, pointF.y);
		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
		itemFrontier = itemFrontier->next;
	}
	int pathSize = path.Count();
	for (size_t i = 0; i < pathSize; i++)
	{
		TileSet* tileset = GetTilesetFromTileId(1);

		SDL_Rect rec = tileset->GetTileRect(1);

		pointPath = { path.At(i)->x,path.At(i)->y };
		tileset = GetTilesetFromTileId(1);
		fPoint pos = MapToWorld(pointPath.x, pointPath.y);
		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
	}
}

int Map::MovementCost(int x, int y) const
{
	int ret = -1;

	if ((x >= 0) && (x < data.width) && (y >= 0) && (y < data.height))
	{
		// Coje el layer de las colisiones que en nuestro caso es el tercero
		int id = data.layer.start->next->next->data->Get(x, y);

		if (id != 0)
		{
			int fisrtGid = GetTilesetFromTileId(id)->firstgid;

			if (id == fisrtGid) ret = 1;
			else if (id == fisrtGid + 1) ret = 0;
			else if (id == fisrtGid + 2) ret = 3;
			//else ret = 1;
		}
		else ret = 1;
	}

	return ret;
}

void Map::ComputePath(float x, float y)
{
	path.Clear();
	fPoint goal = { x, y };
	int size = breadcrumbs.Count() - 1;
	path.PushBack(goal);

	ListItem<fPoint>* iterator = visited.end;
	ListItem<fPoint>* tmp = breadcrumbs.At(size);

	for (iterator; iterator; iterator = iterator->prev)
	{
		size--;
		if (iterator->data == tmp->data)
		{
			path.PushBack(iterator->data);
			tmp = breadcrumbs.At(size);
		}
	}

}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item;
	item = data.layer.start;

	for (item = data.layer.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.GetProperty("Nodraw", 0) == 1)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tileId = layer->Get(x, y);
				TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}