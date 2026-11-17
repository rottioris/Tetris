#pragma once

#include <vector>

class Parts {
private:
    std::vector<std::vector<std::vector<bool>>> parts;

public:
    Parts();

    std::vector<std::vector<bool>> ConsultPart(int index);
    void RotatePart(int index);
    void DerotatePart(int index);
};
