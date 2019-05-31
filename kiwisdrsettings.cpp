// License: GPLv3

#include "util/simpleserializer.h"
#include "kiwisdrsettings.h"

KiwiSDRSettings::KiwiSDRSettings()
{
    resetToDefaults();
}

void KiwiSDRSettings::resetToDefaults()
{
    m_centerFrequency = 1450000;

	m_gain = 20;
	m_useAGC = true;

	m_serverAddress = "127.0.0.1:8073";

    m_useReverseAPI = false;
    m_reverseAPIAddress = "127.0.0.1";
    m_reverseAPIPort = 8888;
    m_reverseAPIDeviceIndex = 0;
}

QByteArray KiwiSDRSettings::serialize() const
{
    SimpleSerializer s(1);

	s.writeU32(4, m_gain);
	s.writeBool(5, m_useAGC);


    s.writeBool(18, m_useReverseAPI);
    s.writeString(19, m_reverseAPIAddress);
    s.writeU32(20, m_reverseAPIPort);
    s.writeU32(21, m_reverseAPIDeviceIndex);

	s.writeString(22, m_serverAddress);

	return s.final();
}

bool KiwiSDRSettings::deserialize(const QByteArray& data)
{
    SimpleDeserializer d(data);

    if (!d.isValid())
    {
        resetToDefaults();
        return false;
    }

    if (d.getVersion() == 1)
    {
        uint32_t utmp;

		d.readU32(4, &m_gain, 20);
		d.readBool(5, &m_useAGC, true);

        d.readBool(18, &m_useReverseAPI, false);
        d.readString(19, &m_reverseAPIAddress, "127.0.0.1");
        d.readU32(20, &utmp, 0);

        if ((utmp > 1023) && (utmp < 65535)) {
            m_reverseAPIPort = utmp;
        } else {
            m_reverseAPIPort = 8888;
        }

        d.readU32(21, &utmp, 0);
        m_reverseAPIDeviceIndex = utmp > 99 ? 99 : utmp;

		d.readString(22, &m_serverAddress, "127.0.0.1:8073");

        return true;
    }
    else
    {
        resetToDefaults();
        return false;
    }
}






