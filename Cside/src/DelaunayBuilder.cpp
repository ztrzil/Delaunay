/**
 * March 2017: trying to get Delaunay triangulations going again. 
 * This is for the euclidean case. For spherical, will need to use 
 * Qhull, and I'm still not able to compile that correctly.
 */
#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <cwchar>
#include <cmath>
#include <sstream>
#include <string>
#include <memory>

/* Zachary: paths will have to be modified (eventually on my machine) */
#include "/Users/zac/work/Stephenson/testDelaunay/Cside/include/jni.h"
#include "/Users/zac/work/Stephenson/testDelaunay/Cside/include/jni_md.h"
//#include "c:\Program Files\Java\jdk1.8.0_77\include\jni.h"
//#include "c:\Program Files\Java\jdk1.8.0_77\include\win32\jni_md.h"

#include "../include/triangle.h"

/* Zachary: these were obtained by running "javah" */
#include "../include/JNI_DelaunayData.h"
#include "../include/exceptions_DelaunayException.h"
// Might need to include complex and point.3D??

#define DELAUNAY_DATA_CLASS_NAME      "LJNI/DelaunayData;"
#define DELAUNAY_EXCEPTION_CLASS_NAME  "Lexceptions/DelaunayException;"

// using namespace std;  // do I need this??

static struct DelaunayFields
{
  jclass delaunayDataClass;
  jfieldID geometryField;
  jfieldID pointCountField;
  jfieldID ptXField;
  jfieldID ptYField;
  jfieldID bdryCountField;
  jfieldID edgeVField;
  jfieldID edgeWField;
  jfieldID faceCountField;
  jfieldID triLiteField;

} DelaunayFields;

int init_tri_data(struct triangulateio *tio);

#ifdef __cplusplus
extern "C" {
#endif

  /*
   * Class:     JNI_DelaunayBuilder
   * Method:    initialize
   * Signature: ()V
   */
  JNIEXPORT void JNICALL Java_JNI_DelaunayBuilder_initialize
    (JNIEnv *env, jclass delaunayBuildClass)
    {
      // Check type safety
      assert(sizeof(jint) == sizeof(int));
      assert(sizeof(jdouble) == sizeof(double));

      // Get the class of the Delaunay function
      DelaunayFields.delaunayDataClass=reinterpret_cast<jclass>(
          env->NewGlobalRef(
            env->FindClass(DELAUNAY_DATA_CLASS_NAME)));
      // Error so return with an exception set by env->FindClass
      if(DelaunayFields.delaunayDataClass == NULL) {
        return;
      }

      // Get the field ids for the fields of the solver data class
      DelaunayFields.geometryField   = env->GetFieldID(DelaunayFields.delaunayDataClass, "geometry", "I");
      DelaunayFields.pointCountField = env->GetFieldID(DelaunayFields.delaunayDataClass, "pointCount", "I");
      DelaunayFields.ptXField	   = env->GetFieldID(DelaunayFields.delaunayDataClass, "ptX", "[D");
      DelaunayFields.ptYField	   = env->GetFieldID(DelaunayFields.delaunayDataClass, "ptY", "[D");
      DelaunayFields.bdryCountField  = env->GetFieldID(DelaunayFields.delaunayDataClass, "bdryCount", "I");
      DelaunayFields.edgeVField   = env->GetFieldID(DelaunayFields.delaunayDataClass, "edgeV","[I");
      DelaunayFields.edgeWField   = env->GetFieldID(DelaunayFields.delaunayDataClass, "edgeW","[I");
      DelaunayFields.faceCountField  = env->GetFieldID(DelaunayFields.delaunayDataClass, "myfaceCount", "I");
      DelaunayFields.triLiteField    = env->GetFieldID(DelaunayFields.delaunayDataClass, "triLite", "[I");

      // Error with the id of the fields so return with an exception set by env->GetFieldID
      if(DelaunayFields.geometryField   == NULL ||
          DelaunayFields.pointCountField == NULL ||
          DelaunayFields.ptXField == NULL ||
          DelaunayFields.ptYField == NULL ||
          DelaunayFields.bdryCountField == NULL ||
          DelaunayFields.edgeVField == NULL ||
          DelaunayFields.edgeWField == NULL ||
          DelaunayFields.faceCountField == NULL ||
          DelaunayFields.triLiteField == NULL) {
        return;
      }
    }

  /*
   * Class:     JNI_DelaunayBuilder
   * Method:    apply
   * Signature: (LJNI/DelaunayData;)LJNI/DelaunayData;
   */
  JNIEXPORT jobject JNICALL Java_JNI_DelaunayBuilder_apply
    // NOTE: *** Why is the 'delaunayBuildObj passed to this function? Does not
    // seem to be used at all..
//      (JNIEnv *env, jobject delaunayBuildObj, jobject delaunayDataObj)
    (JNIEnv *env, jobject delaunayDataObj)
    {
      // debug
     
      //  std:: printf("entering DelaunayBuilder_Apply\n");

      // Get integer fields
      jint geometry = env->GetIntField(delaunayDataObj,DelaunayFields.geometryField);
      jint pointCount = env->GetIntField(delaunayDataObj,DelaunayFields.pointCountField);
      jint bdryCount = env->GetIntField(delaunayDataObj,DelaunayFields.bdryCountField);

      // Caution: seems to generate a crash if these Java fields are null.

      // Get double arrays
      jdoubleArray ptXArray = reinterpret_cast<jdoubleArray>(env->GetObjectField(delaunayDataObj,DelaunayFields.ptXField));
      jdoubleArray ptYArray = reinterpret_cast<jdoubleArray>(env->GetObjectField(delaunayDataObj,DelaunayFields.ptYField));

      // Get integer arrays
      jintArray edgeVArray = reinterpret_cast<jintArray>(env->GetObjectField(delaunayDataObj,DelaunayFields.edgeVField));
      jintArray edgeWArray = reinterpret_cast<jintArray>(env->GetObjectField(delaunayDataObj,DelaunayFields.edgeWField));

      // Get the native element pointers to these arrays
      std::shared_ptr<jdouble> ptX(
          env->GetDoubleArrayElements(ptXArray, NULL),
          [env, ptXArray](jdouble *ptr) { // ::printf("Delete ptX\n");
          env->ReleaseDoubleArrayElements(ptXArray, ptr, JNI_ABORT); });

      std::shared_ptr<jdouble> ptY(
          env->GetDoubleArrayElements(ptYArray, NULL),
          [env, ptYArray](jdouble *ptr) { // ::printf("Delete ptY\n");
          env->ReleaseDoubleArrayElements(ptYArray, ptr, JNI_ABORT); });

      std::shared_ptr<jint> edgeV(
          env->GetIntArrayElements(edgeVArray, NULL),
          [env, edgeVArray](jint *ptr) { // ::printf("Delete edgeV\n");
          env->ReleaseIntArrayElements(edgeVArray, ptr, JNI_ABORT); });

      std::shared_ptr<jint> edgeW(
          env->GetIntArrayElements(edgeWArray, NULL),
          [env, edgeWArray](jint *ptr) { // ::printf("Delete edgeW\n");
          env->ReleaseIntArrayElements(edgeWArray, ptr, JNI_ABORT); });

      // Check that ptX, ptY, and eindx are valid (A Java Exception should already be set (??? I don't recall what this means))
      if (ptX == nullptr || ptY == nullptr || edgeV==nullptr || edgeW==nullptr) {
        // return null
        return env->NewGlobalRef(NULL);
      }

      // -------------- actual work (for now, only euclidean case, geometry==0) ----------------------------
      if (geometry<=0) {
        // set up things for 'triangle'
        struct triangulateio in, out;
        init_tri_data(&in);
        init_tri_data(&out);
        in.pointlist=(double *)calloc(2*pointCount,sizeof(double));
        in.numberofpoints=pointCount;

        // pointlist data is in linear pairs, x,y
        for (int i=0;i<pointCount;i++) {
          in.pointlist[2*i]=ptX.get()[i+1]; // Java data, index starts at 1
          in.pointlist[2*i+1]=ptY.get()[i+1];
        }

        // set up bdry segments if there are any
        if (bdryCount>0) {
          in.numberofsegments= bdryCount;
          in.segmentlist=(int *)calloc(2*bdryCount,sizeof(int));
          for (int i=0;i<bdryCount;i++) {
            in.segmentlist[2*i]=edgeV.get()[i];
            in.segmentlist[2*i+1]=edgeW.get()[i];
          }
          in.segmentmarkerlist=NULL;
        }

        // call 'triangulate' from Shewchuk's 'triangle'. Switches here are:
        //	'Q' for 'quiet' to suppress printing
        //	'I' no '*.node' and '*.poly' files created
        //  'p'  if a boundary path is given.

        if (bdryCount>0) {
          triangulate("pQI",&in,&out,NULL);
        }
        else
          triangulate("QI",&in,&out,(triangulateio *)NULL);

        // check for error
        if(out.numberoftriangles<=0) {
          std::stringstream ss;
          ss << "'triangulate' failed at " << __FILE__ << ':' << __LINE__;
          env->ThrowNew(env->FindClass(DELAUNAY_EXCEPTION_CLASS_NAME), ss.str().c_str());
          return env->NewGlobalRef(NULL);
        }

        // TODO: what is size of
        jint faceCount=out.numberoftriangles;
        jintArray triLiteArray=env->NewIntArray(3*faceCount);
        if (triLiteArray==nullptr)
          return env->NewGlobalRef(NULL);

        env->SetIntArrayRegion(triLiteArray,0,3*faceCount,reinterpret_cast<const jint *>(out.trianglelist));

        // Update triLite and facecount in DelaunayData
        env->SetObjectField(delaunayDataObj,DelaunayFields.triLiteField,triLiteArray);
        env->SetIntField(delaunayDataObj,DelaunayFields.faceCountField,faceCount);

        // Returns the DelaunayData object
        return delaunayDataObj;
      }
    }

#ifdef __cplusplus
}
#endif


/**
 * initiate 'triangulateio' object used with Shewchuk's 'triangle'
 * program; see 'triangle.cpp'.
 */
int init_tri_data(struct triangulateio *tio) {
  tio->numberofpoints=tio->numberofpointattributes=0;
  tio->numberoftriangles=tio->numberofcorners=
    tio->numberoftriangleattributes=0;
  tio->numberofsegments=tio->numberofholes=0;
  tio->numberofregions=tio->numberofedges=0;
  tio->pointlist=NULL;
  tio->pointattributelist=NULL;
  tio->pointmarkerlist=NULL;
  tio->trianglelist=NULL;
  tio->triangleattributelist=NULL;
  tio->trianglearealist=NULL;
  tio->neighborlist=NULL;
  tio->segmentlist=NULL;
  tio->segmentmarkerlist=NULL;
  tio->holelist=NULL;
  tio->regionlist=NULL;
  tio->edgelist=NULL;
  tio->edgemarkerlist=NULL;
  tio->normlist=NULL;

  return 1;
} /* init_tri_data */

/*
int main (int argc, char **argv)
{
  printf("Running the DelaunayBuilder.cpp program!\n");
  return 0;
}
*/
