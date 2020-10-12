// Generated by commsdsl2comms v3.5.2

/// @file
/// @brief Contains definition of <b>"sigAttenCompMode"</b> field.

#pragma once

#include <cstdint>
#include "comms/field/IntValue.h"
#include "comms/options.h"
#include "ublox/field/CfgNavx5SigAttenCompModeCommon.h"
#include "ublox/field/FieldBase.h"
#include "ublox/options/DefaultOptions.h"

namespace ublox
{

namespace field
{

/// @brief Definition of <b>"sigAttenCompMode"</b> field.
/// @tparam TOpt Protocol options.
/// @tparam TExtraOpts Extra options.
template <typename TOpt = ublox::options::DefaultOptions, typename... TExtraOpts>
class CfgNavx5SigAttenCompMode : public
    comms::field::IntValue<
        ublox::field::FieldBase<>,
        std::uint8_t,
        TExtraOpts...
    >
{
    using Base = 
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t,
            TExtraOpts...
        >;
public:
    /// @brief Re-definition of the value type.
    using ValueType = typename Base::ValueType;

    /// @brief Special value <b>"Disabled"</b>.
    static constexpr ValueType valueDisabled()
    {
        return ublox::field::CfgNavx5SigAttenCompModeCommon::valueDisabled();
    }
    
    /// @brief Check the value is equal to special @ref valueDisabled().
    bool isDisabled() const
    {
        return Base::value() == valueDisabled();
    }
    
    /// @brief Assign special value @ref valueDisabled() to the field.
    void setDisabled()
    {
        Base::value() = valueDisabled();
    }
    
    /// @brief Special value <b>"Automatic"</b>.
    static constexpr ValueType valueAutomatic()
    {
        return ublox::field::CfgNavx5SigAttenCompModeCommon::valueAutomatic();
    }
    
    /// @brief Check the value is equal to special @ref valueAutomatic().
    bool isAutomatic() const
    {
        return Base::value() == valueAutomatic();
    }
    
    /// @brief Assign special value @ref valueAutomatic() to the field.
    void setAutomatic()
    {
        Base::value() = valueAutomatic();
    }
    
    /// @brief Name of the field.
    static const char* name()
    {
        return ublox::field::CfgNavx5SigAttenCompModeCommon::name();
    }
    
};

} // namespace field

} // namespace ublox

