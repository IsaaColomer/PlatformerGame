#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__

#include "Module.h"
#include "PQueue.h"
#include "Point.h"
#include "DynArray.h"
#include "List.h"

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255

class PathFinding : public Module
{
public:

	PathFinding();

	// Destructor
	~PathFinding();

	// Called before quitting
	bool CleanUp();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);
	// To request all tiles involved in the last generated path
	DynArray<fPoint>* GetLastPath();
	PQueue<fPoint>* GetFrontier();
	List<fPoint>* GetVisited();
	List<fPoint>* GetBreadcrumbs();
	//int MovementCost(iPoint) const;
	void ResetPath(fPoint start);
	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const fPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const fPoint& pos) const;

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const fPoint& pos) const;

	void PropagateAStar(const fPoint&);
	void ComputePathAStar(const fPoint& origin, const fPoint& destination);

	bool destinationIsFind;

private:

	// size of the map
	uint width;
	uint height;
	PQueue<fPoint> frontier;
	List<fPoint> visited;
	List<fPoint> breadcrumbs;
	// all map walkability values [0..255]
	uchar* map;

	// we store the created path here
	DynArray<fPoint> lastPath;
};

#endif // __PATHFINDING_H__