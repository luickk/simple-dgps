// Generated by commsdsl2comms v3.5.2

#pragma once

#include "comms_champion/MessageBase.h"
#include "ublox/Message.h"

namespace ublox
{

namespace cc_plugin
{

class Message : public
    comms_champion::MessageBase<
        ublox::Message
    >
{
protected:
    virtual QString idAsStringImpl() const override;
    virtual const QVariantList& extraTransportFieldsPropertiesImpl() const override;
};

} // namespace cc_plugin

} // namespace ublox


