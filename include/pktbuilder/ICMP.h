#pragma once

#include <pktbuilder/Layer.h>
#include <pktbuilder/IPv4.h>
#include <array>

namespace pktbuilder {
    namespace ICMP {
        namespace Type {
            enum Type: uint8_t {
                ECHO_REPLY = 0,
                DESTINATION_UNREACHABLE = 3,
                SOURCE_QUENCH = 4,
                REDIRECT_MESSAGE = 5,
                ALTERNATE_HOST_ADDRESS = 6,
                ECHO_REQUEST = 8,
                ROUTER_ADVERTISEMENT = 9,
                ROUTER_SOLICITATION = 10,
                TIME_EXCEEDED = 11,
                PARAMETER_PROBLEM_BAD_IP_HEADER = 12,
                TIMESTAMP = 13,
                TIMESTAMP_REPLY = 14,
                INFORMATION_REQUEST = 15,
                INFORMATION_REPLY = 16,
                ADDRESS_MASK_REQUEST = 17,
                ADDRESS_MASK_REPLY = 18,
                TRACEROUTE = 30,
                DATAGRAM_CONVERSION_ERROR = 31,
                MOBILE_HOST_REDIRECT = 32,
                WHERE_ARE_YOU = 33,
                HERE_I_AM = 34,
                MOBILE_REGISTRATION_REQUEST = 35,
                MOBILE_REGISTRATION_REPLY = 36,
                DOMAIN_NAME_REQUEST = 37,
                DOMAIN_NAME_REPLY = 38,
                SKIP_DISCOVERY = 39,
                PHOTURIS_SECURITY_FAILURE = 40,
                EXPERIMENTAL_MOBILITY_PROTOCOL = 41,
                EXTENDED_ECHO_REQUEST = 42,
                EXTENDED_ECHO_REPLY = 43
            };
        }
        namespace Code {
            namespace EchoReply {
                enum EchoReply: uint8_t {
                    ECHO_REPLY = 0
                };
            }
            namespace DestinationUnreachable {
                enum DestinationUnreachable: uint8_t {
                    DESTINATION_NETWORK_UNREACHABLE = 0,
                    DESTINATION_HOST_UNREACHABLE = 1,
                    DESTINATION_PROTOCOL_UNREACHABLE = 2,
                    DESTINATION_PORT_UNREACHABLE = 3,
                    FRAGMENTATION_REQUIRED = 4,
                    SOURCE_ROUTE_FAILED = 5,
                    DESTINATION_NETWORK_UNKNOWN = 6,
                    DESTINATION_HOST_UNKNOWN = 7,
                    SOURCE_HOST_ISOLATED = 8,
                    NETWORK_ADMINISTRATIVELY_PROHIBITED = 9,
                    HOST_ADMINISTRATIVELY_PROHIBITED = 10,
                    NETWORK_UNREACHABLE_FOR_TOS = 11,
                    HOST_UNREACHABLE_FOR_TOS = 12,
                    COMMUNICATION_ADMINISTRATIVELY_PROHIBITED = 13,
                    HOST_PRECEDENCE_VIOLATION = 14,
                    PRECEDENCE_CUTOFF_IN_EFFECT = 15
                };
            }
            namespace SourceQuench {
                enum SourceQuench: uint8_t {
                    SOURCE_QUENCH = 0
                };
            }
            namespace RedirectMessage {
                enum RedirectMessage: uint8_t {
                    REDIRECT_DATAGRAM_FOR_NETWORK = 0,
                    REDIRECT_DATAGRAM_FOR_HOST = 1,
                    REDIRECT_DATAGRAM_FOR_TOS_NETWORK = 2,
                    REDIRECT_DATAGRAM_FOR_TOS_HOST = 3
                };
            }
            namespace AlternateHostAddress {
                enum AlternateHostAddress: uint8_t {
                    ALTERNATE_HOST_ADDRESS = 0
                };
            }
            namespace EchoRequest {
                enum EchoRequest: uint8_t {
                    ECHO_REQUEST = 0
                };
            }
            namespace RouterAdvertisement {
                enum RouterAdvertisement: uint8_t {
                    ROUTER_ADVERTISEMENT = 0
                };
            }
            namespace RouterSolicitation {
                enum RouterSolicitation: uint8_t {
                    ROUTER_SOLICITATION = 0
                };
            }
            namespace TimeExceeded {
                enum TimeExceeded: uint8_t {
                    TTL_EXPIRED_IN_TRANSIT = 0,
                    FRAGMENT_REASSEMBLY_TIME_EXCEEDED = 1
                };
            }
            namespace ParameterProblemBadIpHeader {
                enum ParameterProblemBadIpHeader: uint8_t {
                    POINTER_INDICATES_ERROR = 0,
                    MISSING_A_REQUIRED_OPTION = 1,
                    BAD_LENGTH = 2
                };
            }
            namespace Timestamp {
                enum Timestamp: uint8_t {
                    TIMESTAMP = 0
                };
            }
            namespace TimestampReply {
                enum TimestampReply: uint8_t {
                    TIMESTAMP_REPLY = 0
                };
            }
            namespace InformationRequest {
                enum InformationRequest: uint8_t {
                    INFORMATION_REQUEST = 0
                };
            }
            namespace InformationReply {
                enum InformationReply: uint8_t {
                    INFORMATION_REPLY = 0
                };
            }
            namespace AddressMaskRequest {
                enum AddressMaskRequest: uint8_t {
                    ADDRESS_MASK_REQUEST = 0
                };
            }
            namespace AddressMaskReply {
                enum AddressMaskReply: uint8_t {
                    ADDRESS_MASK_REPLY = 0
                };
            }
            namespace Traceroute {
                enum Traceroute: uint8_t {
                    INFORMATION_REQUEST = 0
                };
            }
            namespace DatagramConversionError {
                enum DatagramConversionError: uint8_t {
                    DATAGRAM_CONVERSION_ERROR = 0
                };
            }
            namespace MobileHostRedirect {
                enum MobileHostRedirect: uint8_t {
                    MOBILE_HOST_REDIRECT = 0
                };
            }
            namespace WhereAreYou {
                enum WhereAreYou: uint8_t {
                    WHERE_ARE_YOU = 0
                };
            }
            namespace HereIAm {
                enum HereIAm: uint8_t {
                    HERE_I_AM = 0
                };
            }
            namespace MobileRegistrationRequest {
                enum MobileRegistrationRequest: uint8_t {
                    MOBILE_REGISTRATION_REQUEST = 0
                };
            }
            namespace MobileRegistrationReply {
                enum MobileRegistrationReply: uint8_t {
                    MOBILE_REGISTRATION_REPLY = 0
                };
            }
            namespace DomainNameRequest {
                enum DomainNameRequest: uint8_t {
                    DOMAIN_NAME_REQUEST = 0
                };
            }
            namespace DomainNameReply {
                enum DomainNameReply: uint8_t {
                    DOMAIN_NAME_REPLY = 0
                };
            }
            namespace SkipDiscovery {
                enum SkipDiscovery: uint8_t {
                    SKIP_DISCOVERY = 0
                };
            }
            namespace PhoturisSecurityFailure {
                enum PhoturisSecurityFailure: uint8_t {
                    PHOTURIS_SECURITY_FAILURE = 0
                };
            }
            namespace ExtendedEchoRequest {
                enum ExtendedEchoRequest: uint8_t {
                    REQUEST_EXTENDED_ECHO = 0
                };
            }
            namespace ExtendedEchoReply {
                enum ExtendedEchoReply: uint8_t {
                    NO_ERROR = 0,
                    MALFORMED_QUERY = 1,
                    NO_SUCH_INTERFACE = 2,
                    NO_SUCH_TABLE_ENTRY = 3,
                    MULTIPLE_INTERFACES_SATISFY_QUERY = 4
                };
            }
        }
        class Packet: public IntermediaryLayer{
            uint8_t type;
            uint8_t code;
            std::array<uint8_t, 4> header_contents;
            std::vector<uint8_t> body;
        public:
            Packet() = default;
            Packet(uint8_t type, uint8_t code,
                    std::array<uint8_t, 4> const& header_contents);
            Packet(uint8_t type, uint8_t code, 
                    uint16_t identifier, uint16_t sequence_number);
            [[nodiscard]] std::vector<uint8_t> build() const override;
            [[nodiscard]] uint8_t getType() const;
            [[nodiscard]] uint8_t getCode() const;
            [[nodiscard]] const std::vector<uint8_t>& getPayload() const;
            [[nodiscard]] std::array<uint8_t, 4> getHeaderContents() const;
            [[nodiscard]] uint16_t getSequenceNumber() const;
            [[nodiscard]] uint16_t getIdentifier() const;
            IPv4::Packet operator|(IPv4::Packet const& other) const;
        };
    }
}
