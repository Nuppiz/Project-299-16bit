#include "Common.h"
#include "Game.h"
#include "Structs.h"
#include "Loadgfx.h"
#include "Filech.h"
#include "Text.h"
#include "Draw.h"
#include "Movecoll.h"
#include "Action.h"
#include "General.h"

/* Level data and entity loader, savegame functionalities*/

Entity_t Entities[MAX_ENTITIES];
extern System_t System;
extern Timer_t Timers;
extern GameData_t Game;
extern Texture_array TileTextures;
extern Weapon_t Weapons[];
extern ActorTemplate_t ActorTemplates[];
extern int actortemplate_count;
Tile_t TileSet[TILESET_MAX];
Item_t* Items;
char levelname_global[15];

char* entity_type_strings[NUM_ENTITYTYPES] =
{
    "",
    "Door",
    "Button",
    "Spawner",
    "Trigger",
    "Counter",
    "Portal",
};

char* item_type_strings[NUM_ITEMTYPES] =
{
    "Key_Red",
    "Key_Blue",
    "Key_Yellow",
};

void loadTileset(char* filename)
{
    FILE* tileset;
    char c, symbol, tex_id;
    char name[50];

    tileset = fopen(filename, "rb");

    if (tileset == NULL)
        tileset = fopen(DEFAULT_TILESET, "rb");

    while ((c = fgetc(tileset)) != EOF)
    {
        if (c == '$')
        {
            symbol = fgetc(tileset) - 32;
            fscanf(tileset, "%s", name);
            tex_id = loadTexture(name, &TileTextures);
            TileSet[symbol].texture_id = tex_id;

            while((c = fgetc(tileset)) != '\n' && c != EOF)
            {
                switch (c)
                {
                case 'D': TileTextures.textures[tex_id].material_type = MAT_DEFAULT; continue;
                case 'G': TileTextures.textures[tex_id].material_type = MAT_GRASS; continue;
                case 'O': TileSet[symbol].obstacle = 1; continue;
                case 'B': TileSet[symbol].block_bullets = 1; continue;
                case 'I': TileSet[symbol].is_entity = 0; continue;
                case '1': TileSet[symbol].entity_value = TILE_DMG_10; continue;
                
                default:  continue;
                }
            }
        }
    }
    fclose(tileset);
}

void entityLoader(FILE* level_file, int entity_id, int entity_type)
{
    // because it would look messy to put all of this into the main loader function

    int ent_x, ent_y, state, tilemap_location, only_once; double angle; // common variables
    int locked, key; // door variables
    int target; // switch/button variables
    ticks_t last_spawn_time, spawn_interval; id_t toggleable, max_actors, num_actors, spawn_type, trigger_on_death; // spawner variables
    ticks_t last_trigger_time; ticks_t trigger_interval; int target_ids[4]; // trigger variables
    int max_value, target_id; // counter variables
    char level_name[20]; int portal_x, portal_y; // portal variables

    Entity_t* ent; // shorter name of the current entity for more compact code

    if (entity_type != ENT_COUNTER)
        fscanf(level_file, "%d %d %d", &ent_x, &ent_y, &state);

    tilemap_location = ent_y * Game.Map.width + ent_x;

    Game.Map.tilemap[tilemap_location].is_entity = 1;
    Game.Map.tilemap[tilemap_location].entity_value = entity_id;

    ent = &Entities[entity_id];

    ent->type = entity_type;
    if (entity_type != ENT_COUNTER)
    {
        ent->x = ent_x;
        ent->y = ent_y;
        ent->id = entity_id;
        ent->state = state;
    }

    switch(entity_type)
    {
    case ENT_DOOR: fscanf(level_file, "%d %d", &locked, &key),
                ent->data.door.locked = locked,
                ent->data.door.key = key;
                break;
    case ENT_BUTTON: fscanf(level_file, "%d", &target),
                ent->data.button.target = target;
                break;
    case ENT_SPAWNER: fscanf(level_file, "%d %lf %d %d %d %d %d", 
                &spawn_type, &angle, &trigger_on_death, &max_actors, &spawn_interval, &toggleable, &only_once),
                ent->data.spawner.last_spawn_time = 0,
                ent->data.spawner.num_actors = 0,
                ent->data.spawner.spawn_type = spawn_type,
                ent->data.spawner.angle = angle,
                ent->data.spawner.trigger_on_death = trigger_on_death,
                ent->data.spawner.max_actors = max_actors,
                ent->data.spawner.spawn_time_interval = spawn_interval / System.tick_interval,
                ent->data.spawner.toggleable = toggleable,
                ent->data.spawner.only_once = only_once;
                break;
    case ENT_TRIGGER: fscanf(level_file, "%d %d %d %d %d %d", &trigger_interval,
                &target_ids[0], &target_ids[1], &target_ids[2], &target_ids[3], &only_once),
                ent->data.trigger.last_trigger_time = 0,
                ent->data.trigger.trigger_interval = trigger_interval / System.tick_interval,
                ent->data.trigger.target_ids[0] = target_ids[0],
                ent->data.trigger.target_ids[1] = target_ids[1],
                ent->data.trigger.target_ids[2] = target_ids[2],
                ent->data.trigger.target_ids[3] = target_ids[3],
                ent->data.trigger.only_once = only_once;
                break;
    case ENT_COUNTER: fscanf(level_file, "%d %d %d", &max_value, &target_id, &only_once),
                ent->data.counter.value = 0,
                ent->data.counter.max_value = max_value,
                ent->data.counter.target_id = target_id,
                ent->data.counter.only_once = only_once;
                break;
    case ENT_PORTAL: fscanf(level_file, "%s %d %d %lf", level_name, &portal_x, &portal_y, &angle),
                strcpy(ent->data.portal.level_name, level_name),
                ent->data.portal.x = portal_x,
                ent->data.portal.y = portal_y,
                ent->data.portal.angle = angle;
                break;
    }
}

void freeAllEntities()
{
    int i;
    Entity_t* entity;
    Tile_t* tile;

    for (i = 0; i < MAX_ENTITIES; i++)
    {
        entity = &Entities[i];
        tile = getEntityTile(entity);

        tile->is_entity = 0;
        tile->entity_value = 0;
        memset(entity, 0, sizeof(Entity_t));
    }
}

void levelLoader(char* level_name, uint8_t load_type)
{
    // general variables
    FILE* level_file;
    char buffer[100];
    char c;
    int i, levelname_length;

    char level_path[30] = LEVEL_PATH; // default level path
    char temp_level[30]; // temporary storage for the level name, as otherwise the filename pointers go bonkers

    // tileset variables
    int tileset_found = FALSE;
    char tileset_file[20] = DEFAULT_TILESET;

    // actor variables
    int template_id;
    char texture_filename[20];
    char template_filename[30];

    // entity variables
    char entity_name[20];
    int entity_id, entity_type;

    // item tile variables
    char item_name[20];
    int item_type, tilemap_location, state;

    // interactive tile variables
    char interactive_name[20];
    int interactive_type;

    strcat(level_path, level_name);

    level_file = fopen(level_path, "r");

    strcpy(temp_level, level_name);
    
    if (level_file == NULL)
    {
        char level_error_string[200];
        sprintf(level_error_string, "Unable to open level file: %s\n"
                "Please check the file actually exists!\n", level_name);
        fclose(level_file);
        quitError(level_error_string);
    }

    if (Entities != NULL || TileTextures.textures != NULL || Game.Actors != NULL)
    {
        freeAllEntities();
        freeAllTextures();
        freeGameData();
        emptyCorpseArray();
        if (Items != NULL)
        {
            free(Items);
            Game.item_capacity = 0;
        }
    }

    strcpy(Game.current_level_name, temp_level);

    if (load_type == LOAD_NEW_LEVEL)
    {
        initGameData(OBJ_DEFAULT_CAPACITY, OBJ_DEFAULT_CAPACITY);
        Game.item_capacity = 16;
        Items = malloc(Game.item_capacity * sizeof(Item_t));
        memset(Items, 0, Game.item_capacity * sizeof(Item_t));
    }

    while ((c = fgetc(level_file)) != EOF)
    {
        if (c == '$')
        {
            fscanf(level_file, "%s", buffer);
            if (strcmp(buffer, "tileset") == 0)
            {
                fscanf(level_file, "%s", tileset_file);
                tileset_found = TRUE;
                loadTileset(tileset_file);
            }
            else if (strcmp(buffer, "leveldim") == 0)
            {
                fscanf(level_file, "%d %d", &Game.Map.width, &Game.Map.height);
                Game.Map.tilemap = malloc((Game.Map.width * Game.Map.height) * sizeof(Tile_t));
            }
            else if (strcmp(buffer, "tilemap") == 0)
            {
                i = 0;
                while ((c = fgetc(level_file)) != EOF && i < Game.Map.width * Game.Map.height)
                {
                    if (c != '\n')
                    {
                        Game.Map.tilemap[i] = TileSet[c - 32];
                        i++;
                    }
                }
                memset(TileSet, 0, TILESET_MAX * sizeof(Tile_t));
            }
            else if (strcmp(buffer, "player") == 0 && load_type != LOAD_SAVED_LEVEL)
            {
                Actor_t player = {0};
                ActorTemplate_t* template;
                fscanf(level_file, "%s %f %f %lf", template_filename, &player.position.x, &player.position.y, &player.angle);
                
                template_id = loadActorTemplate(template_filename);
                template = &ActorTemplates[template_id];
                Game.player_id = createActorFromTemplate(player, template);
                //Game.player_id = createActor((float)x, (float)y, angle, radius, control, 0, 0, 0, 999, UINT16_MAX, WEAPON_PISTOL, texture_filename);
            }
            else if (strcmp(buffer, "dude") == 0 && load_type != LOAD_SAVED_LEVEL)
            {
                Actor_t actor = {0};
                fscanf(level_file, "%f %f %lf %d %d %d %u %d %d %d %s",
                    &actor.position.x, &actor.position.y, &actor.angle, &actor.radius, &actor.ai_mode, &actor.ai_timer, &actor.target_id_primary,
                    &actor.health, &actor.trigger_on_death, &actor.primary_weapon_id, texture_filename);
                createActor(actor, texture_filename);
            }
            else if (strcmp(buffer, "entity") == 0 && load_type != LOAD_SAVED_LEVEL)
            {
                fscanf(level_file, "%d %s", &entity_id, entity_name);
                entity_type = searchStringArray(entity_name, entity_type_strings, NUM_ENTITYTYPES);
                if (entity_type == RETURN_ERROR)
                {
                    char level_error_string[200];
                    // replace later with just exit to main menu
                    sprintf(level_error_string, "Level load error: invalid entity type in %s.\n"
                              "Please check the level file!\n", level_name);
                    fclose(level_file);
                    quitError(level_error_string);
                }
                entityLoader(level_file, entity_id, entity_type);
            }
            else if (strcmp(buffer, "item") == 0 && load_type != LOAD_SAVED_LEVEL)
            {
                fscanf(level_file, "%s %d %d", item_name, &tilemap_location, &state);
                item_type = searchStringArray(item_name, item_type_strings, NUM_ITEMTYPES);
                if (item_type == RETURN_ERROR)
                {
                    char level_error_string[200];
                    // replace later with just exit to main menu
                    sprintf(level_error_string, "Level load error: invalid item type in %s.\n"
                              "Please check the level file!\n", level_name);
                    fclose(level_file);
                    quitError(level_error_string);
                }
                Items[Game.item_count].index = tilemap_location;
                Items[Game.item_count].state = state;
                Items[Game.item_count].type = item_type;
                Game.item_count++;
                if (Game.item_count >= Game.item_capacity)
                {
                    Game.item_capacity += 8;
                    Items = realloc(Items, Game.item_capacity * sizeof(Item_t));
                }
            }
        }
    }
    if (tileset_found == FALSE)
    {
        loadTileset(tileset_file);
    }
    fclose(level_file);
    if (load_type != LOAD_SAVED_LEVEL)
        testInitPlayerAnim();
}

void setEntityTilemap()
{
    int i;

    for (i = 0; i < MAX_ENTITIES; i++)
    {
        Entity_t* entity = &Entities[i];

        if (entity->type != ENT_COUNTER)
        {
            Tile_t* tile = getEntityTile(entity);
            tile->is_entity = 1;
            tile->entity_value = entity->id;

            if ((entity->type == ENT_DOOR || entity->type == ENT_BUTTON) && entity->state == 1)
            {
                tile->texture_id++;
                
                if (entity->type == ENT_DOOR)
                {
                    tile->obstacle = 0;
                    tile->block_bullets = 0;
                }
            }
        }
    }
}

void saveGameState(char* foldername)
{
    FILE* save_file;
    char savefilepath[50] = "SAVES/";

    strcat(savefilepath, foldername);
    if (!checkDirectoryExists(savefilepath))
    {
        createDirectory(savefilepath);
    }
    strcat(savefilepath, "CURSTATE.SAV");

    save_file = fopen(savefilepath, "wb");
    if (!save_file)
    {
        perror("fopen");
        delay(60000);
    }
    fwrite(&Game.current_level_name, LEVEL_NAME_MAX, 1, save_file);
    fwrite(&PLAYER_ACTOR.health, 2, 1, save_file);
    fwrite(&PLAYER_ACTOR.position.x, 8, 1, save_file);
    fwrite(&PLAYER_ACTOR.position.y, 8, 1, save_file);
    fwrite(&PLAYER_ACTOR.primary_weapon_id, 2, 1, save_file);
    fwrite(&System, sizeof(System_t), 1, save_file);
    fwrite(&Timers, sizeof(Timer_t), 1, save_file);
    fclose(save_file);
}

void saveLevelState(char* foldername, char* levelname)
{
    FILE* save_file;
    char savefilepath[50] = "SAVES/";

    strcat(savefilepath, foldername);
    if (!checkDirectoryExists(savefilepath))
    {
        createDirectory(savefilepath);
    }

    strcat(levelname, ".SAV");
    strcat(savefilepath, levelname);

    save_file = fopen(savefilepath, "wb");
    if (!save_file)
    {
        printf("%s", levelname);
        perror("fopen");
        delay(60000);
    }
    fwrite(&Game, sizeof(GameData_t), 1, save_file);
    fwrite(Game.Actors, sizeof(Actor_t), Game.actor_capacity, save_file);
    fwrite(Game.ActorsById, sizeof(id_t), Game.id_capacity, save_file);
    fwrite(ActorTemplates, sizeof(ActorTemplate_t), actortemplate_count, save_file);
    fwrite(Entities, sizeof(Entity_t), MAX_ENTITIES, save_file);
    fwrite(Items, sizeof(Item_t), Game.item_capacity, save_file);
    System.paused = FALSE;
    fclose(save_file);
}

void loadGameState(char* foldername)
{
    FILE* state_file;
    int player_hp, current_weapon;
    Vec2 player_loc;
    char savefilepath[50] = "SAVES/";

    strcat(savefilepath, foldername);
    strcat(savefilepath, "CURSTATE.SAV");

    if (checkFileExists(savefilepath))
    {
        state_file = fopen(savefilepath, "rb");
        fseek(state_file, 0x0F, SEEK_SET);
        fread(&player_hp, 2, 1, state_file);
        if (player_hp > 0) // avoid infinite death loop
            PLAYER_ACTOR.health = player_hp;
        fread(&player_loc.x, 8, 1, state_file);
        fread(&player_loc.y, 8, 1, state_file);
        fread(&current_weapon, 2, 1, state_file);
        PLAYER_ACTOR.position.x = player_loc.x;
        PLAYER_ACTOR.position.y = player_loc.y;
        PLAYER_ACTOR.primary_weapon_id = current_weapon;
        updateGridLoc(&PLAYER_ACTOR);
        // just in case the player's location in the save file IS on a portal (shouldn't be if level is made correctly)
        if (getEntityTypeAt(PLAYER_ACTOR.grid_loc) == ENT_PORTAL)
        {
            PLAYER_ACTOR.position = forceMove(PLAYER_ACTOR.position);
            updateGridLoc(&PLAYER_ACTOR);
        }
        fread(&System, sizeof(System_t), 1, state_file);
        fread(&Timers, sizeof(Timer_t), 1, state_file);
        fclose(state_file);
    }
    testInitPlayerAnim();
}

void loadLevelState(char* foldername, char* savename)
{
    FILE* save_file;
    char savefilepath[50] = "SAVES/";

    strcat(savefilepath, foldername);
    strcat(savefilepath, savename);

    save_file = fopen(savefilepath, "rb");
    if (!save_file)
    {
        printf("%s", savename);
        perror("fopen");
        delay(60000);
    }
    fseek(save_file, 0x21, SEEK_SET);
    fread(&Game.actor_count, 2, 1, save_file);
    fread(&Game.actor_capacity, 2, 1, save_file);
    fread(&Game.id_capacity, 2, 1, save_file);
    initGameData(Game.actor_capacity, Game.id_capacity);
    fread(&Game.item_count, 1, 1, save_file);
    fread(&Game.item_capacity, 1, 1, save_file);
    Items = malloc(Game.item_capacity * sizeof(Item_t));
    fread(&Game.player_id, 2, 1, save_file);
    fread(Game.Actors, sizeof(Actor_t), Game.actor_capacity, save_file);
    fread(Game.ActorsById, sizeof(id_t), Game.id_capacity, save_file);
    fread(ActorTemplates, sizeof(ActorTemplate_t), actortemplate_count, save_file);
    fread(Entities, sizeof(Entity_t), MAX_ENTITIES, save_file);
    fread(Items, sizeof(Item_t), Game.item_capacity, save_file);
    fclose(save_file);
    setEntityTilemap();
}

void levelTransition(char* prevlevelname, char* newlevelname)
{
    char prevsavename[30] = {'\0'};
    char newsavename[30] = {'\0'};
    char loadsavename[30] = {'\0'};
    char savepath[45] = "SAVES/CURRENT/";

    // create save file names
    strncpy(prevsavename, prevlevelname, (strlen(prevlevelname) - 4)); // drop the level filename ending
    strncpy(newsavename, newlevelname, (strlen(newlevelname) - 4)); // drop the level filename ending
    strcpy(loadsavename, newsavename); // copy the name of the new level
    strcat(loadsavename, ".SAV"); // add save filename ending
    strcat(savepath, loadsavename); // construct folder path for file existence check

    // save the level we're exiting and stats
    saveLevelState("CURRENT/", prevsavename);
    saveGameState("CURRENT/");

    if (checkFileExists(savepath)) // if savefile exists, load save
    {
        levelLoader(newlevelname, LOAD_SAVED_LEVEL);
        loadLevelState("CURRENT/", loadsavename);
    }
    else // else load everything from level file
        levelLoader(newlevelname, LOAD_NEW_LEVEL);

    // save current status of the level we're entering
    saveLevelState("CURRENT/", newsavename);

    // load stats
    loadGameState("CURRENT/");
}

void loadAfterDeath(char* currentlevel)
{
    char loadsavename[30] = {'\0'};
    char savepath[45] = "SAVES/CURRENT/";

    strncpy(loadsavename, currentlevel, (strlen(currentlevel) - 4)); // drop the level filename ending
    strcat(loadsavename, ".SAV"); // add save filename ending
    strcat(savepath, loadsavename); // construct folder path for file existence check

    if (checkFileExists(savepath)) // if savefile exists, load save
    {
        levelLoader(currentlevel, LOAD_SAVED_LEVEL);
        loadLevelState("CURRENT/", loadsavename);
        loadGameState("CURRENT/");
    }
    else // else load everything from level file
        levelLoader(currentlevel, LOAD_NEW_LEVEL);
}

// check level name from save file, return FALSE if CURSTATE cannot be opened
int checkLevelFromSave(char* foldername)
{
    FILE* save_file;
    char savefilepath[50] = "SAVES/";
    int i;

    strcat(savefilepath, foldername);
    strcat(savefilepath, "CURSTATE.SAV");

    if (checkFileExists(savefilepath))
    {
        save_file = fopen(savefilepath, "rb");
        for (i = 0; i < LEVEL_NAME_MAX; i++)
        {
            fread(&levelname_global[i], 1, 1, save_file);
        }
        fclose(save_file);
        return TRUE;
    }
    return FALSE;
}

void saveGame(char* foldername)
{
    char savepath[25] = "SAVES/"; // needed for the copy function
    char savefilename[15] = {'\0'};

    strcat(foldername, "/"); // add slash to the save subfolder name
    strcat(savepath, foldername);

    if (!checkDirectoryExists(savepath)) // if folder doesn't exist, create it
        createDirectory(savepath);

    copyAllFolderToFolder("SAVES/CURRENT/", savepath);
    strncpy(savefilename, Game.current_level_name, (strlen(Game.current_level_name) - 4)); // drop the level filename ending
    saveLevelState(foldername, savefilename);
    saveGameState(foldername);
}

int loadGame(char* foldername)
{
    char loadname[30] = {'\0'};
    char savepath[45] = "SAVES/";
    char savefolder[45] = "SAVES/";
    strcat(foldername, "/"); // add subfolder slash
    strcat(savepath, foldername); // construct full save folder path
    strcat(savefolder, foldername); // separate copy for the file copying function
    if (checkLevelFromSave(foldername)) // if CURSTATE is opened successfully, continue function
    {
        strncpy(loadname, levelname_global, (strlen(levelname_global) - 4)); // drop the level filename ending
        strcat(loadname, ".SAV"); // add save filename ending
        strcat(savepath, loadname); // construct full save file path

        if (checkFileExists(savepath)) // if savefile exists, load save, else do nothing
        {
            levelLoader(levelname_global, LOAD_SAVED_LEVEL);
            loadLevelState(foldername, loadname);
            loadGameState(foldername);
            deleteDirectoryContents("SAVES/CURRENT/"); // empty CURRENT folder
            copyAllFolderToFolder(savefolder, "SAVES/CURRENT/"); // copy all contents to the CURRENT save folder
            levelname_global[0] = '\0'; // reset levelname
            return TRUE;
        }
    }
    levelname_global[0] = '\0'; // reset levelname
    return FALSE;
}

void quickLoad()
{
    loadGame("QUICK");
}

void quickSave()
{
    saveGame("QUICK");
}