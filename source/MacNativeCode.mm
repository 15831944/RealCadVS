#include <QtCore>
#ifdef Q_OS_MAC
#include <AppKit/AppKit.h>
#include <Cocoa/Cocoa.h>
#include "MacNativeCode.h"
#include <Foundation/Foundation.h>

class CocoaInitializer::Private
{
    public:
        NSAutoreleasePool* autoReleasePool_;
};

CocoaInitializer::CocoaInitializer()
{
    d = new CocoaInitializer::Private();
    NSApplicationLoad();
    d->autoReleasePool_ = [[NSAutoreleasePool alloc] init];
}

CocoaInitializer::~CocoaInitializer()
{
    [d->autoReleasePool_ release];
    delete d;
}

void fixMacMenus()
{
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"NSDisableDictationMenuItem"];
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"NSDisableCharacterPaletteMeniItem"];

}

int GetUserNameMac(QString &name)
{
    NSString *userName = NSUserName();
    //NSString *fullUserName = NSFullUserName();
    name = QString(userName.cString);
    return name.length();
}
#else

#endif
