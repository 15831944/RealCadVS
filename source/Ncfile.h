#ifndef _NCFILE_H_
#define _NCFILE_H_
//
//  Low level CAD File read/write functions
//

typedef unsigned char Byte;

class GeometryDatabase;

class CadFile
  {protected:
     FILE *fp;
     char *buffer,*memoryfilebuffer;
     RCCHAR *rcbuffer,*rcmemoryfilebuffer;
     long memoryfileposition,memoryfilelength;
     long rcmemoryfileposition,rcmemoryfilelength;
     long version,flength;
   public:
     CadFile();
     ~CadFile();
     virtual int open(const RCCHAR *,const RCCHAR *);
     virtual int open(const RCCHAR *,const char *);
     int open(char *,int length);
     int open(RCCHAR *,int length);
     long position(void);
     void position(long);
     long length(void)  {  return flength;  }
     int readheader(void);
     int readheaderimage(unsigned char *&image, int &width, int &height, int &bpp);
     long getversion(void) {  return version;  }
     virtual long getentityindex(Entity *e);
     virtual int close(void);
     virtual int blockwrite(void *,int);
     virtual int blockread(void *,int);
     virtual int closed(void) { return fp == NULL;  }
     virtual int read(Byte *);
     virtual int read(short *);
     virtual int read(long * ,unsigned int size=4);
     virtual int read(RCLONGLONG *);
     virtual int read(RCLONG *);
     virtual int read(RCCHAR **);
     virtual int read(char **);
     virtual int read(double *);
     virtual int read(float *);
     virtual int read(BitMask *);
     virtual int read(Entity **);
     virtual int write(const Byte *);
     virtual int write(const short *);
     virtual int write(const long *,unsigned int size=4);
     virtual int write(const RCLONGLONG *);
     virtual int write(const RCCHAR *);
     virtual int write(const char *);
     virtual int write(const double *);
     virtual int write(const float *);
     virtual int write(const BitMask *);
     virtual int write(const Entity **);
     Entity *nextentity(Database *,Values *v = 0);
  };

class UndoFile : public CadFile
  {public:
     void clear(void);
     int previous(void);
     int uwrite(const Entity **);
     int uread(Entity **);
     int uwrite(const GeometryDatabase **);
     int uread(GeometryDatabase **);
     long getentityindex(Entity *e);
#ifdef USING_WIDGETS
     //int write(const long *);
     //int read(long *);
#endif
  };

#if defined(WIN32)

typedef IStorage *LPSTORAGE;
typedef IStream *LPSTREAM;

class StorageFile : public CadFile
  {protected:
     LPSTORAGE storage;
     LPSTREAM stream;
   public:
     StorageFile();
     ~StorageFile();
     int open(const char *,const char *);
#if defined(_WIN32_WCE)
     int open(const TCHAR *,const TCHAR *);
#endif
     int close(void);
     int blockwrite(void *,int);
     int blockread(void *,int);
     int closed(void) { return storage == NULL;  }
     int read(Byte *);
     int read(short *);
     int read(long *);
     int read(char **);
     int read(double *);
     int read(BitMask *);
     int read(Entity **);
     int write(const Byte *);
     int write(const short *);
     int write(const long *);
     int write(const char *);
     int write(const double *);
     int write(const BitMask *);
     int write(const Entity **);
  };
#endif

#endif
