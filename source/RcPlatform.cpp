/*! platform dependent code for RealCAD
 *
 */
#include <string.h>
#include <locale.h>
#include "ncwin.h"
#include "qtdefs.h"
#include "RCPlatform.h"
#include <QtTest/QtTest>

/*! Using the standard library to get the local string.
 * The actual returned string is platform dependant
 * so fix it in the caller
 * Return:
 * if successful modifes the input string with the result
 *
 */
#ifdef _WIN32
void GetLanguageString( char *lang )
{
    char *locale = setlocale( LC_ALL, "" );
    char *ub = strstr(locale,"_");
    if(ub != 0)
        strcpy(lang,locale);
}
#endif

void convertLinePatternFileName(RCCHAR *filename, RCCHAR *patternname)
{
    // filename is the windows path for the pattern file
    // pattername is the mac (Qt style) path for the pattern file

    // convert filename to Qt style
    QString qtFilename(filename),qtPathname("Line-Styles/");
    // get the last slash
    QString namepart,sep("\\");
    int pos = qtFilename.lastIndexOf(sep)+1;
    if(pos > 0)
        namepart = qtFilename.mid(pos);
    else
    {
        sep = "/";
        pos = qtFilename.lastIndexOf(sep)+1;
        if(pos >= 0)
            namepart = qtFilename.mid(pos);
        else
            namepart = qtFilename;
    }
    qtPathname += namepart;
    // get the Qt platform path for the line patterns
    QString qtPatternname(home.getpublicfilename(qtPathname.data()));
    qtPatternname = QDir::toNativeSeparators(qtPatternname);
    strcpy(patternname,qtPatternname);
}

bool IsZero( ::Point p, const double Tol )
{
        return (p.x >= -Tol && p.y >= -Tol && p.z >= -Tol
                &&
                p.x <=  Tol && p.y <=  Tol && p.z <=  Tol);
}

bool IsEqual(const ::Point &Pos1, const ::Point &Pos2, const double Tol )
{
    double dx = fabs( Pos1.x - Pos2.x );
    if( dx > Tol ) return false;

    double dy = fabs( Pos1.y - Pos2.y );
    if( dy > Tol ) return false;

    double dz = fabs( Pos1.z - Pos2.z );
    if( dz > Tol ) return false;

    return (dx * dx + dy * dy + dz * dz <= Tol*Tol);

}

inline double SafeCos( double angle )
{
    return angle>1 ? 0  :  angle<-1 ? M_PI  :  acos(angle);
}

inline double SafeAtan2(double x, double y)
{
    if(x == 0 && y == 0)
        return 0.0;
    else
        return atan2(x,y);
}

double AngleBetween(
    const ::Point& v1,
    const ::Point& v2,
    const ::Point& vz
    )
{
    //::Point v1 = ::Point(.x,y,z);

    if(&vz==NULL)
        return SafeCos(v1.dot(v2));

    if (::IsZero(v1) || ::IsZero(v2) || ::IsZero(vz)) return -1;
    if (IsEqual(v1, v2)) return 0;

    ::Point uy  = vz.cross(v1).normalize();
    double dx  = v2.dot(v1);
    double dy  = v2.dot(uy);
    double ang = SafeAtan2(dy, dx);
    if(ang < 0) ang +=  2*M_PI;
    return ang;
}

