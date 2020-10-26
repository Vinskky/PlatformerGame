#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"


#include "PugiXml\src\pugixml.hpp"

//Homework Tile
// L03: DONE 1: We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct MapLayer
{
    SString name;
    int width;
    int height;
	uint* data;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L04: TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[y*width + x];
	}
};
// L03: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
    SString name;
    int firstGid;
    int tileWidth;
    int tileHeight;
    int spacing;
    int margin;
    
	SDL_Texture* imageSource;
    int imageWitdth;
    int imageHeight;

	// L04: TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
};

// L03: TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    int nextObjectId;
    SString orientation;
    SString rendereOrder;
    
    List<TileSet*> tileSets;
    // L04: TODO 2: Add a list/array of layers to the map
    List<MapLayer*> layers;
	//added in L04
	SDL_Color backgroundColor;
	MapTypes type;
	
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called each loop iteration
    void Draw();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(const char* path);

    inline SString GetLevel2Load()const
    {
        return level2Load;
    }
	// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;
    // L03: TODO 1: Add your struct for map info as public for now
    MapData mapInfo;
private:
    bool LoadMapData();
    bool LoadTileSet(pugi::xml_node&, TileSet*);
    bool LoadTileSetImage(pugi::xml_node&, TileSet*);
    bool LoadMapLayers(pugi::xml_node&, MapLayer*);

private:

    pugi::xml_document mapFile;
    SString folder;
    SString level2Load;
    bool mapLoaded;
};

#endif // __MAP_H__