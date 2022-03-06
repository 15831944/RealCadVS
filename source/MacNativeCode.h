#ifndef _MACNATIVECODE_INCLUDE_
#define _MACNATIVECODE_INCLUDE_

#include <QtCore>

#ifdef Q_OS_MAC
class CocoaInitializer
{
    public:
        CocoaInitializer();
        ~CocoaInitializer();

    private:
        class Private;
        Private* d;
};

void fixMacMenus();
int GetUserNameMac(QString &name);
#endif

#endif
