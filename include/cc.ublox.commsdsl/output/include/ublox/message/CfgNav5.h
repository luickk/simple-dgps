// Generated by commsdsl2comms v3.5.2

/// @file
/// @brief Contains definition of <b>"CFG-NAV5"</b> message and its fields.

#pragma once

#include <cstdint>
#include <tuple>
#include <type_traits>
#include "comms/MessageBase.h"
#include "comms/field/BitmaskValue.h"
#include "comms/field/EnumValue.h"
#include "comms/field/IntValue.h"
#include "comms/options.h"
#include "ublox/MsgId.h"
#include "ublox/field/FieldBase.h"
#include "ublox/field/Res2.h"
#include "ublox/field/Res5.h"
#include "ublox/message/CfgNav5Common.h"
#include "ublox/options/DefaultOptions.h"

namespace ublox
{

namespace message
{

/// @brief Fields of @ref CfgNav5.
/// @tparam TOpt Extra options
/// @see @ref CfgNav5
/// @headerfile "ublox/message/CfgNav5.h"
template <typename TOpt = ublox::options::DefaultOptions>
struct CfgNav5Fields
{
    /// @brief Definition of <b>"mask"</b> field.
    class Mask : public
        comms::field::BitmaskValue<
            ublox::field::FieldBase<>,
            comms::option::def::FixedLength<2U>,
            comms::option::def::BitmaskReservedBits<0xFA00U, 0x0U>
        >
    {
        using Base = 
            comms::field::BitmaskValue<
                ublox::field::FieldBase<>,
                comms::option::def::FixedLength<2U>,
                comms::option::def::BitmaskReservedBits<0xFA00U, 0x0U>
            >;
    public:
        /// @brief Provide names for internal bits.
        /// @details See definition of @b COMMS_BITMASK_BITS macro
        ///     related to @b comms::field::BitmaskValue class from COMMS library
        ///     for details.
        ///
        ///      The generated enum values:
        ///      @li @b BitIdx_dyn.
        ///      @li @b BitIdx_minEl.
        ///      @li @b BitIdx_posFixMode.
        ///      @li @b BitIdx_drLim.
        ///      @li @b BitIdx_posMask.
        ///      @li @b BitIdx_timeMask.
        ///      @li @b BitIdx_staticHoldMask.
        ///      @li @b BitIdx_dgpsMask.
        ///      @li @b BitIdx_cnoThreshold.
        ///      @li @b BitIdx_utc.
        COMMS_BITMASK_BITS(
            dyn=0,
            minEl=1,
            posFixMode=2,
            drLim=3,
            posMask=4,
            timeMask=5,
            staticHoldMask=6,
            dgpsMask=7,
            cnoThreshold=8,
            utc=10
        );
        
        /// @brief Generates independent access functions for internal bits.
        /// @details See definition of @b COMMS_BITMASK_BITS_ACCESS macro
        ///     related to @b comms::field::BitmaskValue class from COMMS library
        ///     for details.
        ///
        ///     The generated access functions are:
        ///      @li @b getBitValue_dyn() and @b setBitValue_dyn().
        ///      @li @b getBitValue_minEl() and @b setBitValue_minEl().
        ///      @li @b getBitValue_posFixMode() and @b setBitValue_posFixMode().
        ///      @li @b getBitValue_drLim() and @b setBitValue_drLim().
        ///      @li @b getBitValue_posMask() and @b setBitValue_posMask().
        ///      @li @b getBitValue_timeMask() and @b setBitValue_timeMask().
        ///      @li @b getBitValue_staticHoldMask() and @b setBitValue_staticHoldMask().
        ///      @li @b getBitValue_dgpsMask() and @b setBitValue_dgpsMask().
        ///      @li @b getBitValue_cnoThreshold() and @b setBitValue_cnoThreshold().
        ///      @li @b getBitValue_utc() and @b setBitValue_utc().
        COMMS_BITMASK_BITS_ACCESS(
            dyn,
            minEl,
            posFixMode,
            drLim,
            posMask,
            timeMask,
            staticHoldMask,
            dgpsMask,
            cnoThreshold,
            utc
        );
        
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::MaskCommon::name();
        }
        
        /// @brief Retrieve name of the bit.
        static const char* bitName(BitIdx idx)
        {
            return
                ublox::message::CfgNav5FieldsCommon::MaskCommon::bitName(
                    static_cast<std::size_t>(idx));
        }
        
    };
    
    /// @brief Definition of <b>"dynModel"</b> field.
    /// @see @ref ublox::message::CfgNav5FieldsCommon::DynModelVal
    class DynModel : public
        comms::field::EnumValue<
            ublox::field::FieldBase<>,
            ublox::message::CfgNav5FieldsCommon::DynModelVal,
            comms::option::def::ValidNumValue<0>,
            comms::option::def::ValidNumValueRange<2, 9>
        >
    {
        using Base = 
            comms::field::EnumValue<
                ublox::field::FieldBase<>,
                ublox::message::CfgNav5FieldsCommon::DynModelVal,
                comms::option::def::ValidNumValue<0>,
                comms::option::def::ValidNumValueRange<2, 9>
            >;
    public:
        /// @brief Re-definition of the value type.
        using ValueType = typename Base::ValueType;
    
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::DynModelCommon::name();
        }
        
        /// @brief Retrieve name of the enum value
        static const char* valueName(ValueType val)
        {
            return ublox::message::CfgNav5FieldsCommon::DynModelCommon::valueName(val);
        }
        
        /// @brief Retrieve name of the @b current value
        const char* valueName() const
        {
            return valueName(Base::value());
        }
        
    };
    
    /// @brief Definition of <b>"fixMode"</b> field.
    /// @see @ref ublox::message::CfgNav5FieldsCommon::FixModeVal
    class FixMode : public
        comms::field::EnumValue<
            ublox::field::FieldBase<>,
            ublox::message::CfgNav5FieldsCommon::FixModeVal,
            comms::option::def::ValidNumValueRange<1, 3>
        >
    {
        using Base = 
            comms::field::EnumValue<
                ublox::field::FieldBase<>,
                ublox::message::CfgNav5FieldsCommon::FixModeVal,
                comms::option::def::ValidNumValueRange<1, 3>
            >;
    public:
        /// @brief Re-definition of the value type.
        using ValueType = typename Base::ValueType;
    
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::FixModeCommon::name();
        }
        
        /// @brief Retrieve name of the enum value
        static const char* valueName(ValueType val)
        {
            return ublox::message::CfgNav5FieldsCommon::FixModeCommon::valueName(val);
        }
        
        /// @brief Retrieve name of the @b current value
        const char* valueName() const
        {
            return valueName(Base::value());
        }
        
    };
    
    /// @brief Definition of <b>"fixedAlt"</b> field.
    struct FixedAlt : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::int32_t,
            comms::option::def::ScalingRatio<1, 100>,
            comms::option::def::UnitsMeters
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::FixedAltCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"fixedAltVar"</b> field.
    struct FixedAltVar : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint32_t,
            comms::option::def::ScalingRatio<1, 10000>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::FixedAltVarCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"minElev"</b> field.
    struct MinElev : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::int8_t,
            comms::option::def::UnitsDegrees
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::MinElevCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"drLimit"</b> field.
    struct DrLimit : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t,
            comms::option::def::UnitsSeconds
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::DrLimitCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"pDop"</b> field.
    struct PDop : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint16_t,
            comms::option::def::ScalingRatio<1, 10>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::PDopCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"tDop"</b> field.
    struct TDop : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint16_t,
            comms::option::def::ScalingRatio<1, 10>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::TDopCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"pAcc"</b> field.
    struct PAcc : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint16_t,
            comms::option::def::UnitsMeters
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::PAccCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"tAcc"</b> field.
    struct TAcc : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint16_t,
            comms::option::def::UnitsMeters
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::TAccCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"staticHoldThresh"</b> field.
    struct StaticHoldThresh : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t,
            comms::option::def::UnitsCentimetersPerSecond
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::StaticHoldThreshCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"dgnssTimeout"</b> field.
    struct DgnssTimeout : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t,
            comms::option::def::UnitsSeconds
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::DgnssTimeoutCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"cnoThreshNumSVs"</b> field.
    struct CnoThreshNumSVs : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::CnoThreshNumSVsCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"cnoThresh"</b> field.
    struct CnoThresh : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::CnoThreshCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"reserved1"</b> field.
    struct Reserved1 : public
        ublox::field::Res2<
            TOpt
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::Reserved1Common::name();
        }
        
    };
    
    /// @brief Definition of <b>"staticHoldMaxDist"</b> field.
    struct StaticHoldMaxDist : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint16_t,
            comms::option::def::UnitsMeters
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::StaticHoldMaxDistCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"utcStandard"</b> field.
    /// @see @ref ublox::message::CfgNav5FieldsCommon::UtcStandardVal
    class UtcStandard : public
        comms::field::EnumValue<
            ublox::field::FieldBase<>,
            ublox::message::CfgNav5FieldsCommon::UtcStandardVal,
            comms::option::def::ValidNumValue<0>,
            comms::option::def::ValidNumValue<3>,
            comms::option::def::ValidNumValueRange<6, 7>
        >
    {
        using Base = 
            comms::field::EnumValue<
                ublox::field::FieldBase<>,
                ublox::message::CfgNav5FieldsCommon::UtcStandardVal,
                comms::option::def::ValidNumValue<0>,
                comms::option::def::ValidNumValue<3>,
                comms::option::def::ValidNumValueRange<6, 7>
            >;
    public:
        /// @brief Re-definition of the value type.
        using ValueType = typename Base::ValueType;
    
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::UtcStandardCommon::name();
        }
        
        /// @brief Retrieve name of the enum value
        static const char* valueName(ValueType val)
        {
            return ublox::message::CfgNav5FieldsCommon::UtcStandardCommon::valueName(val);
        }
        
        /// @brief Retrieve name of the @b current value
        const char* valueName() const
        {
            return valueName(Base::value());
        }
        
    };
    
    /// @brief Definition of <b>"reserved2"</b> field.
    struct Reserved2 : public
        ublox::field::Res5<
            TOpt
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::CfgNav5FieldsCommon::Reserved2Common::name();
        }
        
    };
    
    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        Mask,
        DynModel,
        FixMode,
        FixedAlt,
        FixedAltVar,
        MinElev,
        DrLimit,
        PDop,
        TDop,
        PAcc,
        TAcc,
        StaticHoldThresh,
        DgnssTimeout,
        CnoThreshNumSVs,
        CnoThresh,
        Reserved1,
        StaticHoldMaxDist,
        UtcStandard,
        Reserved2
    >;
};

/// @brief Definition of <b>"CFG-NAV5"</b> message class.
/// @details
///     See @ref CfgNav5Fields for definition of the fields this message contains.
/// @tparam TMsgBase Base (interface) class.
/// @tparam TOpt Extra options
/// @headerfile "ublox/message/CfgNav5.h"
template <typename TMsgBase, typename TOpt = ublox::options::DefaultOptions>
class CfgNav5 : public
    comms::MessageBase<
        TMsgBase,
        comms::option::def::StaticNumIdImpl<ublox::MsgId_CfgNav5>,
        comms::option::def::FieldsImpl<typename CfgNav5Fields<TOpt>::All>,
        comms::option::def::MsgType<CfgNav5<TMsgBase, TOpt> >,
        comms::option::def::HasName
    >
{
    // Redefinition of the base class type
    using Base =
        comms::MessageBase<
            TMsgBase,
            comms::option::def::StaticNumIdImpl<ublox::MsgId_CfgNav5>,
            comms::option::def::FieldsImpl<typename CfgNav5Fields<TOpt>::All>,
            comms::option::def::MsgType<CfgNav5<TMsgBase, TOpt> >,
            comms::option::def::HasName
        >;

public:
    /// @brief Provide names and allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    ///     The generated types and functions are:
    ///     @li @b Field_mask type and @b field_mask() access fuction
    ///         for @ref CfgNav5Fields::Mask field.
    ///     @li @b Field_dynModel type and @b field_dynModel() access fuction
    ///         for @ref CfgNav5Fields::DynModel field.
    ///     @li @b Field_fixMode type and @b field_fixMode() access fuction
    ///         for @ref CfgNav5Fields::FixMode field.
    ///     @li @b Field_fixedAlt type and @b field_fixedAlt() access fuction
    ///         for @ref CfgNav5Fields::FixedAlt field.
    ///     @li @b Field_fixedAltVar type and @b field_fixedAltVar() access fuction
    ///         for @ref CfgNav5Fields::FixedAltVar field.
    ///     @li @b Field_minElev type and @b field_minElev() access fuction
    ///         for @ref CfgNav5Fields::MinElev field.
    ///     @li @b Field_drLimit type and @b field_drLimit() access fuction
    ///         for @ref CfgNav5Fields::DrLimit field.
    ///     @li @b Field_pDop type and @b field_pDop() access fuction
    ///         for @ref CfgNav5Fields::PDop field.
    ///     @li @b Field_tDop type and @b field_tDop() access fuction
    ///         for @ref CfgNav5Fields::TDop field.
    ///     @li @b Field_pAcc type and @b field_pAcc() access fuction
    ///         for @ref CfgNav5Fields::PAcc field.
    ///     @li @b Field_tAcc type and @b field_tAcc() access fuction
    ///         for @ref CfgNav5Fields::TAcc field.
    ///     @li @b Field_staticHoldThresh type and @b field_staticHoldThresh() access fuction
    ///         for @ref CfgNav5Fields::StaticHoldThresh field.
    ///     @li @b Field_dgnssTimeout type and @b field_dgnssTimeout() access fuction
    ///         for @ref CfgNav5Fields::DgnssTimeout field.
    ///     @li @b Field_cnoThreshNumSVs type and @b field_cnoThreshNumSVs() access fuction
    ///         for @ref CfgNav5Fields::CnoThreshNumSVs field.
    ///     @li @b Field_cnoThresh type and @b field_cnoThresh() access fuction
    ///         for @ref CfgNav5Fields::CnoThresh field.
    ///     @li @b Field_reserved1 type and @b field_reserved1() access fuction
    ///         for @ref CfgNav5Fields::Reserved1 field.
    ///     @li @b Field_staticHoldMaxDist type and @b field_staticHoldMaxDist() access fuction
    ///         for @ref CfgNav5Fields::StaticHoldMaxDist field.
    ///     @li @b Field_utcStandard type and @b field_utcStandard() access fuction
    ///         for @ref CfgNav5Fields::UtcStandard field.
    ///     @li @b Field_reserved2 type and @b field_reserved2() access fuction
    ///         for @ref CfgNav5Fields::Reserved2 field.
    COMMS_MSG_FIELDS_NAMES(
        mask,
        dynModel,
        fixMode,
        fixedAlt,
        fixedAltVar,
        minElev,
        drLimit,
        pDop,
        tDop,
        pAcc,
        tAcc,
        staticHoldThresh,
        dgnssTimeout,
        cnoThreshNumSVs,
        cnoThresh,
        reserved1,
        staticHoldMaxDist,
        utcStandard,
        reserved2
    );
    
    // Compile time check for serialisation length.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static const std::size_t MsgMaxLen = Base::doMaxLength();
    static_assert(MsgMinLen == 36U, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 36U, "Unexpected max serialisation length");
    
    /// @brief Name of the message.
    static const char* doName()
    {
        return ublox::message::CfgNav5Common::name();
    }
    
    
};

} // namespace message

} // namespace ublox

