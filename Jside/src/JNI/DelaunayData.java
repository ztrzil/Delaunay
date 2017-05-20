package JNI;
import java.util.Random;

/**
 * This is a shortened version of 'DelaundayData.java' for purposes 
 * of getting calls to native library EDelaunay running. 4/28/17.
 */
public class DelaunayData {
  public int geometry;		// 0 for eucl 2D, 1 for spherical

  // point info
  public int pointCount;		// number of nodes of point set
  public double []ptX;
  public double []ptY;

  // bdry info if there's a boundary; note, it must close up
  public int bdryCount;	// edge count of closed bdry 
  public int []edgeV;		// indices of bdry edge starts
  public int []edgeW;     // bdry edge ends

  // face information
  public int myfaceCount;	// number of faces on return
  public int []triLite;	// linearized array of face indices; for face f use indices {3*f, 3*f+1, 3*f+2}

  // constructor(s)
  public DelaunayData() {
    geometry=0;
    pointCount=0;
    ptX=null;
    ptY=null;
    bdryCount=0;
    myfaceCount=0;
    edgeV=null;
    edgeW=null;
    triLite=null;
  }

  // Constructor with x,y data
//  public DelaunayData(double []ptX,double []ptY) {
  public DelaunayData(double []vec1, double []vec2) {
    this();
    if (vec1 != null) pointCount = vec1.length;
    int i;

    // New ~Zac
    ptX = new double [pointCount];
    ptY = new double [pointCount];

    for (i = 1; i < pointCount; i++) {
      ptX[i] = vec1[i];
      ptY[i] = vec2[i];
    }
    // indexed from 1
    geometry=0; // euclidean
  }
}
