#define HMD_CMD			JOY_CAL_READVONLY

#define HMD_STATUS		HMD_CMD
#define HMD_CENTER		JOY_CAL_READXONLY
#define HMD_STEREOOFF	JOY_CAL_READYONLY
#define HMD_STEREOON	JOY_CAL_READZONLY
#define HMD_POWERDOWN	JOY_CAL_READRONLY
#define HMD_POWERUP		JOY_CAL_READUONLY
#define HMD_FLIP_YAW	JOY_CAL_READXONLY
#define HMD_FLIP_PITCH	JOY_CAL_READYONLY
#define HMD_FLIP_ROLL	JOY_CAL_READZONLY
#define HMD_CONFIGURE	JOY_CAL_READRONLY
#define HMD_JOYEMU		JOY_CAL_READUONLY

#define HMD_STEREOBIT		0x01
#define HMD_POWERBIT		0x02
#define HMD_FLIPY			0x04
#define HMD_FLIPP			0x08
#define HMD_FLIPR			0x10
#define HMD_JOY_EMULATION	0x20

#define VXD_NOTLOADED		0x10000
#define VXD_NOVIPSTR		0x20000
#define VXD_BADCONFIG		0x40000
#define VXD_BADALLOC		0x80000
#define VXD_NODETECT		0x100000
#define VXD_NORESET			0x200000

DWORD	HMDStatus ( DWORD	HMDId );
UINT	HMDZeroset ( DWORD	HMDId );
UINT	HMDMonoVideo ( DWORD HMDId );
UINT	HMDStereoVideo ( DWORD HMDId );
UINT	HMDPowerDown ( DWORD HMDId );
UINT	HMDPowerUp ( DWORD HMDId );
UINT	HMDFlipYaw ( DWORD HMDId );
UINT	HMDFlipPitch ( DWORD HMDId );
UINT	HMDFlipRoll ( DWORD HMDId );

VOID	HMDPoll( DWORD HMDId, DWORD *Roll, DWORD *Pitch, DWORD *Yaw );
VOID	HANDPoll( DWORD HANDId, DWORD *Roll, DWORD *Pitch, DWORD *buttons );

UINT	CheckDeviceStr( DWORD JoyId, LPBYTE IdStr );
BOOL	GetStringFromRegistry(int id, char *regKey, unsigned char *string, int length);
