#pragma once

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
    Context& m_ctx;
};
