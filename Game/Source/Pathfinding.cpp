#include "App.h"
#include "PathFinding.h"

#include "Defs.h"
#include "Log.h"

PathFinding::PathFinding() : Module(), map(NULL), lastPath(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.Create("pathfinding");
}

// Destructor
PathFinding::~PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	lastPath.Clear();
	RELEASE_ARRAY(map);

	return true;
}
void PathFinding::ResetPath(fPoint start)
{
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();

	frontier.Push(start, 0);
	visited.add(start);
	breadcrumbs.add(start);
}
void PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}
DynArray<fPoint>* PathFinding::GetLastPath()
{
	return &lastPath;
}
PQueue<fPoint>* PathFinding::GetFrontier()
{
	return &frontier;
}
List<fPoint>* PathFinding::GetVisited()
{
	return &visited;
}
List<fPoint>* PathFinding::GetBreadcrumbs()
{
	return &breadcrumbs;
}
bool PathFinding::CheckBoundaries(const fPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}
bool PathFinding::IsWalkable(const fPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}
uchar PathFinding::GetTileAt(const fPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[((int)pos.y * width) + (int)pos.x];

	return INVALID_WALK_CODE;
}
void PathFinding::PropagateAStar(const fPoint& destination)
{
	fPoint curr;
	int newCost[4];
	if (frontier.Pop(curr))
	{
		fPoint neighbors[4];
		neighbors[0].create(curr.x + 0, curr.y + 1);
		neighbors[1].create(curr.x - 1, curr.y + 0);
		neighbors[2].create(curr.x + 0, curr.y - 1);
		neighbors[3].create(curr.x + 1, curr.y + 0);

		int j = 0;
		bool init = false;
		for (uint i = 0; i < 4; ++i)
		{
			if (GetTileAt(neighbors[i]) > 0)
			{
				if (visited.find(neighbors[i]) == -1)
				{
					newCost[i] = neighbors[i].DistanceManhattan(visited.start->data) + neighbors[i].DistanceManhattan(destination);
					if (newCost[i] >= 0 && init == false)j = i, init = true;
				}
			}
		}

		for (int i = 0; i < 3; i++)
		{
			if (visited.find(neighbors[i]) == -1)
			{
				if (GetTileAt(neighbors[i]) > 0 && newCost[i + 1] >= 0 && newCost[j] >= newCost[i + 1])j = i + 1;
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (visited.find(neighbors[i]) == -1 && GetTileAt(neighbors[i]) > 0)
			{
				visited.add(neighbors[i]);
				breadcrumbs.add(curr);
				if (newCost[j] == newCost[i])
				{
					frontier.Push(neighbors[i], newCost[i]);
				}
			}
		}
	}
}
void PathFinding::ComputePathAStar(const fPoint& origin, const fPoint& destination)
{
	int count = 0;
	while (destinationIsFind == false)
	{
		PropagateAStar(destination);
		for (count; count < visited.Count(); count++)
		{
			if (visited.At(count)->data.x == destination.x && visited.At(count)->data.y == destination.y)
			{
				destinationIsFind = true;
				break;
			}
		}
	}
	destinationIsFind = false;
	lastPath.Clear();

	lastPath.PushBack(destination);
	lastPath.PushBack(breadcrumbs.At(count)->data);
	for (int i = visited.Count() - 1; i > 0; i--)
	{
		if (visited.At(i)->data == breadcrumbs.At(count)->data)
		{
			lastPath.PushBack(breadcrumbs.At(i)->data);
			count = i;
		}
	}
	lastPath.Flip();
}