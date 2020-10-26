
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());
    level2Load.Create(config.child("level").child_value());
    return ret;
}

void Map::Draw()
{
	if (mapLoaded == false) return;

    
    MapLayer* layer = mapInfo.layers.start->data;
    for (int y = 0; y < mapInfo.height; ++y)
    {
        for (int x = 0; x < mapInfo.width; ++x)
        {
            uint tileId = layer->Get(x, y);

            TileSet* set = mapInfo.tileSets.start->data;
            SDL_Rect r = set->GetTileRect(tileId);
            iPoint pos = MapToWorld(x, y);
            app->render->DrawTexture(set->imageSource, pos.x, pos.y, &r);
        }
    }
       
    // L04: TODO 5: Prepare the loop to draw all tilesets + DrawTexture()
	
	// L04: TODO 9: Complete the draw function
	
    // L03: TODO 6: Iterate all tilesets and draw all their 
    // images in 0,0 (you should have only one tileset for now)
    /*ListItem<TileSet*>* item = mapInfo.tileSets.end;
    
    while (item != NULL)
    {
        Textures* text = new Textures();
        SString path = "Assets/maps/";
        path += item->data->imageSource;
              
        app->render->DrawTexture(item->data->imageSource, 0, 0);
        //delete text;
        item = item->prev;
    }*/
}
// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * mapInfo.tileWidth;
	ret.y = y * mapInfo.tileHeight;

	return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };
    int relativeId = id - firstGid;
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + ((rect.w + spacing) * (relativeId % (imageWitdth / tileWidth))); // imageWitdth / tileWidth = nº of tiles for the tileset
    rect.y = margin + ((rect.h + spacing) * (relativeId / (imageWitdth / tileWidth)));
    
	// L04: TODO 7: Get relative Tile rectangle

	return rect;
}
// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L03: TODO 2: Make sure you clean up any memory allocated from tilesets/map
    //Delete list of TileSets---------------------
    ListItem<TileSet*>* item;
    item = mapInfo.tileSets.start;
    
    while (item != NULL)
    {
        RELEASE(item->data);
        item = item->next;
    }
    mapInfo.tileSets.clear();

    //Delete list of layers---------------------
    ListItem<MapLayer*>* item2;
    item2 = mapInfo.layers.start;

    while (item2 != NULL)
    {
        RELEASE(item2->data);
        item2 = item2->next;
    }
    mapInfo.layers.clear();


    mapFile.reset();

    return true;
}

// Load new map
bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

    if(ret == true)
    {
        // L03: TODO 3: Create and call a private function to load and fill all your map data
		ret = LoadMapData();
    }

    // L03: TODO 4: Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
    for (pugi::xml_node tiledata = mapFile.child("map").child("tileset"); tiledata && ret; tiledata = tiledata.next_sibling("tileset"))
    {
        TileSet* tileSets = new TileSet();

        if (ret == true) ret = LoadTileSet(tiledata, tileSets);
        
        if (ret == true) ret = LoadTileSetImage(tiledata, tileSets);

        if (ret == true) mapInfo.tileSets.add(tileSets);
    }

    for (pugi::xml_node layerdata = mapFile.child("map").child("layer"); layerdata && ret; layerdata = layerdata.next_sibling("layer"))
    {
        MapLayer* lay = new MapLayer();

        if (ret == true) ret = LoadMapLayers(layerdata, lay);

        if (ret == true) mapInfo.layers.add(lay);

    }

    if(ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
        LOG("Successfully parsed map XML file : %s", level2Load.GetString());
        LOG(" width : %d height : %d", mapInfo.width, mapInfo.height);
        LOG(" tile_width : %d tile_height : %d", mapInfo.tileWidth, mapInfo.tileHeight);

        ListItem<TileSet*>* item = mapInfo.tileSets.end;

        while (item != NULL)
        {
            LOG("Tileset----");
            LOG("name : %s firstgid : %d", item->data->name.GetString(), item->data->firstGid);
            LOG("tile width : %d tile height : %d", item->data->tileWidth, item->data->tileHeight);
            LOG("spacing : %d margin : %d", item->data->spacing, item->data->margin);
            item = item->prev;
        }

        ListItem<MapLayer*>* item2 = mapInfo.layers.end;

        while (item2 != NULL)
        {
            LOG("Layer----");
            LOG("name : %s ", item2->data->name.GetString());
            LOG("width : %d height : %d", item2->data->width, item2->data->height);
            LOG("Tiles in Layer %s ----", item2->data->name.GetString());
            for (int i = 0; i < (item2->data->width * item2->data->height * sizeof(uint)); i++)
            {
                LOG("tile gid: %d", item2->data->data[i]);
            }

            item2 = item2->prev;
        }
        
    }

    mapLoaded = ret;

    return ret;
}

bool Map::LoadMapData()
{
    bool ret = false;
    SString path = folder.GetString();
    path += level2Load.GetString();
    pugi::xml_node mapNode;
    pugi::xml_parse_result result = mapFile.load_file(path.GetString());

    if (result != NULL)
    {
        mapNode = mapFile.child("map");
        mapInfo.height = mapNode.attribute("height").as_int();
        mapInfo.width = mapNode.attribute("width").as_int();
        mapInfo.nextObjectId = mapNode.attribute("nextobjectid").as_int();
        mapInfo.orientation = mapNode.attribute("orientation").as_string();
        mapInfo.rendereOrder = mapNode.attribute("renderorder").as_string();
        mapInfo.tileHeight = mapNode.attribute("tileheight").as_int();
        mapInfo.tileWidth = mapNode.attribute("tilewidth").as_int();

        LOG("Map loaded correctly!");
        mapLoaded = true;
        ret = true;
    }
    else
    {
        LOG("Fail loading the tmx file. %s", result.description());
    }
	return ret;
}

bool Map::LoadTileSet(pugi::xml_node& node, TileSet* tileSetInput)
{
    tileSetInput->name.Create(node.attribute("name").as_string());
    tileSetInput->firstGid = node.attribute("firstgid").as_int();
    tileSetInput->tileWidth = node.attribute("tilewidth").as_int();
    tileSetInput->tileHeight = node.attribute("tileheight").as_int();
    tileSetInput->spacing = node.attribute("spacing").as_int();
    tileSetInput->margin = node.attribute("margin").as_int();

	return true;
}

bool Map::LoadTileSetImage(pugi::xml_node& node, TileSet* tileSetInput)
{
    SString path = folder;
    path += node.child("image").attribute("source").as_string();
    tileSetInput->imageSource = app->tex->Load(path.GetString());
    tileSetInput->imageWitdth = node.child("image").attribute("width").as_int();
    tileSetInput->imageHeight = node.child("image").attribute("height").as_int();
    

    return true;
}

bool Map::LoadMapLayers(pugi::xml_node &node, MapLayer* layer)
{
    bool ret = true;
    layer->name.Create(node.attribute("name").as_string());
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();
    
    pugi::xml_node layerData = node.child("data");

    if (layerData == NULL)
    {
        LOG("Error loading node child data, inside LoadMapLayers");
        ret = false;

    }
    else
    {
        layer->data = new uint[layer->width * layer->height * sizeof(uint)];
        memset(layer->data, 0, layer->width * layer->height * sizeof(uint));
        int i = 0;
        for (pugi::xml_node tile = layerData.child("tile"); tile; tile = tile.next_sibling("tile"))
        {
            layer->data[i++] = tile.attribute("gid").as_int();
        }
    }
    
    return ret;
}
