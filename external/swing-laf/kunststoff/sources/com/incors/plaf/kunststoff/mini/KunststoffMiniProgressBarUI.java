package com.incors.plaf.kunststoff.mini;

/*
 * This code was developed by INCORS GmbH (www.incors.com)
 * It is published under the terms of the Lesser GNU Public License.
 *
 * The original class was contributed by
 * Julien Ponge
 * julien@izforge.com
 * http://www.izforge.com/
 *
 */

import java.awt.*;
import javax.swing.*;
import javax.swing.plaf.*;
import javax.swing.plaf.basic.*;
import com.incors.plaf.kunststoff.KunststoffUtilities;


public class KunststoffMiniProgressBarUI extends BasicProgressBarUI
{
   // Creates the UI
   public static ComponentUI createUI(JComponent x)
   {
      JProgressBar prog_bar = (JProgressBar) x;
      Dimension pref_size = prog_bar.getPreferredSize();

      if ( prog_bar.getOrientation() == JProgressBar.HORIZONTAL )
      {
         pref_size.height = 8;
      }
      else
      {
         pref_size.width = 8;
      }

      prog_bar.setPreferredSize(pref_size);

      return new KunststoffMiniProgressBarUI();
   }

   public void paint(Graphics g, JComponent c)
   {
      super.paint(g, c);

      JProgressBar prog = (JProgressBar) c;
      if (prog.getOrientation() == JProgressBar.HORIZONTAL)
      {

         // paint upper gradient
         Color colorReflection = KunststoffMiniLookAndFeel.getComponentGradientColorReflection();
         if (colorReflection != null)
         {
            Color colorReflectionFaded = KunststoffUtilities.getTranslucentColor(colorReflection, 0);
            Rectangle rect = new Rectangle(0, 0, c.getWidth(), c.getHeight()/2);
            KunststoffUtilities.drawGradient(g, colorReflection, colorReflectionFaded, rect, true);
         }

         // paint lower gradient
         Color colorShadow = KunststoffMiniLookAndFeel.getComponentGradientColorShadow();
         if (colorShadow != null)
         {
            Color colorShadowFaded = KunststoffUtilities.getTranslucentColor(colorShadow, 0);
            Rectangle rect = new Rectangle(0, c.getHeight()/2, c.getWidth(), c.getHeight()/2);
            KunststoffUtilities.drawGradient(g, colorShadowFaded, colorShadow, rect, true);
         }

      }
      else  // if progress bar is vertical
      {
         // paint left gradient
         Color colorReflection = KunststoffMiniLookAndFeel.getComponentGradientColorReflection();
         if (colorReflection != null)
         {
            Color colorReflectionFaded = KunststoffUtilities.getTranslucentColor(colorReflection, 0);
            Rectangle rect = new Rectangle(0, 0, c.getWidth()/2, c.getHeight());
            KunststoffUtilities.drawGradient(g, colorReflection, colorReflectionFaded, rect, false);
         }

         // paint right gradient
         Color colorShadow = KunststoffMiniLookAndFeel.getComponentGradientColorShadow();
         if (colorShadow != null)
         {
            Color colorShadowFaded = KunststoffUtilities.getTranslucentColor(colorShadow, 0);
            Rectangle rect = new Rectangle(c.getWidth()/2, 0, c.getWidth()/2, c.getHeight());
            KunststoffUtilities.drawGradient(g, colorShadowFaded, colorShadow, rect, false);
         }
      }
   }
}
