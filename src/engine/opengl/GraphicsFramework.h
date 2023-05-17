#pragma once

// Graphics Framework
class GraphicsFramework
{
public:
    GraphicsFramework();

    ~GraphicsFramework();

    constexpr bool IsInitialized() const { return m_Initialized; }

private:
    bool m_Initialized{ false };
};
