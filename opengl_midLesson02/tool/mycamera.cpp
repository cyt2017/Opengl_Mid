#include "mycamera.h"

MYCAMERA::MYCAMERA(QObject *parent) : QObject(parent)
{
    _radius     =   400;
    _yaw        =   0;
    _viewSize   =   CELL::float2(100,100);
    _matView.identify();
    _matProj.identify();
    _matWorld.identify();
}

float MYCAMERA::getRadius() const
{
    return _radius;
}

void MYCAMERA::setRadius(float val)
{
    _radius = val;
}

CELL::float3 MYCAMERA::getEye() const
{
    return _eye;
}

void MYCAMERA::setEye(CELL::float3 val)
{
    _eye    =   val;
}

void MYCAMERA::calcDir()
{
    _dir    =   _target - _eye;
    _dir    =   CELL::normalize(_dir);
}

CELL::float3 MYCAMERA::getTarget() const
{
    return _target;
}

void MYCAMERA::setTarget(CELL::float3 val)
{
    _target = val;
}

CELL::float3 MYCAMERA::getUp() const
{
    return _up;
}

void MYCAMERA::setUp(CELL::float3 val)
{
    _up = val;
}

CELL::float3 MYCAMERA::getDir() const
{
    return  _dir;
}

CELL::float3 MYCAMERA::getRight() const
{
    return  _right;
}

void MYCAMERA::update()
{
    CELL::float3  upDir   =   CELL::normalize(_up);
    _eye        =   _target - _dir  * _radius;
    _right      =   CELL::normalize(CELL::cross(_dir, upDir));

    _matView    =   CELL::lookAt(_eye,_target,_up);
}

void MYCAMERA::setViewSize(const CELL::float2 &viewSize)
{
    _viewSize   =   viewSize;
}

void MYCAMERA::setViewSize(float x, float y)
{
    _viewSize   =   CELL::float2(x,y);
}

CELL::float2 MYCAMERA::getViewSize()
{
    return  _viewSize;
}

void MYCAMERA::setProject(const CELL::matrix4 &proj)
{
    _matProj    =   proj;
}

const CELL::matrix4 &MYCAMERA::getProject() const
{
    return  _matProj;
}

const CELL::matrix4 &MYCAMERA::getView() const
{
    return  _matView;
}

void MYCAMERA::perspective(float fovy, float aspect, float zNear, float zFar)
{
    _matProj    =   CELL::perspective<float>(fovy,aspect,zNear,zFar);
}

bool MYCAMERA::project(const CELL::float4 &world, CELL::float4 &screen)
{
    screen  =   (_matProj * _matView * _matWorld) * world;
    if (screen.w == 0.0f)
    {
        return false;
    }
    screen.x    /=  screen.w;
    screen.y    /=  screen.w;
    screen.z    /=  screen.w;

    // map to range 0 - 1
    screen.x    =   screen.x * 0.5f + 0.5f;
    screen.y    =   screen.y * 0.5f + 0.5f;
    screen.z    =   screen.z * 0.5f + 0.5f;

    // map to viewport
    screen.x    =   screen.x * _viewSize.x;
    screen.y    =   _viewSize.y - (screen.y * _viewSize.y);
    return  true;
}

CELL::float2 MYCAMERA::worldToScreen(const CELL::float3 &world)
{
    CELL::float4  worlds(world.x,world.y,world.z,1);
    CELL::float4  screens;
    project(worlds,screens);
    return  CELL::float2(screens.x,screens.y);
}

CELL::float3 MYCAMERA::screenToWorld(const CELL::float2 &screen)
{
    CELL::float4  screens(screen.x,screen.y,0,1);
    CELL::float4  world;
    unProject(screens,world);
    return  CELL::float3(world.x,world.y,world.z);
}

CELL::float3 MYCAMERA::screenToWorld(float x, float y)
{
    CELL::float4  screens(x,y,0,1);
    CELL::float4  world;
    unProject(screens,world);
    return  CELL::float3(world.x,world.y,world.z);
}

bool MYCAMERA::unProject(const CELL::float4 &screen, CELL::float4 &world)
{
    CELL::float4 v;
    v.x =   screen.x;
    v.y =   screen.y;
    v.z =   screen.z;
    v.w =   1.0;

    // map from viewport to 0 - 1
    v.x =   (v.x) /_viewSize.x;
    v.y =   (_viewSize.y - v.y) /_viewSize.y;
    //v.y = (v.y - _viewPort.Y) / _viewPort.Height;

    // map to range -1 to 1
    v.x =   v.x * 2.0f - 1.0f;
    v.y =   v.y * 2.0f - 1.0f;
    v.z =   v.z * 2.0f - 1.0f;

    CELL::matrix4  inverse = (_matProj * _matView * _matWorld).inverse();

    v   =   v * inverse;
    if (v.w == 0.0f)
    {
        return false;
    }
    world   =   v / v.w;
    return true;
}

CELL::Ray MYCAMERA::createRayFromScreen(int x, int y)
{
    CELL::float4  minWorld;
    CELL::float4  maxWorld;

    CELL::float4  screen(float(x),float(y),0,1);
    CELL::float4  screen1(float(x),float(y),1,1);

    unProject(screen,minWorld);
    unProject(screen1,maxWorld);
    CELL::Ray     ray;
    ray.setOrigin(CELL::float3(minWorld.x,minWorld.y,minWorld.z));

    CELL::float3  dir(maxWorld.x - minWorld.x,maxWorld.y - minWorld.y, maxWorld.z - minWorld.z);
    ray.setDirection(CELL::normalize(dir));
    return  ray;
}

void MYCAMERA::rotateView(float angle)
{
    _dir    =   CELL::rotateY<float>(_dir,angle);
}

void MYCAMERA::scaleCameraByPos(const CELL::float3 &pos, float persent)
{

    CELL::float3   dir     =   CELL::normalize(pos - _eye);

    float    dis     =   CELL::length(pos - _eye) * persent;

    float    disCam  =   CELL::length(_target - _eye) * persent;

    CELL::float3   dirCam  =   CELL::normalize(_target - _eye);

    _eye    =   pos - dir * dis;
    _target =   _eye + dirCam * disCam;

    update();
}


void MYCAMERA::reciveKeyEvent(KEYMODE type)
{
    //    _type = type;
}
void MYCAMERA::rotateViewY(float angle)
{
    float        len(0);
    CELL::matrix4    mat(1);
    mat.rotate(angle, CELL::real3(0, 1, 0));
    _dir        =   _dir * mat;
    _up         =   _up * mat;
    _right      =   CELL::normalize(CELL::cross(_dir, _up));
    len         =   CELL::length(_eye - _target);
    _eye        =   _target - _dir * len;
    _matView    =   CELL::lookAt<CELL::real>(_eye, _target, _up);
}

void MYCAMERA::rotateViewX(float angle)
{
    float       len(0);
    CELL::matrix4     mat(1);
    mat.rotate(angle,_right);
    _dir        =   _dir * mat;
    _up         =   _up * mat;
    _right      =   CELL::normalize(CELL::cross(_dir,_up));
    len         =   CELL::length(_eye - _target);
    _eye        =   _target - _dir * len;
    _matView    =   CELL::lookAt(_eye,_target,_up);
}


void MYCAMERA::reciveMouseEvent(MYCAMERA::MOUSEMODE type, CELL::int2 pS, CELL::int2 pE)
{
    if(type == MOUSE_RIGHTDOWN)
    {
        CELL::int2    offset = pE - pS;
        rotateViewY(offset.x * 0.5f);
        rotateViewX(offset.y * 0.5f);
    }
    update();
}

void MYCAMERA::reciveWheelEvent(MYCAMERA::MOUSEMODE type, int p, CELL::int2 point)
{
    if(p>0)
    {
        _radius = _radius*1.1;
    }
    else if(p<0)
    {
        _radius = _radius*0.9;
    }
//    CELL::real    persent =   p > 0 ? 1.1f : 0.9f;
//    CELL::Ray     ray     =   createRayFromScreen(point.x, point.y);
//    CELL::float3  pos     =   ray.getOrigin();
//    float   tm      =   abs((pos.y - 0) / ray.getDirection().y);
//    CELL::float3  center  =   ray.getPoint(tm);
//    center.y = 0;

//    scaleCameraByPos(center, persent);
}

