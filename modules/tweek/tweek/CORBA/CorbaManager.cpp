/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

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

#include <tweek/tweekConfig.h>

#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#include <tweek/CORBA/SubjectManager.h>
#include <tweek/CORBA/CorbaManager.h>


namespace tweek
{

vpr::ReturnStatus CorbaManager::init (const std::string& local_id, int argc,
                                      char** argv)
{
   vpr::ReturnStatus status;

   try
   {
      // Initialize the ORB.
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Initializing ORB\n"
                                             << vprDEBUG_FLUSH;
      m_orb = CORBA::ORB_init(argc, argv, "omniORB3");

      status = createChildPOA(local_id);

      try
      {
         status = initNamingService("NameService", local_id);
      }
      catch (CORBA::ORB::InvalidName& ex)
      {
         // This should not happen!
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "NameService name invalid in CorbaManager::init!\n"
            << vprDEBUG_FLUSH;
      }

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Starting ORB thread\n"
                                             << vprDEBUG_FLUSH;
      vpr::ThreadMemberFunctor<CorbaManager>* corba_run =
         new vpr::ThreadMemberFunctor<CorbaManager>(this, &CorbaManager::run);

      m_my_thread = new vpr::Thread(corba_run);
   }
   catch (CORBA::SystemException&)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Caught CORBA::SystemException\n" << vprDEBUG_FLUSH;
   }
   catch (CORBA::Exception&)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Caught CORBA::Exception.\n" << vprDEBUG_FLUSH;
   }
   catch (omniORB::fatalException& fe)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Caught omniORB::fatalException:\n" << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "  file: " << fe.file() << endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "  line: " << fe.line() << endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "  mesg: " << fe.errmsg() << endl << vprDEBUG_FLUSH;
   }
   catch(...)
   {
      cerr << "Caught unknown exception." << endl;
   }

   return status;
}

vpr::ReturnStatus CorbaManager::registerSubjectManager (tweek::SubjectManagerImpl* mgr)
{
   vprASSERT(! CORBA::is_nil(m_root_context) && "No naming service available");
   vprASSERT(! CORBA::is_nil(m_local_context) && "No naming service available");
   vpr::ReturnStatus status;

   try
   {
      tweek::SubjectManager_ptr mgr_ptr;
      const char* id   = "SubjectManager";
      const char* kind = "Object";
      CosNaming::Name context_name;

      mgr_ptr = mgr->_this();

      context_name.length(1);
      context_name[0].id   = CORBA::string_dup(id);
      context_name[0].kind = CORBA::string_dup(kind);

      // Bind the Subject Manager reference and activate the object within the
      // POA.  If a Subject Manager is already bound, the exceptoin thrown
      // prevents either operation from happening.  This is correct since we
      // only want one Subject Manager per address space.
      try
      {
         m_local_context->bind(context_name, mgr_ptr);
//         m_subj_mgr_id = m_child_poa->activate_object(mgr);
      }
      catch (CosNaming::NamingContext::AlreadyBound& ex)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "WARNING: Subject manager reference already bound!\n"
            << vprDEBUG_FLUSH;
      }
   }
   catch (CORBA::COMM_FAILURE& ex)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Unable to contact the naming service\n" << vprDEBUG_FLUSH;
   }
   catch (CORBA::SystemException&)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Caught a CORBA::SystemException while using the naming service\n"
         << vprDEBUG_FLUSH;
   }

   return status;
}

// ============================================================================
// Private methods.
// ============================================================================

vpr::ReturnStatus CorbaManager::createChildPOA (const std::string& local_id)
{
   vpr::ReturnStatus status;
   CORBA::Object_var obj;
   CORBA::PolicyList policy_list;

   // Obtain a reference to the root POA.  The caller will have to catch any
   // thrown exceptions.
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Requesting Root POA\n"
                                          << vprDEBUG_FLUSH;
   obj        = m_orb->resolve_initial_references("RootPOA");
   m_root_poa = PortableServer::POA::_narrow(obj);

   vprASSERT(! CORBA::is_nil(m_root_poa) && "Failed to get Root POA");

   PortableServer::IdUniquenessPolicy_var uniq_policy =
      m_root_poa->create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);

   policy_list.length(1);
   policy_list[0] =
      PortableServer::IdUniquenessPolicy::_duplicate(uniq_policy);

   std::string poa_name = "tweek_" + local_id;

/*
   try
   {
*/
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "Creating child of root POA named " << poa_name << std::endl
         << vprDEBUG_FLUSH;
      m_child_poa = m_root_poa->create_POA(poa_name.c_str(),
                                           PortableServer::POAManager::_nil(),
                                           policy_list);
/*
   }
   catch (PortableServer::AdapterAlreadyExists& ex)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "WARNING: Child POA named '" << poa_name << "' already exists!\n"
         << vprDEBUG_FLUSH;
   }
   catch (PortableServer::InvalidPolicy& ex)
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "WARNING: Failed to set IdUniquenessPolicy for child POA\n"
         << vprDEBUG_FLUSH;
   }
*/

   uniq_policy->destroy();

   return status;
}

/**
 *
 * @post The root context is retrieved through m_orb, and a sub-context is
 *       created for use within this memory space.
 */
vpr::ReturnStatus CorbaManager::initNamingService (const std::string& ref_name,
                                                   const std::string& local_id)
{
   CORBA::Object_var name_obj;
   vpr::ReturnStatus status;

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Requesting Name Service\n"
                                          << vprDEBUG_FLUSH;
   name_obj       = m_orb->resolve_initial_references(ref_name.c_str());
   m_root_context = CosNaming::NamingContext::_narrow(name_obj);

   if ( CORBA::is_nil(m_root_context) )
   {
      status.setCode(vpr::ReturnStatus::Fail);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "Failed to narrow Naming Service root context\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      std::string id("tweek");
      std::string kind("context");
      CosNaming::Name tweek_context_name;

//      id.append(local_id);

      tweek_context_name.length(1);
      tweek_context_name[0].id   = CORBA::string_dup(id.c_str());
      tweek_context_name[0].kind = CORBA::string_dup(kind.c_str());

      try
      {
         m_local_context = m_root_context->bind_new_context(tweek_context_name);
      }
      catch (CosNaming::NamingContext::AlreadyBound& ex)
      {
         CORBA::Object_var temp_obj;

         temp_obj        = m_root_context->resolve(tweek_context_name);
         m_local_context = CosNaming::NamingContext::_narrow(temp_obj);

         if ( CORBA::is_nil(m_local_context) )
         {
            status.setCode(vpr::ReturnStatus::Fail);
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "Failed to narrow Naming Service local Tweek context\n"
               << vprDEBUG_FLUSH;
         }
      }
   }

   return status;
}

void CorbaManager::run(void* args)
{
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Server is running!\n"
                                          << vprDEBUG_FLUSH;

   PortableServer::POAManager_var pman = m_child_poa->the_POAManager();

   pman->activate();
   m_orb->run();
   m_orb->destroy();
}

} // End of tweek namespace
