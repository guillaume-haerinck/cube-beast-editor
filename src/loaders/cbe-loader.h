#pragma once

#include <nlohmann/json.hpp>
#include "context.h"

class CbeLoader {
public:
    CbeLoader(Context& ctx);
    ~CbeLoader();

    /**
     * @brief Empty scene and add fill it with .cbe file data
     * 
     * @param cbeFilePath 
     */
    void loadFile(const char* cbeFilePath);

private:
    void generation(const nlohmann::json& json);

private:
    Context& m_ctx;
};
