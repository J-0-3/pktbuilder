#pragma once
#include <pktbuilder/addresses.h>
#include <pktbuilder/Layer.h>
#include <vector>

namespace pktbuilder {
    enum class EtherType: uint16_t {
        IPv4 = 0x0800,
        ARP = 0x0806,
        WakeOnLAN = 0x0842,
        AVTP = 0x22f0,
        SRP = 0x22ea,
        DEC_MOP_RP = 0x6002,
        DECNet = 0x6003,
        DECLAT = 0x6004,
        RARP = 0x8035,
        AppleTalk = 0x809b,
        AARP = 0x80f3,
        IEE802_1Q = 0x8100,
        SLPP = 0x8102,
        VLACP = 0x8103,
        IPX = 0x8137,
        QNX_QNet = 0x8204,
        IPv6 = 0x86dd,
        EthernetFlowControl = 0x8808,
        LACP = 0x8809,
        CobraNet = 0x8819,
        MPLS_UC = 0x8847,
        MPLS_MC = 0x8848,
        PPPoE_Discovery = 0x8863,
        PPPoE_Session = 0x8864,
        HomePlug_MME = 0x887b,
        EAPOverLAN = 0x888e,
        PROFINET = 0x8892,
        HyperSCSI = 0x889a,
        ATA = 0x88a2,
        EtherCAT = 0x88a4,
        STag_Q_in_Q = 0x88a8,
        Powerlink = 0x88ab,
        GOOSE = 0x88b8,
        GSE = 0x88b9,
        SV = 0x88ba,
        MikroTik_RoMON = 0x88Bf,
        LLDP = 0x88cc,
        SERCOS_III = 0x88cd,
        HomePlug_Green_PHY = 0x88e1,
        MRP = 0x88e3,
        MACsec = 0x88e5,
        PBB = 0x88e7,
        PTP = 0x88f7,
        NC_SI = 0x88f8,
        PRP = 0x88fb,
        CFM = 0x8902,
        FCoE = 0x8906,
        FCoE_Init = 0x8914,
        RoCE = 0x8915,
        TTE = 0x891d,
        IEEE_1905_1 = 0x893a,
        HSR = 0x892f,
        ECTP = 0x9000,
        Redundancy_Tag = 0xf1c1
    };

    class EthernetFrame: public IntermediaryLayer{
        mac_addr_t destination_mac_{};
        mac_addr_t source_mac_{};
        EtherType ethertype_;
    public:
        EthernetFrame();
        EthernetFrame(mac_addr_t const& destination_mac, mac_addr_t const& source_mac,
                      EtherType ethertype);
        [[nodiscard]] std::vector<uint8_t> build() const override;
        [[nodiscard]] const mac_addr_t& getDestinationMac() const;
        [[nodiscard]] const mac_addr_t& getSourceMac() const;
        [[nodiscard]] const EtherType& getEthertype() const;
    };
}