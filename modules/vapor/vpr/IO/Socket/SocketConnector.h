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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _VPR_SOCK_CONNECTOR_H_
#define _VPR_SOCK_CONNECTOR_H_

#include <vpr/vprConfig.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/**
 * @class SocketConnector
 *
 * Defines a factory for creating new connections both synchronously and
 * asynchronously.
 *
 * The socket factory creates a new socket for the user that connects
 * to the given remote destination.  It allows for synchronous and async
 * connection establishment.
 *
 * This means that there is no state associated with the connector.
 */
class VPR_CLASS_API SocketConnector
{
public:
  SocketConnector()
  {;}

  ~SocketConnector()
  {;}

  /**
   * Actively connect to the remote address and set the newStream
   * to the new connection.
   *
   * @param new_stream The stream to connect
   * @param remoteAddr The remote address to connect to
   * @param timeout    The num msecs to wait (0 - NonBlocking)
   */
  vpr::ReturnStatus connect(vpr::SocketStream& newStream,
                            const vpr::InetAddr& remoteAddr,
                            vpr::Interval timeout = vpr::Interval::NoTimeout,
                            const vpr::InetAddr& localAddr = vpr::InetAddr::AnyAddr);

  /**
   * Complete a non-blocking connection
   * Try to complete a non-blocking connection.
   */
  vpr::ReturnStatus complete(vpr::SocketStream &newStream,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout);

protected:
   // Make sure we have opened the socket
   // If not, then open it with the given params
   bool checkOpen(SocketStream& newStream);

   // Do preconnection rituals
   // - If not bound, then bind to local addr
   // - If timeout == NoWait, then try to set nonblocking
   bool connectStart(vpr::SocketStream& newStream,
                     vpr::Interval timeout = vpr::Interval::NoTimeout,
                     const vpr::InetAddr& localAddr = vpr::InetAddr::AnyAddr);
};

}  // namespace vpr


#endif /* _VPR_SOCK_CONNECTOR_H_ */
