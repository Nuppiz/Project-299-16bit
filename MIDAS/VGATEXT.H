/*      VGATEXT.H
 *
 * VGA text output routines for MIDAS Sound System
 *
 * Copyright 1995 Petteri Kangaslampi and Jarno Paananen
 *
 * This file is part of the MIDAS Sound System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Sound System license, LICENSE.TXT. By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/


#ifndef __VGATEXT_H
#define __VGATEXT_H


#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************\
*
* Function:     void vgaWriteText(int x, int y, char *txt);
*
* Description:  Writes text on the screen
*
* Input:        int x                   X coordinate of string (up-left
*                                       corner is (1,1))
*               int y                   Y coordinate
*               char *txt               pointer to null-terminated text
*                                       string, which may contain also the
*                                       following special characters:
*                                           \xFF - next char is attribute
*                                           \x7F - next char is RLE count for
*                                               the character following it
*
\****************************************************************************/

void CALLING vgaWriteText(int x, int y, char *txt);




/****************************************************************************\
*
* Function:     void vgaWriteStr(int x, int y, char *str, char attr,
*               int txtlen);
*
* Description:  Writes a string on the screen
*
* Input:        int x                   X coordinate of the string
*               int y                   Y coordinate
*               char *str               pointer to a ASCIIZ string
*               char attr               attribute for the string
*               int txtlen              number of characters to be printed on
*                                       screen - padded with spaces
*
\****************************************************************************/

void CALLING vgaWriteStr(int x, int y, char *str, char attr, int txtlen);



/****************************************************************************\
*
* Function:     void vgaSetMode(int mode)
*
* Description:  Sets a VGA BIOS display mode
*
* Input:        int mode                BIOS mode number
*
\****************************************************************************/

void CALLING vgaSetMode(int mode);



/****************************************************************************\
*
* Function:     void vgaWriteByte(int x, int y, uchar byte, char attr);
*
* Description:  Writes a hex byte on the screen
*
* Input:        int x                   X coordinate
*               int y                   Y coordinate
*               uchar byte              byte to be written
*               char attr               attribute for the byte
*
\****************************************************************************/

void CALLING vgaWriteByte(int x, int y, uchar byte, char attr);




/****************************************************************************\
*
* Function:     void vgaFillRect(int x1, int y1, int x2, int y2, char attr);
*
* Description:  Draws a filled rectangle on the screen
*
* Input:        int x1                  X-coordinate of upper left corner
*               int y1                  Y-coordinate of upper left corner
*               int x2                  X-coordinate of lower left corner
*               int y2                  Y-coordinate of lower left corner
*               char attr               rectangle attribute
*
\****************************************************************************/

void CALLING vgaFillRect(int x1, int y1, int x2, int y2, char attr);




/****************************************************************************\
*
* Function:     void vgaDrawChar(int x, int y, char ch, char attr);
*
* Description:  Draws a single character on the screen
*
* Input:        int x                   character X-coordinate
*               int y                   character Y-coordinate
*               char ch                 character
*               char attr               character attribute
*
\****************************************************************************/

void CALLING vgaDrawChar(int x, int y, char ch, char attr);




/****************************************************************************\
*
* Function:     void vgaDrawChars(int x, int y, char ch, char attr, int num);
*
* Description:  Draws many charactersr on the screen
*
* Input:        int x                   character X-coordinate
*               int y                   character Y-coordinate
*               char ch                 character
*               char attr               character attribute
*               int num                 number characters to draw
*
\****************************************************************************/

void CALLING vgaDrawChars(int x, int y, char ch, char attr, int num);




/****************************************************************************\
*
* Function:     void vgaMoveCursor(int x, int y);
*
* Description:  Moves the text mode cursor to a new location
*
* Input:        int x                   cursor X-coordinate
*               int y                   cursor Y-coordinate
*
\****************************************************************************/

void CALLING vgaMoveCursor(int x, int y);




#ifdef __cplusplus
}
#endif


#endif
