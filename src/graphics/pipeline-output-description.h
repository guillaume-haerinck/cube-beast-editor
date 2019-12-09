#pragma once

#include <vector>
#include <string>

enum class RenderTargetType {
    Texture = 0, RenderBuffer
};

enum class RenderTargetUsage {
    Color = 0, Depth, DepthStencil
};

enum class RenderTargetChannels {
    R = 0, RG, RGB, RGBA
};

struct RenderTargetDescription {
    RenderTargetType type;
    RenderTargetUsage usage;
    RenderTargetChannels channels;
    std::string name;

    RenderTargetDescription() {}

    RenderTargetDescription(RenderTargetUsage usage, RenderTargetType type, RenderTargetChannels channels, std::string name) 
        : usage(usage), type(type), channels(channels), name(name)
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
