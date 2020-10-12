// Generated by commsdsl2comms v3.5.2

#include "CfgLogfilter.h"

#include "comms_champion/property/field.h"
#include "comms_champion/ProtocolMessageBase.h"
#include "ublox/message/CfgLogfilter.h"
namespace cc = comms_champion;

namespace ublox
{

namespace cc_plugin
{

namespace message
{

namespace
{

static QVariantMap createProps_version()
{
    using Field = ublox::message::CfgLogfilterFields<>::Version;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_flags()
{
    using Field = ublox::message::CfgLogfilterFields<>::Flags;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .add(0U, "recordEnabled")
            .add(1U, "psmOncePerWakupEnabled")
            .add(2U, "applyAllFilterSettings")
            .asMap();
    
}

static QVariantMap createProps_minInterval()
{
    using Field = ublox::message::CfgLogfilterFields<>::MinInterval;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_timeThreshold()
{
    using Field = ublox::message::CfgLogfilterFields<>::TimeThreshold;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_speedThreshold()
{
    using Field = ublox::message::CfgLogfilterFields<>::SpeedThreshold;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_positionThres()
{
    using Field = ublox::message::CfgLogfilterFields<>::PositionThres;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

QVariantList createProps()
{
    QVariantList props;
    props.append(createProps_version());
    props.append(createProps_flags());
    props.append(createProps_minInterval());
    props.append(createProps_timeThreshold());
    props.append(createProps_speedThreshold());
    props.append(createProps_positionThres());
    return props;
}

} // namespace

class CfgLogfilterImpl : public
    comms_champion::ProtocolMessageBase<
        ublox::message::CfgLogfilter<ublox::cc_plugin::Message>,
        CfgLogfilterImpl
    >
{
public:
    CfgLogfilterImpl() = default;
    CfgLogfilterImpl(const CfgLogfilterImpl&) = delete;
    CfgLogfilterImpl(CfgLogfilterImpl&&) = delete;
    virtual ~CfgLogfilterImpl() = default;
    CfgLogfilterImpl& operator=(const CfgLogfilterImpl&) = default;
    CfgLogfilterImpl& operator=(CfgLogfilterImpl&&) = default;

protected:
    virtual const QVariantList& fieldsPropertiesImpl() const override
    {
        static const QVariantList Props = createProps();
        return Props;
    }
};

CfgLogfilter::CfgLogfilter() : m_pImpl(new CfgLogfilterImpl) {}
CfgLogfilter::~CfgLogfilter() = default;

CfgLogfilter& CfgLogfilter::operator=(const CfgLogfilter& other)
{
    *m_pImpl = *other.m_pImpl;
    return *this;
}

CfgLogfilter& CfgLogfilter::operator=(CfgLogfilter&& other)
{
    *m_pImpl = std::move(*other.m_pImpl);
    return *this;
}

const char* CfgLogfilter::nameImpl() const
{
    return static_cast<const cc::Message*>(m_pImpl.get())->name();
}

const QVariantList& CfgLogfilter::fieldsPropertiesImpl() const
{
    return m_pImpl->fieldsProperties();
}

void CfgLogfilter::dispatchImpl(cc::MessageHandler& handler)
{
    static_cast<cc::Message*>(m_pImpl.get())->dispatch(handler);
}

void CfgLogfilter::resetImpl()
{
    m_pImpl->reset();
}

bool CfgLogfilter::assignImpl(const cc::Message& other)
{
    auto* castedOther = dynamic_cast<const CfgLogfilter*>(&other);
    if (castedOther == nullptr) {
        return false;
    }
    return m_pImpl->assign(*castedOther->m_pImpl);
}

CfgLogfilter::MsgIdParamType CfgLogfilter::getIdImpl() const
{
    return m_pImpl->getId();
}

comms::ErrorStatus CfgLogfilter::readImpl(ReadIterator& iter, std::size_t len)
{
    return m_pImpl->read(iter, len);
}

comms::ErrorStatus CfgLogfilter::writeImpl(WriteIterator& iter, std::size_t len) const
{
    return m_pImpl->write(iter, len);
}

bool CfgLogfilter::validImpl() const
{
    return m_pImpl->valid();
}

std::size_t CfgLogfilter::lengthImpl() const
{
    return m_pImpl->length();
}

bool CfgLogfilter::refreshImpl()
{
    return m_pImpl->refresh();
}

} // namespace message

} // namespace cc_plugin

} // namespace ublox

