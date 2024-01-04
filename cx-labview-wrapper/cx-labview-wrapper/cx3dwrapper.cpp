#include "cx3dwrapper.h"

// In order to enable OpenCV support in AT's C++ wrapper classes include OpenCV header before AT headers or manually define CX_SUPPORT_OPENCV
#include "opencv2/opencv.hpp"
#include "opencv2/viz.hpp"
// C++ Wrapper
#include "cx_3d_common.h"
#include "AT/cx/CVUtils.h"


int32_t atCxImageFromLvArr(AT::cx::Image& img, Arr2D_U16Hdl arr) {
	try
	{

		/* dimSizes[0] is the number of rows */
		uint32_t numrow = static_cast<uint32_t>((*arr)->dimSizes[0]);
		/* dimSizes[1] is the number of columns */
		uint32_t numcol = static_cast<uint32_t>((*arr)->dimSizes[1]);

		auto elSize = sizeof((*arr)->elt[0]);
		size_t sz(numrow * numcol * elSize);

		img = AT::cx::Image(numrow, numcol, CX_PF_MONO_16, (*arr)->elt, sz);
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int32_t createPointcloud(char* calibId, char* calibFile, char* pcdFile, Arr2D_U16Hdl arr) {
	try
	{
		if ((*arr)->dimSizes[0] <= 0 || (*arr)->dimSizes[1] <= 0)
			return EXIT_FAILURE;

		std::string calib_fname(calibFile);
		std::string calib_id(calibId);
		std::string pcd_fname(pcdFile);

		// 1. load calibration from file
		AT::cx::c3d::Calib calib;
		calib.load(calib_fname, calib_id);

		// 2. load range image
		AT::cx::Image img;
		if (atCxImageFromLvArr(img, arr))
			return EXIT_FAILURE;


		// Set invalid data value (IDV). That value is used to mark invalid values in rectified image.
		// Zero range values are considered invalid.
		float ivd = NAN;	// could be any number e.g. -10.0f, if NAN is used the points are automatically suppressed in showPointCloud 
		calib.setParam(CX_3D_PARAM_METRIC_IDV, ivd);

		// Trigger update of internal cache. When calling with argument > 0, the internal cache gets updated.
		// This can be used to prevent a longer exectution time due to cache update at first call to function cx_3d_range2calibratedABC.
		calib.setParam(CX_3D_PARAM_METRIC_CACHE_MODE, AT::cx::Variant(1));

		//calculate point cloud
		AT::cx::c3d::PointCloud pc(img.height(), img.width());
		AT::cx::c3d::calculatePointCloud(calib, img, pc);

		pc.save(pcd_fname);

	}
	catch (...)
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;

}


int32_t writeTif_U16(char* file, Arr2D_U16Hdl arr) {
	try
	{
		if ((*arr)->dimSizes[0] <= 0 || (*arr)->dimSizes[1] <= 0)
			return EXIT_FAILURE;

		std::string fname(file);

		AT::cx::Image img;
		if (atCxImageFromLvArr(img, arr))
			return EXIT_FAILURE;

		img.save(fname);
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;

}


int32_t readTif_U16(char* file, Arr2D_U16Hdl arr) {
	try
	{
		std::string fname(file);

		// load range image
		AT::cx::Image img;
		img.load(fname);
		//make sure image is u16

		MgErr err = NumericArrayResize(uW, 1L, (UHandle*)&arr, img.dataSz());

		if (err)
			return err;

		MoveBlock(img.data(), (*arr)->elt, img.dataSz());
		(*arr)->dimSizes[0] = img.height();
		(*arr)->dimSizes[1] = img.width();
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;

}

