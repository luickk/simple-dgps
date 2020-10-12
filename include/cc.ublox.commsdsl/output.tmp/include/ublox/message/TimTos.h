// Generated by commsdsl2comms v3.5.2

/// @file
/// @brief Contains definition of <b>"TIM-TOS"</b> message and its fields.

#pragma once

#include <cstdint>
#include <tuple>
#include <type_traits>
#include "comms/MessageBase.h"
#include "comms/field/Bitfield.h"
#include "comms/field/BitmaskValue.h"
#include "comms/field/EnumValue.h"
#include "comms/field/IntValue.h"
#include "comms/options.h"
#include "ublox/MsgId.h"
#include "ublox/field/Day.h"
#include "ublox/field/FieldBase.h"
#include "ublox/field/GnssId.h"
#include "ublox/field/Hour.h"
#include "ublox/field/Min.h"
#include "ublox/field/Month.h"
#include "ublox/field/Res2.h"
#include "ublox/field/Sec.h"
#include "ublox/field/Year.h"
#include "ublox/message/TimTosCommon.h"
#include "ublox/options/DefaultOptions.h"

namespace ublox
{

namespace message
{

/// @brief Fields of @ref TimTos.
/// @tparam TOpt Extra options
/// @see @ref TimTos
/// @headerfile "ublox/message/TimTos.h"
template <typename TOpt = ublox::options::DefaultOptions>
struct TimTosFields
{
    /// @brief Definition of <b>"version"</b> field.
    struct Version : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t,
            comms::option::def::ValidNumValue<0>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::VersionCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"gnssId"</b> field.
    using GnssId =
        ublox::field::GnssId<
            TOpt
        >;
    
    /// @brief Definition of <b>"reserved1"</b> field.
    struct Reserved1 : public
        ublox::field::Res2<
            TOpt
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::Reserved1Common::name();
        }
        
    };
    
    /// @brief Scope for all the member fields of
    ///     @ref Flags bitfield.
    struct FlagsMembers
    {
        /// @brief Definition of <b>""</b> field.
        class BitsLow : public
            comms::field::BitmaskValue<
                ublox::field::FieldBase<>,
                comms::option::def::FixedBitLength<8U>
            >
        {
            using Base = 
                comms::field::BitmaskValue<
                    ublox::field::FieldBase<>,
                    comms::option::def::FixedBitLength<8U>
                >;
        public:
            /// @brief Provides names and generates access functions for internal bits.
            /// @details See definition of @b COMMS_BITMASK_BITS_SEQ macro
            ///     related to @b comms::field::BitmaskValue class from COMMS library
            ///     for details.
            ///
            ///      The generated enum values and functions are:
            ///      @li @b BitIdx_leapNow, @b getBitValue_leapNow() and @b setBitValue_leapNow().
            ///      @li @b BitIdx_leapSoon, @b getBitValue_leapSoon() and @b setBitValue_leapSoon().
            ///      @li @b BitIdx_leapPositive, @b getBitValue_leapPositive() and @b setBitValue_leapPositive().
            ///      @li @b BitIdx_timeInLimit, @b getBitValue_timeInLimit() and @b setBitValue_timeInLimit().
            ///      @li @b BitIdx_intOscInLimit, @b getBitValue_intOscInLimit() and @b setBitValue_intOscInLimit().
            ///      @li @b BitIdx_extOscInLimit, @b getBitValue_extOscInLimit() and @b setBitValue_extOscInLimit().
            ///      @li @b BitIdx_gnssTimeValid, @b getBitValue_gnssTimeValid() and @b setBitValue_gnssTimeValid().
            ///      @li @b BitIdx_UTCTimeValid, @b getBitValue_UTCTimeValid() and @b setBitValue_UTCTimeValid().
            COMMS_BITMASK_BITS_SEQ(
                leapNow,
                leapSoon,
                leapPositive,
                timeInLimit,
                intOscInLimit,
                extOscInLimit,
                gnssTimeValid,
                UTCTimeValid
            );
            
            /// @brief Name of the field.
            static const char* name()
            {
                return ublox::message::TimTosFieldsCommon::FlagsMembersCommon::BitsLowCommon::name();
            }
            
            /// @brief Retrieve name of the bit.
            static const char* bitName(BitIdx idx)
            {
                return
                    ublox::message::TimTosFieldsCommon::FlagsMembersCommon::BitsLowCommon::bitName(
                        static_cast<std::size_t>(idx));
            }
            
        };
        
        /// @brief Definition of <b>"DiscSrc"</b> field.
        /// @see @ref ublox::message::TimTosFieldsCommon::FlagsMembersCommon::DiscSrcVal
        class DiscSrc : public
            comms::field::EnumValue<
                ublox::field::FieldBase<>,
                ublox::message::TimTosFieldsCommon::FlagsMembersCommon::DiscSrcVal,
                comms::option::def::FixedBitLength<3U>,
                comms::option::def::ValidNumValueRange<0, 5>
            >
        {
            using Base = 
                comms::field::EnumValue<
                    ublox::field::FieldBase<>,
                    ublox::message::TimTosFieldsCommon::FlagsMembersCommon::DiscSrcVal,
                    comms::option::def::FixedBitLength<3U>,
                    comms::option::def::ValidNumValueRange<0, 5>
                >;
        public:
            /// @brief Re-definition of the value type.
            using ValueType = typename Base::ValueType;
        
            /// @brief Name of the field.
            static const char* name()
            {
                return ublox::message::TimTosFieldsCommon::FlagsMembersCommon::DiscSrcCommon::name();
            }
            
            /// @brief Retrieve name of the enum value
            static const char* valueName(ValueType val)
            {
                return ublox::message::TimTosFieldsCommon::FlagsMembersCommon::DiscSrcCommon::valueName(val);
            }
            
            /// @brief Retrieve name of the @b current value
            const char* valueName() const
            {
                return valueName(Base::value());
            }
            
        };
        
        /// @brief Definition of <b>""</b> field.
        class BitsHigh : public
            comms::field::BitmaskValue<
                ublox::field::FieldBase<>,
                comms::option::def::FixedBitLength<21U>,
                comms::option::def::BitmaskReservedBits<0x1FFFF8UL, 0x0U>
            >
        {
            using Base = 
                comms::field::BitmaskValue<
                    ublox::field::FieldBase<>,
                    comms::option::def::FixedBitLength<21U>,
                    comms::option::def::BitmaskReservedBits<0x1FFFF8UL, 0x0U>
                >;
        public:
            /// @brief Provides names and generates access functions for internal bits.
            /// @details See definition of @b COMMS_BITMASK_BITS_SEQ macro
            ///     related to @b comms::field::BitmaskValue class from COMMS library
            ///     for details.
            ///
            ///      The generated enum values and functions are:
            ///      @li @b BitIdx_raim, @b getBitValue_raim() and @b setBitValue_raim().
            ///      @li @b BitIdx_cohPulse, @b getBitValue_cohPulse() and @b setBitValue_cohPulse().
            ///      @li @b BitIdx_lockedPulse, @b getBitValue_lockedPulse() and @b setBitValue_lockedPulse().
            COMMS_BITMASK_BITS_SEQ(
                raim,
                cohPulse,
                lockedPulse
            );
            
            /// @brief Name of the field.
            static const char* name()
            {
                return ublox::message::TimTosFieldsCommon::FlagsMembersCommon::BitsHighCommon::name();
            }
            
            /// @brief Retrieve name of the bit.
            static const char* bitName(BitIdx idx)
            {
                return
                    ublox::message::TimTosFieldsCommon::FlagsMembersCommon::BitsHighCommon::bitName(
                        static_cast<std::size_t>(idx));
            }
            
        };
        
        /// @brief All members bundled in @b std::tuple.
        using All =
            std::tuple<
               BitsLow,
               DiscSrc,
               BitsHigh
            >;
    };
    
    /// @brief Definition of <b>"flags"</b> field.
    class Flags : public
        comms::field::Bitfield<
            ublox::field::FieldBase<>,
            typename FlagsMembers::All
        >
    {
        using Base = 
            comms::field::Bitfield<
                ublox::field::FieldBase<>,
                typename FlagsMembers::All
            >;
    public:
        /// @brief Allow access to internal fields.
        /// @details See definition of @b COMMS_FIELD_MEMBERS_NAMES macro
        ///     related to @b comms::field::Bitfield class from COMMS library
        ///     for details.
        ///
        ///     The generated types and access functions are:
        ///     @li @b Field_bitsLow type and @b field_bitsLow() access function -
        ///         for FlagsMembers::BitsLow member field.
        ///     @li @b Field_discSrc type and @b field_discSrc() access function -
        ///         for FlagsMembers::DiscSrc member field.
        ///     @li @b Field_bitsHigh type and @b field_bitsHigh() access function -
        ///         for FlagsMembers::BitsHigh member field.
        COMMS_FIELD_MEMBERS_NAMES(
            bitsLow,
            discSrc,
            bitsHigh
        );
        
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::FlagsCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"year"</b> field.
    using Year =
        ublox::field::Year<
            TOpt
        >;
    
    /// @brief Definition of <b>"month"</b> field.
    using Month =
        ublox::field::Month<
            TOpt
        >;
    
    /// @brief Definition of <b>"day"</b> field.
    using Day =
        ublox::field::Day<
            TOpt
        >;
    
    /// @brief Definition of <b>"hour"</b> field.
    using Hour =
        ublox::field::Hour<
            TOpt
        >;
    
    /// @brief Definition of <b>"min"</b> field.
    using Min =
        ublox::field::Min<
            TOpt
        >;
    
    /// @brief Definition of <b>"sec"</b> field.
    using Sec =
        ublox::field::Sec<
            TOpt
        >;
    
    /// @brief Definition of <b>"utcStandard"</b> field.
    /// @see @ref ublox::message::TimTosFieldsCommon::UtcStandardVal
    class UtcStandard : public
        comms::field::EnumValue<
            ublox::field::FieldBase<>,
            ublox::message::TimTosFieldsCommon::UtcStandardVal,
            comms::option::def::ValidNumValue<0>,
            comms::option::def::ValidNumValue<3>,
            comms::option::def::ValidNumValueRange<6, 7>
        >
    {
        using Base = 
            comms::field::EnumValue<
                ublox::field::FieldBase<>,
                ublox::message::TimTosFieldsCommon::UtcStandardVal,
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
            return ublox::message::TimTosFieldsCommon::UtcStandardCommon::name();
        }
        
        /// @brief Retrieve name of the enum value
        static const char* valueName(ValueType val)
        {
            return ublox::message::TimTosFieldsCommon::UtcStandardCommon::valueName(val);
        }
        
        /// @brief Retrieve name of the @b current value
        const char* valueName() const
        {
            return valueName(Base::value());
        }
        
    };
    
    /// @brief Definition of <b>"utcOffset"</b> field.
    struct UtcOffset : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::int32_t,
            comms::option::def::UnitsNanoseconds
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::UtcOffsetCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"utcUncertainty"</b> field.
    struct UtcUncertainty : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint32_t,
            comms::option::def::UnitsNanoseconds
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::UtcUncertaintyCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"week"</b> field.
    struct Week : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint32_t,
            comms::option::def::UnitsWeeks
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::WeekCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"TOW"</b> field.
    struct TOW : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint32_t,
            comms::option::def::UnitsSeconds
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::TOWCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"gnssOffset"</b> field.
    struct GnssOffset : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::int32_t,
            comms::option::def::UnitsNanoseconds
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::GnssOffsetCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"gnssUncertainy"</b> field.
    struct GnssUncertainy : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint32_t,
            comms::option::def::UnitsNanoseconds
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::GnssUncertainyCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"intOscOffset"</b> field.
    struct IntOscOffset : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::int32_t,
            comms::option::def::ScalingRatio<1, 256>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::IntOscOffsetCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"intOscUncertainty"</b> field.
    struct IntOscUncertainty : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint32_t,
            comms::option::def::ScalingRatio<1, 256>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::IntOscUncertaintyCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"extOscOffset"</b> field.
    struct ExtOscOffset : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::int32_t,
            comms::option::def::ScalingRatio<1, 256>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::ExtOscOffsetCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"extOscUncertainty"</b> field.
    struct ExtOscUncertainty : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint32_t,
            comms::option::def::ScalingRatio<1, 256>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::TimTosFieldsCommon::ExtOscUncertaintyCommon::name();
        }
        
    };
    
    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        Version,
        GnssId,
        Reserved1,
        Flags,
        Year,
        Month,
        Day,
        Hour,
        Min,
        Sec,
        UtcStandard,
        UtcOffset,
        UtcUncertainty,
        Week,
        TOW,
        GnssOffset,
        GnssUncertainy,
        IntOscOffset,
        IntOscUncertainty,
        ExtOscOffset,
        ExtOscUncertainty
    >;
};

/// @brief Definition of <b>"TIM-TOS"</b> message class.
/// @details
///     See @ref TimTosFields for definition of the fields this message contains.
/// @tparam TMsgBase Base (interface) class.
/// @tparam TOpt Extra options
/// @headerfile "ublox/message/TimTos.h"
template <typename TMsgBase, typename TOpt = ublox::options::DefaultOptions>
class TimTos : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::TimTos,
        comms::option::def::StaticNumIdImpl<ublox::MsgId_TimTos>,
        comms::option::def::FieldsImpl<typename TimTosFields<TOpt>::All>,
        comms::option::def::MsgType<TimTos<TMsgBase, TOpt> >,
        comms::option::def::HasName
    >
{
    // Redefinition of the base class type
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::TimTos,
            comms::option::def::StaticNumIdImpl<ublox::MsgId_TimTos>,
            comms::option::def::FieldsImpl<typename TimTosFields<TOpt>::All>,
            comms::option::def::MsgType<TimTos<TMsgBase, TOpt> >,
            comms::option::def::HasName
        >;

public:
    /// @brief Provide names and allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    ///     The generated types and functions are:
    ///     @li @b Field_version type and @b field_version() access fuction
    ///         for @ref TimTosFields::Version field.
    ///     @li @b Field_gnssId type and @b field_gnssId() access fuction
    ///         for @ref TimTosFields::GnssId field.
    ///     @li @b Field_reserved1 type and @b field_reserved1() access fuction
    ///         for @ref TimTosFields::Reserved1 field.
    ///     @li @b Field_flags type and @b field_flags() access fuction
    ///         for @ref TimTosFields::Flags field.
    ///     @li @b Field_year type and @b field_year() access fuction
    ///         for @ref TimTosFields::Year field.
    ///     @li @b Field_month type and @b field_month() access fuction
    ///         for @ref TimTosFields::Month field.
    ///     @li @b Field_day type and @b field_day() access fuction
    ///         for @ref TimTosFields::Day field.
    ///     @li @b Field_hour type and @b field_hour() access fuction
    ///         for @ref TimTosFields::Hour field.
    ///     @li @b Field_min type and @b field_min() access fuction
    ///         for @ref TimTosFields::Min field.
    ///     @li @b Field_sec type and @b field_sec() access fuction
    ///         for @ref TimTosFields::Sec field.
    ///     @li @b Field_utcStandard type and @b field_utcStandard() access fuction
    ///         for @ref TimTosFields::UtcStandard field.
    ///     @li @b Field_utcOffset type and @b field_utcOffset() access fuction
    ///         for @ref TimTosFields::UtcOffset field.
    ///     @li @b Field_utcUncertainty type and @b field_utcUncertainty() access fuction
    ///         for @ref TimTosFields::UtcUncertainty field.
    ///     @li @b Field_week type and @b field_week() access fuction
    ///         for @ref TimTosFields::Week field.
    ///     @li @b Field_tOW type and @b field_tOW() access fuction
    ///         for @ref TimTosFields::TOW field.
    ///     @li @b Field_gnssOffset type and @b field_gnssOffset() access fuction
    ///         for @ref TimTosFields::GnssOffset field.
    ///     @li @b Field_gnssUncertainy type and @b field_gnssUncertainy() access fuction
    ///         for @ref TimTosFields::GnssUncertainy field.
    ///     @li @b Field_intOscOffset type and @b field_intOscOffset() access fuction
    ///         for @ref TimTosFields::IntOscOffset field.
    ///     @li @b Field_intOscUncertainty type and @b field_intOscUncertainty() access fuction
    ///         for @ref TimTosFields::IntOscUncertainty field.
    ///     @li @b Field_extOscOffset type and @b field_extOscOffset() access fuction
    ///         for @ref TimTosFields::ExtOscOffset field.
    ///     @li @b Field_extOscUncertainty type and @b field_extOscUncertainty() access fuction
    ///         for @ref TimTosFields::ExtOscUncertainty field.
    COMMS_MSG_FIELDS_NAMES(
        version,
        gnssId,
        reserved1,
        flags,
        year,
        month,
        day,
        hour,
        min,
        sec,
        utcStandard,
        utcOffset,
        utcUncertainty,
        week,
        tOW,
        gnssOffset,
        gnssUncertainy,
        intOscOffset,
        intOscUncertainty,
        extOscOffset,
        extOscUncertainty
    );
    
    // Compile time check for serialisation length.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static const std::size_t MsgMaxLen = Base::doMaxLength();
    static_assert(MsgMinLen == 56U, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 56U, "Unexpected max serialisation length");
    
    /// @brief Name of the message.
    static const char* doName()
    {
        return ublox::message::TimTosCommon::name();
    }
    
    
};

} // namespace message

} // namespace ublox

