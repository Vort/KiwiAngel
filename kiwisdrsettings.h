// License: GPLv3

#ifndef _KIWISDR_KIWISDRSETTINGS_H_
#define _KIWISDR_KIWISDRSETTINGS_H_

#include <QString>

struct KiwiSDRSettings {
	uint32_t m_gain;
	bool m_useAGC;

    quint64 m_centerFrequency;
    QString m_fileRecordName;
	QString m_serverAddress;
	bool m_useReverseAPI;
    QString m_reverseAPIAddress;
    uint16_t m_reverseAPIPort;
    uint16_t m_reverseAPIDeviceIndex;

	KiwiSDRSettings();
	void resetToDefaults();
	QByteArray serialize() const;
	bool deserialize(const QByteArray& data);
};

#endif /* _KIWISDR_KIWISDRSETTINGS_H_ */
