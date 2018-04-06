#ifndef VEC2D_H
#define VEC2D_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <string>

class Vec2d
{
public:
    double x;   // fields (also member) (variables that are part of the class)
    double y;

    // constructor
    Vec2d();
    Vec2d(double xvalue, double yvalue);

    // methods
    double magnitude();

    void scale(double s);
    void rotate(double radians);
    void translate(Vec2d offset);

    Vec2d rotated(double radians) { Vec2d res = *this; res.rotate(radians); return res; }
    Vec2d scaled(double s) { Vec2d res = *this; res.scale(s); return res; }
    Vec2d translated(Vec2d offset) { Vec2d res = *this; res.translate(offset); return res; }

    bool equals(Vec2d other, double threshold);

};

// some handy operators
Vec2d operator+(Vec2d p1, Vec2d p2);
Vec2d operator-(Vec2d p1, Vec2d p2);
Vec2d operator*(Vec2d p1, double s);
Vec2d operator*(double s, Vec2d p1);
bool  operator== (Vec2d p1, Vec2d p2);

double collisionTime(Vec2d p1, Vec2d v1, Vec2d p2, Vec2d v2, Vec2d& pc, Vec2d& qc, double collisionDist);
double wallCollisionTime(Vec2d pos, Vec2d vel, double minx, double maxx, double miny, double maxy, double radius, Vec2d& collidePos, double& wallNormal);


#endif // VEC2D_H
