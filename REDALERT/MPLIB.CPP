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

#include "types.h"
#include "mgenord.h"
#include "magic.h"
#include "rtq.h"
#include <mem.h>
#include <i86.h>
#include <assert.h>
#include "mplib.h"

#define CHUNNEL_INT 0x48

typedef union REGS REGISTERS;

void
Yield(void)
{
   REGISTERS   regs;

   regs.w.ax = 0x1680;
   int386(0x2f, &regs, &regs);
}

void
PostWindowsMessage(void)
{
   REGISTERS regs;

   regs.x.eax = DPMIAPI_POST_WINDOWS_ORD << 16 | MGENVXD_DEVICE_ID;
   regs.x.ebx = 0;
   regs.x.ecx = 0;
   int386(CHUNNEL_INT, &regs, &regs);
}

int MGenGetQueueCtr(int qNo)
{
   REGISTERS   regs;

   regs.x.eax = MGENVXD_GETQUEUECTR_ORD << 16 | MGENVXD_DEVICE_ID;
   regs.x.ebx = qNo;
   int386(CHUNNEL_INT, &regs, &regs);

   return regs.x.eax;
}

RTQ_NODE *MGenMoveTo(int qFrom, int qTo)
{
   REGISTERS   regs;

   regs.x.eax = MGENVXD_MOVENODE_ORD << 16 | MGENVXD_DEVICE_ID;
   regs.x.ebx = qFrom;
   regs.x.ecx = qTo;
   int386(CHUNNEL_INT, &regs, &regs);

   return (RTQ_NODE *) regs.x.eax;
}

RTQ_NODE *MGenGetNode(int q)
{
   REGISTERS   regs;

   regs.x.eax = MGENVXD_GETNODE_ORD << 16 | MGENVXD_DEVICE_ID;
   regs.x.ebx = q;
   int386(CHUNNEL_INT, &regs, &regs);

   return (RTQ_NODE *) regs.x.eax;
}

RTQ_NODE *MGenGetMasterNode(unsigned *size)
{
   REGISTERS   regs;

   regs.x.eax = MGENVXD_MASTERNODE_ORD << 16 | MGENVXD_DEVICE_ID;
   int386(CHUNNEL_INT, &regs, &regs);
   *size = regs.x.ecx;

   return (RTQ_NODE *) regs.x.eax;
}

int MGenFlushNodes(int qFrom, int qTo)
{
   REGISTERS   regs;

   regs.x.eax = MGENVXD_FLUSHNODE_ORD << 16 | MGENVXD_DEVICE_ID;
   regs.x.ebx = qFrom;
   regs.x.ecx = qTo;
   int386(CHUNNEL_INT, &regs, &regs);

   return regs.x.eax;
}

int MGenMCount(unsigned lowerOrderBits, unsigned upperOrderBits)
{
   REGISTERS   regs;

   regs.x.eax = MGENVXD_MCOUNT_ORD << 16 | MGENVXD_DEVICE_ID;
   regs.x.ebx = lowerOrderBits;
   regs.x.ecx = upperOrderBits;
   int386(CHUNNEL_INT, &regs, &regs);

   return regs.x.eax;
}

int MGenSanityCheck(void)
{
   REGISTERS   regs;

   regs.x.eax = MGENVXD_SANITYCHECK_ORD << 16 | MGENVXD_DEVICE_ID;
   int386(CHUNNEL_INT, &regs, &regs);

   return regs.x.eax;
}
