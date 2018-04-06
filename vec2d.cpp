#define _USE_MATH_DEFINES
#include "vec2d.h"
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;

Vec2d::Vec2d()
{
    x = 0;
    y = 0;
}

Vec2d::Vec2d(double xvalue, double yvalue)
{
    x = xvalue;
    y = yvalue;
}

double Vec2d::magnitude()
{
    return sqrt(x*x + y*y);
}

void Vec2d::scale(double s)
{
    x *= s;
    y *= s;
}

void Vec2d::rotate(double radians)
{
    *this = { x * cos(radians) - y * sin(radians), x * sin(radians) + y * cos(radians) };
}

void Vec2d::translate(Vec2d offset)
{
    x += offset.x;
    y += offset.y;
}

bool Vec2d::equals(Vec2d other, double threshold)
{
    return fabs(x - other.x) < threshold && fabs(y - other.y) < threshold;
}

Vec2d operator-(Vec2d p1, Vec2d p2)
{
    return Vec2d {p1.x - p2.x, p1.y - p2.y };
}

Vec2d operator+(Vec2d p1, Vec2d p2)
{
    return Vec2d {p1.x + p2.x, p1.y + p2.y };
}

Vec2d operator*(Vec2d v, double s)
{
    return Vec2d { v.x * s, v.y * s };
}

Vec2d operator*(double s, Vec2d v)
{
    return Vec2d { v.x * s, v.y * s };
}

double wallCollisionTime(Vec2d pos, Vec2d vel, double minx, double maxx, double miny, double maxy, double radius, Vec2d& collidePos, double& wallNormal)
{
    Vec2d wallNormalVec { 0, 0};

    if ((pos.x < minx + radius) ||
        (pos.x > maxx - radius) ||
        (pos.y < miny + radius) ||
        (pos.y > maxy - radius))
    {
        collidePos = pos;

        if (pos.x < minx + radius)
        {
            wallNormalVec.x = 1;
            collidePos.x = minx + radius;
        }

        if (pos.x > maxx - radius)
        {
            wallNormalVec.x = -1;
            collidePos.x = maxx - radius;
        }

        if (pos.y < miny + radius)
        {
            wallNormalVec.y = 1;
            collidePos.y = miny + radius;
        }

        if (pos.y > maxy - radius)
        {
            wallNormalVec.y = -1;
            collidePos.y = maxy - radius;
        }

        wallNormal = atan2(wallNormalVec.y, wallNormalVec.x);

        return 0;
    }

    double xtime = std::numeric_limits<double>::max();
    double ytime = std::numeric_limits<double>::max();

    if (vel.x < 0)
    {
        // check min x
        double wallDist = minx + radius - pos.x;  // it must be that wallDist <= 0
        xtime = wallDist / vel.x;                 // therefore xtime >= 0
        wallNormalVec.x = 1;
    }
    else if (vel.x > 0)
    {
        // check max x
        double wallDist = maxx - radius - pos.x;  // it must be that wallDist >= 0
        xtime = wallDist / vel.x;                 // therefore xtime >= 0
        wallNormalVec.x = -1;
    }
    // else xtime still is max double and wallNormalVec.x = 0

    if (vel.y < 0)
    {
        // check min y
        double wallDist = miny + radius - pos.y; // it must be that wallDist <= 0
        ytime = wallDist / vel.y;                // therefore xtime >= 0
        wallNormalVec.y = 1;
    }
    else if (vel.y > 0)
    {
        double wallDist = maxy - radius - pos.y; // it must be that wallDist >= 0
        ytime = wallDist / vel.y;                // therefore xtime >= 0
        wallNormalVec.y = -1;
    }
    // else ytime still is max double and wallNormalVec.y = 0

    double collideTime;

    if (xtime < ytime)
    {
        // x collision will be first, so kill the y normal
        wallNormalVec.y = 0;
        collideTime = xtime;
    }
    else if (ytime < xtime)
    {
        // y collision will be first, so kill the x normal
        wallNormalVec.x = 0;
        collideTime = ytime;
    }
    else
    {
        // simultaneous (corner) collision
        collideTime = ytime; // which also == xtime
    }

    collidePos = pos + collideTime * vel;

    if (collidePos.x < minx + radius)
    {
        collidePos.x = minx + radius;
    }

    if (collidePos.x > maxx - radius)
    {
        collidePos.x = maxx - radius;
    }

    if (collidePos.y < miny + radius)
    {
        collidePos.y = miny + radius;
    }

    if (collidePos.y > maxy - radius)
    {
        collidePos.y = maxy - radius;
    }

    wallNormal = atan2(wallNormalVec.y, wallNormalVec.x);


    return collideTime;
}

double collisionTime(Vec2d p0, Vec2d vp, Vec2d q0, Vec2d vq, Vec2d& p, Vec2d& q, double collisionDist)
{
    double parallelThreshold = 0.0001;

    double dvx = vp.x - vq.x;
    double dvy = vp.y - vq.y;
    double dpx = p0.x - q0.x;
    double dpy = p0.y - q0.y;

    double a = dvx*dvx + dvy*dvy;  // non-negative
    double b = 2 * dpx * dvx + 2 * dpy * dvy;
    double c = dpx * dpx + dpy * dpy - collisionDist*collisionDist;

    if (a < parallelThreshold)
    {
        p = p0;
        q = q0;
        return -1;
    }

    double sq = b*b - 4*a*c;

    if (sq < 0)
    {
        // never collides
        p = p0;
        q = q0;
        return -1;
    }

    sq = sqrt(sq);

    double t1 = (-b+sq)/(2*a);
    double t2 = (-b-sq)/(2*a);

    if (t1 < 0 & t2 < 0)
    {
        // collision happens in the past
        p = p0;
        q = q0;
        return std::max(t1,t2);  // return the closest time to the present (will be negative)
    }

    if (t1 < 0 && t2 >= 0)
    {
        cout << "I don't think this can happen" << endl;
        p = p0;
        q = q0;
        return 0;
    }

    if (t2 < 0 && t1 >= 0)
    {

        p = p0;
        q = q0;

        if (-t2 > t1)
        {
            // escapeable collision (we are embedded, but pointed so as to decrease the
            // embedding
            //cout << "Escapeable " << -t2 << " " << t1 << endl;
            return -1;
        }

        // non escapeable
        //cout << "Non Escapeable " << -t2 << " " << t1 << endl;

        return 0;
    }

    double t = std::min(t1, t2);

    p = p0 + t * vp;
    q = q0 + t * vq;

    return t;
}



