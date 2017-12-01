#include "qmyrole.h"

QMyRole::QMyRole()
{
    _speed = 5;
}

void QMyRole::setTarget(CELL::float3 target)
{
    _target =   target;
}

void QMyRole::setPosition(CELL::float3 pos)
{
    _pos    =   pos;
    _pos.y  =   1;
}

void QMyRole::moveCheck(const float elasped)
{
    /**
    *   目标位置不是当前位置。
    */
//    printf("_target:[%f,%f,%f]\t_pos:[%f,%f,%f]\n",_target.x,_target.y,_target.z,_pos.x,_pos.y,_pos.z);
//    fflush(NULL);
    if (_target == _pos)
    {
        return;
    }
    /**
    *   获取当前玩家位置与目标位置的偏移量
    */
    CELL::float3  offset  =   _target - _pos;
    /**
    *   获取到移动的方向
    */
    CELL::float3  dir     =   CELL::normalize(offset);

    if (CELL::distance(_target,_pos) > 1)
    {
        float   speed   =   elasped * _speed;

        _pos    +=  CELL::float3(dir.x * speed,0,dir.z  * speed) ;
    }
    else
    {
        _target  = _pos;
    }
}

void QMyRole::render(float fElapsed)
{
    moveCheck(fElapsed);
}
