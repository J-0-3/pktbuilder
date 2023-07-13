#include <pktbuilder/DNS.h>
#include <pktbuilder/utils.h>
#include <stdexcept>

namespace {
    using namespace pktbuilder;
    using namespace pktbuilder::DNS;
    void appendResourceRecord(ResourceRecord const& rr, std::vector<uint8_t> &dest) {
        std::vector<uint8_t> name = formatDomainName(rr.domain_name);
        std::array<uint8_t, 2> type_b = splitBytesBigEndian(rr.rrtype);
        std::array<uint8_t, 2> class_b = splitBytesBigEndian(rr.rrclass);
        std::array<uint8_t, 4> ttl_b = splitBytesBigEndian(rr.ttl);
        size_t rdlength = rr.rdata.size();
        if (rdlength > UINT16_MAX) {
            throw(std::runtime_error("RR Data too long"));
        }
        std::array<uint8_t, 2> rdlen_b = splitBytesBigEndian(static_cast<uint16_t>(rdlength));
        dest.insert(dest.end(), name.begin(), name.end());
        dest.insert(dest.end(), type_b.begin(), type_b.end());
        dest.insert(dest.end(), class_b.begin(), class_b.end());
        dest.insert(dest.end(), ttl_b.begin(), ttl_b.end());
        dest.insert(dest.end(), rdlen_b.begin(), rdlen_b.end());
        dest.insert(dest.end(), rr.rdata.begin(), rr.rdata.end());
    }  
}
namespace pktbuilder {
    namespace DNS {
        Message::Message(uint16_t id, MessageType qr, uint8_t opcode, 
                uint8_t flags, uint8_t rcode, 
                std::vector<Question> const& questions, 
                std::vector<ResourceRecord> const& answers, 
                std::vector<ResourceRecord> const& authorities, 
                std::vector<ResourceRecord> const& additional) {
            this->id = id;
            this->qr = qr;
            this->opcode = opcode;
            this->flags = flags;
            this->rcode = rcode;
            this->questions = questions;
            this->answers = answers;
            this->authorities = authorities;
            this->additional = additional;
        }

        std::vector<uint8_t> Message::build() const {
            std::vector<uint8_t> data;
            std::array<uint8_t, 2> id_b = splitBytesBigEndian(this->id);
            data.push_back(id_b[0]);
            data.push_back(id_b[1]);
            if (this->opcode > 15) {
                throw(std::runtime_error("DNS Opcode too large"));
            }
            if (this->flags > 15) {
                throw(std::runtime_error("Flags too large"));
            }
            data.push_back(static_cast<uint8_t>(this->qr) << 7 | this->opcode << 3
                | this->flags >> 1);
            if (this->rcode > 15) {
                throw(std::runtime_error("RCode too large"));
            }
            data.push_back((this->flags & 1) << 7 | this->rcode);
            if (this->questions.size() > UINT16_MAX) {
                throw(std::runtime_error("Too many question sections"));
            }
            std::array<uint8_t, 2> qd_c_b = splitBytesBigEndian(
                static_cast<uint16_t>(this->questions.size()));
            data.push_back(qd_c_b[0]);
            data.push_back(qd_c_b[1]);
            if (this->answers.size() > UINT16_MAX) {
                throw(std::runtime_error("Too many answer sections"));
            }
            std::array<uint8_t, 2> an_c_b = splitBytesBigEndian(
                static_cast<uint16_t>(this->answers.size()));
            data.push_back(an_c_b[0]);
            data.push_back(an_c_b[1]);
            if (this->authorities.size() > UINT16_MAX) {
                throw(std::runtime_error("Too many nameserver sections"));
            }
            std::array<uint8_t, 2> ns_c_b = splitBytesBigEndian(
                static_cast<uint16_t>(this->authorities.size()));
            data.push_back(ns_c_b[0]);
            data.push_back(ns_c_b[1]);
            if (this->additional.size() > UINT16_MAX) {
                throw(std::runtime_error("Too many additional sections"));
            }
            std::array<uint8_t, 2> ar_c_b = splitBytesBigEndian(
                static_cast<uint16_t>(this->additional.size()));
            data.push_back(ar_c_b[0]);
            data.push_back(ar_c_b[1]);
            
            for(Question const& q: this->questions) {
                std::vector<uint8_t> qname = formatDomainName(q.domain_name);
                data.insert(data.end(), qname.begin(), qname.end());
                std::array<uint8_t, 2> qtype_b = splitBytesBigEndian(q.qtype);
                data.push_back(qtype_b[0]);
                data.push_back(qtype_b[1]);
                std::array<uint8_t, 2> qclass_b = splitBytesBigEndian(q.qclass);
                data.push_back(qclass_b[0]);
                data.push_back(qclass_b[1]);
            }

            for (ResourceRecord const& rr: this->answers) {
                appendResourceRecord(rr, data);
            }

            for (ResourceRecord const& rr: this->authorities) {
                appendResourceRecord(rr, data);
            }

            for (ResourceRecord const& rr: this->additional) {
                appendResourceRecord(rr, data);
            }
            return data;
        }

        std::vector<uint8_t> formatDomainName(std::string const& domain_name) {
            std::vector<uint8_t> bytes;
            size_t period_pos = domain_name.find('.');
            size_t prev = -1;
            bool done = false;
            while (!done) {
                if (period_pos == domain_name.npos) {
                    period_pos = domain_name.length();
                    done = true;
                }
                size_t section_length = period_pos - prev - 1;
                if (section_length > UINT8_MAX) {
                    throw(std::runtime_error("Domain name too long"));
                }
                bytes.push_back(static_cast<uint8_t>(section_length));
                for (uint8_t c: domain_name.substr(prev + 1, section_length)) {
                    bytes.push_back(c);
                }
                prev = period_pos;
                period_pos = domain_name.find('.', prev + 1);
            }
            bytes.push_back(0);
            return bytes;
        }
    }
}