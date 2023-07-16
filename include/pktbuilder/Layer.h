#pragma once

#include <cstdint>
#include <vector>
#include <type_traits>
#include <concepts>

template<typename T>
concept Container = requires(T a) {
    requires std::same_as<typename T::reference, typename T::value_type&>;
    requires std::same_as<typename T::const_reference, const typename T::value_type&>;
    requires std::forward_iterator<typename T::iterator>;
    requires std::forward_iterator<typename T::const_iterator>;
    requires std::same_as<typename T::difference_type, typename std::iterator_traits<typename T::iterator>::difference_type>;
    requires std::same_as<typename T::difference_type, typename std::iterator_traits<typename T::const_iterator>::difference_type>;
    { a.begin() } -> std::same_as<typename T::iterator>;
    { a.end() } -> std::same_as<typename T::iterator>;
    { a.cbegin() } -> std::same_as<typename T::const_iterator>;
    { a.cend() } -> std::same_as<typename T::const_iterator>;
};


namespace pktbuilder {
    class Layer {
    public:
        [[nodiscard]] virtual std::vector<uint8_t> build() const = 0;
        virtual ~Layer() = default;
    };

    class IntermediaryLayer: public Layer {
    protected:
        std::vector<uint8_t> payload;
    public:
        void setPayload(std::vector<uint8_t> const& data);
        std::vector<uint8_t> const& getPayload();
    };

    class DataLayer: public IntermediaryLayer {
    protected:
        std::vector<uint8_t> data;
    public:
        explicit DataLayer(std::vector<uint8_t> const& data);
        [[nodiscard]] std::vector<uint8_t> build() const override;
    };


    template<Container C, typename T> 
    T operator| (const C& input, const T& destination) {
        static_assert(std::is_base_of_v<IntermediaryLayer, T>, 
            "Cannot pipe data to non child of IntermediaryLayer");
        std::vector<uint8_t> payload;
        for (const auto& x: input) {
            payload.push_back(static_cast<uint8_t>(x));
        }
        T new_packet = destination;
        new_packet.setPayload(payload);
        return new_packet;
    }

    template<typename T>
    T operator| (const Layer& input, const T& destination) {
        static_assert(std::is_base_of_v<IntermediaryLayer, T>, 
            "Cannot pipe packet to non child of IntermediaryLayer");
        T new_packet = destination;
        new_packet.setPayload(input.build());
        return new_packet;
    }
}