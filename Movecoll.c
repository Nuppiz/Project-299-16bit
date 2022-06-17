#include "Common.h"
#include "Structs.h"
#include "Vectors.h"
#include "Keyb.h"

/* Object_t movement and collision detection */

extern GameData_t Game;

void checkGridLoc(Object_t* obj) // circle's location on the grid
{   
    // calculated by dividing the circle's x/y location by square size
    obj->grid_loc.x = obj->position.x / SQUARE_SIZE;
    obj->grid_loc.y = obj->position.y / SQUARE_SIZE;
}

int getTileType(Vec2 pos)
{
    int object_tile; // tile which the object is on (or attempting to be), i.e. array index number from grid_array
    uint8_t tile_type;
    
    // calculate current grid position
    pos.x /= SQUARE_SIZE;
    pos.y /= SQUARE_SIZE;
    
    // check which grid_array index it corresponds to
    object_tile = (int)pos.y * Game.Map.width + (int)pos.x;
    
    tile_type = Game.Map.collision[object_tile]; // check which colour is at that index
    
    return tile_type; // return said colour
}

void edgeDetectObject(Object_t* obj)
{
    if (obj->position.x - obj->radius <= 0) // left edge
    {
        obj->position.x = 0 + obj->radius;
        obj->velocity.x = 0.0;
    }
    else if (obj->position.x + obj->radius >= (SCREEN_WIDTH - 1)) // right edge
    {
        obj->position.x = (SCREEN_WIDTH - 1) - obj->radius;
        obj->velocity.x = 0.0;
    }
    
    if (obj->position.y - obj->radius <= 0) // top edge
    {
        obj->position.y = 0 + obj->radius;
        obj->velocity.y = 0.0;
    }
    else if (obj->position.y + obj->radius >= (SCREEN_HEIGHT - 1)) // bottom edge
    {
        obj->position.y = (SCREEN_HEIGHT - 1) - obj->radius;
        obj->velocity.y = 0.0;
    }
}

void edgeDetectAllObjects()
{
    int i = 0;
    while (i < Game.object_count)
    {
        edgeDetectObject(&Game.Objects[i]);
        i++;
    }
}

void controlObject(Object_t* obj)
{
    float max_speed;
    double turn_rate;

    max_speed = (isBitSet(obj->control, CONTROL_FAST)) ? RUN_SPEED : WALK_SPEED;
    turn_rate = (isBitSet(obj->control, CONTROL_FAST)) ? FAST_TURN_RATE : TURN_RATE;

    obj->direction.x = cos(obj->angle); // calculate directional vector
    obj->direction.y = sin(obj->angle);
    
    if (isBitSet(obj->control, CONTROL_UP) && obj->magnitude <= max_speed)
    {
        obj->velocity.x += obj->direction.x * ACCELERATION_RATE;
        obj->velocity.y += obj->direction.y * ACCELERATION_RATE;
    }
    else if (isBitSet(obj->control, CONTROL_DOWN) && obj->magnitude <= max_speed)
    {
        obj->velocity.x -= obj->direction.x * ACCELERATION_RATE;
        obj->velocity.y -= obj->direction.y * ACCELERATION_RATE;
    }
    else if (obj->magnitude > 0)
    {
        if (obj->magnitude < STOP_SPEED)
        {
            obj->velocity.x = 0;
            obj->velocity.y = 0;
        }
        else
        {
            obj->velocity.x /= DECELERATION_RATE;
            obj->velocity.y /= DECELERATION_RATE;
        }
    }

    if (isBitSet(obj->control, CONTROL_LEFT))
    {
        obj->angle -= turn_rate;
        if (obj->angle < 0)
            obj->angle = RAD_360;
    }
    if (isBitSet(obj->control, CONTROL_RIGHT))
    {
        obj->angle += turn_rate;
        if (obj->angle > RAD_360)
            obj->angle = 0;
    }
}

void controlAllObjects()
{
    int i = 0;
    
    // copy control variable from Input.c to the player object's control variable
    // in this way, completely separating input handling and physics with a single-variable "abstraction layer"
    extern uint8_t player_control;
    PlayerObject.control = player_control;

    while (i < Game.object_count)
    {
        controlObject(&Game.Objects[i]);
        i++;
    }
}

void moveObject(Object_t* obj, Vec2 movement)
{
    // collision box around the object
    Vec2 test_point_a;
    Vec2 test_point_b;
    
    checkGridLoc(obj);
    
    if (movement.x > 0) // if moving to the right
    {
        obj->position.x += movement.x; // temporarily move the object
        // test_point_a = top right corner
        test_point_a.x = obj->position.x + obj->radius;
        test_point_a.y = obj->position.y - obj->radius;
        // test_point_b = bottom right corner
        test_point_b.x = obj->position.x + obj->radius;
        test_point_b.y = obj->position.y + obj->radius;
        
        // if the movement would result in the object moving inside of a wall...
        if (getTileType(test_point_a) == WALL || getTileType(test_point_b) == WALL)
        {
            // ...cancel movement and set velocity to 0
            obj->position.x = (obj->grid_loc.x + 1) * SQUARE_SIZE - obj->radius - 1;
            obj->velocity.x = 0.0;
        }
    }
    else if (movement.x < 0) // if moving to the left
    {
        obj->position.x += movement.x;
        // test_point_a = top left corner
        test_point_a.x = obj->position.x - obj->radius;
        test_point_a.y = obj->position.y - obj->radius;
        // test_point_b = bottom left corner
        test_point_b.x = obj->position.x - obj->radius;
        test_point_b.y = obj->position.y + obj->radius;
        
        if (getTileType(test_point_a) == WALL || getTileType(test_point_b) == WALL)
        {
            obj->position.x = obj->grid_loc.x * SQUARE_SIZE + obj->radius;
            obj->velocity.x = 0.0;
        }
    }

    if (movement.y < 0) // if moving towards the top
    {
        obj->position.y += movement.y;
        // test_point_a = top right corner
        test_point_a.x = obj->position.x + obj->radius;
        test_point_a.y = obj->position.y - obj->radius;
        // test_point_b = top left corner
        test_point_b.x = obj->position.x - obj->radius;
        test_point_b.y = obj->position.y - obj->radius;
        
        if (getTileType(test_point_a) == WALL || getTileType(test_point_b) == WALL)
        {
            obj->position.y = obj->grid_loc.y * SQUARE_SIZE + obj->radius;
            obj->velocity.y = 0.0;
        }
    }
    else if (movement.y > 0) // if moving towards the bottom
    {
        obj->position.y += movement.y;
        // test_point_a = bottom right corner
        test_point_a.x = obj->position.x + obj->radius;
        test_point_a.y = obj->position.y + obj->radius;
        // test_point_b = bottom left corner
        test_point_b.x = obj->position.x - obj->radius;
        test_point_b.y = obj->position.y + obj->radius;
        
        if (getTileType(test_point_a) == WALL || getTileType(test_point_b) == WALL)
        {
            obj->position.y = (obj->grid_loc.y + 1) * SQUARE_SIZE - obj->radius - 1;
            obj->velocity.y = 0.0;
        }
    }
    checkGridLoc(obj);
}

void moveAllObjects()
{
    int i = 0;
    
    // iterate through the object array
    while (i < Game.object_count)
    {
        moveObject(&Game.Objects[i], Game.Objects[i].velocity);
        Game.Objects[i].magnitude = getVec2Length(Game.Objects[i].velocity);
        i++;
    }
}

void collideTwoObjects(Object_t* object_a, Object_t* object_b)
{
    float distance_x;
    float distance_y;
    float distance;
    float collision_depth;
    //int tile_type;
    Vec2 u; // how much each object moves in case of a collision
    
    distance_x = object_a->position.x - object_b->position.x;  // x-distance between the two objects
    distance_y = object_a->position.y - object_b->position.y;  // y-distance between the two objects
    
    // actual distance between the two objects (hypotenuse of the x and y distances)
    distance = sqrt((distance_x * distance_x) + (distance_y * distance_y));
    
    // if distance is less than combined radiuses
    if (distance < (object_a->radius + object_b->radius))
    {
        // calculate how much the objects are "inside" each other
        collision_depth = (object_a->radius + object_b->radius) - distance;
        
        //each object is moved for half of that
        u.x = (distance_x/distance) * (collision_depth/2);
        u.y = (distance_y/distance) * (collision_depth/2);
        
        // first object gets the values as is...
        moveObject(object_a, u);
        
        // ...and for the second object they are flipped
        u.x = -u.x;
        u.y = -u.y;
        moveObject(object_b, u);
    }
}

void collideAllObjects()
{
    int x;
    int i;
    
    // iterate through each object pair to see if they collide
    for (i = 0; i < Game.object_count; i++)
    {
        for (x = i; x < Game.object_count-1; x++)
        {
            collideTwoObjects(&Game.Objects[i], &Game.Objects[x+1]);
        }
    }
    
    // also check that none of the objects are going beyond the screen boundaries
    // edgeDetect();
}

void physics()
{
    controlAllObjects();
    moveAllObjects();
    collideAllObjects();

    #if DEBUG == 1
    sprintf(debug[DEBUG_VELOCITY], "V.X: %f\nV.Y %f", PlayerObject.velocity.x, PlayerObject.velocity.y);
    #endif
}