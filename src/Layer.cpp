#include <pktbuilder/Layer.h>

namespace pktbuilder {

    void IntermediaryLayer::setPayload(std::vector<uint8_t> const& data) {
        this->payload = data;
    }

    std::vector<uint8_t> const &IntermediaryLayer::getPayload() {
        return this->payload;
    }

    DataLayer::DataLayer(std::vector<uint8_t> const& data): IntermediaryLayer() {
        this->data = data;
    }

    std::vector<uint8_t> DataLayer::build() const {
        std::vector<uint8_t> built_data = this->data;
        built_data.insert(built_data.end(), this->payload.begin(), this->payload.end());
        return built_data;
    }
}