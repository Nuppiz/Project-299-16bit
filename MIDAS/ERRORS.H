/*      ERRORS.H
 *
 * MIDAS Sound System error codes and error message strings
 *
 * Copyright 1995 Petteri Kangaslampi and Jarno Paananen
 *
 * This file is part of the MIDAS Sound System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Sound System license, LICENSE.TXT. By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/


#ifndef __ERRORS_H
#define __ERRORS_H


#define MAXERRORS 256       /* maximum number of errors to store */


/****************************************************************************\
*       enum ErrorCodes
*       ---------------
* Description:  MIDAS Sound System error codes
\****************************************************************************/

enum ErrorCodes
{
    OK = 0,                             /* no error */
    errUndefined,                       /* undefined error */
    errOutOfMemory,                     /* out of (conventional) memory */
    errHeapCorrupted,                   /* (conventional memory) heap
                                           corrupted */
    errInvalidBlock,                    /* invalid memory block */
    errOutOfEMS,                        /* out of EMS memory */
    errEMSHeapCorrupted,                /* EMS heap corrupted */
    errInvalidEMSBlock,                 /* invalid EMS memory block */
    errEMMFailure,                      /* Expanded Memory Manager failure */
    errOutOfCardMemory,                 /* out of soundcard memory */
    errCardHeapCorrupted,               /* soundcard heap corrupted */
    errInvalidCardBlock,                /* invalid soundcard memory block */
    errNoInstHandles,                   /* out of instrument handles */
    errFileOpen,                        /* unable to open file */
    errFileRead,                        /* unable to read file */
    errInvalidModule,                   /* invalid module file */
    errInvalidInst,                     /* invalid instrument in module */
    errInvalidPatt,                     /* invalid pattern data in module */
    errInvalidChanNumber,               /* invalid channel number */
    errInvalidInstHandle,               /* invalid instrument handle */
    errNoChannels,                      /* Sound Device channels not open */
    errSDFailure,                       /* Sound Device hardware failure */
    errInvalidArguments,                /* invalid function arguments */
    errFileNotFound,                    /* file does not exist */
    errInvalidFileHandle,               /* invalid file handle */
    errAccessDenied,                    /* access denied */
    errFileExists,                      /* file exists */
    errTooManyFiles,                    /* too many open files */
    errDiskFull,                        /* disk full */
    errEndOfFile,                       /* unexpected end of file */
    errInvalidPath,                     /* invalid path */
    errFileWrite,                       /* unable to write file */
    errVDSLock,                         /* unable to lock VDS DMA buffer */
    errVDSUsage,                        /* unable to use Virtual DMA */
    errBadVDS,                          /* invalid VDS version */
    errDPMIFailure,                     /* DPMI failure */
    errInvalidDescriptor,               /* invalid segment descriptor */
    errOutOfResources,                  /* out of system resources */
    errNumErrorCodes
};



/****************************************************************************\
*       enum FunctionIDs
*       ----------------
* Description:  ID numbers for first functions in all modules
\****************************************************************************/

enum FunctionIDs
{
    ID_error = 0,                       /* error handling */
    ID_dma = 100,                       /* DMA handling routines */
    ID_dsm = 200,                       /* Digital Sound Mixer */
    ID_ems = 300,                       /* EMS heap manager */
    ID_mem = 400,                       /* Conventional memory management */
    ID_mod = 500,                       /* Protracker Module Player */
    ID_s3m = 600,                       /* Scream Tracker 3 Module Player */
    ID_mtm = 700,                       /* Multitracker Module Player */
    ID_tmr = 1000,                      /* TempoTimer */
    ID_vu = 1100,                       /* Real VU meters */
    ID_rf = 1200,                       /* Raw file I/O */
    ID_file = 1300,                     /* High-level file I/O */
    ID_dpmi = 1400,                     /* DPMI functions */
    ID_gus = 2000,                      /* GUS Sound Device */
    ID_pas = 2100,                      /* PAS Sound Device */
    ID_wss = 2200,                      /* WSS Sound Device */
    ID_sb = 2300,                       /* SB Sound Device */
    ID_nsnd = 2900                      /* No Sound Sound Device */
};



#ifdef __cplusplus
extern "C" {
#endif


#ifdef DEBUG

/****************************************************************************\
*       struct errRecord
*       ----------------
* Description:  Error record for error list
\****************************************************************************/

typedef struct
{
    int         errorCode;              /* error code number */
    unsigned    functID;                /* ID for function that caused the
                                           error */
} errRecord;


extern errRecord GLOBALVAR  errorList[MAXERRORS];       /* error list */
extern unsigned GLOBALVAR   numErrors;  /* number of errors in list */



/****************************************************************************\
*
* Function:     void errAdd(int errorCode, unsigned functID);
*
* Description:  Add an error to error list
*
* Input:        int errorCode           error code
*               unsigned functID        ID for function that caused the error
*
\****************************************************************************/

void CALLING errAdd(int errorCode, unsigned functID);




/****************************************************************************\
*
* Function:     void errClearList(void)
*
* Description:  Clears the error list. Can be called if a error has been
*               handled without exiting the program to avoid filling the
*               error list with handled errors.
*
\****************************************************************************/

void CALLING errClearList(void);




/****************************************************************************\
*
* Function:     void errPrintList(void);
*
* Description:  Prints the error list to stderr
*
\****************************************************************************/

void CALLING errPrintList(void);


#endif




/****************************************************************************\
*
* Function:     void errErrorExit(char *msg)
*
* Description:  Set up standard text mode, print an error message and exit
*
* Input:        char *msg               pointer to error message, ASCIIZ
*
\****************************************************************************/

void CALLING errErrorExit(char *msg);




#ifdef __cplusplus
}
#endif




/* error message string pointers: */
extern char * GLOBALVAR errorMsg[errNumErrorCodes];



/* ERROR - adds an error to the MIDAS error list if DEBUG is defined.
   Does nothing otherwise. */

#ifdef DEBUG
    #define ERROR(errCode, functID) errAdd(errCode, functID)
#else
    #define ERROR(errCode, functID)
#endif


/* PASSERROR - passes error value in variable error on */

#ifdef DEBUG
    #define PASSERROR(functID) { errAdd(error, functID); return error; }
#else
    #define PASSERROR(functID) return error;
#endif



/* CLEARERRORLIST - clear the MIDAS error list if DEBUG is defined. */
#ifdef DEBUG
    #define CLEARERRORLIST errClearList();
#else
    #define CLEARERRORLIST ;
#endif



#endif
