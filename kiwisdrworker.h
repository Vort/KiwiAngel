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

	uint32_t m_gain;
	bool m_useAGC;

	void sendCenterFrequency();
	void sendGain();

signals:
	void updateStatus(int status);

public slots:
	void onCenterFrequencyChanged(quint64 centerFrequency);
	void onServerAddressChanged(QString serverAddress);
	void onGainChanged(quint32 gain, bool useAGC);

private slots:
	void onConnected();
	void onBinaryMessageReceived(const QByteArray &message);
	void onSocketError(QAbstractSocket::SocketError error);

    void tick();
};

#endif // _KIWISDR_KIWISDRWORKER_H_
