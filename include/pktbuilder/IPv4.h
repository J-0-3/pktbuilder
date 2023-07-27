#pragma once
#include <pktbuilder/Layer.h>
#include <pktbuilder/Ethernet.h>
#include <pktbuilder/addresses.h>

namespace pktbuilder {
    namespace IPv4 {
        namespace ProtocolNumber {
            enum ProtocolNumber : uint8_t {
                HOPOPT = 0,
                ICMP = 1,
                IGMP = 2,
                GGP = 3,
                IP_in_IP = 4,
                ST = 5,
                TCP = 6,
                CBT = 7,
                EGP = 8,
                IGP = 9,
                BBN_RCC_MON = 10,
                NVP_II = 11,
                PUP = 12,
                ARGUS = 13,
                EMCON = 14,
                XNET = 15,
                CHAOS = 16,
                UDP = 17,
                MUX = 18,
                DCN_MEAS = 19,
                HMP = 20,
                PRM = 21,
                XNS_IDP = 22,
                TRUNK_1 = 23,
                TRUNK_2 = 24,
                LEAF_1 = 25,
                LEAF_2 = 26,
                RDP = 27,
                IRTP = 28,
                ISO_TP4 = 29,
                NETBLT = 30,
                MFE_NSP = 31,
                MERIT_INP = 32,
                DCCP = 33,
                TPCP = 34,
                IDPR = 35,
                XTP = 36,
                DDP = 37,
                IDPR_CMTP = 38,
                TPPP = 39,
                IL = 40,
                IPv6 = 41,
                SDRP = 42,
                IPv6_Route = 43,
                IPV6_Frag = 44,
                IDRP = 45,
                RSVP = 46,
                GRE = 47,
                DSR = 48,
                BNA = 49,
                ESP = 50,
                AH = 51,
                I_NLSP = 52,
                SwIPe = 53,
                NARP = 54,
                MOBILE = 55,
                TLSP = 56,
                SKIP = 57,
                IPv6_ICMP = 58,
                IPv6_NoNxt = 59,
                IPv6_Opts = 60,
                HOST_INTERNAL = 61,
                CFTP = 62,
                LOCAL_NETWORK = 63,
                SAT_EXPAK = 64,
                KRYPTOLAN = 65,
                RVD = 66,
                IPPC = 67,
                DISTRIBUTED_FILE_SYSTEM = 68,
                SAT_MON = 69,
                VISA = 70,
                IPCU = 71,
                CPNX = 72,
                CPHB = 73,
                WSN = 74,
                PVP = 75,
                BR_SAT_MON = 76,
                SUN_ND = 77,
                WB_MON = 78,
                WB_EXPAK = 79,
                ISO_IP = 80,
                VMTP = 81,
                SECURE_VMTP = 82,
                VINES = 83,
                TTP = 84,
                IPTM = 84,
                NSFNET_IGP = 85,
                DGP = 86,
                TCF = 87,
                EIGRP = 88,
                OSPF = 89,
                Sprite_RPC = 90,
                LARP = 91,
                MTP = 92,
                AX_25 = 93,
                OS = 94,
                MICP = 95,
                SCC_SP = 96,
                ETHERIP = 97,
                ENCAP = 98,
                PRIVATE_ENCRYPTION = 99,
                GMTP = 100,
                IFMP = 101,
                PNNI = 102,
                PIM = 103,
                ARIS = 104,
                SCPS = 105,
                QNX = 106,
                A_N = 107,
                IPComp = 108,
                SNP = 109,
                Compaq_Peer = 110,
                IPX_in_IP = 111,
                VRRP = 112,
                PGM = 113,
                ZERO_HOP_PROTOCOL = 114,
                L2TP = 115,
                DDDX = 116,
                IATP = 117,
                STP = 118,
                SRP = 119,
                UTI = 120,
                SMP = 121,
                SM = 122,
                PTP = 123,
                IS_IS = 124,
                FIRE = 125,
                CRTP = 126,
                CRUDP = 127,
                SSCOPMCE = 128,
                IPLT = 129,
                SPS = 130,
                PIPE = 131,
                SCTP = 132,
                FC = 133,
                RSVP_E2E_IGNORE = 134,
                Mobility_Header = 135,
                UDPLite = 136,
                MPLS = 137,
                manet = 138,
                HIP = 139,
                Shim6 = 140,
                WESP = 141,
                ROHC = 142,
                SRv6 = 143,
                AGGFRAG = 144,
                NSH = 145
            };
        }

        enum class ECNCodePoint: uint8_t {
            NOT_ECT = 0,
            ECT_CAPABLE_1 = 1,
            ECT_CAPABLE_0 = 2,
            CONGESTION_EXPERIENCED = 3
        };

        namespace OptionType {
            enum OptionType: uint8_t {
                EOOL = 0,
                NOP = 1,
                SEC = 2,
                RR = 7,
                ZSU = 10,
                MTUP = 11,
                MTUR = 12,
                ENCODE = 15,
                QS = 25,
                TS = 68,
                TR = 82,
                SEC_RIPSO = 130,
                LSR = 131,
                ESEC = 133,
                CIPSO = 134,
                SID = 136,
                SSR = 137,
                VISA = 142,
                IMITD = 144,
                EIP = 145,
                ADDEXT = 147,
                RTRALT = 148,
                SDB = 149,
                DPS = 151,
                UMP = 152,
                FINN = 205
            };
        }
        typedef struct Option{
            uint8_t option_type;
            std::vector<uint8_t> option_value;
        } Option;

        class Packet: public IntermediaryLayer {
            ipv4_addr_t source_ip{};
            ipv4_addr_t destination_ip{};
            uint8_t dscp{};
            ECNCodePoint ecn = ECNCodePoint::NOT_ECT;
            uint16_t identification{};
            bool dont_fragment{};
            bool more_fragments{};
            uint16_t fragment_offset{};
            uint8_t ttl{};
            uint8_t protocol = ProtocolNumber::HOPOPT;
            std::vector<Option> options;
        public:
            Packet() = default;
            explicit Packet(ipv4_addr_t destination_ip,
                                uint8_t protocol = 0,
                                ipv4_addr_t source_ip = { 0, 0, 0, 0 });
            Packet(ipv4_addr_t destination_ip, ipv4_addr_t source_ip,
                    uint8_t protocol, uint16_t identification,
                    uint8_t, ECNCodePoint ecn,
                    bool df_flag, bool mf_flag,
                    uint16_t fragment_offset, uint8_t ttl,
                    std::vector<Option> const* options);
            
            [[nodiscard]] std::vector<uint8_t> build() const override;
            Ethernet::Frame operator| (const Ethernet::Frame& other) const;
            [[nodiscard]] ipv4_addr_t const& getSourceAddress() const;
            [[nodiscard]] ipv4_addr_t const& getDestinationAddress() const;
            [[nodiscard]] uint8_t const& getProtocolNumber() const;
            void setProtocolNumber(uint8_t protocol);
        };
    }
}