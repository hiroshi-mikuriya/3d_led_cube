#include "led.h"
#include "SensorManager.hpp"
#include "ledUtil.h"
#include <iostream>
#include <cassert>
#include <opencv2/opencv.hpp>
#include <cassert>
#include <memory>

#if defined _DEBUG
#pragma comment(lib, "libpxc_d.lib")
#pragma comment(lib, "libpxcutils_d.lib")
#else
#pragma comment(lib, "libpxc.lib")
#pragma comment(lib, "libpxcutils.lib")
#endif

namespace
{
	class ImageReader
	{
		PXCImage * m_img;
		PXCImage::ImageData m_data;
	public:
		ImageReader(PXCImage * img) : m_img(img)
		{
			assert(m_img);
			if (m_img->AcquireAccess(PXCImage::ACCESS_READ, m_img->QueryInfo().format, &m_data) < PXC_STATUS_NO_ERROR){
				throw std::runtime_error("Failed PXCImage::AcquireAccess");
			}
		}
		~ImageReader()
		{
			m_img->ReleaseAccess(&m_data);
		}
		PXCImage::ImageData get()const
		{
			return m_data;
		}
	};
}

cv::Mat CreateCvMat(PXCImage * img)
{
	assert(img);
	auto info = img->QueryInfo();
	auto reader = ImageReader(img);
	auto * begin = reader.get().planes[0];
	auto * end = begin + info.height * reader.get().pitches[0];
	switch (info.format){
	default:
		throw std::runtime_error("Unsupported format.");
	case PXCImage::PIXEL_FORMAT_YUY2:{
		cv::Size s(info.width, info.height);
		cv::Mat copy(s, CV_8UC2);
		std::copy(begin, end, copy.data);
		cv::cvtColor(copy, copy, CV_YUV2BGR_YUYV);
		return copy;
	}
	case PXCImage::PIXEL_FORMAT_RGB32:{
		cv::Size s(info.width, info.height);
		cv::Mat copy(s, CV_8UC4);
		std::copy(begin, end, copy.data);
		return copy;
	}
	case PXCImage::PIXEL_FORMAT_DEPTH:{
		cv::Size s(reader.get().pitches[0] / 2, info.height);
		cv::Mat copy(s, CV_16U);
		std::copy(begin, end, copy.data);
		cv::Mat dst;
		copy.convertTo(dst, CV_8U, 1.0 / 8);
		return (255 - dst)&(dst * 255);
	}
	}
}

cv::Mat ConvertLedSize(cv::Mat const & src)
{
	int unit = std::min(src.cols / 4, src.rows / 3);
	cv::Rect rc(src.cols / 2 - unit * 3 / 4, 0, unit * 3 / 2, unit * 3);
	using makerfaire::fxat::Led;
	cv::Mat dst;
	cv::resize(src(rc), dst, cv::Size(Led::Width, Led::Height));
	return dst;
}

/*!
*/
const cv::Point3i IrregularPoint(INT_MIN, INT_MIN, INT_MIN);

/*!
画像内座標 -> 3D LED座標（平面版）
*/
cv::Point ConvertLedPoint(int x, int y, cv::Size const & original)
{
	using makerfaire::fxat::Led;
	int unit = std::min(original.width / 4, original.height / 3);
	cv::Rect rc(original.width / 2 - unit * 3 / 4, 0, unit * 3 / 2, unit * 3);
	double ratio = 1.0 * Led::Width / rc.width;
	return{ static_cast<int>((x - rc.x)*ratio), static_cast<int>((y - rc.y)*ratio) };
}

/*!
SkeletonPointの座標を変換する
画像内座標 -> 3D LED座標
@param[in] joints SkeletonPoint座標
@param[in] original 画像内座標空間の範囲
@return 3D LED座標
*/
std::vector<cv::Point3i> ConvertJointPoints(
	std::vector<PXCPersonTrackingData::PersonJoints::SkeletonPoint> const & joints,
	cv::Size const & original)
{
	using makerfaire::fxat::Led;
	std::vector<cv::Point3i> dst;
	for (auto joint : joints) {
		if (joint.image.x == 0 && joint.image.y == 0) {
			dst.push_back(IrregularPoint);
			continue;
		}
		cv::Point3i p;
		auto pt2 = ConvertLedPoint(joint.image.x, joint.image.y, original);
		p.x = pt2.x;
		p.y = pt2.y;
		float const MINZ = 750, MAXZ = 2000; // 500 - 3200くらいの範囲（適当な実測にて）
		p.z = joint.world.z < MINZ ? 0
			: MAXZ < joint.world.z ? Led::Depth - 1
			: Led::Depth * (joint.world.z - MINZ) / (MAXZ - MINZ);
		dst.push_back(p);
	}
	return dst;
}

/*!
ジョイントの連結ペア
棒人間を作成するために使用する
*/
const std::vector<std::pair<int, int>> JointPair{ { 0, 4 }, { 4, 5 }, { 5, 1 }, { 2, 3 } };

/*!
顔、ジョイントを実画像に描画した画像を取得する

*/
cv::Mat CreateDebugMat(
	cv::Mat const & depth,
	cv::Mat const & color,
	PXCFaceData * face,
	std::vector<PXCPersonTrackingData::PersonJoints::SkeletonPoint> const & joints)
{
	auto dst = color.clone();
	for (int ix = 0; ix < face->QueryNumberOfDetectedFaces(); ++ix){
		PXCRectI32 rc = { 0 };
		face->QueryFaceByIndex(ix)->QueryDetection()->QueryBoundingRect(&rc);
		cv::rectangle(dst, { rc.x, rc.y, rc.w, rc.h }, cv::Scalar(0x00, 0xFF, 0x00), CV_FILLED);
	}
	for (size_t ix = 0; ix < joints.size(); ++ix){
		cv::Point2f pt(joints[ix].image.x, joints[ix].image.y);
		cv::circle(dst, pt, 20, cv::Scalar(0x00, 0x88, 0xFF), CV_FILLED);
		std::stringstream ss;
		ss << ix;
		pt.x -= 10;
		pt.y += 20;
		cv::putText(dst, ss.str(), pt, cv::FONT_HERSHEY_TRIPLEX, 1.5, cv::Scalar(0x00, 0x00, 0xFF), 2, CV_AA);
	}
	if (joints.size()){
		for (auto pair : JointPair){
			cv::Point const pt1(joints[pair.first].image.x, joints[pair.first].image.y);
			cv::Point const pt2(joints[pair.second].image.x, joints[pair.second].image.y);
			if (pt1 != cv::Point(0, 0) && pt2 != cv::Point(0, 0)) {
				cv::line(dst, pt1, pt2, cv::Scalar(0x00, 0xFF, 0x00), 1);
			}
		}
	}
	return dst;
}

void SetLed(
	cv::Mat const & depth,
	cv::Mat const & color,
	PXCFaceData * face,
	std::vector<PXCPersonTrackingData::PersonJoints::SkeletonPoint> const & joints,
	makerfaire::fxat::Led & led)
{
	cv::imshow("face & joints", CreateDebugMat(depth, color, face, joints));
	using makerfaire::fxat::Led;
	led.Clear();
#if 0
	color2 = ConvertLedSize(color);
	for (int z = 0; z < Led::Depth; ++z){
		int th = (Led::Depth - z) * 20;
		cv::Mat d;
		cv::threshold(depth, d, th, 255, cv::THRESH_BINARY);
		d = ConvertLedSize(d);
		for (int y = 0; y < Led::Height; ++y){
			for (int x = 0; x < Led::Width; ++x){
				if (0 < d.at<uchar>(y, x)){
					auto s = color2.at<cv::Vec4b>(y, x);
					int rgb = (s[2] << 16) + (s[1] << 8) + (s[0] << 0);
					led.SetLed(x, y, z, rgb);
				}
			}
		}
	}
#endif
	auto joints2 = ConvertJointPoints(joints, color.size());
	if (6 <= joints2.size()){	// jointsは毎回取得できるわけではないため
		for (auto pair : JointPair){
			auto const pt1 = joints2[pair.first];
			auto const pt2 = joints2[pair.second];
			if (pt1 != IrregularPoint && pt2 != IrregularPoint){
				DrawLine(led, pt1, pt2, cv::Scalar(0x00, 0xFF, 0x00), 2);
			}
		}
		int const smile[] = { 0x42, 0xA5, 0x00, 0x00, 0x00, 0x24, 0x18 };
		for (int ix = 0; ix < face->QueryNumberOfDetectedFaces(); ++ix){
			auto f = face->QueryFaceByIndex(ix);
			PXCRectI32 rc = { 0 };
			f->QueryDetection()->QueryBoundingRect(&rc);
			auto const pt = ConvertLedPoint(rc.x + rc.w / 2, rc.y + rc.h, color.size());
			int z = joints2[2].z; // joint2[2]が顔の位置を指していることが多いため。指していないこともあるが。
			for (int ry = 0; ry < (sizeof(smile) / sizeof(*smile)); ++ry){
				for (int rx = 0; rx < 8; ++rx){
					bool b = !!(smile[ry] & (1 << (7 - rx)));
					int x = pt.x - 3 + rx;
					int y = pt.y - 3 + ry;
					if (b){
						makerfaire::fxat::DrawCube(led, { x, y, z }, cv::Scalar(0x00, 0x00, 0xFF), 1);
					}
				}
			}
		}
	}
}

namespace
{
	void FromAcquireToRelease(std::shared_ptr<SenseManager> rs, makerfaire::fxat::Led & led)
	{
		static cv::Mat color;
		static std::vector<PXCPersonTrackingData::PersonJoints::SkeletonPoint> joints;
		static int ageOfJoint = 0;
		++ageOfJoint;
		rs->face()->Update() | THROW_IF_ERROR;
		if (auto per = rs->person()){
			if (0 < per->QueryNumberOfPeople()){
//			for (int ix = 0; ix < per->QueryNumberOfPeople(); ++ix){
				auto skeleton = per->QueryPersonData(PXCPersonTrackingData::ACCESS_ORDER_BY_ID, 0)->QuerySkeletonJoints();
				if (0 < skeleton->QueryNumJoints()){
					joints.resize(skeleton->QueryNumJoints());
					skeleton->QueryJoints(&joints[0]);
					ageOfJoint = 0;
				}
			}
		}
		if (10 < ageOfJoint){
			joints.clear();
		}
		const auto *sample = rs->sensor()->QueryFaceSample();
		if (sample && sample->depth && !color.empty()){
			cv::Mat depth = CreateCvMat(sample->depth);
			cv::imshow("depth", depth);
			SetLed(depth, color, rs->face(), joints, led);
			led.Show();
		}
		if (sample && sample->color){
			color = CreateCvMat(sample->color);
			cv::imshow("color", color);
		}
		cv::waitKey(1);
	}

	void impl(makerfaire::fxat::Led & led)
	{
		std::shared_ptr<SenseManager> rs = std::make_shared<SenseManager>();
		try{
			rs->Init();
		}
		catch (std::exception &) {
			std::cerr << "Failed to init sensor." << std::endl;
			throw;
		}
		cv::Mat color;
		for (;;) {
			rs->sensor()->AcquireFrame(false) | THROW_IF_ERROR;
			try{
				FromAcquireToRelease(rs, led);
			}
			catch (std::exception const & e){
				std::cerr << e.what() << std::endl;
			}
			rs->sensor()->ReleaseFrame();
		}
	}
}

int main(int argc, const char * argv[])
{
	makerfaire::fxat::Led led(1 < argc ? argv[1] : "127.0.0.1");
	try{
		impl(led);
	}
	catch (std::exception const & e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}