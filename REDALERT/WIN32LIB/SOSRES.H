//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/****************************************************************************

   File              : sosres.h

   Programmer(s)     : Don Fowler, Nick Skrepetos
   Date              :

   Purpose           : Include Files For Zortech C++ Compiler

   Last Updated      :

****************************************************************************
               Copyright(c) 1993,1994 Human Machine Interfaces 
                            All Rights Reserved
****************************************************************************/

#define  _SOS_RESOURCE
#ifndef  _SOS_RESOURCE
#define  _SOS_RESOURCE

// structure for resource file header
typedef struct
{
   // file version
   WORD     wVersion;

   // file size
   LONG     dwFileSize;

   // number of resources in file
   WORD     wResCount;

   // offset of resource data from top of file
   LONG     dwResOffset;

   // offset of sync track from top of file
   LONG     dwSyncTrackOffset;

}  _RES_FILE_HEADER;

// structure for resource block header
typedef struct
{
   // resource id
   WORD     wID;

   // resource type
   WORD     wResType;

   // offset of next block
   LONG     dwNextBlock; 

   // size of the current resource information
   LONG     dwResSize;

   // rate to play block at
   WORD     wBlockRate;

   // id for the sync track to use
   WORD     wSyncTrackID;

}  _RES_BLOCK_HEADER;

// structure for sync mark tag
typedef struct _tagSYNCMARK
{
   // ID of the type of mark being used
   WORD  wID;

   // location in data of sync mark
   LONG  dwSyncOffset;

   // length of sync block
   LONG  dwSyncSize;

   // start sample data
   _SOS_START_SAMPLE sampleData;
    
} _RES_SYNCMARK;

typedef union
{
   // structure for sync mark tag
   _RES_SYNCMARK  syncMark; 

} _RES_TAG;

// union for filter information for prepareWave
typedef union
{
   // filter type
   WORD     wFilterID;

   // structure for volume
   struct   volume 
   {
      WORD  wVolume;
   };

   // structure for delay
   struct   delay
   {  
      WORD  wDelaySamples;
   };

} _SOS_FILTER; 

 
#endif

