/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 */

#ifndef _SIMPLE_PF_NAV_APP_H_
#define _SIMPLE_PF_NAV_APP_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <gl/device.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfEarthSky.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pf/pfTraverser.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pfdu.h>
#include <Performer/pfutil.h>

    // --- VR Juggler Stuff --- //
#include <Kernel/vjKernel.h>
#include <Kernel/Pf/vjPfApp.h>    // the performer application base type
#include <Kernel/vjDebug.h>
#include <Kernel/vjProjection.h>  // for setNearFar (for setting clipping planes)
#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>

#include <Utils/vjFileIO.h>

#include <Sound/vjSoundManager.h>
#include <Sound/pf/pfSoundNode.h> //performer-juggler sound node.
#include <Sound/pf/pfSoundTraverser.h>

// nav includes
#include <pfNavDCS.h>
#include <velocityNav.h>
#include <collider.h>
#include <planeCollider.h>
#include <pfPogoCollider.h>
#include <pfRayCollider.h>
#include <pfBoxCollider.h>
#include <vjPfAppStats.h>

#include "pfFileIO.h" // handy fileloading/caching functions

#include <Performer/pf/pfTraverser.h>

int AppNotifyPreTrav(pfTraverser* trav, void* data);
int AppNotifyPostTrav(pfTraverser* trav, void* data);

// Declare my application class
class simplePfApp : public vjPfApp
{
public:
   simplePfApp()
      mLightGroup( NULL ),
      mModels( NULL ),
      mRootNode( NULL ),
      mNavigationDCS( NULL )
   {
   }

   virtual ~simplePfApp (void) 
   {
      /* Do nothing. */ ;
   }

   virtual void init()
   {
      /* vjProjection::setNearFar( 0.4f, 200000 ); XXXX: */
   }

   virtual void apiInit()
   {
   }

   virtual void preForkInit()
   {
         std::cout << "simplePfNavAPP: Initializing performer file loaders for types like: "
                   << "terrain.flt" << "\n" << std::flush;

            pfdInitConverter( "terrain.flt" );
      }
   }

   // Initialize the scene graph
   virtual void initScene();

   // load the model into the scene graph
   // If another model is already in the scene graph, we destroy it and load the newly configured one.
   // The model loaded is based on the configuration information that we currently have
   // This may be called multiple times
   virtual void initializeModels();
   virtual void initializeSounds();

   /// Return the current scene graph
   virtual pfGroup* getScene()
   {
      return mRootNode;
   }

   //: Function called in application process for each active channel each frame
   // Called immediately before draw (pfFrame())
   virtual void appChanFunc(pfChannel* chan)
   {
   }

   // Function called by the DEFAULT drawChan function before clearing the channel
   // and drawing the next frame (pfFrame())
   virtual void preDrawChan(pfChannel* chan, void* chandata)
   {
   }

   /// Function called after pfSync and before pfDraw
   virtual void preFrame()
   {
   }

   /// Function called after pfDraw
   virtual void intraFrame()
   {
   }

   //: Reset the application to initial state
   virtual void reset()
   {
   }

public:
   // CONFIG PARAMS
   std::string    mFilePath;
   vjVec3         mInitialPos;

   // navigation objects.
   vjDigitalInterface         mVJButton;

   // SCENE GRAPH NODES
   pfGroup*       mLightGroup;
   pfLightSource* mSun;                      // Sun to light the environment
   pfGroup*       mRootNode;                 // The root of the scene graph
};

// ------- SCENE GRAPH ----
// a standard organized interface for derived applications:
//
//                            /-- mLightGroup -- mSun
// mRootNode -- mNavigationDCS -- mCollidableModelGroup -- mConfiguredCollideModels -- loaded stuff...
//          \-- mNoNav        \-- mUnCollidableModelGroup -- mConfiguredNoCollideModels -- loaded stuff...
//                                                       \-- mSoundNodes -- loaded stuff...
void simplePfApp::initScene()
{
   // Load the scene
   vjDEBUG(vjDBG_ALL, 0) << "simplePfApp::initScene\n" << vjDEBUG_FLUSH;

   // Allocate all the nodes needed
   mRootNode             = new pfGroup;       // Root of our graph
   mRootNode->setName("simplePfApp::mRootNode");
   mNavigationDCS        = new pfDCS;      // DCS to navigate with

   // Create the SUN light source
   mLightGroup = new pfGroup;
   mSun = new pfLightSource;
   mLightGroup->addChild( mSun );
   mSun->setPos( 0.3f, 0.0f, 0.3f, 0.0f );
   mSun->setColor( PFLT_DIFFUSE,1.0f,1.0f,1.0f );
   mSun->setColor( PFLT_AMBIENT,0.3f,0.3f,0.3f );
   mSun->setColor( PFLT_SPECULAR, 1.0f, 1.0f, 1.0f );
   mSun->on();

   // --- CONSTRUCT STATIC Structure of SCENE GRAPH -- //
   mRootNode->addChild( mNavigationDCS );
   mNavigationDCS->addChild(mLightGroup);
   mNavigationDCS->addChild( pfdLoadModel( "terrain.flt" ) );

   std::cout<<"[simplePfApp] Saving entire scene into lastscene.pfb, COULD TAKE A WHILE!\n"<<std::flush;
   pfuTravPrintNodes( mRootNode, "lastscene.out" );
   pfdStoreFile( mRootNode, "lastscene.pfb" );
}

#endif

