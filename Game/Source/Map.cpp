
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Collider.h"
#include "Player.h"
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

// L06: TODO 7: Ask for the value of a custom property
SString Properties::GetProperty(const char* value, int defaultValue) const
{
	//...
    ListItem<Property*>* item = list.start;
    while (item != NULL)
    {
        if (strcmp(item->data->name.GetString(), value) == 0)
            return item->data->value;
        item = item->next;
    }
	return defaultValue;
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());
    level1.Create(config.child("level1").child_value());
    level2.Create(config.child("level2").child_value());
    return ret;
}

void Map::Draw()
{
	if (mapLoaded == false) return;

    
    ListItem<MapLayer*>* item = mapInfo.layers.start; 

    while(item != NULL)
    {
        MapLayer* layer = item->data;
        // L06: TODO 4: Make sure we draw all the layers and not just the first one
        for (int y = 0; y < mapInfo.height; ++y)
        {
            if (layer->properties.GetProperty("Drawable") == "true")
            {
                for (int x = 0; x < mapInfo.width; ++x)
                {
                    uint tileId = layer->Get(x, y);
                    if (tileId > 0)
                    {
                        TileSet* set = GetTilesetFromTileId(tileId);
                        SDL_Rect r = set->GetTileRect(tileId);
                        iPoint pos = MapToWorld(x, y);

                        if (strcmp(layer->name.GetString(), "Parallax 0") == 0 || strcmp(layer->name.GetString(), "Parallax 2") == 0) // Player moves to right
                        {
                            if (app->player->isMoving == true && app->player->playerInfo.currentDir == RIGHT_DIR)
                            {
                                backgroudPos.x += PARALLAX_VEL;
                                app->render->DrawTexture(set->imageSource, pos.x + backgroudPos.x, pos.y, &r);
                            }
                            else
                            {
                                app->render->DrawTexture(set->imageSource, pos.x + backgroudPos.x, pos.y, &r);
                            }
                        }
                        else if (strcmp(layer->name.GetString(), "Parallax 1") == 0 || strcmp(layer->name.GetString(), "Parallax 3") == 0) // Player moves to left
                        {
                            if (app->player->isMoving == true && app->player->playerInfo.currentDir == LEFT_DIR)
                            {
                                backgroudPos.x -= PARALLAX_VEL;
                                app->render->DrawTexture(set->imageSource, pos.x + backgroudPos.x, pos.y, &r);
                            }
                            else
                            {
                                app->render->DrawTexture(set->imageSource, pos.x + backgroudPos.x, pos.y, &r);
                            }
                        }
                        else
                        {
                            app->render->DrawTexture(set->imageSource, pos.x , pos.y, &r);
                        }                        
                    }
                }
            }
            
        }
        item = item->next;
    }

    
}
// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    switch (mapInfo.type)
    {
        case MAPTYPE_ORTHOGONAL:
        {
            iPoint ret;
            ret.x = x * mapInfo.tileWidth;
            ret.y = y * mapInfo.tileHeight;
            return ret;
        }break;
            
        case MAPTYPE_ISOMETRIC:
        {
            iPoint ret2;
            ret2.x = (x - y) * (mapInfo.tileWidth * 0.5);
            ret2.y = (x + y) * (mapInfo.tileHeight * 0.5);
            return ret2;
        }break;
 
    }
	
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);
    switch (mapInfo.type)
    {
    case MAPTYPE_ORTHOGONAL:
    {
        

        // L05: TODO 3: Add the case for isometric maps to WorldToMap
        ret.x = x / mapInfo.tileWidth;
        ret.y = y / mapInfo.tileHeight;

        return ret;
    }break;

    case MAPTYPE_ISOMETRIC:
    {
        float half_width = mapInfo.tileWidth * 0.5f;
		float half_height = mapInfo.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
        return ret;
    }break;

    }
    
    
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
    backgroudPos.SetToZero();

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
        LOG("Successfully parsed map XML file : %s", GetLevel2Load().GetString());  
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
            /*for (int i = 0; i < (item2->data->width * item2->data->height * sizeof(uint)); i++)
            {
                LOG("tile gid: %d", item2->data->data[i]);
            }*/

            item2 = item2->prev;
        }
        
    }

    mapLoaded = ret;

    return ret;
}

SString Map::GetFolder() const
{
    return folder;
}

bool Map::LoadMapData()
{
    bool ret = false;
    SString path = folder.GetString();
    path += GetLevel2Load(); // change 1 to the type inside player of which its the current level
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

        if (mapInfo.orientation == "orthogonal")
        {
            mapInfo.type = MAPTYPE_ORTHOGONAL;
        }
        if (mapInfo.orientation == "isometric")
        {
            mapInfo.type = MAPTYPE_ISOMETRIC;
        }
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
    LoadProperties(node, layer->properties);
    pugi::xml_node layerData = node.child("data");

    if (layerData == NULL)
    {
        LOG("Error loading node child data, inside LoadMapLayers");
        ret = false;

    }
    else
    {
        layer->data = new uint[layer->width * layer->height];
        memset(layer->data, 0, layer->width * layer->height * sizeof(uint));
        int i = 0;
        for (pugi::xml_node tile = layerData.child("tile"); tile; tile = tile.next_sibling("tile"))
        {
            layer->data[i++] = tile.attribute("gid").as_int();
        }
        if (strcmp(layer->name.GetString(), "Colision Layer") == 0) 
        {
            ColliderAsign(layer);
        }
    }
    
    return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = true;
    pugi::xml_node nodeP = node.child("properties");

    if (node != NULL)
    {  
        for (pugi::xml_node p = nodeP.child("property"); p; p = p.next_sibling("property"))
        {
            Properties::Property* prop = new Properties::Property();

            prop->name.Create(p.attribute("name").as_string());
            prop->type.Create(p.attribute("type").as_string());
            prop->value.Create(p.attribute("value").as_string());

            properties.list.add(prop);
        }
    }
    else
    {
        LOG("Error loading the child node properties");
        ret = false;
    }

    return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
    TileSet* set = mapInfo.tileSets.start->data;
    ListItem<TileSet*>* item = mapInfo.tileSets.start;
    while (item != NULL)
    {
        if (id < item->data->firstGid)
        {
            set = item->prev->data;
            break;
        }
        set = item->data;
        item = item->next;
    }
    return set;
}

void Map::ColliderAsign(MapLayer* layer)
{
    for (int y = 0; y < mapInfo.height; ++y)
    {
        for (int x = 0; x < mapInfo.width; ++x)
        {
            uint tileId = layer->Get(x, y);
            if (tileId > 0)
            {

                TileSet* set = mapInfo.tileSets.start->data;

                SDL_Rect r = set->GetTileRect(tileId);

                iPoint pos = MapToWorld(x, y);

                r.x = pos.x;
                r.y = pos.y;

                switch (tileId)
                {
                case 273:
                    app->collision->AddCollider(r, Type::DIE);
                    break;

                case 274:
                    app->collision->AddCollider(r, Type::WALL);
                    break;

                case 275:
                    app->collision->AddCollider(r, Type::START);
                    break;

                case 276:
                    app->collision->AddCollider(r, Type::END);
                    break;

                case 277:
                    app->collision->AddCollider(r, Type::BOOST);
                    break;
                case 1:
                    app->collision->AddCollider(r, Type::DIE); //  level 2
                    break;

                case 2:
                    app->collision->AddCollider(r, Type::WALL); //  level 2
                    break;

                case 3:
                    app->collision->AddCollider(r, Type::START); //  level 2
                    break;

                case 4:
                    app->collision->AddCollider(r, Type::END); //  level 2
                    break;
                }
            }
        }
    }
}

iPoint Map::GetPlayerInitialPos()
{
    bool founded = false;
    iPoint initPos = { 0,0 };
    ListItem<MapLayer*>* iteratorLayer = app->map->mapInfo.layers.start;
    while (iteratorLayer != NULL)
    {
        MapLayer* layer = iteratorLayer->data;
        if (strcmp(layer->name.GetString(), "Colision Layer") == 0)
        {
            for (int y = 0; y < app->map->mapInfo.height; y++)
            {
                for (int x = 0; x < app->map->mapInfo.width; x++)
                {
                    uint tileId = layer->Get(x, y);
                    if (tileId == 275 || tileId == 3)//275 or 3 = Start tile for lvl 1 and 2
                    {
                        TileSet* set = app->map->GetTilesetFromTileId(tileId);
                        iPoint pos = app->map->MapToWorld(x, y);
                        initPos = pos;
                        founded = true;
                    }
                    if (founded == true) break;
                }
                if (founded == true) break;
            }
        }
        iteratorLayer = iteratorLayer->next;
    }

    return initPos;
}
