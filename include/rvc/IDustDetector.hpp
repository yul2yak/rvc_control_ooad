#pragma once

namespace rvc {

class IDustDetector {
public:
    virtual ~IDustDetector() = default;
    virtual bool isDustPresent() const = 0;
};

}  // namespace rvc
