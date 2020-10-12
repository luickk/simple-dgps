// Generated by commsdsl2comms v3.5.2

#include "Heading.h"

#include "comms_champion/property/field.h"
#include "ublox/field/Heading.h"


namespace cc = comms_champion;

namespace ublox
{

namespace cc_plugin
{

namespace field
{

QVariantMap createProps_heading(const char* name, bool serHidden)
{
    static_cast<void>(serHidden);
    using Field = ublox::field::Heading<>;
    return
        cc::property::field::ForField<Field>()
            .name(name)
            .serialisedHidden(serHidden)
            .scaledDecimals(5U)
            .asMap();
    
}

} // namespace field

} // namespace cc_plugin

} // namespace ublox

