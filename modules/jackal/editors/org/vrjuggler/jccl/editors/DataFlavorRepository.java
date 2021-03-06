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

package org.vrjuggler.jccl.editors;

import java.awt.datatransfer.DataFlavor;

import org.vrjuggler.jccl.config.ConfigElement;


/**
 * Simple factory-like helper class that centralizes the creation of the
 * data flavors that are needed for copy/paste and drag-and-drop support.
 *
 * @since 0.92.8
 */
public class DataFlavorRepository
{
   public static DataFlavor getConfigElementFlavor()
   {
      return configElementFlavor;
   }

   private static DataFlavor configElementFlavor =
      new DataFlavor(ConfigElement.class, "VR Juggler Config Element");
}

