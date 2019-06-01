// License: GPLv3

#include <boost/endian/conversion.hpp>
#include "kiwisdrworker.h"

KiwiSDRWorker::KiwiSDRWorker(SampleSinkFifo* sampleFifo)
	: QObject(),
	m_timer(this),
	m_sampleFifo(sampleFifo),
	m_samplesBuf(),
	m_centerFrequency(1450000),
	m_gain(20),
	m_useAGC(true)
{
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

	m_webSocket.setParent(this);
	connect(&m_webSocket, &QWebSocket::connected,
		this, &KiwiSDRWorker::onConnected);
	connect(&m_webSocket, &QWebSocket::binaryMessageReceived,
		this, &KiwiSDRWorker::onBinaryMessageReceived);
	connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
		this, &KiwiSDRWorker::onSocketError);
}

void KiwiSDRWorker::onConnected()
{
	m_webSocket.sendTextMessage("SET auth t=kiwi p=#");
}

void KiwiSDRWorker::onSocketError(QAbstractSocket::SocketError error)
{
	emit updateStatus(3);
}

void KiwiSDRWorker::sendCenterFrequency()
{
	if (!m_webSocket.isValid())
		return;

	QString freq = QString::number(m_centerFrequency / 1000.0, 'f', 3);
	QString msg = "SET mod=iq low_cut=-5980 high_cut=5980 freq=" + freq;
	m_webSocket.sendTextMessage(msg);
}

void KiwiSDRWorker::sendGain()
{
	if (!m_webSocket.isValid())
		return;

	QString msg("SET agc=");
	msg.append(m_useAGC ? "1" : "0");
	msg.append(" hang=0 thresh=-130 slope=6 decay=1000 manGain=");
	msg.append(QString::number(m_gain));
	m_webSocket.sendTextMessage(msg);
}

void KiwiSDRWorker::onBinaryMessageReceived(const QByteArray &message)
{
	if (message[0] == 'M' && message[1] == 'S' && message[2] == 'G')
	{
		QStringList al = QString::fromUtf8(message).split(' ');
		if (al[1] == "audio_init=0" &&
			al[2] == "audio_rate=12000")
		{
			m_webSocket.sendTextMessage("SET AR OK in=12000 out=48000");
			m_webSocket.sendTextMessage("SERVER DE CLIENT KiwiAngel SND");
			sendGain();
			sendCenterFrequency();
			m_timer.start(5000);
			emit updateStatus(2);
		}
	}
	else if (message[0] == 'S' && message[1] == 'N' && message[2] == 'D')
	{
		int dataOffset = 20;
		int sampleCount = 512;
		const int16_t* messageSamples = (const int16_t*)(message.constData() + dataOffset);

		m_samplesBuf.clear();
		for (int i = 0; i < sampleCount; i++)
		{
			m_samplesBuf.push_back(Sample(
				boost::endian::endian_reverse(messageSamples[i * 2]) << (SDR_RX_SAMP_SZ - 16),
				boost::endian::endian_reverse(messageSamples[i * 2 + 1]) << (SDR_RX_SAMP_SZ - 16)
			));
		}

		m_sampleFifo->write(m_samplesBuf.begin(), m_samplesBuf.end());
	}
}

void KiwiSDRWorker::onCenterFrequencyChanged(quint64 centerFrequency)
{
	if (m_centerFrequency == centerFrequency)
		return;

	m_centerFrequency = centerFrequency;
	sendCenterFrequency();
}

void KiwiSDRWorker::onGainChanged(quint32 gain, bool useAGC)
{
	if (m_gain == gain && m_useAGC == useAGC)
		return;

	m_gain = gain;
	m_useAGC = useAGC;

	sendGain();
}

void KiwiSDRWorker::onServerAddressChanged(QString serverAddress)
{
	if (m_serverAddress == serverAddress)
		return;
	m_serverAddress = serverAddress;

	emit updateStatus(1);

	QString url("ws://");
	url.append(m_serverAddress);
	url.append("/kiwi/");
	url.append(QString::number(QDateTime::currentMSecsSinceEpoch()));
	url.append("/SND");
	m_webSocket.open(QUrl(url));
}

void KiwiSDRWorker::tick()
{
	m_webSocket.sendTextMessage("SET keepalive");
}