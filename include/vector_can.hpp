#pragma once

#include <iostream>
#include <Windows.h>
#include <vxlapi.h>
#include <string>

namespace vector
{
	bool InitDriver();
	bool CloseDriver();

	class Port;

	class App
	{
	public:
		App(const std::string& name, uint32_t hwType);

		Port OpenPort(uint32_t rxQueueSize);

	private:
		std::string m_Name;
		uint32_t m_Channel;
		uint32_t m_HwType;
		uint32_t m_HwIdx;
		uint32_t m_HwChannel;
	};

	class Port
	{
	public:
		Port(
			const std::string& appName,
			uint32_t hwType,
			uint32_t hwIdx,
			uint32_t hwChannel,
			uint32_t rxQueueSize);
		
		void Close();
		void SetTimerRate(uint64_t timerRate);
		void ResetClock();
		XLhandle SetNotification(int queueLevel);
		void FlushRxQueue();
		int GetRxQueueLevel();
		void ActivateChannel();
		void Receive(uint32_t& eventCount, XLevent* events);
		uint64_t GetSyncTime();
		uint64_t GetChannelTime();
		void GenSyncPulse();
		void DeactivateChannel();

		void CanSetChannelMode(int tx, int txrq);
		void CanSetChannelOutput(uint8_t mode);
		void CanSetReceiveMode(uint8_t errorFrame, uint8_t chipState);
		void CanSetChannelTransceiver(int type, int lineMode, int resNet);
		void CanSetChannelParams(XLchipParams* chipParams);
		void CanSetChannelParamsC200(uint8_t btr0, uint8_t btr1);
		void CanSetChannelBitrate(uint64_t bitrate);
		void CanSetChannelAcceptance(uint64_t code, uint64_t mask, uint32_t idRange);
		void CanRequestChipState();
		void CanTransmit(uint32_t& eventCount, void* events);
		void CanFlushTransmitQueue();

	private:
		XLportHandle m_Handle;
		XLaccess m_ChannelMask;
	};
}
