#include <pktbuilder/DNS.h>
#include <cassert>

int main(void) {
    std::string domain_name = "host.somedomain.co.uk";
    std::vector<uint8_t> formatted = pktbuilder::DNS::formatDomainName(domain_name);
    assert(formatted == std::vector<uint8_t>(
        {4, 'h', 'o', 's', 't', 10, 's', 'o', 'm', 'e', 'd', 'o', 'm', 
        'a', 'i', 'n', 2, 'c', 'o', 2, 'u', 'k', 0}));
    
    domain_name = "mydomain";
    formatted = pktbuilder::DNS::formatDomainName(domain_name);
    assert(formatted == std::vector<uint8_t>({8, 'm', 'y', 'd', 'o', 'm', 'a', 
        'i', 'n', 0}));

    domain_name = "dom.com";
    formatted = pktbuilder::DNS::formatDomainName(domain_name);
    assert(formatted == std::vector<uint8_t>({3, 'd', 'o', 'm', 3, 'c', 'o', 'm', 0}));
    return 0;
}