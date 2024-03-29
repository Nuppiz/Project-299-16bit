#ifndef DEFINES_H
#define DEFINES_H

#define DEBUG               1
#define DEBUG_STR_LEN       128

#define FRAME_RATE          30
#define TICK_RATE           30

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */

#define PALETTE_WRITE       0x03C8
#define PALETTE_DATA        0x03C9

#if SCREEN_WIDTH == 320
#define SET_PIXEL(x,y,color) screen_buf[(((y)<<8)+((y)<<6)) + (x)] = color
#else
#define SET_PIXEL(x,y,color) screen_buf[(y)*SCREEN_WIDTH + (x)] = color
#endif/* DEFINES_H */

#if SCREEN_WIDTH == 320
#define SET_PIXEL_VGA(x,y,color) VGA[(((y)<<8)+((y)<<6)) + (x)] = color
#else
#define SET_PIXEL_VGA(x,y,color) VGA[(y)*SCREEN_WIDTH + (x)] = color
#endif/* DEFINES_H */

#define KB_ARRAY_LENGTH     256
#define KB_QUEUE_LENGTH     256

#define KEY_HIT_FLAG        1
#define KEY_PRESSED_FLAG    2
#define KEY_RELEASED_FLAG   0x80 // 128, MSB
#define KEY_SPECIAL_CODE    0xE0 // 224
#define KEY_IS_PRESSED(k)   (Keyboard.keystates[k] & KEY_PRESSED_FLAG)
#define KEY_WAS_HIT(k)      (Keyboard.keystates[k] & KEY_HIT_FLAG)
#define KEY_WAS_RELEASED(k) (Keyboard.keystates[k] & KEY_RELEASED_FLAG)
#define KEY_WAS_TOGGLED(k)  (Keyboard.keystates[k] & (KEY_HIT_FLAG|KEY_RELEASED_FLAG))

#define wrapEnd(x,max)    if ((x)>=(max))   (x)=0
#define wrapStart(x,max)  if ((x)<0)        (x)=(max)-1   
#define incAndWrap(x,max) if (++(x)>=(max)) (x)=0
#define decAndWrap(x,max) if (--(x)<0)      (x)=(max)-1
#define rWrapEnd(x,max)    ((x)>=(max)) ? 0:(x)
#define rWrapStart(x,max)  ((x)<0) ? (max)-1:(x)
#define rIncAndWrap(x,max) ((x)+1>=(max)) ? 0:(x)+1
#define rDecAndWrap(x,max) ((x)-1<0) ? (max)-1:(x)-1

#define SCAN_BUFFER_SIZE    32
#define KEYHANDLER_INT      9
 
#define X_AXIS              1
#define Y_AXIS              2

#define SQUARE_SIZE         20
#define SCREEN_SIZE         64000
#define SQUARE_ROWS         (SCREEN_HEIGHT/SQUARE_SIZE)
#define SQUARE_COLUMNS      (SCREEN_WIDTH/SQUARE_SIZE)
#define degToRad(degree)    ((degree)*M_PI/180.0)

#define RAD_360             (2*M_PI)
#define RAD_345             (23*M_PI/12.0)
#define RAD_315             (21*M_PI/12.0)
#define RAD_285             (19*M_PI/12.0)
#define RAD_270             (1.5*M_PI)
#define RAD_255             (17*M_PI/12.0)
#define RAD_225             (15*M_PI/12.0)
#define RAD_195             (13*M_PI/12.0)
#define RAD_180             M_PI
#define RAD_165             (11*M_PI/12.0)
#define RAD_135             (9*M_PI/12.0)
#define RAD_120             (M_PI/1.5)
#define RAD_105             (7*M_PI/12.0)
#define RAD_90              (M_PI/2.0)
#define RAD_75              (5*M_PI/12.0)
#define RAD_60              (M_PI/3.0)
#define RAD_45              (M_PI/4.0)
#define RAD_30              (M_PI/6.0)
#define RAD_15              (M_PI/12.0)
#define RAD_10              (M_PI/18.0)
#define RAD_5               (M_PI/36.0)
#define RAD_1               (M_PI/180.0)

#define VIEW_ANGLE_COS      0.5
#define LOS_STEP            10
#define LOS_STEP_SQ         (LOS_STEP*LOS_STEP)
#define OUT_OF_SIGHT        0
#define IN_SIGHT            1

#define WALL                0
#define COLOUR_WHITE        4
#define COLOUR_RED          40
#define COLOUR_BLUE         200
#define COLOUR_YELLOW       160
#define COLOUR_PEACH        64
#define TRANSPARENT_COLOR   251

#define TILESET_MAX         100
#define DEFAULT_TILESET     "LEVELS/Default.til"
#define LEVEL_PATH          "LEVELS/"

#define DOT_DISTANCE        30
#define LOOK_DISTANCE       30

#define MAX_PARTICLES       128
#define MAX_ENTITIES        32
#define MAX_CORPSES         16
#define CORPSE_DIAMETER     16
#define MAX_TEMPSPRITES     32
#define NUM_ROTATIONS       12

#define WALK_SPEED          2.0
#define RUN_SPEED           3.5
#define ACCELERATION_RATE   0.5
#define DECELERATION_RATE   1.5
#define BRAKE_RATE          0.9 // this is not used anywhere right now
#define STOP_SPEED          0.2
#define TURN_RATE           RAD_5
#define FAST_TURN_RATE      RAD_10
#define STRAFE_SPEED        0.5
#define FAST_STRAFE_SPEED   1

#define CHASE_DISTANCE          75
#define CHASE_DISTANCE_SQ       (CHASE_DISTANCE*CHASE_DISTANCE)
#define MIN_CHASE_DISTANCE      60
#define MIN_CHASE_DISTANCE_SQ   (MIN_CHASE_DISTANCE*MIN_CHASE_DISTANCE)
#define CHASE_TIMEOUT           200
#define TURN_THRESHOLD          5.0

#define CHARACTER_WIDTH     8
#define CHARACTER_HEIGHT    8
#define CHARACTER_SIZE      72

#define SPRITE_IS_STATIC    0
#define SPRITE_IS_ANIM      1

#define TRUE                1
#define FALSE               0
#define RETURN_ERROR        -1

#define FULLY_OUT           0
#define PARTIAL             1
#define FULLY_IN            2

#define LEFT_SIDE           1
#define RIGHT_SIDE          2

#define VOLUME_DOWN         1
#define VOLUME_UP           2

#define CONTROL_8253        0x43
#define CONTROL_WORD        0x3C
#define COUNTER_0           0x40
#define TIME_KEEPER_INT     0x08 
#define TIMER_18HZ          0
#define TIMER_1000HZ        1193
#define PIC1_COMMAND        0x20
#define PIC2_COMMAND        0xA0
#define PIC_EOI             0x20

#define DWORD               uint32_t

#define LOW_BYTE(n)         (n & 0x00ff)
#define HIGH_BYTE(n)        ((n>>8) & 0x00ff)

#define BIT_0  1
#define BIT_1  2
#define BIT_2  4
#define BIT_3  8
#define BIT_4  16
#define BIT_5  32
#define BIT_6  64
#define BIT_7  128
#define BIT_8  256
#define BIT_9  512
#define BIT_10 1024
#define BIT_11 2048
#define BIT_12 4096
#define BIT_13 8192
#define BIT_14 16384
#define BIT_15 32768
#define BIT_16 65536
#define BIT_17 131072
#define BIT_18 262144
#define BIT_19 524288
#define BIT_20 1048576
#define BIT_21 2097152
#define BIT_22 4194304
#define BIT_23 8388608
#define BIT_24 16777216
#define BIT_25 33554432
#define BIT_26 67108864
#define BIT_27 134217728
#define BIT_28 268435456
#define BIT_29 536870912
#define BIT_30 1073741824
#define BIT_31 2147483648

#define CONTROL_UP          BIT_0
#define CONTROL_DOWN        BIT_1
#define CONTROL_LEFT        BIT_2
#define CONTROL_RIGHT       BIT_3
#define CONTROL_FAST        BIT_4
#define CONTROL_STRAFE_L    BIT_5
#define CONTROL_STRAFE_R    BIT_6
#define CONTROL_STRAFE_MOD  BIT_7
#define CONTROL_MOUSE_AIM   BIT_8

#define MOUSE_TURN_THRESHOLD   10.0

#define OBJ_DEFAULT_CAPACITY   16

#define OBJ_FLAG_DEAD          BIT_0

#define MAX_PROJECTILES        64
#define BULLET_STEP            4
#define BULLET_MAX_DISTANCE    150
#define BULLET_HIT_ANGLE       0.5
#define HITSCAN                -1
#define ACTIVE                 1
#define INACTIVE               0

#define RUN_ONCE               0
#define KILL_COMMAND           1
#define STATIC_SPRITE          0
#define MOVING_SPRITE          1

#define LOAD_NEW_LEVEL         0
#define LOAD_SAVED_LEVEL       1
#define LEVEL_NAME_MAX         15

#define MAX_SAVES              10
#define SAVENAME_MAX           12

#define HURT_INTERVAL          40
#define SFX_INTERVAL           20

#define STATE_IS_ACTIVE        BIT_0
#define STATE_ENABLE_UPDATE    BIT_1
#define STATE_ENABLE_DRAW      BIT_2

#define ALL_STATE_FLAGS (STATE_IS_ACTIVE|STATE_ENABLE_UPDATE|STATE_ENABLE_DRAW)

/* should be removed, just use bitwise operations directly */
#define setBit(bitfield, bit)   (bitfield) |= (bit)
#define clearBit(bitfield, bit) (bitfield) &= ~(bit)
#define isBitSet(bitfield, bit) ((bitfield) & (bit))

#define PLAYER_ACTOR Game.Actors[Game.ActorsById[Game.player_id]]

#endif/* DEFINES_H */