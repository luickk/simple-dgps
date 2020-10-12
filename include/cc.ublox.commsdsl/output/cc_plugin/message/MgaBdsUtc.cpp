// Generated by commsdsl2comms v3.5.2

#include "MgaBdsUtc.h"

#include "comms_champion/property/field.h"
#include "comms_champion/ProtocolMessageBase.h"
#include "ublox/message/MgaBdsUtc.h"
#include "cc_plugin/field/Res1.h"
#include "cc_plugin/field/Res2.h"

namespace cc = comms_champion;

namespace ublox
{

namespace cc_plugin
{

namespace message
{

namespace
{

static QVariantMap createProps_type()
{
    using Field = ublox::message::MgaBdsUtcFields<>::Type;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .readOnly()
            .asMap();
    
}

static QVariantMap createProps_version()
{
    using Field = ublox::message::MgaBdsUtcFields<>::Version;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_reserved1()
{
    using Field = ublox::message::MgaBdsUtcFields<>::Reserved1;
    auto props = cc_plugin::field::createProps_res2(Field::name());
    return props;
    
}

static QVariantMap createProps_a0UTC()
{
    using Field = ublox::message::MgaBdsUtcFields<>::A0UTC;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_a1UTC()
{
    using Field = ublox::message::MgaBdsUtcFields<>::A1UTC;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_dtLS()
{
    using Field = ublox::message::MgaBdsUtcFields<>::DtLS;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_reserved2()
{
    using Field = ublox::message::MgaBdsUtcFields<>::Reserved2;
    auto props = cc_plugin::field::createProps_res1(Field::name());
    return props;
    
}

static QVariantMap createProps_wnRec()
{
    using Field = ublox::message::MgaBdsUtcFields<>::WnRec;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_wnLSF()
{
    using Field = ublox::message::MgaBdsUtcFields<>::WnLSF;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_dN()
{
    using Field = ublox::message::MgaBdsUtcFields<>::DN;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_dtLSF()
{
    using Field = ublox::message::MgaBdsUtcFields<>::DtLSF;
    return
        cc::property::field::ForField<Field>()
            .name(Field::name())
            .asMap();
    
}

static QVariantMap createProps_reserved3()
{
    using Field = ublox::message::MgaBdsUtcFields<>::Reserved3;
    auto props = cc_plugin::field::createProps_res2(Field::name());
    return props;
    
}

QVariantList createProps()
{
    QVariantList props;
    props.append(createProps_type());
    props.append(createProps_version());
    props.append(createProps_reserved1());
    props.append(createProps_a0UTC());
    props.append(createProps_a1UTC());
    props.append(createProps_dtLS());
    props.append(createProps_reserved2());
    props.append(createProps_wnRec());
    props.append(createProps_wnLSF());
    props.append(createProps_dN());
    props.append(createProps_dtLSF());
    props.append(createProps_reserved3());
    return props;
}

} // namespace

class MgaBdsUtcImpl : public
    comms_champion::ProtocolMessageBase<
        ublox::message::MgaBdsUtc<ublox::cc_plugin::Message>,
        MgaBdsUtcImpl
    >
{
public:
    MgaBdsUtcImpl() = default;
    MgaBdsUtcImpl(const MgaBdsUtcImpl&) = delete;
    MgaBdsUtcImpl(MgaBdsUtcImpl&&) = delete;
    virtual ~MgaBdsUtcImpl() = default;
    MgaBdsUtcImpl& operator=(const MgaBdsUtcImpl&) = default;
    MgaBdsUtcImpl& operator=(MgaBdsUtcImpl&&) = default;

protected:
    virtual const QVariantList& fieldsPropertiesImpl() const override
    {
        static const QVariantList Props = createProps();
        return Props;
    }
};

MgaBdsUtc::MgaBdsUtc() : m_pImpl(new MgaBdsUtcImpl) {}
MgaBdsUtc::~MgaBdsUtc() = default;

MgaBdsUtc& MgaBdsUtc::operator=(const MgaBdsUtc& other)
{
    *m_pImpl = *other.m_pImpl;
    return *this;
}

MgaBdsUtc& MgaBdsUtc::operator=(MgaBdsUtc&& other)
{
    *m_pImpl = std::move(*other.m_pImpl);
    return *this;
}

const char* MgaBdsUtc::nameImpl() const
{
    return static_cast<const cc::Message*>(m_pImpl.get())->name();
}

const QVariantList& MgaBdsUtc::fieldsPropertiesImpl() const
{
    return m_pImpl->fieldsProperties();
}

void MgaBdsUtc::dispatchImpl(cc::MessageHandler& handler)
{
    static_cast<cc::Message*>(m_pImpl.get())->dispatch(handler);
}

void MgaBdsUtc::resetImpl()
{
    m_pImpl->reset();
}

bool MgaBdsUtc::assignImpl(const cc::Message& other)
{
    auto* castedOther = dynamic_cast<const MgaBdsUtc*>(&other);
    if (castedOther == nullptr) {
        return false;
    }
    return m_pImpl->assign(*castedOther->m_pImpl);
}

MgaBdsUtc::MsgIdParamType MgaBdsUtc::getIdImpl() const
{
    return m_pImpl->getId();
}

comms::ErrorStatus MgaBdsUtc::readImpl(ReadIterator& iter, std::size_t len)
{
    return m_pImpl->read(iter, len);
}

comms::ErrorStatus MgaBdsUtc::writeImpl(WriteIterator& iter, std::size_t len) const
{
    return m_pImpl->write(iter, len);
}

bool MgaBdsUtc::validImpl() const
{
    return m_pImpl->valid();
}

std::size_t MgaBdsUtc::lengthImpl() const
{
    return m_pImpl->length();
}

bool MgaBdsUtc::refreshImpl()
{
    return m_pImpl->refresh();
}

} // namespace message

} // namespace cc_plugin

} // namespace ublox

