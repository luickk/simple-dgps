// Generated by commsdsl2comms v3.5.2

/// @file
/// @brief Contains definition of <b>"MON-RXBUF"</b> message and its fields.

#pragma once

#include <cstdint>
#include <tuple>
#include "comms/MessageBase.h"
#include "comms/field/ArrayList.h"
#include "comms/field/IntValue.h"
#include "comms/options.h"
#include "ublox/MsgId.h"
#include "ublox/field/FieldBase.h"
#include "ublox/message/MonRxbufCommon.h"
#include "ublox/options/DefaultOptions.h"

namespace ublox
{

namespace message
{

/// @brief Fields of @ref MonRxbuf.
/// @tparam TOpt Extra options
/// @see @ref MonRxbuf
/// @headerfile "ublox/message/MonRxbuf.h"
template <typename TOpt = ublox::options::DefaultOptions>
struct MonRxbufFields
{
    /// @brief Scope for all the member fields of
    ///     @ref Pending list.
    struct PendingMembers
    {
        /// @brief Definition of <b>""</b> field.
        struct Element : public
            comms::field::IntValue<
                ublox::field::FieldBase<>,
                std::uint16_t
            >
        {
            /// @brief Name of the field.
            static const char* name()
            {
                return ublox::message::MonRxbufFieldsCommon::PendingMembersCommon::ElementCommon::name();
            }
            
        };
        
    };
    
    /// @brief Definition of <b>"pending"</b> field.
    struct Pending : public
        comms::field::ArrayList<
            ublox::field::FieldBase<>,
            typename PendingMembers::Element,
            typename TOpt::message::MonRxbufFields::Pending,
            comms::option::def::SequenceFixedSize<6U>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::MonRxbufFieldsCommon::PendingCommon::name();
        }
        
    };
    
    /// @brief Scope for all the member fields of
    ///     @ref Usage list.
    struct UsageMembers
    {
        /// @brief Definition of <b>""</b> field.
        struct Element : public
            comms::field::IntValue<
                ublox::field::FieldBase<>,
                std::uint8_t
            >
        {
            /// @brief Name of the field.
            static const char* name()
            {
                return ublox::message::MonRxbufFieldsCommon::UsageMembersCommon::ElementCommon::name();
            }
            
        };
        
    };
    
    /// @brief Definition of <b>"usage"</b> field.
    struct Usage : public
        comms::field::ArrayList<
            ublox::field::FieldBase<>,
            typename UsageMembers::Element,
            typename TOpt::message::MonRxbufFields::Usage,
            comms::option::def::SequenceFixedSize<6U>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::MonRxbufFieldsCommon::UsageCommon::name();
        }
        
    };
    
    /// @brief Scope for all the member fields of
    ///     @ref PeakUsage list.
    struct PeakUsageMembers
    {
        /// @brief Definition of <b>""</b> field.
        struct Element : public
            comms::field::IntValue<
                ublox::field::FieldBase<>,
                std::uint8_t
            >
        {
            /// @brief Name of the field.
            static const char* name()
            {
                return ublox::message::MonRxbufFieldsCommon::PeakUsageMembersCommon::ElementCommon::name();
            }
            
        };
        
    };
    
    /// @brief Definition of <b>"peakUsage"</b> field.
    struct PeakUsage : public
        comms::field::ArrayList<
            ublox::field::FieldBase<>,
            typename PeakUsageMembers::Element,
            typename TOpt::message::MonRxbufFields::PeakUsage,
            comms::option::def::SequenceFixedSize<6U>
        >
    {
        /// @brief Name of the field.
        static const char* name()
        {
            return ublox::message::MonRxbufFieldsCommon::PeakUsageCommon::name();
        }
        
    };
    
    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        Pending,
        Usage,
        PeakUsage
    >;
};

/// @brief Definition of <b>"MON-RXBUF"</b> message class.
/// @details
///     See @ref MonRxbufFields for definition of the fields this message contains.
/// @tparam TMsgBase Base (interface) class.
/// @tparam TOpt Extra options
/// @headerfile "ublox/message/MonRxbuf.h"
template <typename TMsgBase, typename TOpt = ublox::options::DefaultOptions>
class MonRxbuf : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::MonRxbuf,
        comms::option::def::StaticNumIdImpl<ublox::MsgId_MonRxbuf>,
        comms::option::def::FieldsImpl<typename MonRxbufFields<TOpt>::All>,
        comms::option::def::MsgType<MonRxbuf<TMsgBase, TOpt> >,
        comms::option::def::HasName
    >
{
    // Redefinition of the base class type
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::MonRxbuf,
            comms::option::def::StaticNumIdImpl<ublox::MsgId_MonRxbuf>,
            comms::option::def::FieldsImpl<typename MonRxbufFields<TOpt>::All>,
            comms::option::def::MsgType<MonRxbuf<TMsgBase, TOpt> >,
            comms::option::def::HasName
        >;

public:
    /// @brief Provide names and allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    ///     The generated types and functions are:
    ///     @li @b Field_pending type and @b field_pending() access fuction
    ///         for @ref MonRxbufFields::Pending field.
    ///     @li @b Field_usage type and @b field_usage() access fuction
    ///         for @ref MonRxbufFields::Usage field.
    ///     @li @b Field_peakUsage type and @b field_peakUsage() access fuction
    ///         for @ref MonRxbufFields::PeakUsage field.
    COMMS_MSG_FIELDS_NAMES(
        pending,
        usage,
        peakUsage
    );
    
    // Compile time check for serialisation length.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static const std::size_t MsgMaxLen = Base::doMaxLength();
    static_assert(MsgMinLen == 24U, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 24U, "Unexpected max serialisation length");
    
    /// @brief Name of the message.
    static const char* doName()
    {
        return ublox::message::MonRxbufCommon::name();
    }
    
    
};

} // namespace message

} // namespace ublox

