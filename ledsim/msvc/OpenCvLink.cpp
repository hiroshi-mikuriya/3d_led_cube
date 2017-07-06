#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#define LIB_EXTENSION "d.lib"
#elif defined NDEBUG
#define LIB_EXTENSION ".lib"
#else
#error Unselected build mode Debug or Release.
#endif

#define OPENCV_VERSION CVAUX_STR(CV_VERSION_EPOCH) CVAUX_STR(CV_VERSION_MAJOR) CVAUX_STR(CV_VERSION_MINOR)


#pragma comment( lib, "opencv_contrib" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_core" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_features2d" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_flann" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_imgproc" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_objdetect" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_highgui" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_legacy" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_calib3d" OPENCV_VERSION  LIB_EXTENSION )
#pragma comment( lib, "opencv_ml" OPENCV_VERSION  LIB_EXTENSION )

//#pragma comment( lib, "opencv_nonfree" OPENCV_VERSION  LIB_EXTENSION )

#pragma comment( lib, "IlmImf" LIB_EXTENSION)
#pragma comment( lib, "libjasper" LIB_EXTENSION)
#pragma comment( lib, "libjpeg" LIB_EXTENSION)
#pragma comment( lib, "libpng" LIB_EXTENSION)
#pragma comment( lib, "libtiff" LIB_EXTENSION)
#pragma comment( lib, "zlib" LIB_EXTENSION)

#pragma comment( lib, "comctl32.lib" )
