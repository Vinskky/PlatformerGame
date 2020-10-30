#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#define PARALLAX_VEL 0.0001


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

// L06: TODO 5: Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		//...
        SString name;
        SString type;
        SString value;
	};
	
	~Properties()
	{
		//...
        ListItem<Property*>* item = list.start;
        while (item != list.end)
        {
            RELEASE(item->data);
            item = item->next;
        }
        list.clear();
	}

	// L06: TODO 7: Method to ask for the value of a custom property
	SString GetProperty(const char* name, int default_value = 0) const;

	List<Property*> list;
};

// L04: DONE 1: Create a struct for the map layer
struct MapLayer
{
    SString name;
    int width;
    int height;
	uint* data;

	// L06: DONE 1: Support custom properties
	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L04: TODO 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
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

    inline SString GetLevel2Load(int level)const
    {
        if (level == 1)
            return level1;
        else
            return level2;
    }
	// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;
    // L05: TODO 2: Add orthographic world to map coordinates
    iPoint WorldToMap(int x, int y) const;

    void ColliderAsign(MapLayer* layer);
    iPoint GetPlayerInitialPos();
    // L03: TODO 1: Add your struct for map info as public for now
    MapData mapInfo;
    TileSet* GetTilesetFromTileId(int id) const;
private:
    bool LoadMapData();
    bool LoadTileSet(pugi::xml_node&, TileSet*);
    bool LoadTileSetImage(pugi::xml_node&, TileSet*);
    bool LoadMapLayers(pugi::xml_node&, MapLayer*);

	// L06: TODO 6: Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	// L06: TODO 3: Pick the right Tileset based on a tile id
	//TileSet* GetTilesetFromTileId(int id) const;

private:

    fPoint backgroudPos;
    pugi::xml_document mapFile;
    SString folder;
    SString level1;
    SString level2;
    bool mapLoaded;
};

#endif // __MAP_H__