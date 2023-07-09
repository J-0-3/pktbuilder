#pragma once

#include <cstdint>
#include <vector>
#include <type_traits>

namespace pktbuilder {
    class Layer {
    public:
        [[nodiscard]] virtual std::vector<uint8_t> build() const = 0;

    };

    class IntermediaryLayer: public Layer {
    protected:
        std::vector<uint8_t> payload;
    public:
        void setPayload(std::vector<uint8_t> const& payload);
        std::vector<uint8_t> const& getPayload();
    };

    class DataLayer : public IntermediaryLayer {
    protected:
        std::vector<uint8_t> data;
    public:
        explicit DataLayer(std::vector<uint8_t> const& data);
        [[nodiscard]] std::vector<uint8_t> build() const override;
    };

    template<typename T>
    T operator| (const Layer& input, const T& destination) {
        static_assert(std::is_base_of<IntermediaryLayer, T>::value, "Cannot pipe packet to non child of IntermediaryLayer");
        T new_packet = destination;
        new_packet.setPayload(input.build());
        return new_packet;
    }
}