#pragma once

#include "pxcsensemanager.h"
#include "pxcmetadata.h"
#include "pxcfaceconfiguration.h"
#include "pxcfacemodule.h"
#include "pxcpersontrackingconfiguration.h"
#include "pxcpersontrackingmodule.h"
#include <stdexcept>
#include <cassert>
#include <sstream>
#include <iostream>

struct ThrowIfErrorType
{
	int m_line;
	ThrowIfErrorType(int line) : m_line(line){}
};
#define THROW_IF_ERROR	ThrowIfErrorType(__LINE__)

pxcStatus operator|(pxcStatus sts, ThrowIfErrorType const & obj)
{
	if (sts < PXC_STATUS_NO_ERROR){
		std::stringstream ss;
		ss << "PXC STATUS ERROR :" << sts << "  LINE(" << obj.m_line << ")";
		throw std::runtime_error(ss.str());
	}
	return sts;
}

class SenseManager
{
	PXCSenseManager * m_sensor;
	PXCFaceData * m_face;
	PXCPersonTrackingData * m_person;
public:
	SenseManager()
		: m_sensor(PXCSenseManager::CreateInstance())
		, m_face(0)
		, m_person(0)
	{
		if (!m_sensor){
			throw std::runtime_error("Failed to create instance PXCSenseManager.");
		}
	}
	~SenseManager()
	{
		assert(m_sensor);
		if (m_sensor->IsConnected()){
			m_sensor->Close();
		}
		m_sensor->Release();
	}
	void Init()
	{
		PXCVideoModule::DataDesc desc = {};
		for (auto type : {
			PXCCapture::STREAM_TYPE_COLOR,
			PXCCapture::STREAM_TYPE_DEPTH,
		})
		{
			desc.deviceInfo.streams = type | desc.deviceInfo.streams;
			desc.streams[type].sizeMax = { 640, 480 }; // EnableFace‚Ì‚Æ‚«‚ÍA320*240‚¾‚ÆInit‚ªƒGƒ‰[‚É‚È‚é
			desc.streams[type].frameRate.min = 30;
		}
			m_sensor->EnableStreams(&desc) | THROW_IF_ERROR;
			{
				m_sensor->EnableFace() | THROW_IF_ERROR;
				auto face = m_sensor->QueryFace();
				auto cfg = face->CreateActiveConfiguration();
				cfg->SetTrackingMode(PXCFaceConfiguration::FACE_MODE_COLOR_PLUS_DEPTH) | THROW_IF_ERROR;
				cfg->ApplyChanges() | THROW_IF_ERROR;
				cfg->detection.isEnabled = true;
				cfg->landmarks.isEnabled = true;
				cfg->pose.isEnabled = true;
				m_face = face->CreateOutput();
			}
			{
				m_sensor->EnablePersonTracking();
				auto person = m_sensor->QueryPersonTracking();
				auto cfg = person->QueryConfiguration();
				cfg->QueryTracking()->SetTrackingMode(PXCPersonTrackingConfiguration::TrackingConfiguration::TRACKING_MODE_INTERACTIVE);
#if 0
				auto skelton = cfg->QuerySkeletonJoints();
				skelton->Enable();
				skelton->SetTrackingArea(PXCPersonTrackingConfiguration::SkeletonJointsConfiguration::AREA_FULL_BODY);
				skelton->SetMaxTrackedPersons(1);
#endif
				cfg->QueryGestures()->Enable();
				//				cfg->QueryTracking()->Enable();
				//				cfg->QueryGestures()->EnableAllGestures();
				//				cfg->QueryTracking()->EnablePersonOrientation();
				//				cfg->QueryTracking()->EnableSegmentation();
				//				cfg->QueryTracking()->EnableHeadPose();
				//				cfg->QueryTracking()->EnableBlob();
				//				cfg->QueryTracking()->EnableHeadBoundingBox();
				std::cout
					<< "Tracking : " << cfg->QueryTracking()->IsEnabled() << std::endl
					<< "Gestures : " << cfg->QueryGestures()->IsEnabled() << std::endl
					<< "Skeleton : " << cfg->QuerySkeletonJoints()->IsEnabled() << std::endl
					<< "Pose : " << cfg->QueryPose()->IsEnabled() << std::endl
					<< "Recognition : " << cfg->QueryRecognition()->IsEnabled() << std::endl
					<< "Expressions : " << cfg->QueryExpressions()->IsEnabled() << std::endl
					;
			}
			m_sensor->Init();
			m_person = m_sensor->QueryPersonTracking()->QueryOutput();
			PXCCapture::Device *dev = m_sensor->QueryCaptureManager()->QueryDevice();
			if (!dev){
				throw std::runtime_error("PXCSenseManager::QueryCaptureManager()->QueryDevice() is nil.");
			}
			dev->ResetProperties(PXCCapture::STREAM_TYPE_ANY);
			dev->SetMirrorMode(PXCCapture::Device::MirrorMode::MIRROR_MODE_HORIZONTAL) | THROW_IF_ERROR;
	}
	PXCSenseManager * sensor()const
	{
		return m_sensor;
	}
	PXCFaceData * face()const
	{
		return m_face;
	}
	PXCPersonTrackingData * person()const
	{
		return m_person;
	}
};
