#include <iostream>
#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

static bool readCameraParameters(string filename, Mat &camMatrix,
                                 Mat &distCoeffs) {
  FileStorage fs(filename, FileStorage::READ);
  if (!fs.isOpened())
    return false;
  fs["camera_matrix"] >> camMatrix;
  fs["distortion_coefficients"] >> distCoeffs;
  return true;
}

static bool readDetectorParameters(string filename,
                                   Ptr<aruco::DetectorParameters> &params) {
  FileStorage fs(filename, FileStorage::READ);
  if (!fs.isOpened())
    return false;
  fs["adaptiveThreshWinSizeMin"] >> params->adaptiveThreshWinSizeMin;
  fs["adaptiveThreshWinSizeMax"] >> params->adaptiveThreshWinSizeMax;
  fs["adaptiveThreshWinSizeStep"] >> params->adaptiveThreshWinSizeStep;
  fs["adaptiveThreshConstant"] >> params->adaptiveThreshConstant;
  fs["minMarkerPerimeterRate"] >> params->minMarkerPerimeterRate;
  fs["maxMarkerPerimeterRate"] >> params->maxMarkerPerimeterRate;
  fs["polygonalApproxAccuracyRate"] >> params->polygonalApproxAccuracyRate;
  fs["minCornerDistanceRate"] >> params->minCornerDistanceRate;
  fs["minDistanceToBorder"] >> params->minDistanceToBorder;
  fs["minMarkerDistanceRate"] >> params->minMarkerDistanceRate;
  fs["cornerRefinementMethod"] >> params->cornerRefinementMethod;
  fs["cornerRefinementWinSize"] >> params->cornerRefinementWinSize;
  fs["cornerRefinementMaxIterations"] >> params->cornerRefinementMaxIterations;
  fs["cornerRefinementMinAccuracy"] >> params->cornerRefinementMinAccuracy;
  fs["markerBorderBits"] >> params->markerBorderBits;
  fs["perspectiveRemovePixelPerCell"] >> params->perspectiveRemovePixelPerCell;
  fs["perspectiveRemoveIgnoredMarginPerCell"] >>
      params->perspectiveRemoveIgnoredMarginPerCell;
  fs["maxErroneousBitsInBorderRate"] >> params->maxErroneousBitsInBorderRate;
  fs["minOtsuStdDev"] >> params->minOtsuStdDev;
  fs["errorCorrectionRate"] >> params->errorCorrectionRate;
  return true;
}

int detect_aruco(string filename,
                 string camera_parameters = "camera_calib.yaml") {
  Ptr<aruco::DetectorParameters> detectorParams =
      aruco::DetectorParameters::create();
  Ptr<aruco::Dictionary> dictionary =
      aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(16));

  float markerLength = 0.1;

  Mat camMatrix, distCoeffs;
  bool readOk = readCameraParameters(camera_parameters, camMatrix, distCoeffs);
  if (!readOk) {
    cerr << "Invalid camera file" << endl;
    return -1;
  }
  VideoCapture inputVideo;
  inputVideo.open(filename);
  if (!inputVideo.grab()) {
    cerr << "Could not open video file" << endl;
    return -1;
  }
  Mat image, imageCopy;
  inputVideo.retrieve(image);

  vector<int> ids;
  vector<vector<Point2f>> corners, rejected;
  vector<Vec3d> rvecs, tvecs;
  // detect markers and estimate pose
  aruco::detectMarkers(image, dictionary, corners, ids, detectorParams,
                       rejected);
  if (ids.size() > 0)
    aruco::estimatePoseSingleMarkers(corners, markerLength, camMatrix,
                                     distCoeffs, rvecs, tvecs);

  return 0;
}
