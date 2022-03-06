#ifndef RCPLATFORM_H
#define RCPLATFORM_H

#include "qtdefs.h"
#include "ncent.h"

extern void GetLanguageString( char *lang );
extern void convertLinePatternFileName(RCCHAR *filename, RCCHAR *pattername);
extern bool IsZero( ::Point p, const double Tol=10e-06 );
extern bool IsEqual(const ::Point &Pos1, const ::Point &Pos2, const double Tol=10e-06);
extern double SafeCos( double angle );
extern double SafeAtan2(double x, double y);
extern double AngleBetween(const ::Point& v1,const ::Point& v2,const ::Point& vz);

class Box3
{
private:
    ::Point Bmin, Bmax;

public:
    Box3(void) :
        Bmin(::Point(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX)),
        Bmax(::Point(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX))
    {
        ;// ?
    }
    Box3(const ::Point &min, const ::Point &max)
    {
        Set ( min , max ) ;
    }

    Box3(const Box3 &box)
    {
        Copy(box);
    }
    void Set( const ::Point& min,  const ::Point& max )
    {
        if ( min.x < max.x )
        {
            Bmin.x = min.x;
            Bmax.x = max.x;
        }
        else
        {
            Bmin.x = max.x;
            Bmax.x = min.x;
        }
        if ( min.y < max.y )
        {
            Bmin.y = min.y;
            Bmax.y = max.y;
        }
        else
        {
            Bmin.y = max.y;
            Bmax.y = min.y;
        }
        if ( min.z < max.z )
        {
            Bmin.z = min.z;
            Bmax.z = max.z;
        }
        else
        {
            Bmin.z = max.z;
            Bmax.z = min.z;
        }
    }

    void Get(::Point &minPos, ::Point &maxPos) const
    {
        minPos = Bmin;
        maxPos = Bmax;
    }
    ::Point GetMin() const
    {
        return Bmin;
    }
    ::Point GetMax() const
    {
        return Bmax;
    }
    void Max(::Point &maxPos) const
    {
        maxPos = Bmax;
    }
    void Min(::Point &minPos) const
    {
        minPos = Bmin;
    }
    void Copy(const Box3 &box)
    {
        Bmin = box.Bmin;
        Bmax = box.Bmax;
    }
    void Inflate(const ::Point &Pos)
    {
        if( Pos.x > Bmax.x)
            Bmax.x = Pos.x;
        if( Pos.y > Bmax.y)
            Bmax.y = Pos.y;
        if( Pos.z > Bmax.z)
            Bmax.z = Pos.z;
        //
        if( Pos.x < Bmin.x)
            Bmin.x = Pos.x;
        if( Pos.y < Bmin.y)
            Bmin.y = Pos.y;
        if( Pos.z < Bmin.z)
            Bmin.z = Pos.z;
    }
    void Inflate(const Box3 &box)
    {
        Inflate(box.Bmin);
        Inflate(box.Bmax);
    }
    void Center(::Point &Pos) const
    {
        Pos.x = (Bmin.x + Bmax.x)/2.0;
        Pos.y = (Bmin.y + Bmax.y)/2.0;
        Pos.z = (Bmin.z + Bmax.z)/2.0;
    }
    double Radius()
    {
        ::Point max,center;
        Max(max);
        Center(center);
        return ::Point(max - center).GetLength();
    }
    double Xrange() {return Bmax.x-Bmin.x;}
    double Yrange() {return Bmax.y-Bmin.y;}
    double Zrange() {return Bmax.z-Bmin.z;}
    bool Inside(const ::Point &Pos)
    {
        double res_absolute = 10e-06;
        bool status = false;
        if ( IsZero() )
            return status;
        if (
            ((Pos.x <= Bmax.x+res_absolute)&&
             (Pos.y <= Bmax.y+res_absolute)&&
             (Pos.z <= Bmax.z+res_absolute))
            &&
            ((Pos.x >= Bmin.x-res_absolute)&&
             (Pos.y >= Bmin.y-res_absolute)&&
             (Pos.z >= Bmin.z-res_absolute))
            )
            status = true;

        return status;
    }
    bool Outside(const ::Point &Pos)
    {
        return (!Inside(Pos));
    }
    const Box3 &operator=(const Box3 &box)
    {
        Copy(box);
        return(*this);
    }
    bool IsZero()
    {
        return ::IsZero(Bmin) && ::IsZero(Bmax);
    }
};

class Box2
{
private:
    ::Point Bmin, Bmax;

public:
    Box2(void) :
        Bmin(::Point(RCDBL_MAX,RCDBL_MAX,0.0)),
        Bmax(::Point(-RCDBL_MAX,-RCDBL_MAX,0.0))
    {
        ;// ?
    }
    Box2(const ::Point &min, const ::Point &max)
    {
        Set ( min , max ) ;
    }

    Box2(const Box2 &box)
    {
        Copy(box);
    }
    void Set( const ::Point& min,  const ::Point& max )
    {
        if ( min.x < max.x )
        {
            Bmin.x = min.x;
            Bmax.x = max.x;
            Bmin.z = 0.0;
            Bmax.z = 0.0;
        }
        else
        {
            Bmin.x = max.x;
            Bmax.x = min.x;
            Bmin.z = 0.0;
            Bmax.z = 0.0;
        }
        if ( min.y < max.y )
        {
            Bmin.y = min.y;
            Bmax.y = max.y;
            Bmin.z = 0.0;
            Bmax.z = 0.0;
        }
        else
        {
            Bmin.y = max.y;
            Bmax.y = min.y;
            Bmin.z = 0.0;
            Bmax.z = 0.0;
        }
    }

    void Get(::Point &minPos, ::Point &maxPos) const
    {
        minPos = Bmin;
        maxPos = Bmax;
    }
    ::Point GetMin() const
    {
        return Bmin;
    }
    ::Point GetMax() const
    {
        return Bmax;
    }
    void Max(::Point &maxPos) const
    {
        maxPos = Bmax;
    }
    void Min(::Point &minPos) const
    {
        minPos = Bmin;
    }
    void Copy(const Box2 &box)
    {
        Bmin = box.Bmin;
        Bmax = box.Bmax;
    }
    void Inflate(const ::Point &Pos)
    {
        if( Pos.x > Bmax.x)
            Bmax.x = Pos.x;
        if( Pos.y > Bmax.y)
            Bmax.y = Pos.y;
        //
        if( Pos.x < Bmin.x)
            Bmin.x = Pos.x;
        if( Pos.y < Bmin.y)
            Bmin.y = Pos.y;
    }
    void Inflate(const Box2 &box)
    {
        Inflate(box.Bmin);
        Inflate(box.Bmax);
    }
    void Center(::Point &Pos) const
    {
        Pos.x = (Bmin.x + Bmax.x)/2.0;
        Pos.y = (Bmin.y + Bmax.y)/2.0;
        Pos.z = 0.0;
    }
    double Radius()
    {
        ::Point max,center;
        Max(max);
        Center(center);
        return ::Point(max - center).GetLength();
    }
    double Xrange() {return Bmax.x-Bmin.x;}
    double Yrange() {return Bmax.y-Bmin.y;}
    bool Inside(const ::Point &Pos)
    {
        double res_absolute = 10e-06;
        bool status = false;
        if ( IsZero() )
            return status;
        if (
            ((Pos.x <= Bmax.x+res_absolute)&&
             (Pos.y <= Bmax.y+res_absolute))
            &&
            ((Pos.x >= Bmin.x-res_absolute)&&
             (Pos.y >= Bmin.y-res_absolute))
            )
            status = true;

        return status;
    }
    bool Outside(const ::Point &Pos)
    {
        return (!Inside(Pos));
    }
    const Box2 &operator=(const Box2 &box)
    {
        Copy(box);
        return(*this);
    }
    bool IsZero()
    {
        return ::IsZero(Bmin) && ::IsZero(Bmax);
    }
};

#endif // RCPLATFORM_H
