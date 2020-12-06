#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"

#include "PugiXml\src\pugixml.hpp"

#define COST_OF_MAP_SIZE	100

// L03: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;

	SDL_Texture* texture;
	int	texWidth;
	int	texHeight;
	int	numTilesWidth;
	int	numTilesHeight;
	int	offsetX;
	int	offsetY;

	SDL_Rect GetTileRect(int id) const;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		SString name;
		int value;
	};

	~Properties()
	{

	}

	// L06: TODO 7: Method to ask for the value of a custom property

	int GetProperty(const char* name, int default_value = 0) const;

	List<Property*> list;
	Property property;
};

// L04: DONE 1: Create a struct for the map layer
struct MapLayer
{
	SString	name;
	int width;
	int height;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	SDL_Color backgroundColor;
	MapTypes type;
	List<TileSet*> tilesets;

	List<MapLayer*> layer;
};

class Map : public Module
{
public:

	Map();

	virtual ~Map();

	bool Awake(pugi::xml_node& conf);

	void Draw();

	bool CleanUp();

	bool Load(const char* path);

	fPoint MapToWorld(int x, int y) const;

	fPoint WorldToMap(int x, int y) const;

	void ShowCollider() { DrawColliders = !DrawColliders; }

	void LoadColliders();

	fPoint GetDimensionsMap();

	void ResetPath(fPoint start);
	void DrawPath();

	int MovementCost(int x, int y) const;
	void ComputePath(float x, float y);

	void PropagateDijkstra();

	void ComputePathAStar(int x, int y);

	void PropagateAStar(int heuristic);

	int CalculateDistanceToDestiny(fPoint node);
	int CalculateDistanceToStart(fPoint node);

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	void CheckPointActive(fPoint position);

private:
	bool LoadMap();
	bool LoadTiles(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadImg(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	int LoadCheckPoint();

	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* GetTilesetFromTileId(int id) const;

private:


public:
	MapData data;
	fPoint tileDestiny;
private:

	pugi::xml_document mapFile;
	SString folder;
	bool mapLoaded;
	bool DrawColliders = false;
	PQueue<fPoint> frontier;
	List<fPoint> visited;

	List<fPoint> breadcrumbs;
	uint costSoFar[COST_OF_MAP_SIZE][COST_OF_MAP_SIZE];
	DynArray<fPoint> path;

	fPoint goalAStar;
	bool finishAStar = false;

	SDL_Texture* tileX = nullptr;
};

#endif // __MAP_H__