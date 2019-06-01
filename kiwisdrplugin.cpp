// License: GPLv3

#include <QtPlugin>

#include "plugin/pluginapi.h"
#include "util/simpleserializer.h"

#ifdef SERVER_MODE
#include "kiwisdrinput.h"
#else
#include "kiwisdrgui.h"
#endif
#include "kiwisdrplugin.h"

const PluginDescriptor KiwiSDRPlugin::m_pluginDescriptor = {
	QString("KiwiSDR input"),
	QString("0.0.1"),
	QString("(c) "),
	QString("https://github.com/Vort/KiwiAngel"),
	true,
	QString("https://github.com/Vort/KiwiAngel")
};

const QString KiwiSDRPlugin::m_hardwareID = "KiwiSDR";
const QString KiwiSDRPlugin::m_deviceTypeID = KIWISDR_DEVICE_TYPE_ID;

KiwiSDRPlugin::KiwiSDRPlugin(QObject* parent) :
	QObject(parent)
{
}

const PluginDescriptor& KiwiSDRPlugin::getPluginDescriptor() const
{
	return m_pluginDescriptor;
}

void KiwiSDRPlugin::initPlugin(PluginAPI* pluginAPI)
{
	pluginAPI->registerSampleSource(m_deviceTypeID, this);
}

PluginInterface::SamplingDevices KiwiSDRPlugin::enumSampleSources()
{
	SamplingDevices result;

    result.append(SamplingDevice(
            "KiwiSDR",
            m_hardwareID,
            m_deviceTypeID,
            QString::null,
            0,
            PluginInterface::SamplingDevice::BuiltInDevice,
            PluginInterface::SamplingDevice::StreamSingleRx,
            1,
            0));

	return result;
}

#ifdef SERVER_MODE
PluginInstanceGUI* KiwiSDRPlugin::createSampleSourcePluginInstanceGUI(
        const QString& sourceId,
        QWidget **widget,
        DeviceUISet *deviceUISet)
{
    return 0;
}
#else
PluginInstanceGUI* KiwiSDRPlugin::createSampleSourcePluginInstanceGUI(
        const QString& sourceId,
        QWidget **widget,
        DeviceUISet *deviceUISet)
{
	if(sourceId == m_deviceTypeID) {
		KiwiSDRGui* gui = new KiwiSDRGui(deviceUISet);
		*widget = gui;
		return gui;
	} else {
		return 0;
	}
}
#endif

DeviceSampleSource *KiwiSDRPlugin::createSampleSourcePluginInstance(const QString& sourceId, DeviceAPI *deviceAPI)
{
    if (sourceId == m_deviceTypeID)
    {
		KiwiSDRInput* input = new KiwiSDRInput(deviceAPI);
        return input;
    }
    else
    {
        return 0;
    }
}

