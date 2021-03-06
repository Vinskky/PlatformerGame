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

// Sets up the walkability map
void PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t >= 0 && t != 254;
}

// Utility: return the walkability value of a tile
uchar PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const DynArray<iPoint>* PathFinding::GetLastPath() const
{
	return &lastPath;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
	ListItem<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
	ListItem<PathNode>* ret = NULL;
	int min = 65535;

	ListItem<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& listToFill) const
{
	iPoint cell;
	uint before = listToFill.list.Count();

	// north
	cell.Create(pos.x, pos.y + 1);
	if(app->pathfinding->IsWalkable(cell))
		listToFill.list.Add(PathNode(-1, -1, cell, this));

	// south
	cell.Create(pos.x, pos.y - 1);
	if(app->pathfinding->IsWalkable(cell))
		listToFill.list.Add(PathNode(-1, -1, cell, this));

	// east
	cell.Create(pos.x + 1, pos.y);
	if(app->pathfinding->IsWalkable(cell))
		listToFill.list.Add(PathNode(-1, -1, cell, this));

	// west
	cell.Create(pos.x - 1, pos.y);
	if(app->pathfinding->IsWalkable(cell))
		listToFill.list.Add(PathNode(-1, -1, cell, this));

	return listToFill.list.Count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}


// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int PathFinding::CreatePath(DynArray<iPoint>& path, const iPoint& origin, const iPoint& destination)
{
	// L12b: TODO 1: if origin or destination are not walkable, return -1
	if (IsWalkable(destination) == false || IsWalkable(origin) == false)
	{
		return -1;
	}
	else
	{
		PathList close;
		PathList open;

		open.list.Add(PathNode(0, origin.DistanceManhattan(destination), origin, nullptr));

		while (open.list.Count() != 0)
		{
			close.list.Add(open.GetNodeLowestScore()->data);
			open.list.Del(open.GetNodeLowestScore());

			if (close.list.end->data.pos == destination)
			{
				for (ListItem<PathNode>* iterator = close.list.end; iterator->data.parent != nullptr; iterator = close.Find(iterator->data.parent->pos))
				{
					iPoint worldPos = iterator->data.parent->pos;
					lastPath.PushBack(worldPos);
				}
				lastPath.Flip();
				path = lastPath;
				return 0;
			}
			else
			{
				PathList neighbours;
				close.list.end->data.FindWalkableAdjacents(neighbours);

				for (ListItem<PathNode>* iterator = neighbours.list.start; iterator != NULL; iterator = iterator->next)
				{
					if (close.Find(iterator->data.pos))
					{
						continue;
					}
					else if (open.Find(iterator->data.pos))
					{
						PathNode tmp = open.Find(iterator->data.pos)->data;
						iterator->data.CalculateF(destination);
						if (tmp.g > iterator->data.g)
						{
							tmp.parent = iterator->data.parent;
						}
					}
					else
					{
						iterator->data.CalculateF(destination);
						open.list.Add(iterator->data);
					}
				}
				neighbours.list.Clear();
			}
			
		}
	}
	return -1;
}

