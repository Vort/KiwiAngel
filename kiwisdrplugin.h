// License: GPLv3

#ifndef _KIWISDR_KIWISDRPLUGIN_H
#define _KIWISDR_KIWISDRPLUGIN_H

#include <QObject>
#include "plugin/plugininterface.h"

class PluginAPI;

#define KIWISDR_DEVICE_TYPE_ID "sdrangel.samplesource.kiwisdrsource"

class KiwiSDRPlugin : public QObject, public PluginInterface {
	Q_OBJECT
	Q_INTERFACES(PluginInterface)
	Q_PLUGIN_METADATA(IID KIWISDR_DEVICE_TYPE_ID)

public:
	explicit KiwiSDRPlugin(QObject* parent = NULL);

	const PluginDescriptor& getPluginDescriptor() const;
	void initPlugin(PluginAPI* pluginAPI);

	virtual SamplingDevices enumSampleSources();
	virtual PluginInstanceGUI* createSampleSourcePluginInstanceGUI(
	        const QString& sourceId,
	        QWidget **widget,
	        DeviceUISet *deviceUISet);
	virtual DeviceSampleSource* createSampleSourcePluginInstance(const QString& sourceId, DeviceAPI *deviceAPI);

	static const QString m_hardwareID;
    static const QString m_deviceTypeID;

private:
	static const PluginDescriptor m_pluginDescriptor;
};

#endif // _KIWISDR_KIWISDRPLUGIN_H
