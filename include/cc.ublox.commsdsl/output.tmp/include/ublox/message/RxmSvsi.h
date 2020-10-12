// Generated by commsdsl2comms v3.5.2

/// @file
/// @brief Contains definition of <b>"RXM-SVSI"</b> message and its fields.

#pragma once

#include <cstdint>
#include <tuple>
#include "comms/MessageBase.h"
#include "comms/field/ArrayList.h"
#include "comms/field/Bitfield.h"
#include "comms/field/BitmaskValue.h"
#include "comms/field/Bundle.h"
#include "comms/field/IntValue.h"
#include "comms/options.h"
#include "ublox/MsgId.h"
#include "ublox/field/FieldBase.h"
#include "ublox/field/Itow.h"
#include "ublox/message/RxmSvsiCommon.h"
#include "ublox/options/DefaultOptions.h"

namespace ublox
{

namespace message
{

/// @brief Fields of @ref RxmSvsi.
/// @tparam TOpt Extra options
/// @see @ref RxmSvsi
/// @headerfile "ublox/message/RxmSvsi.h"
template <typename TOpt = ublox::options::DefaultOptions>
struct RxmSvsiFields
{
    /// @brief Definition of <b>"iTOW"</b> field.
    using Itow =
        ublox::field::Itow<
            TOpt
        >;
    
    /// @brief Definition of <b>"week"</b> field.
    struct Week : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::int16_t,
            comms::option::def::UnitsWeeks
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::RxmSvsiFieldsCommon::WeekCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"numVis"</b> field.
    struct NumVis : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::RxmSvsiFieldsCommon::NumVisCommon::name();
        }
        
    };
    
    /// @brief Definition of <b>"numSV"</b> field.
    struct NumSV : public
        comms::field::IntValue<
            ublox::field::FieldBase<>,
            std::uint8_t
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::RxmSvsiFieldsCommon::NumSVCommon::name();
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
                    return ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementMembersCommon::SvidCommon::name();
                }
                
            };
            
            /// @brief Scope for all the member fields of
            ///     @ref SvFlag bitfield.
            struct SvFlagMembers
            {
                /// @brief Definition of <b>"ura"</b> field.
                struct Ura : public
                    comms::field::IntValue<
                        ublox::field::FieldBase<>,
                        std::uint8_t,
                        comms::option::def::FixedBitLength<4U>
                    >
                {
                    /// @brief Name of the field.
                    static const char* name()
                    {
                        return ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementMembersCommon::SvFlagMembersCommon::UraCommon::name();
                    }
                    
                };
                
                /// @brief Definition of <b>""</b> field.
                class Bits : public
                    comms::field::BitmaskValue<
                        ublox::field::FieldBase<>,
                        comms::option::def::FixedBitLength<4U>
                    >
                {
                    using Base = 
                        comms::field::BitmaskValue<
                            ublox::field::FieldBase<>,
                            comms::option::def::FixedBitLength<4U>
                        >;
                public:
                    /// @brief Provides names and generates access functions for internal bits.
                    /// @details See definition of @b COMMS_BITMASK_BITS_SEQ macro
                    ///     related to @b comms::field::BitmaskValue class from COMMS library
                    ///     for details.
                    ///
                    ///      The generated enum values and functions are:
                    ///      @li @b BitIdx_healthy, @b getBitValue_healthy() and @b setBitValue_healthy().
                    ///      @li @b BitIdx_ephVal, @b getBitValue_ephVal() and @b setBitValue_ephVal().
                    ///      @li @b BitIdx_almVal, @b getBitValue_almVal() and @b setBitValue_almVal().
                    ///      @li @b BitIdx_notAvail, @b getBitValue_notAvail() and @b setBitValue_notAvail().
                    COMMS_BITMASK_BITS_SEQ(
                        healthy,
                        ephVal,
                        almVal,
                        notAvail
                    );
                    
                    /// @brief Name of the field.
                    static const char* name()
                    {
                        return ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementMembersCommon::SvFlagMembersCommon::BitsCommon::name();
                    }
                    
                    /// @brief Retrieve name of the bit.
                    static const char* bitName(BitIdx idx)
                    {
                        return
                            ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementMembersCommon::SvFlagMembersCommon::BitsCommon::bitName(
                                static_cast<std::size_t>(idx));
                    }
                    
                };
                
                /// @brief All members bundled in @b std::tuple.
                using All =
                    std::tuple<
                       Ura,
                       Bits
                    >;
            };
            
            /// @brief Definition of <b>"svFlag"</b> field.
            class SvFlag : public
                comms::field::Bitfield<
                    ublox::field::FieldBase<>,
                    typename SvFlagMembers::All
                >
            {
                using Base = 
                    comms::field::Bitfield<
                        ublox::field::FieldBase<>,
                        typename SvFlagMembers::All
                    >;
            public:
                /// @brief Allow access to internal fields.
                /// @details See definition of @b COMMS_FIELD_MEMBERS_NAMES macro
                ///     related to @b comms::field::Bitfield class from COMMS library
                ///     for details.
                ///
                ///     The generated types and access functions are:
                ///     @li @b Field_ura type and @b field_ura() access function -
                ///         for SvFlagMembers::Ura member field.
                ///     @li @b Field_bits type and @b field_bits() access function -
                ///         for SvFlagMembers::Bits member field.
                COMMS_FIELD_MEMBERS_NAMES(
                    ura,
                    bits
                );
                
                /// @brief Name of the field.
                static const char* name()
                {
                    return ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementMembersCommon::SvFlagCommon::name();
                }
                
            };
            
            /// @brief Definition of <b>"azim"</b> field.
            struct Azim : public
                comms::field::IntValue<
                    ublox::field::FieldBase<>,
                    std::int16_t
                >
            {
                /// @brief Name of the field.
                static const char* name()
                {
                    return ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementMembersCommon::AzimCommon::name();
                }
                
            };
            
            /// @brief Definition of <b>"elev"</b> field.
            struct Elev : public
                comms::field::IntValue<
                    ublox::field::FieldBase<>,
                    std::int8_t
                >
            {
                /// @brief Name of the field.
                static const char* name()
                {
                    return ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementMembersCommon::ElevCommon::name();
                }
                
            };
            
            /// @brief Scope for all the member fields of
            ///     @ref Age bitfield.
            struct AgeMembers
            {
                /// @brief Definition of <b>"almAge"</b> field.
                struct AlmAge : public
                    comms::field::IntValue<
                        ublox::field::FieldBase<>,
                        std::uint8_t,
                        comms::option::def::FixedBitLength<4U>
                    >
                {
                    /// @brief Name of the field.
                    static const char* name()
                    {
                        return ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementMembersCommon::AgeMembersCommon::AlmAgeCommon::name();
                    }
                    
                };
                
                /// @brief Definition of <b>"ephAge"</b> field.
                struct EphAge : public
                    comms::field::IntValue<
                        ublox::field::FieldBase<>,
                        std::uint8_t,
                        comms::option::def::FixedBitLength<4U>
                    >
                {
                    /// @brief Name of the field.
                    static const char* name()
                    {
                        return ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementMembersCommon::AgeMembersCommon::EphAgeCommon::name();
                    }
                    
                };
                
                /// @brief All members bundled in @b std::tuple.
                using All =
                    std::tuple<
                       AlmAge,
                       EphAge
                    >;
            };
            
            /// @brief Definition of <b>"age"</b> field.
            class Age : public
                comms::field::Bitfield<
                    ublox::field::FieldBase<>,
                    typename AgeMembers::All
                >
            {
                using Base = 
                    comms::field::Bitfield<
                        ublox::field::FieldBase<>,
                        typename AgeMembers::All
                    >;
            public:
                /// @brief Allow access to internal fields.
                /// @details See definition of @b COMMS_FIELD_MEMBERS_NAMES macro
                ///     related to @b comms::field::Bitfield class from COMMS library
                ///     for details.
                ///
                ///     The generated types and access functions are:
                ///     @li @b Field_almAge type and @b field_almAge() access function -
                ///         for AgeMembers::AlmAge member field.
                ///     @li @b Field_ephAge type and @b field_ephAge() access function -
                ///         for AgeMembers::EphAge member field.
                COMMS_FIELD_MEMBERS_NAMES(
                    almAge,
                    ephAge
                );
                
                /// @brief Name of the field.
                static const char* name()
                {
                    return ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementMembersCommon::AgeCommon::name();
                }
                
            };
            
            /// @brief All members bundled in @b std::tuple.
            using All =
                std::tuple<
                   Svid,
                   SvFlag,
                   Azim,
                   Elev,
                   Age
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
            ///     @li @b Field_svidtype and @b field_svid() access function -
            ///         for ElementMembers::Svid member field.
            ///     @li @b Field_svFlagtype and @b field_svFlag() access function -
            ///         for ElementMembers::SvFlag member field.
            ///     @li @b Field_azimtype and @b field_azim() access function -
            ///         for ElementMembers::Azim member field.
            ///     @li @b Field_elevtype and @b field_elev() access function -
            ///         for ElementMembers::Elev member field.
            ///     @li @b Field_agetype and @b field_age() access function -
            ///         for ElementMembers::Age member field.
            COMMS_FIELD_MEMBERS_NAMES(
                svid,
                svFlag,
                azim,
                elev,
                age
            );
            
            /// @brief Name of the field.
            static const char* name()
            {
                return ublox::message::RxmSvsiFieldsCommon::ListMembersCommon::ElementCommon::name();
            }
            
        };
        
    };
    
    /// @brief Definition of <b>"list"</b> field.
    struct List : public
        comms::field::ArrayList<
            ublox::field::FieldBase<>,
            typename ListMembers::Element,
            typename TOpt::message::RxmSvsiFields::List,
            comms::option::def::SequenceSizeForcingEnabled
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::RxmSvsiFieldsCommon::ListCommon::name();
        }
        
    };
    
    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        Itow,
        Week,
        NumVis,
        NumSV,
        List
    >;
};

/// @brief Definition of <b>"RXM-SVSI"</b> message class.
/// @details
///     See @ref RxmSvsiFields for definition of the fields this message contains.
/// @tparam TMsgBase Base (interface) class.
/// @tparam TOpt Extra options
/// @headerfile "ublox/message/RxmSvsi.h"
template <typename TMsgBase, typename TOpt = ublox::options::DefaultOptions>
class RxmSvsi : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::RxmSvsi,
        comms::option::def::StaticNumIdImpl<ublox::MsgId_RxmSvsi>,
        comms::option::def::FieldsImpl<typename RxmSvsiFields<TOpt>::All>,
        comms::option::def::MsgType<RxmSvsi<TMsgBase, TOpt> >,
        comms::option::def::HasName,
        comms::option::def::HasCustomRefresh
    >
{
    // Redefinition of the base class type
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::RxmSvsi,
            comms::option::def::StaticNumIdImpl<ublox::MsgId_RxmSvsi>,
            comms::option::def::FieldsImpl<typename RxmSvsiFields<TOpt>::All>,
            comms::option::def::MsgType<RxmSvsi<TMsgBase, TOpt> >,
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
    ///         for @ref RxmSvsiFields::Itow field.
    ///     @li @b Field_week type and @b field_week() access fuction
    ///         for @ref RxmSvsiFields::Week field.
    ///     @li @b Field_numVis type and @b field_numVis() access fuction
    ///         for @ref RxmSvsiFields::NumVis field.
    ///     @li @b Field_numSV type and @b field_numSV() access fuction
    ///         for @ref RxmSvsiFields::NumSV field.
    ///     @li @b Field_list type and @b field_list() access fuction
    ///         for @ref RxmSvsiFields::List field.
    COMMS_MSG_FIELDS_NAMES(
        itow,
        week,
        numVis,
        numSV,
        list
    );
    
    // Compile time check for serialisation length.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static_assert(MsgMinLen == 8U, "Unexpected min serialisation length");
    
    /// @brief Name of the message.
    static const char* doName()
    {
        return ublox::message::RxmSvsiCommon::name();
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
            static_cast<std::size_t>(field_numSV().value()));
        
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
            auto expectedValue = static_cast<std::size_t>(field_numSV().value());
            auto realValue = field_list().value().size();
            if (expectedValue != realValue) {
                using PrefixValueType = typename std::decay<decltype(field_numSV().value())>::type;
                field_numSV().value() = static_cast<PrefixValueType>(realValue);
                updated = true;
            }
        } while (false);
        
        return updated;
        
    }
    
};

} // namespace message

} // namespace ublox

