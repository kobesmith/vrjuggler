/* Generated by Together */

#ifndef AJOPENALSOUNDIMPLEMENTATION_H
#define AJOPENALSOUNDIMPLEMENTATION_H

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h> // for FILE

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>

#include "CFileIO.h"

#include "ajSoundImplementation.h"
#include "ajSoundInfo.h"

class ajOpenALSoundImplementation : public ajSoundImplementation
{
public:
    
   ajOpenALSoundImplementation() : mContextId( NULL ), mDev( NULL )
   {
   }
   
   virtual ~ajOpenALSoundImplementation()
   {
      
   }  
    
   /**
    * @input alias of the sound to trigger, and number of times to play
    * @preconditions alias does not have to be associated with a loaded sound.
    * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
    * @semantics Triggers a sound
    */
   virtual void trigger( const std::string& alias, const unsigned int& looping = 0)
   {
      ajSoundImplementation::trigger( alias, looping );

      if (mBindLookup.count( alias ) > 0)
      {
         alSourcePlay( mBindLookup[alias].source );
      }
   }
   
   bool isPlaying( const std::string& alias )
   {
      if (mBindLookup.count( alias ) > 0)
      {
         assert( alIsSource( mBindLookup[alias].source ) != AL_FALSE && "weird, shouldn't happen...\n" );
		 
         ALint state( AL_INITIAL ); // initialized
	      alGetSourceiv( mBindLookup[alias].source, AL_SOURCE_STATE, &state );
         
         switch(state) 
         {
		      case AL_PLAYING:
		      case AL_PAUSED:
		        return true;
		      default:
              return false;
		        break;
	      }
      }
      
	   return false;
   }
  
   /**
    * @semantics stop the sound
    * @input alias of the sound to be stopped
    */
   virtual void stop( const std::string& alias )
   {
      ajSoundImplementation::stop( alias );
      
      if (mBindLookup.count( alias ) > 0)
      {
         alSourceStop( mBindLookup[alias].source );
      }
   }

   /**
    * take a time step of [timeElapsed] seconds.
    * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
    * @input time elapsed since last frame
    */
   virtual void step( const float & timeElapsed )
   {
      ajSoundImplementation::step( timeElapsed );
   }


  /**
    * associate a name (alias) to the description
    * @preconditions provide an alias and a SoundInfo which describes the sound
    * @postconditions alias will point to loaded sound data
    * @semantics associate an alias to sound data.  later this alias can be used to operate on this sound data.
    */
   virtual void associate( const std::string& alias, const ajSoundInfo& description )
   {
      ajSoundImplementation::associate( alias, description );
   }

  
   /**
    * remove alias->sounddata association 
    */
   virtual void remove( const std::string alias )
   {
      ajSoundImplementation::remove( alias );
   }

   /**
    * set sound's 3D position 
    */
   virtual void setPosition( const std::string& alias, float x, float y, float z )
   {
      ajSoundImplementation::setPosition( alias, x, y, z );

      if (mBindLookup.count( alias ) > 0)
      {
         float pos[3];
         pos[0] = x; pos[1] = y; pos[2] = z;
         alSourcefv( mBindLookup[alias].source, AL_POSITION, pos );
      }
   }

   /**
    * get sound's 3D position
    * @input alias is a name that has been associate()d with some sound data
    * @output x,y,z are returned in OpenGL coordinates.
    */
   virtual void getPosition( const std::string& alias, float& x, float& y, float& z )
   {
      ajSoundImplementation::getPosition( alias, x, y, z );
   }
   
   /**
    * set the position of the listener
    */
   virtual void setListenerPosition( const float& x, const float& y, const float& z )
   {
      ajSoundImplementation::setListenerPosition( x, y, z );

      ALfloat position[] = { x, y, z };
      alListenerfv( AL_POSITION, position );
   }

   /**
    * get the position of the listener
    */
   virtual void getListenerPosition( float& x, float& y, float& z )
   {
      ajSoundImplementation::getListenerPosition( x, y, z );
   }
   
   /**
    * set the orientation of the listener
    */
   virtual void setListenerOrientation( const float& rad, const float& x, const float& y, const float&  z )
   {
      //ajSoundImplementation::setListenerOrientation( rad, x, y, z );
   }
   
   /**
    * set the orientation of the listener
    */
   virtual void setListenerOrientation( const float& dirx, const float& diry, const float& dirz, const float& upx, const float& upy, const float& upz )
   {
      //ajSoundImplementation::setListenerOrientation( dirx, dirx, dirx, upx, upy, upz );
      ALfloat orientation[]  = { dirx, dirx, dirx, upx, upy, upz };
	   alListenerfv( AL_ORIENTATION, orientation );
   }
   
public:
   /**
    * start the sound API, creating any contexts or other configurations at startup
    * @postconditions sound API is ready to go.
    * @semantics this function should be called before using the other functions in the class.
    */
   virtual void startAPI()
   {
      if (mContextId == NULL && mDev == NULL)
      {
         mDev = alcOpenDevice( NULL );
	      if (mDev == NULL) 
         {
		      std::cerr << "Could not open device\n" << std::flush;
   	      return;
	      }

         // Initialize ALUT
         std::vector<int> attrlist;
         attrlist.push_back( ALC_FREQUENCY );
         attrlist.push_back( 22050 );
         attrlist.push_back( ALC_INVALID );
               
         // create context
	      mContextId = alcCreateContext( mDev, &attrlist[0] );
         if (mContextId == NULL) 
         {
            std::string err = (char*)alGetString( alcGetError() );
		      std::cerr << "Could not open context: " << err.c_str() << "\n" << std::flush;
		      return;
	      }

         // make context active...
	      alcMakeContextCurrent( mContextId );
      }
      else
      {
         std::cerr << "startAPI called when API is already started\n" << std::flush;
      }      
      
      // init the listener...
      
      this->setListenerPosition( 0.0f, 0.0f, 0.0f );
	   this->setListenerOrientation( 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f );
	   
      // ALfloat velocity[] = { 0.0f, 0.0f,  0.0f };
	   // alListenerfv( AL_VELOCITY, velocity );
   }

   /**
    * kill the sound API, deallocating any sounds, etc...
    * @postconditions sound API is ready to go.
    * @semantics this function could be called any time, the function could be called multiple times, so it should be smart.
    */
   virtual void shutdownAPI()
   {
      alcDestroyContext( mContextId );
	   alcCloseDevice(  mDev  );
   }   

   /**
    * clear all associate()tions.
    * @semantics any existing aliases will be stubbed. sounds will be unbind()ed
    */
   virtual void clear()
   {
      ajSoundImplementation::clear();
   }   
   
   /**
    * bind: load (or reload) all associate()d sounds
    * @postconditions all sound associations are buffered by the sound API
    */
   virtual void _bindAll()
   {
      std::map< std::string, ajSoundInfo >::iterator it;
      for( it = mSounds.begin(); it != mSounds.end(); ++it)
      {
         std::cout<<(*it).first<<"\n"<<std::flush;
         this->_bind( (*it).first );
      }
   }   

   /**
    * unbind: unload/deallocate all associate()d sounds.
    * @postconditions all sound associations are unbuffered by the sound API
    */
   virtual void _unbindAll()
   {
      std::map< std::string, ajAlSoundInfo >::iterator it;
      for( it = mBindLookup.begin(); it != mBindLookup.end(); ++it)
      {
         this->_unbind( (*it).first );
      }
      
      assert( mBindLookup.size() == 0 && "_unbindAll failed" );
   }

   
   
   /**
    * load/allocate the sound data this alias refers to the sound API
    * @postconditions the sound API has the sound buffered.
    */
   virtual void _bind( const std::string& alias )
   {
      ajSoundInfo& soundInfo = this->lookup( alias );
      
      // if alias is already bound, then unbind it...
      // TODO: we want a way to force a rebind, but do we _always_ want to force it?
      if (mBindLookup.count( alias ) > 0)
      {
         this->_unbind( alias );
      }
      
      // load the data 
      switch (soundInfo.datasource)
      {
         default:
         case ajSoundInfo::FILESYSTEM:
         {
            ALuint bufferID;
            ALuint sourceID;
            
            // open the file as readonly binary
	         if (!CFileIO::fileExists( soundInfo.filename.c_str() )) 
            {
		         std::cerr<<"file doesn't exist: "<<soundInfo.filename<<"\n" << std::flush;
               return;
	         }

            // read the data from the file.
            std::cout<<"loading: "<<soundInfo.filename<<"... " << std::flush;
            CFileIO::fileLoad( soundInfo.filename.c_str(), mBindLookup[alias].data );
            std::cout<<"done\n" << std::flush;
            
	         // put the data in an OpenAL buffer
	         alGenBuffers( 1, &bufferID );
            alBufferData( bufferID, AL_FORMAT_WAVE_EXT, &(mBindLookup[alias].data[0]), mBindLookup[alias].data.size(), 0 );
	         if (alGetError() != AL_NO_ERROR)
            {
		         std::cerr << "Could not buffer data\n" << std::flush;
               alDeleteBuffers( 1, &bufferID );
               mBindLookup.erase( alias );
		         return;
	         }

            // associate a source with the buffer
	         alGenSources( 1, &sourceID );
            if (alGetError() != AL_NO_ERROR)
            {
		         std::cerr << "Could not gen a source\n" << std::flush;
               alDeleteBuffers( 1, &bufferID );
               mBindLookup.erase( alias );
		         return;
	         }
            
	         alSourcei( sourceID, AL_BUFFER, bufferID );
	         alSourcei( sourceID, AL_LOOPING, AL_FALSE );
            
            // store the resource IDs for later use...
            mBindLookup[alias].source = sourceID;
            mBindLookup[alias].buffer = bufferID;
            break;
         }
      }
      
   }   

   /**
    * unload/deallocate the sound data this alias refers from the sound API
    * @postconditions the sound API no longer has the sound buffered.
    */
   virtual void _unbind( const std::string& alias )
   {
      if (mBindLookup.count( alias ) > 0)
      {
         alDeleteSources( 1, &mBindLookup[alias].source );
         alDeleteBuffers( 1, &mBindLookup[alias].buffer );
         mBindLookup.erase( alias );
      }
      
      assert( mBindLookup.count( alias ) == 0 && "_unbind failed" );
   }
   
private:
    /** @link dependency */
    /*#  ajSoundInfo lnkSoundInfo; */
   
   struct ajAlSoundInfo
   {
      ALuint source, buffer;
      std::vector<unsigned char> data;
   };
   std::map< std::string, ajAlSoundInfo > mBindLookup;
   void*       mContextId;
   ALCdevice*  mDev;
};
#endif //AJOPENALSOUNDIMPLEMENTATION_H
