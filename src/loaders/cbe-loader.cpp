#include "cbe-loader.h"

#include <spdlog/spdlog.h>
#include <vector>
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>

CbeLoader::CbeLoader(Context& ctx) : m_ctx(ctx) {}

CbeLoader::~CbeLoader() {}

void CbeLoader::loadFile(const char* cbeFilePath) {
    std::ifstream fileStream(cbeFilePath);
    if (!fileStream) {
		spdlog::error("[CBEloader] Cannot load file : {}", cbeFilePath);
		assert(false);
	}

    const nlohmann::json parsed = nlohmann::json::parse(fileStream);
    fileStream.close();

    for (const auto& element : parsed.items()) {
        if (element.key() == "geometry") {

        } else if (element.key() == "palette") {

        } else if (element.key() == "generation") {
            generation(parsed);
        }
    }
}

void CbeLoader::generation(const nlohmann::json& json) {
    const auto& type = json["generation"].at(0)["type"];
    const auto& interpolation = json["generation"].at(0)["interpolation"];
    const auto& mode = json["generation"].at(0)["mode"];
    const auto& controlPoints = json["generation"].at(0)["controlPoints"];

    std::vector<glm::ivec3> controlPointsXYZ;
    // Eigen::VectorXd controlPointWeights(controlPointsXYZ.size());
    // controlPointWeights << 1.0f, 10.0f;

    for (const auto& point : controlPoints) {
        const auto& pos = point["position"];
        const auto& weight = point["weight"];
    }
}