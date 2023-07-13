#include <string>
#include <pktbuilder/Layer.h>

namespace pktbuilder {
    enum class DNSMessageType {
        QUERY = 0,
        RESPONSE = 1
    };
    namespace DNSOpcode {
        enum DNSOpcode: uint8_t {
            QUERY = 0,
            IQUERY = 1,
            STATUS = 2
        };
    }
    namespace DNSFlag {
        enum DNSFlag: uint8_t {
            AA = 0x08,
            TC = 0x04,
            RD = 0x02,
            RA = 0x01
        };
    }
    namespace DNSResponseCode {
        enum DNSResponseCode: uint8_t {
            OK = 0,
            FORMAT_ERROR = 1,
            SERVER_FAILURE = 2,
            NAME_ERROR = 3,
            NOT_IMPLEMENTED = 4,
            REFUSED = 5
        };
    }
    namespace DNSType {
        enum DNSType: uint16_t {
            A = 1,
            AAAA = 28,
            AFSDB = 18,
            APL = 42,
            CAA = 257,
            CDNSKEY = 60,
            CDS = 59,
            CERT = 37,
            CNAME = 5,
            CSYNC = 62,
            DHCIP = 49,
            DLV = 32769,
            DNAME = 39,
            DNSKEY = 48,
            DS = 43,
            EUI48 = 108,
            EUI64 = 109,
            HINFO = 13,
            HIP = 55,
            HTTPS = 65,
            IPSECKEY = 45,
            KEY = 25,
            KX = 36,
            LOC = 29,
            MX = 15,
            NAPTR = 35,
            NS = 2,
            NSEC = 47,
            NSEC3 = 50,
            NSEC3PARAM = 51,
            OPENPGPKEY = 61,
            PTR = 12,
            RRSIG = 46,
            RP = 17,
            SIG = 24,
            SMIMEA = 53,
            SOA = 6,
            SRV = 33,
            SSHFP = 44,
            SVCB = 64,
            TA = 32768,
            TKEY = 249,
            TLSA = 52,
            TSIG = 250,
            TXT = 16,
            URI = 256,
            ZONEMD = 63,
            ANY = 255,
            AXFR = 252,
            IXFR = 25,
            OPT = 41,
            NONE = 10,
            WKS = 11,
            X25 = 19,
            ISDN = 20,
            RT = 21,
            NSAP = 22,
            NSAP_PTR = 23,
            PX = 26,
            EID = 31,
            NIMLOC = 32,
            ATMA = 34,
            SINK = 40,
            GPOS = 27,
            UINFO = 100,
            UID = 101,
            GID = 102,
            UNSPEC = 103,
            NINFO = 56,
            RKEY = 57,
            TALINK = 58,
            NID = 104,
            L32 = 105,
            L64 = 106,
            LP = 107,
            DOA = 259
        };
    }
    namespace DNSClass {
        enum DNSClass: uint16_t {
            IN = 1,
            CH = 3,
            HS = 4,
            NONE = 254,
            ANY = 255
        };
    }
    struct DNSQuestion {
        std::string domain_name;
        uint16_t qtype;
        uint16_t qclass;
    };

    struct DNSResourceRecord {
        std::string domain_name;
        uint16_t rrtype;
        uint16_t rrclass;
        uint32_t ttl;
        std::vector<uint8_t> rdata;
    };

    class DNSMessage: public Layer {
        uint16_t id = 0;
        DNSMessageType qr = DNSMessageType::QUERY;
        uint8_t opcode = 0;
        uint8_t flags = 0;
        uint8_t rcode = 0;
        std::vector<DNSQuestion> questions{};
        std::vector<DNSResourceRecord> answers{};
        std::vector<DNSResourceRecord> authorities{};
        std::vector<DNSResourceRecord> additional{};
    public:
        DNSMessage() = default;
        DNSMessage(uint16_t id, DNSMessageType qr, uint8_t opcode, uint8_t flags, 
            uint8_t rcode, std::vector<DNSQuestion> const& questions, 
            std::vector<DNSResourceRecord> const& answers, 
            std::vector<DNSResourceRecord> const& authorities, 
            std::vector<DNSResourceRecord> const& additional);
        [[nodiscard]] std::vector<uint8_t> build() const override;

    };
};