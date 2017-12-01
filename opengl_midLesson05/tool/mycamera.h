#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QObject>
#include <tool/CELLMath.hpp>

class MYCAMERA : public QObject
{
    Q_OBJECT
public:
    explicit MYCAMERA(QObject *parent = 0);
    enum KEYMODE
    {
        KEY_NULL,
        KEY_A,
        KEY_S,
        KEY_D,
        KEY_W,
    };
    enum MOUSEMODE
    {
        MOUSE_NULL,
        MOUSE_LEFTDOWN,
        MOUSE_RIGHTDOWN,
        MOUSE_LEFTUP,
        MOUSE_RIGHTUP,
        MOUSE_WHEEL,
    };

    CELL::float3     _eye;
    CELL::float3     _up;
    CELL::float3     _right;

    CELL::float3     _target;
    CELL::float3     _dir;

    float           _radius;
    CELL::matrix4   _matView;
    CELL::matrix4   _matProj;
    CELL::matrix4   _matWorld;
    CELL::float2    _viewSize;

    float           _yaw;

public:
    float   getRadius() const;
    void    setRadius(float val);

    CELL::float3 getEye() const;
    /**
    *   设置眼睛的位置
    */
    void    setEye(CELL::float3 val);
    /**
    *   计算方向
    */
    void    calcDir();

    CELL::float3 getTarget() const;

    void    setTarget(CELL::float3 val);

    CELL::float3 getUp() const;
    void    setUp(CELL::float3 val);
    CELL::float3  getDir() const;

    CELL::float3  getRight() const;
    void    update();

    void    setViewSize(const CELL::float2& viewSize);
    void    setViewSize(float x,float y);

    CELL::float2  getViewSize();

    void    setProject(const CELL::matrix4& proj);
    const CELL::matrix4& getProject() const;
    const CELL::matrix4&  getView() const;

    void    perspective(float fovy, float aspect, float zNear, float zFar);

     /**
    *   世界坐标转化为窗口坐标
    */
    bool    project( const CELL::float4& world, CELL::float4& screen )
;


    /**
    *   世界坐标转化为窗口坐标
    */
    CELL::float2  worldToScreen( const CELL::float3& world);

    /**
    *   窗口坐标转化为世界坐标
    */
    CELL::float3  screenToWorld(const CELL::float2& screen);

    CELL::float3  screenToWorld(float x,float y);


    /**
    *   窗口坐标转化为世界坐标
    */
    bool    unProject( const CELL::float4& screen, CELL::float4& world )
;

    CELL::Ray createRayFromScreen(int x,int y);
    /**
    *   下面的函数的功能是将摄像机的观察方向绕某个方向轴旋转一定的角度
    *   改变观察者的位置，目标的位置不变化
    */
    void    rotateView(float angle);
    void    scaleCameraByPos(const CELL::float3 &pos, float persent);
    void    rotateViewY(float angle);
    void    rotateViewX(float angle);

//!鼠标事件相关
    MOUSEMODE _type;

public slots:
    void reciveKeyEvent(KEYMODE type);
    void reciveMouseEvent(MOUSEMODE type, CELL::int2 pS, CELL::int2 pE);
    void reciveWheelEvent(MOUSEMODE type,int p,CELL::int2 point);
};

#endif // MYCAMERA_H
