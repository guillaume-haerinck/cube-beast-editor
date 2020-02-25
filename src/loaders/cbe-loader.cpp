#include "cbe-loader.h"

#include <spdlog/spdlog.h>
#include <vector>
#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>

#include "maths/rbf.h"
#include "history/generation/rbf-history.h"

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
    const std::string& type = json["generation"].at(0)["type"];
    const std::string& interpolation = json["generation"].at(0)["interpolation"];
    const std::string& mode = json["generation"].at(0)["mode"];
    const auto& controlPoints = json["generation"].at(0)["controlPoints"];

    // Get values
    std::vector<glm::ivec3> controlPointsXYZ(controlPoints.size());
    Eigen::VectorXd controlPointWeights(controlPoints.size());
    for (unsigned int i = 0; i < controlPoints.size(); i++) {
        const float& weight = controlPoints.at(i)["weight"];
        controlPointWeights[i] = weight;

        const glm::vec3 pos = glm::vec3(controlPoints.at(i)["position"].at(0), controlPoints.at(i)["position"].at(1), controlPoints.at(i)["position"].at(2));
        controlPointsXYZ.at(i) = pos;
    }

    // Get entities
    std::vector<glm::ivec3> coordWithYtoFind;

    m_ctx.history.pushHistory(new RBFHistory(coordWithYtoFind));
    voxmt::rbfInterpolate(coordWithYtoFind, controlPointsXYZ, controlPointWeights, voxmt::RBFType::LINEAR, 0.5f, voxmt::RBFTransformAxis::Y);

    // Changes entities
    for (size_t i = 0; i < coordWithYtoFind.size(); i++) {

    }
}