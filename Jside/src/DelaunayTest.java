import java.lang.reflect.Field;
import java.util.Random;
import JNI.*;

public class DelaunayTest {

  public static void main(String[] args) {

    double []ptX=new double[26];
    double []ptY=new double[26];
    int i;

    // create random points in unit square [0,1]x[0,1]
    Random rand=new Random();
    rand.setSeed(1); // to be deterministic
    for (i = 1; i <= 25; i++) {
      ptX[i] = rand.nextDouble();
      ptY[i] = rand.nextDouble();
    }

    // DEBUG
    for (i = 1; i < 26; i++) {
      System.out.printf("ptX: %f ptY: %f%n", ptX[i], ptY[i]);
    }
    // END DEBUG

    // Load shared lib
    /*
     * Could not get this method to working using -Djava.library.path flag. So
     * unfortunately the path to the library has to be hardcoded in for now.
     try{
       System.setProperty("java.library.path", "/Users/zac/work/Stephenson/testDelaunay/Cside/src");
       Field fieldSysPath = ClassLoader.class.getDeclaredField("sys_paths");
       fieldSysPath.setAccessible(true);
       fieldSysPath.set(null, null);

     } catch (IllegalAccessException e) {
       System.out.println("Failed to get permissions to set library path");
     } catch (NoSuchFieldException e) {
       System.out.println("Failed to get field handle to set library path");
     }
     System.loadLibrary("libDel.so");
     */

    // New code. Load the library file, create a new instance of DelaunayData,
    // for input and output, call apply with the input and capture the output.
    System.load("/Users/zac/work/Stephenson/testDelaunay/Cside/lib/libDel.so");
    DelaunayData inDel = new DelaunayData(ptX,ptY);
    DelaunayData outDel;

    DelaunayBuilder DB = new DelaunayBuilder();
    outDel = DB.apply(inDel);
    
    // print out the resulting triangulation face-by-face
    try {
      int len=outDel.triLite.length;
      if (len<=0) {
        System.err.println("'triLite' is null");
        System.exit(0);
      }
      for (i=0;i<len;i++)
        System.out.println(outDel.triLite[i]);

    } catch(Exception ex) {
      System.err.println("Exception in trying to print out 'triLite'");
    }

    System.out.println("Done.");
  }
}

