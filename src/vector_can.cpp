#include <vector_can.hpp>

namespace vector
{
	bool InitDriver()
	{
		return XL_SUCCESS == xlOpenDriver();
	}

	bool CloseDriver()
	{
		return XL_SUCCESS == xlCloseDriver();
	}

	App::App(const std::string& name, uint32_t hwType) :
		m_Name(name),
		m_Channel(0),
		m_HwType(hwType),
		m_HwIdx(0),
		m_HwChannel(0)
	{
		const XLstatus status = xlSetApplConfig(
			const_cast<char*>(m_Name.c_str()),
			m_Channel,
			m_HwType,
			m_HwIdx,
			m_HwChannel,
			XL_BUS_TYPE_CAN);
		if (XL_SUCCESS != status)
			throw new std::runtime_error("Failed to create Vector application");
	}

	Port App::OpenPort(uint32_t rxQueueSize)
	{
		return Port(m_Name, m_HwType, m_HwIdx, m_HwChannel, rxQueueSize);
	}

	Port::Port(
		const std::string& appName, 
		uint32_t hwType, 
		uint32_t hwIdx, 
		uint32_t hwChannel,
		uint32_t rxQueueSize)
	{
		m_ChannelMask = xlGetChannelMask(hwType, hwIdx, hwChannel);
		if (!m_ChannelMask)
			throw new std::runtime_error("Vector hardware not present");

		XLaccess permMask = m_ChannelMask;

		const XLstatus status = xlOpenPort(
			&m_Handle,
			const_cast<char*>(appName.c_str()),
			m_ChannelMask,
			&permMask,
			rxQueueSize,
			XL_INTERFACE_VERSION,
			XL_BUS_TYPE_CAN);
		if (XL_SUCCESS != status)
			throw new std::runtime_error("Failed to open port");
	}

	void Port::Close()
	{
		if (XL_SUCCESS != xlClosePort(m_Handle))
			throw new std::runtime_error("Failed to close port");
	}

	void Port::SetTimerRate(uint64_t timerRate)
	{
		if (XL_SUCCESS != xlSetTimerRate(m_Handle, timerRate))
			throw new std::runtime_error("Failed to set timer rate for port");
	}

	void Port::ResetClock()
	{
		if (XL_SUCCESS != xlResetClock(m_Handle))
			throw new std::runtime_error("Failed to reset clock for port");
	}

	XLhandle Port::SetNotification(int queueLevel)
	{
		XLhandle handle;
		if (XL_SUCCESS != xlSetNotification(m_Handle, &handle, queueLevel))
			throw new std::runtime_error("Failed to set notification for port");
		return handle;
	}

	void Port::FlushRxQueue()
	{
		if (XL_SUCCESS != xlFlushReceiveQueue(m_Handle))
			throw new std::runtime_error("Failed to flux rx queue for port");
	}

	int Port::GetRxQueueLevel()
	{
		int level;
		if (XL_SUCCESS != xlGetReceiveQueueLevel(m_Handle, &level))
			throw new std::runtime_error("Failed to get rx queue level for port");
		return level;
	}

	void Port::ActivateChannel()
	{
		const XLstatus status = xlActivateChannel(
			m_Handle, 
			m_ChannelMask, 
			XL_BUS_TYPE_CAN, 
			0);
		if (XL_SUCCESS != status)
			throw new std::runtime_error("Failed to activate channel for port");
	}

	void Port::Receive(uint32_t& eventCount, XLevent* events)
	{
		if (XL_SUCCESS != xlReceive(m_Handle, &eventCount, events))
			throw new std::runtime_error("Failed to receive for port");
	}

	uint64_t Port::GetSyncTime()
	{
		uint64_t syncTime;
		if (XL_SUCCESS != xlGetSyncTime(m_Handle, &syncTime))
			throw new std::runtime_error("Failed to get sync time for port");
		return syncTime;
	}

	uint64_t Port::GetChannelTime()
	{
		uint64_t channelTime;
		if (XL_SUCCESS != xlGetChannelTime(m_Handle, m_ChannelMask, &channelTime))
			throw new std::runtime_error("Failed to get channel time for port");
		return channelTime;
	}

	void Port::GenSyncPulse()
	{
		if (XL_SUCCESS != xlGenerateSyncPulse(m_Handle, m_ChannelMask))
			throw new std::runtime_error("Failed to generate sync pulse for port");
	}

	void Port::DeactivateChannel()
	{
		if (XL_SUCCESS != xlDeactivateChannel(m_Handle, m_ChannelMask))
			throw new std::runtime_error("Failed to deactivate channel for port");
	}
}
