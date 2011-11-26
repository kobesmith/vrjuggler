#include <gadget/gadgetConfig.h>

#include <boost/bind.hpp>

#include <vpr/Util/Assert.h>
#include <vpr/Sync/Guard.h>
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/IllegalArgumentException.h>

#include <gadget/Event/AbstractEventInterface.h>
#include <gadget/Event/EventGenerator.h>
#include <gadget/Util/Debug.h>
#include <gadget/InputHandler.h>


namespace gadget
{

InputHandler::InputHandler()
   : mRunning(false)
   , mThread(NULL)
   , mWaitInterval(0)
{
   /* Do nothing. */ ;
}

InputHandler::~InputHandler()
{
   if (isRunning())
   {
      stop();
   }
}

InputHandlerPtr InputHandler::create()
{
   return InputHandlerPtr(new InputHandler());
}

void InputHandler::start(const vpr::Interval& interval)
{
   if (0 == interval.msec())
   {
      throw vpr::IllegalArgumentException("Invalid wait interval 0 ms",
                                          VPR_LOCATION);
   }

   if (! mRunning)
   {
      vprASSERT(NULL == mThread && "Cannot start thread when alredy running!");

      mWaitInterval = static_cast<vpr::Uint32>(interval.msec());
      mThread = new vpr::Thread(boost::bind(&InputHandler::run, this));
   }
}

void InputHandler::stop()
{
   if (mRunning)
   {
      vprASSERT(NULL != mThread && "Cannot stop thread when not running!");

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "Waiting for input handler periorid event emission thread "
         << "to stop..." << std::endl << vprDEBUG_FLUSH;

      mRunning = false;
      mThread->join();

      delete mThread;
      mThread = NULL;
   }
}

void InputHandler::registerPeriodicInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> guard(mPeriodicIfacesLock);
   vprASSERT(iface->getEventGenerator().get() != NULL);
   mPeriodicIfaces[iface] = iface->getEventGenerator();
}

void InputHandler::unregisterPeriodicInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> guard(mPeriodicIfacesLock);
   mPeriodicIfaces.erase(iface);
}

void InputHandler::registerSynchronizedInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> guard(mSyncIfacesLock);
   vprASSERT(iface->getEventGenerator().get() != NULL);
   mSyncIfaces[iface] = iface->getEventGenerator();
}

void
InputHandler::unregisterSynchronizedInterface(AbstractEventInterface* iface)
{
   vpr::Guard<vpr::Mutex> guard(mSyncIfacesLock);
   mSyncIfaces.erase(iface);
}

void InputHandler::sync()
{
   vpr::Guard<vpr::Mutex> guard(mSyncIfacesLock);

   typedef iface_map_type::iterator iter_type;
   for (iter_type i = mSyncIfaces.begin(); i != mSyncIfaces.end(); ++i)
   {
      (*i).second->emitEvents();
   }
}

void InputHandler::run()
{
   vprASSERT(mWaitInterval > 0);

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Input handler periorid event emission thread starting; "
      << "wait interval is " << mWaitInterval << " milliseconds." << std::endl
      << vprDEBUG_FLUSH;

   mRunning = true;

   while (mRunning)
   {
      {
         vpr::Guard<vpr::Mutex> guard(mPeriodicIfacesLock);

         typedef iface_map_type::iterator iter_type;
         for (iter_type i = mPeriodicIfaces.begin();
              i != mPeriodicIfaces.end();
              ++i)
         {
            (*i).second->emitEvents();
         }
      }

      vpr::System::msleep(mWaitInterval);
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "Input handler periorid event emission thread stopped." << std::endl
      << vprDEBUG_FLUSH;
}

}
