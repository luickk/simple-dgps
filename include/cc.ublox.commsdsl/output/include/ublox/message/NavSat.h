// Generated by commsdsl2comms v3.5.2

/// @file
/// @brief Contains definition of <b>"NAV-SAT"</b> message and its fields.

#pragma once

#include <cstdint>
#include <tuple>
#include <type_traits>
#include "comms/MessageBase.h"
#include "comms/field/ArrayList.h"
#include "comms/field/Bitfield.h"
#include "comms/field/BitmaskValue.h"
#include "comms/field/Bundle.h"
#include "comms/field/EnumValue.h"
#include "comms/field/IntValue.h"
#include "comms/options.h"
#include "ublox/MsgId.h"
#include "ublox/field/FieldBase.h"
#include "ublox/field/GnssId.h"
#include "ublox/field/Itow.h"
#include "ublox/field/Res2.h"
#include "ublox/message/NavSatCommon.h"
#include "ublox/options/DefaultOptions.h"

namespace ublox
{

namespace message
{

/// @brief Fields of @ref NavSat.
/// @tparam TOpt Extra options
/// @see @ref NavSat
/// @headerfile "ublox/message/NavSat.h"
template <typename TOpt = ublox::options::DefaultOptions>
struct NavSatFields
{
    /// @brief Definition of <b>"iTOW"</b> field.
    using Itow =
        ublox::field::Itow<
            TOpt
        >;
    
    /// @brief Definition of <b>"version"</b> field.
    struct Version : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t,
            comms::option::def::DefaultNumValue<1>,
            comms::option::def::ValidNumValue<1>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::NavSatFieldsCommon::VersionCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"numSvs"</b> field.
    struct NumSvs : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::NavSatFieldsCommon::NumSvsCommon::name();
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
            return ublox::message::NavSatFieldsCommon::Reserved1Common::name();
        }
        
    };
    
    /// @brief Scope for all the member fields of
    ///     @ref List list.
    struct ListMembers
    {
        /// @brief Scope for all the member fields of
        ///     @ref Element bundle.
        struct ElementMembers
        {
            /// @brief Definition of <b>"gnssId"</b> field.
            using GnssId =
                ublox::field::GnssId<
                    TOpt
                >;
            
            /// @brief Definition of <b>"svid"</b> field.
            struct Svid : public
                comms::field::IntValue<
                    ublox::field::FieldBase<>,
                    std::uint8_t
                >
            {
                /// @brief Name of the field.
                static const char* name()
                {
                    return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::SvidCommon::name();
                }
                
            };
            
            /// @brief Definition of <b>"cno"</b> field.
            struct Cno : public
                comms::field::IntValue<
                    ublox::field::FieldBase<>,
                    std::uint8_t
                >
            {
                /// @brief Name of the field.
                static const char* name()
                {
                    return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::CnoCommon::name();
                }
                
            };
            
            /// @brief Definition of <b>"elev"</b> field.
            struct Elev : public
                comms::field::IntValue<
                    ublox::field::FieldBase<>,
                    std::int8_t,
                    comms::option::def::UnitsDegrees,
                    comms::option::def::ValidNumValueRange<-90, 90>
                >
            {
                /// @brief Name of the field.
                static const char* name()
                {
                    return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::ElevCommon::name();
                }
                
            };
            
            /// @brief Definition of <b>"azim"</b> field.
            struct Azim : public
                comms::field::IntValue<
                    ublox::field::FieldBase<>,
                    std::int16_t,
                    comms::option::def::UnitsDegrees,
                    comms::option::def::ValidNumValueRange<0, 360>
                >
            {
                /// @brief Name of the field.
                static const char* name()
                {
                    return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::AzimCommon::name();
                }
                
            };
            
            /// @brief Definition of <b>"prRes"</b> field.
            struct PrRes : public
                comms::field::IntValue<
                    ublox::field::FieldBase<>,
                    std::int16_t,
                    comms::option::def::ScalingRatio<1, 10>,
                    comms::option::def::UnitsMeters
                >
            {
                /// @brief Name of the field.
                static const char* name()
                {
                    return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::PrResCommon::name();
                }
                
            };
            
            /// @brief Scope for all the member fields of
            ///     @ref Flags bitfield.
            struct FlagsMembers
            {
                /// @brief Definition of <b>"qualityInd"</b> field.
                /// @see @ref ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::QualityIndVal
                class QualityInd : public
                    comms::field::EnumValue<
                        ublox::field::FieldBase<>,
                        ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::QualityIndVal,
                        comms::option::def::FixedBitLength<3U>,
                        comms::option::def::ValidNumValueRange<0, 7>
                    >
                {
                    using Base = 
                        comms::field::EnumValue<
                            ublox::field::FieldBase<>,
                            ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::QualityIndVal,
                            comms::option::def::FixedBitLength<3U>,
                            comms::option::def::ValidNumValueRange<0, 7>
                        >;
                public:
                    /// @brief Re-definition of the value type.
                    using ValueType = typename Base::ValueType;
                
                    /// @brief Name of the field.
                    static const char* name()
                    {
                        return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::QualityIndCommon::name();
                    }
                    
                    /// @brief Retrieve name of the enum value
                    static const char* valueName(ValueType val)
                    {
                        return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::QualityIndCommon::valueName(val);
                    }
                    
                    /// @brief Retrieve name of the @b current value
                    const char* valueName() const
                    {
                        return valueName(Base::value());
                    }
                    
                };
                
                /// @brief Definition of <b>""</b> field.
                class BitsLow : public
                    comms::field::BitmaskValue<
                        ublox::field::FieldBase<>,
                        comms::option::def::FixedBitLength<1U>
                    >
                {
                    using Base = 
                        comms::field::BitmaskValue<
                            ublox::field::FieldBase<>,
                            comms::option::def::FixedBitLength<1U>
                        >;
                public:
                    /// @brief Provides names and generates access functions for internal bits.
                    /// @details See definition of @b COMMS_BITMASK_BITS_SEQ macro
                    ///     related to @b comms::field::BitmaskValue class from COMMS library
                    ///     for details.
                    ///
                    ///      The generated enum values and functions are:
                    ///      @li @b BitIdx_svUsed, @b getBitValue_svUsed() and @b setBitValue_svUsed().
                    COMMS_BITMASK_BITS_SEQ(
                        svUsed
                    );
                    
                    /// @brief Name of the field.
                    static const char* name()
                    {
                        return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::BitsLowCommon::name();
                    }
                    
                    /// @brief Retrieve name of the bit.
                    static const char* bitName(BitIdx idx)
                    {
                        return
                            ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::BitsLowCommon::bitName(
                                static_cast<std::size_t>(idx));
                    }
                    
                };
                
                /// @brief Definition of <b>"health"</b> field.
                /// @see @ref ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::HealthVal
                class Health : public
                    comms::field::EnumValue<
                        ublox::field::FieldBase<>,
                        ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::HealthVal,
                        comms::option::def::FixedBitLength<2U>,
                        comms::option::def::ValidNumValueRange<0, 2>
                    >
                {
                    using Base = 
                        comms::field::EnumValue<
                            ublox::field::FieldBase<>,
                            ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::HealthVal,
                            comms::option::def::FixedBitLength<2U>,
                            comms::option::def::ValidNumValueRange<0, 2>
                        >;
                public:
                    /// @brief Re-definition of the value type.
                    using ValueType = typename Base::ValueType;
                
                    /// @brief Name of the field.
                    static const char* name()
                    {
                        return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::HealthCommon::name();
                    }
                    
                    /// @brief Retrieve name of the enum value
                    static const char* valueName(ValueType val)
                    {
                        return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::HealthCommon::valueName(val);
                    }
                    
                    /// @brief Retrieve name of the @b current value
                    const char* valueName() const
                    {
                        return valueName(Base::value());
                    }
                    
                };
                
                /// @brief Definition of <b>""</b> field.
                class BitsMid : public
                    comms::field::BitmaskValue<
                        ublox::field::FieldBase<>,
                        comms::option::def::FixedBitLength<2U>
                    >
                {
                    using Base = 
                        comms::field::BitmaskValue<
                            ublox::field::FieldBase<>,
                            comms::option::def::FixedBitLength<2U>
                        >;
                public:
                    /// @brief Provides names and generates access functions for internal bits.
                    /// @details See definition of @b COMMS_BITMASK_BITS_SEQ macro
                    ///     related to @b comms::field::BitmaskValue class from COMMS library
                    ///     for details.
                    ///
                    ///      The generated enum values and functions are:
                    ///      @li @b BitIdx_diffCorr, @b getBitValue_diffCorr() and @b setBitValue_diffCorr().
                    ///      @li @b BitIdx_smoothed, @b getBitValue_smoothed() and @b setBitValue_smoothed().
                    COMMS_BITMASK_BITS_SEQ(
                        diffCorr,
                        smoothed
                    );
                    
                    /// @brief Name of the field.
                    static const char* name()
                    {
                        return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::BitsMidCommon::name();
                    }
                    
                    /// @brief Retrieve name of the bit.
                    static const char* bitName(BitIdx idx)
                    {
                        return
                            ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::BitsMidCommon::bitName(
                                static_cast<std::size_t>(idx));
                    }
                    
                };
                
                /// @brief Definition of <b>"orbitSource"</b> field.
                /// @see @ref ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::OrbitSourceVal
                class OrbitSource : public
                    comms::field::EnumValue<
                        ublox::field::FieldBase<>,
                        ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::OrbitSourceVal,
                        comms::option::def::FixedBitLength<3U>,
                        comms::option::def::ValidNumValueRange<0, 4>
                    >
                {
                    using Base = 
                        comms::field::EnumValue<
                            ublox::field::FieldBase<>,
                            ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::OrbitSourceVal,
                            comms::option::def::FixedBitLength<3U>,
                            comms::option::def::ValidNumValueRange<0, 4>
                        >;
                public:
                    /// @brief Re-definition of the value type.
                    using ValueType = typename Base::ValueType;
                
                    /// @brief Name of the field.
                    static const char* name()
                    {
                        return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::OrbitSourceCommon::name();
                    }
                    
                    /// @brief Retrieve name of the enum value
                    static const char* valueName(ValueType val)
                    {
                        return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::OrbitSourceCommon::valueName(val);
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
                        comms::option::def::BitmaskReservedBits<0x1FF190UL, 0x0U>
                    >
                {
                    using Base = 
                        comms::field::BitmaskValue<
                            ublox::field::FieldBase<>,
                            comms::option::def::FixedBitLength<21U>,
                            comms::option::def::BitmaskReservedBits<0x1FF190UL, 0x0U>
                        >;
                public:
                    /// @brief Provide names for internal bits.
                    /// @details See definition of @b COMMS_BITMASK_BITS macro
                    ///     related to @b comms::field::BitmaskValue class from COMMS library
                    ///     for details.
                    ///
                    ///      The generated enum values:
                    ///      @li @b BitIdx_ephAvail.
                    ///      @li @b BitIdx_almAvail.
                    ///      @li @b BitIdx_anoAvail.
                    ///      @li @b BitIdx_aopAvail.
                    ///      @li @b BitIdx_sbasCorrUsed.
                    ///      @li @b BitIdx_rtcmCorrUsed.
                    ///      @li @b BitIdx_prCorrUsed.
                    ///      @li @b BitIdx_crCorrUsed.
                    ///      @li @b BitIdx_doCorrUsed.
                    COMMS_BITMASK_BITS(
                        ephAvail=0,
                        almAvail=1,
                        anoAvail=2,
                        aopAvail=3,
                        sbasCorrUsed=5,
                        rtcmCorrUsed=6,
                        prCorrUsed=9,
                        crCorrUsed=10,
                        doCorrUsed=11
                    );
                    
                    /// @brief Generates independent access functions for internal bits.
                    /// @details See definition of @b COMMS_BITMASK_BITS_ACCESS macro
                    ///     related to @b comms::field::BitmaskValue class from COMMS library
                    ///     for details.
                    ///
                    ///     The generated access functions are:
                    ///      @li @b getBitValue_ephAvail() and @b setBitValue_ephAvail().
                    ///      @li @b getBitValue_almAvail() and @b setBitValue_almAvail().
                    ///      @li @b getBitValue_anoAvail() and @b setBitValue_anoAvail().
                    ///      @li @b getBitValue_aopAvail() and @b setBitValue_aopAvail().
                    ///      @li @b getBitValue_sbasCorrUsed() and @b setBitValue_sbasCorrUsed().
                    ///      @li @b getBitValue_rtcmCorrUsed() and @b setBitValue_rtcmCorrUsed().
                    ///      @li @b getBitValue_prCorrUsed() and @b setBitValue_prCorrUsed().
                    ///      @li @b getBitValue_crCorrUsed() and @b setBitValue_crCorrUsed().
                    ///      @li @b getBitValue_doCorrUsed() and @b setBitValue_doCorrUsed().
                    COMMS_BITMASK_BITS_ACCESS(
                        ephAvail,
                        almAvail,
                        anoAvail,
                        aopAvail,
                        sbasCorrUsed,
                        rtcmCorrUsed,
                        prCorrUsed,
                        crCorrUsed,
                        doCorrUsed
                    );
                    
                    /// @brief Name of the field.
                    static const char* name()
                    {
                        return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::BitsHighCommon::name();
                    }
                    
                    /// @brief Retrieve name of the bit.
                    static const char* bitName(BitIdx idx)
                    {
                        return
                            ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsMembersCommon::BitsHighCommon::bitName(
                                static_cast<std::size_t>(idx));
                    }
                    
                };
                
                /// @brief All members bundled in @b std::tuple.
                using All =
                    std::tuple<
                       QualityInd,
                       BitsLow,
                       Health,
                       BitsMid,
                       OrbitSource,
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
                ///     @li @b Field_qualityInd type and @b field_qualityInd() access function -
                ///         for FlagsMembers::QualityInd member field.
                ///     @li @b Field_bitsLow type and @b field_bitsLow() access function -
                ///         for FlagsMembers::BitsLow member field.
                ///     @li @b Field_health type and @b field_health() access function -
                ///         for FlagsMembers::Health member field.
                ///     @li @b Field_bitsMid type and @b field_bitsMid() access function -
                ///         for FlagsMembers::BitsMid member field.
                ///     @li @b Field_orbitSource type and @b field_orbitSource() access function -
                ///         for FlagsMembers::OrbitSource member field.
                ///     @li @b Field_bitsHigh type and @b field_bitsHigh() access function -
                ///         for FlagsMembers::BitsHigh member field.
                COMMS_FIELD_MEMBERS_NAMES(
                    qualityInd,
                    bitsLow,
                    health,
                    bitsMid,
                    orbitSource,
                    bitsHigh
                );
                
                /// @brief Name of the field.
                static const char* name()
                {
                    return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementMembersCommon::FlagsCommon::name();
                }
                
            };
            
            /// @brief All members bundled in @b std::tuple.
            using All =
                std::tuple<
                   GnssId,
                   Svid,
                   Cno,
                   Elev,
                   Azim,
                   PrRes,
                   Flags
                >;
        };
        
        /// @brief Definition of <b>""</b> field.
        class Element : public
            comms::field::Bundle<
                ublox::field::FieldBase<>,
                typename ElementMembers::All
            >
        {
            using Base = 
                comms::field::Bundle<
                    ublox::field::FieldBase<>,
                    typename ElementMembers::All
                >;
        public:
            /// @brief Allow access to internal fields.
            /// @details See definition of @b COMMS_FIELD_MEMBERS_NAMES macro
            ///     related to @b comms::field::Bundle class from COMMS library
            ///     for details.
            ///
            ///     The generated access types and functions functions are:
            ///     @li @b Field_gnssIdtype and @b field_gnssId() access function -
            ///         for ElementMembers::GnssId member field.
            ///     @li @b Field_svidtype and @b field_svid() access function -
            ///         for ElementMembers::Svid member field.
            ///     @li @b Field_cnotype and @b field_cno() access function -
            ///         for ElementMembers::Cno member field.
            ///     @li @b Field_elevtype and @b field_elev() access function -
            ///         for ElementMembers::Elev member field.
            ///     @li @b Field_azimtype and @b field_azim() access function -
            ///         for ElementMembers::Azim member field.
            ///     @li @b Field_prRestype and @b field_prRes() access function -
            ///         for ElementMembers::PrRes member field.
            ///     @li @b Field_flagstype and @b field_flags() access function -
            ///         for ElementMembers::Flags member field.
            COMMS_FIELD_MEMBERS_NAMES(
                gnssId,
                svid,
                cno,
                elev,
                azim,
                prRes,
                flags
            );
            
            /// @brief Name of the field.
            static const char* name()
            {
                return ublox::message::NavSatFieldsCommon::ListMembersCommon::ElementCommon::name();
            }
            
        };
        
    };
    
    /// @brief Definition of <b>"list"</b> field.
    struct List : public
        comms::field::ArrayList<
            ublox::field::FieldBase<>,
            typename ListMembers::Element,
            typename TOpt::message::NavSatFields::List,
            comms::option::def::SequenceSizeForcingEnabled
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::NavSatFieldsCommon::ListCommon::name();
        }
        
    };
    
    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        Itow,
        Version,
        NumSvs,
        Reserved1,
        List
    >;
};

/// @brief Definition of <b>"NAV-SAT"</b> message class.
/// @details
///     See @ref NavSatFields for definition of the fields this message contains.
/// @tparam TMsgBase Base (interface) class.
/// @tparam TOpt Extra options
/// @headerfile "ublox/message/NavSat.h"
template <typename TMsgBase, typename TOpt = ublox::options::DefaultOptions>
class NavSat : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::NavSat,
        comms::option::def::StaticNumIdImpl<ublox::MsgId_NavSat>,
        comms::option::def::FieldsImpl<typename NavSatFields<TOpt>::All>,
        comms::option::def::MsgType<NavSat<TMsgBase, TOpt> >,
        comms::option::def::HasName,
        comms::option::def::HasCustomRefresh
    >
{
    // Redefinition of the base class type
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::NavSat,
            comms::option::def::StaticNumIdImpl<ublox::MsgId_NavSat>,
            comms::option::def::FieldsImpl<typename NavSatFields<TOpt>::All>,
            comms::option::def::MsgType<NavSat<TMsgBase, TOpt> >,
            comms::option::def::HasName,
            comms::option::def::HasCustomRefresh
        >;

public:
    /// @brief Provide names and allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    ///     The generated types and functions are:
    ///     @li @b Field_itow type and @b field_itow() access fuction
    ///         for @ref NavSatFields::Itow field.
    ///     @li @b Field_version type and @b field_version() access fuction
    ///         for @ref NavSatFields::Version field.
    ///     @li @b Field_numSvs type and @b field_numSvs() access fuction
    ///         for @ref NavSatFields::NumSvs field.
    ///     @li @b Field_reserved1 type and @b field_reserved1() access fuction
    ///         for @ref NavSatFields::Reserved1 field.
    ///     @li @b Field_list type and @b field_list() access fuction
    ///         for @ref NavSatFields::List field.
    COMMS_MSG_FIELDS_NAMES(
        itow,
        version,
        numSvs,
        reserved1,
        list
    );
    
    // Compile time check for serialisation length.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static_assert(MsgMinLen == 8U, "Unexpected min serialisation length");
    
    /// @brief Name of the message.
    static const char* doName()
    {
        return ublox::message::NavSatCommon::name();
    }
    
    /// @brief Custom read functionality.
    template <typename TIter>
    comms::ErrorStatus doRead(TIter& iter, std::size_t len)
    {
        auto es = Base::template doReadUntilAndUpdateLen<FieldIdx_list>(iter, len);
        if (es != comms::ErrorStatus::Success) {
            return es;
        }
        
        field_list().forceReadElemCount(
            static_cast<std::size_t>(field_numSvs().value()));
        
        es = Base::template doReadFrom<FieldIdx_list>(iter, len);
        if (es != comms::ErrorStatus::Success) {
            return es;
        }
        
        return comms::ErrorStatus::Success;
    }
    
    /// @brief Custom refresh functionality.
    bool doRefresh()
    {
        bool updated = Base::doRefresh();
        updated = refresh_list() || updated;
        return updated;
    }
    
    
private:
    bool refresh_list()
    {
        bool updated = false;
        do {
            auto expectedValue = static_cast<std::size_t>(field_numSvs().value());
            auto realValue = field_list().value().size();
            if (expectedValue != realValue) {
                using PrefixValueType = typename std::decay<decltype(field_numSvs().value())>::type;
                field_numSvs().value() = static_cast<PrefixValueType>(realValue);
                updated = true;
            }
        } while (false);
        
        return updated;
        
    }
    
};

} // namespace message

} // namespace ublox

