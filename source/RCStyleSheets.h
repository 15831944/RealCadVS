#ifndef RCSTYLESHEETS
#define RCSTYLESHEETS

#ifdef _MAC
#define RCDarkStyle QStringLiteral(":/styles/mac/Dark.qss")
#define RCLightStyle QStringLiteral(":/styles/mac/Light.qss")
#define RCDarkStyle2x QStringLiteral(":/styles/win/2x/Dark.qss")
#define RCLightStyle2x QStringLiteral(":/styles/win/2x/Light.qss")
#else
#define RCDarkStyle QStringLiteral(":/styles/win/Dark.qss")
#define RCLightStyle QStringLiteral(":/styles/win/Light.qss")
#define RCDarkStyle2x QStringLiteral(":/styles/win/2x/Dark.qss")
#define RCLightStyle2x QStringLiteral(":/styles/win/2x/Light.qss")
#endif

#endif // RCSTYLESHEETS

