// License: GPLv3

#ifndef _KIWISDR_KIWISDRINPUT_H_
#define _KIWISDR_KIWISDRINPUT_H_

#include <QString>
#include <QByteArray>
#include <QTimer>
#include <QThread>
#include <QNetworkRequest>

#include <dsp/devicesamplesource.h>
#include "kiwisdrsettings.h"

class DeviceAPI;
class KiwiSDRWorker;
class FileRecord;
class QNetworkAccessManager;
class QNetworkReply;

class KiwiSDRInput : public DeviceSampleSource {
    Q_OBJECT
public:
	class MsgConfigureKiwiSDR : public Message {
		MESSAGE_CLASS_DECLARATION

	public:
		const KiwiSDRSettings& getSettings() const { return m_settings; }
		bool getForce() const { return m_force; }

		static MsgConfigureKiwiSDR* create(const KiwiSDRSettings& settings, bool force)
		{
			return new MsgConfigureKiwiSDR(settings, force);
		}

	private:
		KiwiSDRSettings m_settings;
		bool m_force;

		MsgConfigureKiwiSDR(const KiwiSDRSettings& settings, bool force) :
			Message(),
			m_settings(settings),
			m_force(force)
		{ }
	};

    class MsgFileRecord : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        bool getStartStop() const { return m_startStop; }

        static MsgFileRecord* create(bool startStop) {
            return new MsgFileRecord(startStop);
        }

    protected:
        bool m_startStop;

        MsgFileRecord(bool startStop) :
            Message(),
            m_startStop(startStop)
        { }
    };

    class MsgStartStop : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        bool getStartStop() const { return m_startStop; }

        static MsgStartStop* create(bool startStop) {
            return new MsgStartStop(startStop);
        }

    protected:
        bool m_startStop;

        MsgStartStop(bool startStop) :
            Message(),
            m_startStop(startStop)
        { }
    };

	KiwiSDRInput(DeviceAPI *deviceAPI);
	virtual ~KiwiSDRInput();
	virtual void destroy();

	virtual void init();
	virtual bool start();
	virtual void stop();

    virtual QByteArray serialize() const;
    virtual bool deserialize(const QByteArray& data);

    virtual void setMessageQueueToGUI(MessageQueue *queue) { m_guiMessageQueue = queue; }
	virtual const QString& getDeviceDescription() const;
	virtual int getSampleRate() const;
    virtual void setSampleRate(int sampleRate) { (void) sampleRate; }
	virtual quint64 getCenterFrequency() const;
    virtual void setCenterFrequency(qint64 centerFrequency);

	virtual bool handleMessage(const Message& message);

    virtual int webapiSettingsGet(
                SWGSDRangel::SWGDeviceSettings& response,
                QString& errorMessage);

    virtual int webapiSettingsPutPatch(
                bool force,
                const QStringList& deviceSettingsKeys,
                SWGSDRangel::SWGDeviceSettings& response, // query + response
                QString& errorMessage);

    virtual int webapiRunGet(
            SWGSDRangel::SWGDeviceState& response,
            QString& errorMessage);

    virtual int webapiRun(
            bool run,
            SWGSDRangel::SWGDeviceState& response,
            QString& errorMessage);

private:
	DeviceAPI *m_deviceAPI;
    FileRecord *m_fileSink; //!< File sink to record device I/Q output
	QMutex m_mutex;
	KiwiSDRSettings m_settings;
	KiwiSDRWorker* m_kiwiSDRWorker;
	QThread m_kiwiSDRWorkerThread;
	QString m_deviceDescription;
	bool m_running;
    const QTimer& m_masterTimer;
    QNetworkAccessManager *m_networkManager;
    QNetworkRequest m_networkRequest;

	bool applySettings(const KiwiSDRSettings& settings, bool force);
    void webapiFormatDeviceSettings(SWGSDRangel::SWGDeviceSettings& response, const KiwiSDRSettings& settings);
    void webapiReverseSendSettings(QList<QString>& deviceSettingsKeys, const KiwiSDRSettings& settings, bool force);
    void webapiReverseSendStartStop(bool start);

signals:
	void startWorker();
	void stopWorker();
	void setWorkerCenterFrequency(quint64 centerFrequency);
	void setWorkerServerAddress(QString serverAddress);

private slots:
    void networkManagerFinished(QNetworkReply *reply);
};

#endif // _KIWISDR_KIWISDRINPUT_H_
