/* Old/deprecated stuff moved to storage */

// array which determines the colour of each square on the grid
/*uint8_t grid_array [] = {7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
                         7,  54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 7,
                         7,  55, 7,  7,  7,  55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 7,
                         7,  54, 7,  54, 55, 54, 55, 54, 55, 54, 55, 54, 7,  7,  55, 7,
                         7,  55, 7,  55, 54, 55, 54, 55, 54, 55, 7,  55, 54, 7,  54, 7,
                         7,  54, 55, 54, 55, 54, 7,  7,  55, 54, 7,  54, 55, 7,  55, 7,
                         7,  55, 54, 55, 54, 55, 7,  7,  54, 55, 7,  55, 54, 55, 54, 7,
                         7,  54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 7,
                         7,  55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 7,
                         7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7
};

// array which determines the texture of each square on the grid
uint8_t TextureMap [] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0,
            0, 2, 0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0,
            0, 1, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 0, 2, 0,
            0, 2, 0, 2, 1, 2, 1, 2, 1, 2, 0, 2, 1, 0, 1, 0,
            0, 1, 2, 1, 2, 1, 0, 0, 2, 1, 0, 1, 2, 0, 2, 0,
            0, 2, 1, 2, 1, 2, 0, 0, 1, 2, 0, 2, 1, 2, 1, 0,
            0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0,
            0, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

uint8_t CollisionMap [] =
{
    7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
    7,  54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 7,
    7,  55, 7,  7,  7,  55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 7,
    7,  54, 7,  54, 55, 54, 55, 54, 55, 54, 55, 54, 7,  7,  55, 54, 55, 54, 7,
    7,  55, 7,  55, 54, 55, 54, 55, 54, 55, 7,  55, 54, 7,  54, 55, 54, 55, 7,
    7,  54, 55, 54, 55, 54, 7,  7,  55, 54, 7,  54, 55, 7,  55, 54, 55, 54, 7,
    7,  55, 54, 55, 54, 55, 7,  7,  54, 55, 7,  55, 54, 55, 54, 55, 54, 55, 7,
    7,  54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 7,
    7,  55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 7,
    7,  54, 55, 54, 55, 54, 7,  7,  55, 54, 7,  54, 55, 54, 55, 54, 55, 54, 7,
    7,  55, 54, 55, 54, 55, 7,  7,  54, 55, 7,  55, 54, 7,  54, 55, 54, 55, 7,
    7,  54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 55, 54, 7,
    7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7
};

// array which determines the texture of each square on the grid
uint8_t TextureMap [] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0,
    0, 2, 0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0,
    0, 1, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, 0, 2, 1, 2, 1, 0,
    0, 2, 0, 2, 1, 2, 1, 2, 1, 2, 0, 2, 1, 0, 1, 2, 1, 2, 0,
    0, 1, 2, 1, 2, 1, 0, 0, 2, 1, 0, 1, 2, 0, 2, 1, 2, 1, 0,
    0, 2, 1, 2, 1, 2, 0, 0, 1, 2, 0, 2, 1, 2, 1, 2, 1, 2, 0,
    0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0,
    0, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 0,
    0, 1, 2, 1, 2, 1, 0, 0, 2, 1, 0, 1, 2, 1, 2, 1, 2, 1, 0,
    0, 2, 1, 2, 1, 2, 0, 0, 1, 2, 0, 2, 1, 0, 1, 2, 1, 2, 0,
    0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};*/