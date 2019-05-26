// License: GPLv3

#ifndef _KIWISDR_KIWISDRWORKER_H_
#define _KIWISDR_KIWISDRWORKER_H_

#include <QTimer>
#include <QtWebSockets/QtWebSockets>

#include "dsp/samplesinkfifo.h"

class KiwiSDRWorker : public QObject {
	Q_OBJECT

public:
	KiwiSDRWorker(SampleSinkFifo* sampleFifo);

private:
	QTimer m_timer;
	QWebSocket m_webSocket;

	SampleVector m_samplesBuf;
	SampleSinkFifo* m_sampleFifo;

	QString m_serverAddress;
	uint64_t m_centerFrequency;

	void sendCenterFrequency();

public slots:
	void onCenterFrequencyChanged(quint64 centerFrequency);
	void onServerAddressChanged(QString serverAddress);

private slots:
	void onConnected();
	void onDisconnected();
	void onBinaryMessageReceived(const QByteArray &message);

    void tick();
};

#endif // _KIWISDR_KIWISDRWORKER_H_
