import junit.framework.*;
import test.*;

/**
 * This is the main class used to drive the VjControl Java test suite. Its
 * static suite() method adds all the tests currently available.
 */
public class AllTests
{
   public static void main( String[] args )
   {
      junit.swingui.TestRunner.run( AllTests.class );
   }

   /**
    * Adds all the tests currently available.
    */
   public static Test suite()
   {
      TestSuite suite = new TestSuite( "All VjControl JUnit Tests" );

      suite.addTestSuite( ValTypeTest.class );
      suite.addTestSuite( VarValueTest.class );
      suite.addTestSuite( DescEnumTest.class );


      return suite;
   }
}
