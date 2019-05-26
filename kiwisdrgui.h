// License: GPLv3

#ifndef _KIWISDR_KIWISDRGUI_H_
#define _KIWISDR_KIWISDRGUI_H_

#include <plugin/plugininstancegui.h>
#include <QTimer>
#include <QWidget>

#include "util/messagequeue.h"

#include "kiwisdrsettings.h"
#include "kiwisdrinput.h"

class DeviceUISet;

namespace Ui {
	class KiwiSDRGui;
}

class KiwiSDRGui : public QWidget, public PluginInstanceGUI {
	Q_OBJECT

public:
	explicit KiwiSDRGui(DeviceUISet *deviceUISet, QWidget* parent = 0);
	virtual ~KiwiSDRGui();
	virtual void destroy();

	void setName(const QString& name);
	QString getName() const;

	void resetToDefaults();
	virtual qint64 getCenterFrequency() const;
	virtual void setCenterFrequency(qint64 centerFrequency);
	QByteArray serialize() const;
	bool deserialize(const QByteArray& data);
	virtual MessageQueue *getInputMessageQueue() { return &m_inputMessageQueue; }
	virtual bool handleMessage(const Message& message);

private:
	Ui::KiwiSDRGui* ui;

	DeviceUISet* m_deviceUISet;
	KiwiSDRSettings m_settings;
    QTimer m_updateTimer;
    QTimer m_statusTimer;
	bool m_doApplySettings;
    bool m_forceSettings;
	DeviceSampleSource* m_sampleSource;
	std::size_t m_tickCount;
    int m_deviceSampleRate;
    quint64 m_deviceCenterFrequency; //!< Center frequency in device
	int m_lastEngineState;
	MessageQueue m_inputMessageQueue;

	void blockApplySettings(bool block) { m_doApplySettings = !block; }
	void displaySettings();
	void sendSettings();
    void updateSampleRateAndFrequency();

private slots:
    void handleInputMessages();
	void on_startStop_toggled(bool checked);
    void on_centerFrequency_changed(quint64 value);
	void on_record_toggled(bool checked);
	void on_serverAddressApplyButton_clicked();
	void openDeviceSettingsDialog(const QPoint& p);
    void updateStatus();
    void updateHardware();
};

#endif // _KIWISDR_KIWISDRGUI_H_
