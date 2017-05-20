The makefile to create the shared library is in the lib folder. Using make will
create the .o files and the shared library, storing them in the lib directory. 
Running the 'DelaunayTest' java file currently will complete running, printing 
out that there was an exception printing. This is due to some issues with the
DelaunayBuilder.cpp file. The initial upload will have the entire native
funciton call commented out except for a print statement proving that the
funciton is called and the return statement. Uncommenting out this section,
which I will do for the second commit, causes a compilation error in regards to
the shared pointers. The error message is as follows:

/Users/zac/work/Stephenson/testDelaunay/Cside/src/DelaunayBuilder.cpp:134:11: error: expected expression
          [env, ptXArray](jdouble *ptr) { // ::printf("Delete ptX\n");
          ^
/Users/zac/work/Stephenson/testDelaunay/Cside/src/DelaunayBuilder.cpp:139:11: error: expected expression
          [env, ptYArray](jdouble *ptr) { // ::printf("Delete ptY\n");
          ^
/Users/zac/work/Stephenson/testDelaunay/Cside/src/DelaunayBuilder.cpp:144:11: error: expected expression
          [env, edgeVArray](jint *ptr) { // ::printf("Delete edgeV\n");
          ^
/Users/zac/work/Stephenson/testDelaunay/Cside/src/DelaunayBuilder.cpp:149:11: error: expected expression
          [env, edgeWArray](jint *ptr) { // ::printf("Delete edgeW\n");

If you know what is causing this issue, please let me know. I'm also not sure
why there is an error printing out the output of the native function call when
the value passed into the function is immediately returned. I was under the
impression that it would print out the contents of the input DelaunayData, but
apparently this is incorrect. Could you please clarify what this function is
supposed to be doing? Once this issue is fixed, I believe we can test to see if 
the DelaunayTest java program is actually making use of the triangle program's 
functionality. 

~Zac
