/*      MPLAYER.H
 *
 * Module Player definitions
 *
 * Copyright 1995 Petteri Kangaslampi and Jarno Paananen
 *
 * This file is part of the MIDAS Sound System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Sound System license, LICENSE.TXT. By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/


#ifndef __MPLAYER_H
#define __MPLAYER_H


#define MPHDRSIZE 1084                  /* header size needed for
                                           ModulePlayer.Identify() */



/****************************************************************************\
*       enum modIDNumbers
*       -----------------
* Description:  mpModule ID numbers
\****************************************************************************/

enum modIDNumbers
{
    idS3M = 0,                          /* Scream Tracker 3 module */
    idMOD = 1,                          /* Protracker module */
    idMTM = 2                           /* Multitracker module */
};



/****************************************************************************\
*       struct mpChanInfo
*       -----------------
* Description:  Module Player channel info structure
\****************************************************************************/

typedef struct
{
    uchar   flags;                      /* bits 0-4 - UNDEFINED
                                           bit 5 - new note and/or instrument
                                           bit 6 - new volume (S3M)
                                           bit 7 - command */
    uchar   note;                       /* note number */
    uchar   instrument;                 /* instrument number */
    uchar   volume;                     /* playing volume */
    uchar   command;                    /* command number */
    uchar   infobyte;                   /* command infobyte */
    uchar   volumebar;                  /* "fake" volume bar */
    char    (far *commandname);         /* pointer to command name string,
                                           ASCIIZ */
} mpChanInfo;




/****************************************************************************\
*       struct mpInformation
*       --------------------
* Description:  Module Player information structure
\****************************************************************************/

typedef struct
{
    ushort      numChannels;            /* number of channels */
    ushort      setFrame;               /* 1 if "set frame", 0 if not */
    ushort      speed;                  /* playing speed */
    ushort      BPM;                    /* playing BPM tempo */
    ushort      pos;                    /* current playing position */
    ushort      pattern;                /* current pattern number */
    ushort      row;                    /* current row number */
    ushort      loopCnt;                /* song loop counter */
    mpChanInfo  *chans;                 /* pointer to channel data
                                           structures. */
} mpInformation;



/****************************************************************************\
*       struct mpInstrument
*       -------------------
* Description:  Module Player instrument structure in memory
\****************************************************************************/

typedef struct
{
    char        fileName[13];           /* DOS file name, ASCIIZ */
    char        iname[32];              /* instrument name, ASCIIZ */
    uchar       *sample;                /* pointer to sample data or NULL
                                           if not available (no sample or
                                           deallocated) */
    ushort      length;                 /* sample length */
    ushort      loopStart;              /* sample loop start */
    ushort      loopEnd;                /* sample loop end */
    uchar       looping;                /* 1 if looping sample, 0 if not */
    uchar       volume;                 /* sample default playing volume */
    ulong       c2Rate;                 /* C2 sampling rate */
    uchar       finetune;               /* sample default finetune value */
    ushort      sdInstHandle;           /* Sound Device instrument handle for
                                           this instrument or 0 if not added
                                           to Sound Device instrument list */
} mpInstrument;



/****************************************************************************\
*       struct mpPattern
*       ----------------
* Description:  Module Player pattern structure in memory
\****************************************************************************/

typedef struct
{
    ushort      length;                 /* pattern data length in bytes */
    uchar       data[];                 /* actual pattern data */
} mpPattern;



struct mpStruct;



/****************************************************************************\
*       struct mpModule
*       ---------------
* Description:  Module Player module in memory
\****************************************************************************/

typedef struct
{
    char        ID[4];                  /* Module identifier */
    ushort      IDnum;                  /* ID number (0=S3M, 1=MOD, 2=MTM) */
    struct mpStruct *MP;                /* Module Player used for loading,
                                           playing etc. */
    char        songName[32];           /* song name, ASCIIZ */
    ushort      songLength;             /* number of orders */
    ushort      numInsts;               /* number of instruments */
    ushort      numPatts;               /* number of patterns */
    ushort      numChans;               /* number of channels */
    struct                              /* flags: (S3M) */
	{
		int 	st2Vibrato : 1; 		/* Scream Tracker 2 vibrato */
		int 	st2Tempo : 1;			/* Scream Tracker 2 tempo */
		int 	ptSlides : 1;			/* ProTracker slides */
		int 	zeroVolOpt : 1; 		/* 0-volume optimizations */
		int 	ptLimits : 1;			/* ProTracker limits */
		int 	filter : 1; 			/* Enable filter / sfx */
        int     fastVolSlide : 1;       /* Fast volume slides ( < ST3.2) */
        int     requiresALE : 1;        /* Requires ALE-logic in playing */
        int     extendedOctaves : 1;    /* Requires extended octaves */
        int     unused : 7;
    } flags;
    uchar       masterVol;              /* master volume */
    uchar       speed;                  /* initial speed */
    uchar       tempo;                  /* initial BPM tempo */
    uchar       masterMult;             /* master multiplier */
    uchar       stereo;                 /* 1 = stereo, 0 = mono */
    signed char chanSettings[32];       /* channel panning values */
    uchar       *orders;                /* pointer to pattern orders */
    mpInstrument *insts;                /* pointer to instrument datas */
    mpPattern   **patterns;             /* pointer to array of pattern
                                           pointers */
    uchar       *instsUsed;             /* one byte for each instrument - 1 if
                                           instrument is used, 0 if not */
} mpModule;



/****************************************************************************\
*       struct ModulePlayer
*       -------------------
* Description:  Module Player structure. See MPLAYER.TXT for documentation
\****************************************************************************/

typedef struct mpStruct
{
    int CALLING (far *Identify)(uchar *header, int *recognized);

    int CALLING (far *Init)(SoundDevice *SD);
    int CALLING (far *Close)(void);

    int CALLING (far *LoadModule)(char *fileName, SoundDevice *SD,
        int CALLING (*SaveSampleInfo)(ushort sdInstHandle, uchar *sample,
        ushort slength, ushort loopStart, ushort loopEnd), mpModule **module);
    int CALLING (far *FreeModule)(mpModule *module, SoundDevice *SD);

    int CALLING (far *PlayModule)(mpModule *module, ushort firstSDChannel, ushort
        numChannels, ushort loopStart, ushort loopEnd);
    int CALLING (far *StopModule)(void);

    int CALLING (far *SetUpdRateFunct)(int CALLING
        (*SetUpdRate)(ushort updRate));

    int CALLING (far *Play)(void);

    int CALLING (far *SetPosition)(ushort pos);
    int CALLING (far *GetInformation)(mpInformation **info);
    int CALLING (far *SetMasterVolume)(uchar volume);
} ModulePlayer;



#endif
