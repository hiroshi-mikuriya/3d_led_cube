#include "ledUtil.h"

namespace
{
	template<typename T>
	double Distance2(cv::Point3_<T> const & pt1, cv::Point3_<T> const & pt2)
	{
		return (pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y) + (pt1.z - pt2.z)*(pt1.z - pt2.z);
	}
}

/*!
uint8_tのRGB値を3D LEDで使用するRGB値に変換する
@param[in] r R値
@param[in] g G値
@param[in] b B値
@return RGB値
*/
int makerfaire::fxat::UcharToRgb(uint8_t r, uint8_t g, uint8_t b)
{
	return (static_cast<int>(r) << 16) + (static_cast<int>(g) << 8) + (static_cast<int>(b) << 0);
}

/*!
cv::Scalarを3D LEDで使用するRGB値に変換する
@param[in] sc cv::Scalar
@return RGB値
*/
int makerfaire::fxat::CvScalarToRgb(cv::Scalar const & sc)
{
	return UcharToRgb(sc[2], sc[1], sc[0]);
}

/*!
3D LEDに球を描く（内部は塗りつぶす）
範囲外の座標を指定しても描画せずエラーにならない
@param[in] led 3D LED
@param[in] pt 座標（ドットの中心座標）
@param[in] sc 色
@param[in] r 半径（0の場合中心だけ塗る）
*/
void makerfaire::fxat::DrawBall(
	makerfaire::fxat::Led & led,
	cv::Point3i const & pt,
	cv::Scalar sc,
	double r)
{
	assert(0 <= r);
	double th = r*r + 1.0e10; // のちの比較演算のために小さい値を足しておく
	auto conv = [th](uint8_t v, double d){
		return (th==0)? v : static_cast<uint8_t>(v * (th - d) / th);
	};
	int begin = static_cast<int>(-r);
	int end = static_cast<int>(r);
	for (int x = begin; x <= end; ++x) {
		for (int y = begin; y <= end; ++y) {
			for (int z = begin; z <= end; ++z) {
				cv::Point3i const pt2(pt.x + x, pt.y + y, pt.z + z);
				auto const d = Distance2(pt, pt2);
				if (Contain(pt2) && d < th) {
					int const rgb = UcharToRgb(conv(sc[2], d), conv(sc[1], d), conv(sc[0], d));
					led.SetLed(pt2.x, pt2.y, pt2.z, rgb);
				}
			}
		}
	}
	std::cout << std::endl;
}

/*!
3D LEDに立方体を描く（内部は塗りつぶす）
範囲外の座標を指定しても描画せずエラーにならない
@param[in] led 3D LED
@param[in] pt 座標（立方体の中心座標）
@param[in] sc 色
@param[in] width 幅
*/
void makerfaire::fxat::DrawCube(
	makerfaire::fxat::Led & led,
	cv::Point3i const & pt,
	cv::Scalar sc,
	int width)
{
	int const rgb = CvScalarToRgb(sc);
	int begin = 1 - (width + 1) / 2;
	int end = width / 2;
	for (int x = begin; x <= end; ++x) {
		for (int y = begin; y <= end; ++y) {
			for (int z = begin; z <= end; ++z) {
				cv::Point3i const pt2{ pt.x + x, pt.y + y, pt.z + z };
				if (Contain(pt2)){
					led.SetLed(pt2.x, pt2.y, pt2.z, rgb);
				}
			}
		}
	}
}

/*!
3D LEDにラインをひく
@param[in] led 3D LED
@param[in] pt1 始点
@param[in] pt2 終点
@param[in] sc 色
@param[in] width 幅
*/
void makerfaire::fxat::DrawLine(
	makerfaire::fxat::Led & led,
	cv::Point3i const & pt1,
	cv::Point3i const & pt2,
	cv::Scalar sc,
	int width)
{
	int divCount = 2 * std::max(std::abs(pt1.z - pt2.z), std::max(std::abs(pt1.x - pt2.x), std::abs(pt1.y - pt2.y)));
	auto divValue = [](int a, int b, double r1, double r2){
		return static_cast<int>((a * r1 + b * r2) / (r1 + r2));
	};
	for (int i = 0; i < divCount; ++i){
		double r1 = i;
		double r2 = divCount - i - 1;
		cv::Point3i const pt(divValue(pt1.x, pt2.x, r1, r2), divValue(pt1.y, pt2.y, r1, r2), divValue(pt1.z, pt2.z, r1, r2));
		DrawCube(led, pt, sc, width);
	}
}

/*!
座標が3D LEDに含まれているかを返す
@param[in] x X座標
@param[in] y Y座標
@param[in] z Z座標
@return true 含まれている
@return false 含まれていない
*/
bool makerfaire::fxat::Contain(int x, int y, int z)
{
	return 0 <= x  && x < makerfaire::fxat::Led::Width
		&& 0 <= y  && y < makerfaire::fxat::Led::Height
		&& 0 <= z  && z < makerfaire::fxat::Led::Depth;
}

/*!
座標が3D LEDに含まれているかを返す
@param[in] pt 座標
@return true 含まれている
@return false 含まれていない
*/
bool makerfaire::fxat::Contain(cv::Point3i const & pt)
{
	return Contain(pt.x, pt.y, pt.z);
}
