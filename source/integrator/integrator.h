#pragma once

class Integrator {
public:
    virtual ~Integrator() = default;

    virtual void blit(void* rgba, int pitch) = 0;
};
