/*      TIMER.H
 *
 * TempoTimer, v1.20
 *
 * Copyright 1995 Petteri Kangaslampi and Jarno Paananen
 *
 * This file is part of the MIDAS Sound System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Sound System license, LICENSE.TXT. By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/


#ifndef __TIMER_H
#define __TIMER_H

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************\
*
* Function:     int tmrGetScrSync(ushort *scrSync);
*
* Description:  Calculates the screen synchronization value for timer
*
* Input:        ushort *scrSync         pointer to screen synchronization
*                                       value
*
* Returns:      MIDAS error code.
*               Screen syncronization value used with tmrSyncScr() is stored
*               in *scrSync.
*
\****************************************************************************/

int CALLING tmrGetScrSync(ushort *scrSync);




/****************************************************************************\
*
* Function:     int tmrInit(void);
*
* Description:  Initializes TempoTimer.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING tmrInit(void);




/****************************************************************************\
*
* Function:     int tmrClose(void);
*
* Description:  Uninitializes TempoTimer. MUST be called if and ONLY if
*               tmrInit() has been called.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING tmrClose(void);




/****************************************************************************\
*
* Function:     int tmrPlaySD(SoundDevice *SD);
*
* Description:  Starts playing sound with a Sound Device ie. calling its
*               Play() function in the update rate, which is set to
*               50Hz.
*
* Input:        SoundDevice *SD         Sound Device that will be used
*
* Returns:      MIDAS error code.
*
\****************************************************************************/

int CALLING tmrPlaySD(SoundDevice *SD);




/****************************************************************************\
*
* Function:     int tmrStopSD(void);
*
* Description:  Stops playing sound with the Sound Device.
*
* Returns:      MIDAS error code.
*
\****************************************************************************/

int CALLING tmrStopSD(void);




/****************************************************************************\
*
* Function:     int tmrPlayMusic(void *play, int *playerNum);
*
* Description:  Starts playing music with the timer.
*
* Input:        void *play              Pointer to music playing function,
*                                       must return MIDAS error codes
*               int *playerNum          Pointer to player number, used
*                                       for stopping music
*
* Returns:      MIDAS error code. Player number is written to *playerNum.
*
* Notes:        There can be a maximum of 16 music players active at the
*               same time.
*
\****************************************************************************/

int CALLING tmrPlayMusic(void CALLING *play, int *playerNum);




/****************************************************************************\
*
* Function:     int tmrStopMusic(int playerNum);
*
* Description:  Stops playing music with the timer.
*
* Input:        int playerNum           Number of player to be stopped.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING tmrStopMusic(int playerNum);




/****************************************************************************\
*
* Function:     int tmrSyncScr(ushort sync, void (*preVR)(), void (*immVR)(),
*               void (*inVR)());
*
* Description:  Synchronizes the timer to screen refresh.
*
* Input:        ushort sync             Screen synchronization value returned
*                                       by tmrGetScrSync().
*               void (*preVR)()         Pointer to the routine that will be
*                                       called BEFORE Vertical Retrace
*               void (*immVR)()         Pointer to the routine that will be
*                                       called immediately after Vertical
*                                       Retrace starts
*               void (*inVR)()          Pointer to the routine that will be
*                                       called some time during Vertical
*                                       Retrace
*
* Returns:      MIDAS error code
*
* Notes:        preVR() and immVR() functions must be as short as possible
*               and do nothing else than update counters or set some VGA
*               registers to avoid timer synchronization problems. inVR()
*               can take a longer time and can be used for, for example,
*               setting the palette.
*
\****************************************************************************/

int CALLING tmrSyncScr(ushort sync, void CALLING (*preVR)(),
    void CALLING (*immVR)(), void CALLING (*inVR)());




/****************************************************************************\
*
* Function:     int tmrStopScrSync(void);
*
* Description:  Stops synchronizing the timer to the screen.
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING tmrStopScrSync(void);




/****************************************************************************\
*
* Function:     int tmrSetUpdRate(ushort updRate);
*
* Description:  Sets the timer update rate, ie. the rate at which the music
*               playing routines are called
*
* Input:        ushort updRate          updating rate, in 100*Hz (5000=50Hz)
*
* Returns:      MIDAS error code
*
\****************************************************************************/

int CALLING tmrSetUpdRate(ushort updRate);


#ifdef __cplusplus
}
#endif


/****************************************************************************\
*       enum tmrFunctIDs
*       ----------------
* Description:  ID numbers for TempoTimer functions
\****************************************************************************/

enum tmrFunctIDs
{
    ID_tmrGetScrSync = ID_tmr,
    ID_tmrInit,
    ID_tmrClose,
    ID_tmrPlaySD,
    ID_tmrStopSD,
    ID_tmrPlayMusic,
    ID_tmrStopMusic,
    ID_tmrSyncScr,
    ID_tmrStopScrSync,
    ID_tmrSetUpdRate
};



#endif
