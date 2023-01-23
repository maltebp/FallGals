#pragma once

namespace fg::utility {

    class Math {
    public:

        static float wrapDegrees(float value);
        static float randomFloat(float min, float max);

    private:

        Math() = delete;

    };

}