#ifndef QMYROLE_H
#define QMYROLE_H

#include "tool/CELLMath.hpp"

class QMyRole
{
public:
        CELL::float3  _pos;
        CELL::float3  _target;
        float   _speed;
public:
    QMyRole();
    /**
    *   设置移动的目标点
    */
    void    setTarget(CELL::float3 target);
    /**
    *   更新位置
    */
    void    setPosition(CELL::float3 pos);

    void moveCheck(const float elasped);
    /**
    *   绘制角色
    */
    void    render(float fElapsed);
};

#endif // QMYROLE_H
