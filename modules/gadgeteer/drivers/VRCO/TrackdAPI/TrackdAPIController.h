/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_TRACKD_API_CONTROLLER_H_
#define _GADGET_TRACKD_API_CONTROLLER_H_

#include <gadget/Devices/DriverConfig.h>

#include <vector>
#include <boost/mpl/inherit.hpp>

#include <gadget/Type/InputDevice.h>

#define TRACKD_DLL_EXPORTS 0
#include GADGET_TRACKD_API_H


namespace gadget
{

/**
 * Class interfacing with trackdAPI sensor data located on the local machine
 * in a shared memory arena.
 *
 * @note A note on reciever access:
 *  Clients of Gadgeteer should access tracker recievers as [0-n].  For
 *  example, if you have recievers 1,2, and 4 with transmitter on 3, then
 *  you can access the data, in order, as 0, 1, 2.
 *
 * @see gadget::Digital
 * @see gadget::Analog
 */
class TrackdAPIController
   : public InputDevice<boost::mpl::inherit<Digital, Analog>::type>
{
public:

   /**
    * Constructor.
    */
   TrackdAPIController();

   /**
    * Destructor.
    *
    * @pre None.
    * @post Shared memory is released.
    */
   ~TrackdAPIController();

   /**
    * Configures the trackd controller with the given config element.
    *
    * @pre e must be a element that has trackd controller config information
    * @post If e is a valid config element, the device is configured using its
    *        contents.  Otherwise, configuration fails and false is returned
    *        to the caller.
    *
    * @param e A pointer to a config element.
    *
    * @return true if the device was configured succesfully, false if the
    *         config element is invalid.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /** Begins sampling. */
   bool startSampling() { return 1;}

   /** Stops sampling. */
   bool stopSampling() { return 1;}

   /** Samples a value. */
   bool sample() { return 1;}

   /**
    * Update to the sampled data.
    *
    * @pre None.
    * @post Most recent value is copied over to temp area
    */
   void updateData();

   /** Return what element type is associated with this class. */
   static std::string getElementType();

   /** Returns digital data. */
   virtual const DigitalData getDigitalData(int devNum=0)
   {
      vprASSERT(devNum < (int)mCurButtons.size() && "Digital index out of range");    // Make sure we have enough space
      return (mCurButtons[devNum]);
   }

   /**
    *  Return "analog data"..
    *  Gee, that's ambiguous especially on a discrete system such as a digital computer....
    *
    * @pre give the device number you wish to access.
    */
   virtual AnalogData* getAnalogData(int devNum=0)
   {
      vprASSERT(devNum < (int)mCurValuators.size() && "Analog index out of range");    // Make sure we have enough space
      return &(mCurValuators[devNum]);
   }

private:
   ControllerReader*            mControllerReader;    /**< The controller reader to use */
   std::vector<AnalogData>      mCurValuators;        /**< The current (up-to-date) values */
   std::vector<DigitalData>     mCurButtons;          /**< The current button states */
};

} // End of gadget namespace


#endif
