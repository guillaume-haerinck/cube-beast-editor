#pragma once

#include <vector>
#include <string>

enum class RenderTargetOperation {
    None = 0, ReadPixel
};

enum class RenderTargetType {
    Texture = 0, RenderBuffer
};

enum class RenderTargetUsage {
    Color = 0, Depth, DepthStencil
};

enum class RenderTargetChannels {
    R = 1, RG = 2, RGB = 3, RGBA = 4
};

enum class RenderTargetDataType {
    FLOAT = 0, UCHAR, UINT, USHORT
};

struct RenderTargetDescription {
    RenderTargetType type;
    RenderTargetDataType dataType;
    RenderTargetUsage usage;
    RenderTargetChannels channels;
    std::string name;
    RenderTargetOperation operation;

    RenderTargetDescription() {}

    RenderTargetDescription(RenderTargetUsage usage, RenderTargetType type, RenderTargetDataType dataType, RenderTargetChannels channels, std::string name, RenderTargetOperation operation = RenderTargetOperation::None) 
        : usage(usage), type(type), dataType(dataType), channels(channels), name(name), operation(operation)
    {}
};

class PipelineOutputDescription {
public:
    PipelineOutputDescription() {}
    PipelineOutputDescription(const std::initializer_list<RenderTargetDescription>& targets) : m_targets(targets) {}

    inline const std::vector<RenderTargetDescription>& getTargets() const { return m_targets; }

    std::vector<RenderTargetDescription>::iterator begin() { return m_targets.begin(); }
    std::vector<RenderTargetDescription>::iterator end() { return m_targets.end(); }
    std::vector<RenderTargetDescription>::const_iterator begin() const { return m_targets.begin(); }
    std::vector<RenderTargetDescription>::const_iterator end() const { return m_targets.end(); }

    size_t size() const { return m_targets.size(); }

private:
    std::vector<RenderTargetDescription> m_targets;
};
