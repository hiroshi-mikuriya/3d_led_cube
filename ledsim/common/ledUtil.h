#ifndef LED_UTIL_H__
#define LED_UTIL_H__

#include "led.h"
#include <opencv2/opencv.hpp>
#include <cstdint>

namespace makerfaire
{
	namespace fxat
	{

		/*!
		uint8_tのRGB値を3D LEDで使用するRGB値に変換する
		@param[in] r R値
		@param[in] g G値
		@param[in] b B値
		@return RGB値
		*/
		int UcharToRgb(uint8_t r, uint8_t g, uint8_t b);

		/*!
		cv::Scalarを3D LEDで使用するRGB値に変換する
		@param[in] sc cv::Scalar
		@return RGB値
		*/
		int CvScalarToRgb(cv::Scalar const & sc);

		/*!
		3D LEDに球を描く（内部は塗りつぶす）
		範囲外の座標を指定しても描画せずエラーにならない
		@param[in] led 3D LED
		@param[in] pt 座標（ドットの中心座標）
		@param[in] sc 色
		@param[in] r 半径（0の場合中心だけ塗る）
		*/
		void DrawBall(
			makerfaire::fxat::Led & led,
			cv::Point3i const & pt,
			cv::Scalar sc,
			double r);

		/*!
		3D LEDに立方体を描く（内部は塗りつぶす）
		範囲外の座標を指定しても描画せずエラーにならない
		@param[in] led 3D LED
		@param[in] pt 座標（立方体の中心座標）
		@param[in] sc 色
		@param[in] width 幅
		*/
		void DrawCube(
			makerfaire::fxat::Led & led,
			cv::Point3i const & pt,
			cv::Scalar sc,
			int width);

		/*!
		3D LEDにラインをひく
		@param[in] led 3D LED
		@param[in] pt1 始点
		@param[in] pt2 終点
		@param[in] sc 色
		@param[in] width 幅
		*/
		void DrawLine(
			makerfaire::fxat::Led & led,
			cv::Point3i const & pt1,
			cv::Point3i const & pt2,
			cv::Scalar sc,
			int width);

		/*!
		座標が3D LEDに含まれているかを返す
		@param[in] x X座標
		@param[in] y Y座標
		@param[in] z Z座標
		@return true 含まれている
		@return false 含まれていない
		*/
		bool Contain(int x, int y, int z);

		/*!
		座標が3D LEDに含まれているかを返す
		@param[in] pt 座標
		@return true 含まれている
		@return false 含まれていない
		*/
		bool Contain(cv::Point3i const & pt);
	}
}

#endif // LED_UTIL_H__