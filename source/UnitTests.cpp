
#include "ncwin.h"

/**
 * @brief testFindFirst
 * @return
 *
 * Unit test for the RCFINDFIRST,RCFINDNEXT and RCFINDCLOSE wrappers.
 *
 */
int testFindFirst()
{
    RC_finddata_t fd1,fd2;

    int handle1 = _findfirst(_RCT("."),&fd1);
    int handle2 = _findfirst(_RCT("/Users/ken"),&fd2);
    while (1)
    {
        qDebug() << "fd1 file : " << QString(fd1.name);
        if (_findnext(handle1,&fd1) != 0)
        {
             _findclose(handle1);
             handle1 = -1;
        }
        qDebug() << "fd2 file : " << QString(fd2.name);
        if (_findnext(handle2,&fd2) != 0)
        {
             _findclose(handle2);
             handle2 = -1;
        }
        if(handle1 == -1 && handle2 == -1)
            break;
    }
    return 0;
}
