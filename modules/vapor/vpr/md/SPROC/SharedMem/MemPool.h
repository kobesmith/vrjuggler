/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_MemPool_h_
#define _VPR_MemPool_h_
//#pragma once

#include <vpr/vprConfig.h>
#include <stdlib.h>

namespace vpr 
{

//: Shared memory pool.
//
//    Base class that defines how all memory pools
// should behave.
//
// Author:
//	Allen Bierbaum
//
// Date: 1-9-97
//
class MemPool {

public:
    // ---> This is a prototype for what all derived constructors should take
    // as params
    // MemPool(size_t initialSize,  int numProcs, char* staticTempName)
    virtual ~MemPool() {;}

public:
    virtual void* allocate(size_t size) = 0;
    virtual void  deallocate(void* ptr) = 0;
    virtual void* reallocate(void *ptr, size_t new_sz) = 0;

};

} // End of vpr namespace

#if defined(VPR_USE_IRIX_SPROC)
#   include <vpr/md/SPROC/SharedMem/MemPoolSGI.h>

namespace vpr {
    typedef MemPoolSGI  SharedPool;
    typedef MemPoolSGI  SyncPool;
};
#endif	/* VPR_IRIX_SPROC */

#endif	/* _VPR_MemPool_h_ */
