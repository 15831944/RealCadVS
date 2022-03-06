
#include "ncwin.h"
#include <stdio.h>
#include <errno.h>
#include <limits.h>
//#include <CoreFoundation/CoreFoundation.h>

#define BUFFER_SIZE 32256

CadFile::CadFile()
{ fp = NULL;
  memoryfilebuffer = 0;
  memoryfilelength = 0;
  memoryfileposition = 0;
  //
  rcmemoryfilebuffer = 0;
  rcmemoryfilelength = 0;
  rcmemoryfileposition = 0;
  //rcbuffer = new RCCHAR[BUFFER_SIZE];
  buffer = new char[BUFFER_SIZE];
}

CadFile::~CadFile()
{ if (fp != NULL) fclose(fp);
  delete [] buffer;
  //delete [] rcbuffer;
}

int CadFile::open(char *mfb,int l)
{ if (mfb == 0)
    return 0;
  if (fp != NULL) 
    {  fclose(fp);
       fp = 0;
    }
  memoryfilebuffer = mfb;
  memoryfilelength = l;
  memoryfileposition = 0;
  //
  rcmemoryfilebuffer = 0;
  rcmemoryfilelength = 0;
  rcmemoryfileposition = 0;
  return 1;
}

int CadFile::open(RCCHAR *mfb,int l)
{ if (mfb == 0)
    return 0;
  if (fp != NULL)
    {  fclose(fp);
       fp = 0;
    }
  rcmemoryfilebuffer = mfb;
  rcmemoryfilelength = l;
  rcmemoryfileposition = 0;
  //
  memoryfilebuffer = 0;
  memoryfilelength = 0;
  memoryfileposition = 0;
  return 1;
}

int CadFile::open(const RCCHAR *name,const char *mode)
{
    return open( name, CHAR2RCCHR(mode));
}

int CadFile::open(const RCCHAR *name,const RCCHAR *md)
{RCCHAR filename[600],path[600],mode[20];
int error1=0,errno2=0,errno3=0;
  strcpy(mode,md);
  strcpy(filename,name);
  if (strrchr(filename,'.') != 0 && strchr(strrchr(filename,'.'),'#') != 0)
    strchr(strrchr(filename,'.'),'#')[0] = 0;

  strcpy(filename,QDir::toNativeSeparators(QString(filename)));

  if (strlen(filename) == 0)
    fp = NULL;
  else if ((fp = fopen(filename,mode)) != NULL)
    {  if (buffer != NULL) 
         setvbuf(fp,buffer,_IOFBF,BUFFER_SIZE);
    }
  else
    {
       error1 = errno;
       _getcwd(path,600);
       //strcat(path,"\\");
       strcat(path,"/");
       strcat(path,filename);
       strcpy(path,QDir::toNativeSeparators(QString(path)));
       if ((fp = fopen(path,mode)) != NULL)
         {  if (buffer != NULL) 
              setvbuf(fp,buffer,_IOFBF,BUFFER_SIZE);
         }
       else           
         {
           errno2 = errno;
           strcpy(filename,QDir::toNativeSeparators(QString(filename)));
           if ((fp = fopen(home.getpublicfilename(filename),mode)) != NULL)
              {  if (buffer != NULL) 
                   setvbuf(fp,buffer,_IOFBF,BUFFER_SIZE);
              }
           else
               errno3 = errno;
         }
    }

  if (fp != 0 && strchr(mode,'r') != 0)
    {  fseek(fp,0L,SEEK_END);
       flength = ftell(fp);
       fseek(fp,0L,SEEK_SET);
    }

  return fp != NULL;

}

int CadFile::readheader(void)
{
  char *header;
  if (! read(&header)) 
    {  delete [] header;
       return 0;
    }
  if (strcmp(header,"CAD For Windows : Dedicated to Irina and Spot.") != 0 &&
      strcmp(header,"The Authentic CAD File.") != 0)
    {  delete [] header;
       return 0;
    }
  read(&version);
  // KMJ read or skip thumbnail/preview comes here
  delete [] header;
  if(version >= 4)
  {
      // KMJ write thumbnail/preview comes here
      // write type number just in case we want to change this in the future
      // type number -1 means that the preview has not been saved
      // type zero is rgb format image data
      // image is stored low to high pixel order (upside down)
      long previewType=-1;
      if( !read(&previewType))
          return 0;
      if(previewType >= 0)
      {
          // read the width and height of the preview so we can change it in the future
          long previewWidth = 0;
          if( !read(&previewWidth))
              return 0;
          long previewHeight = 0;
          if( !read(&previewHeight))
              return 0;
          // read the number of bytes in a pixel
          long previewBPP = 4;
          if( !read(&previewBPP))
              return 0;
          // read the preview image data
          // the length of this data is width * height * bytes per pixel
          if(previewBPP == 0)
              return 1;
          char *image = new char[previewWidth *previewHeight * 4];
          if( !blockread(image,previewWidth *previewHeight * 4))
          {
              delete [] image;
              return 0;
          }
          // not using the preview image yet.
          delete [] image;
      }
      //
  }
  //
  return 1;
}

/**
 * @brief CadFile::readheaderimage
 * @param image
 * @param width
 * @param height
 * @param bpp
 * @return type of preview
 * -1 = no preview present or error reading the preview
 *  0 = screen shot type preview
 *  1 = drawn type preview (saved as a figure)
 */
int CadFile::readheaderimage(unsigned char *&image, int &width, int &height, int &bpp)
{char *header;
  if (! read(&header))
    {  delete [] header;
       return 0;
    }
  if (strcmp(header,"CAD For Windows : Dedicated to Irina and Spot.") != 0 &&
      strcmp(header,"The Authentic CAD File.") != 0)
    {  delete [] header;
       return 0;
    }
  read(&version);
  // KMJ read or skip thumbnail/preview comes here
  delete [] header;
  long previewType=-1;
  if(version >= 4)
  {
      // KMJ write thumbnail/preview comes here
      // write type number just in case we want to change this in the future
      // type number -1 means that the preview has not been saved
      // type zero is rgb format image data
      // image is stored low to high pixel order (upside down)
      if( !read(&previewType))
          return -1;
      if(previewType >= 0)
      {
          // read the width and height of the preview so we can change it in the future
          long previewWidth = 0;
          if( !read(&previewWidth))
              return -1;
          width = previewWidth;
          long previewHeight = 0;
          if( !read(&previewHeight))
              return -1;
          height = previewHeight;
          // read the number of bytes in a pixel
          long previewBPP = 4;
          if( !read(&previewBPP))
              return -1;
          bpp = previewBPP;
          // read the preview image data
          // the length of this data is width * height * bytes per pixel
          image = new uchar[previewWidth *previewHeight * 4];
          if( !blockread(image,previewWidth *previewHeight * 4))
          {
              delete [] image;
              image = 0;
              return -1;
          }
          // not using the preview image yet.
          //delete [] image;
      }
      //
  }
  //
  return previewType;
}

long CadFile::position(void)
{ if (fp != NULL) 
    return ftell(fp);
  else if (memoryfilebuffer != 0) 
    return memoryfileposition;
  else
    return -1;
}
 
void CadFile::position(long pos)
{ if (fp != NULL)
    fseek(fp,pos,0);
  else if (memoryfilebuffer != 0)
    memoryfileposition = pos;
}
 
int CadFile::close(void)
{int status;
  if (fp != NULL)
    {  status = fclose(fp) == 0;
       if (! status)
         {  fseek(fp,0L,0);
            fclose(fp);
         }
    }
  else
    status = 1;
  fp = NULL;
  memoryfilebuffer = 0;
  return status;
}

int CadFile::read(Byte *b)
{ if (fp != 0)
    {  if (fread(b,1,1,fp) != 1) return 0;
    }
  else if (memoryfilebuffer != 0)
    {  if (memoryfileposition+1 > memoryfilelength) return 0;
       *b = memoryfilebuffer[memoryfileposition++];
    }
  return 1;
}

int CadFile::read(short *i)
{unsigned char buffer[2];
  if (fp != 0)
    {  if (fread(buffer,2,1,fp) != 1) return 0;
    }
  else if (memoryfilebuffer != 0)
    {  if (memoryfileposition+2 > memoryfilelength) return 0;
       buffer[0] = memoryfilebuffer[memoryfileposition++];
       buffer[1] = memoryfilebuffer[memoryfileposition++];
    }
  *i = short((short(buffer[1]) << 8) | short(buffer[0]));
  return 1;
}

int CadFile::read(long *l, unsigned int size)
{
#ifdef USING_WIDGETS
    //unsigned char buffer[8];
    if(size == 8)
    {
        unsigned char buffer[8] = { };
        if (fp != 0)
        {
            if (fread(buffer,8,1,fp) != 1) return 0;
        }
        else
        {
            if (memoryfileposition+8 > memoryfilelength) return 0;
             buffer[0] = memoryfilebuffer[memoryfileposition++];
             buffer[1] = memoryfilebuffer[memoryfileposition++];
             buffer[2] = memoryfilebuffer[memoryfileposition++];
             buffer[3] = memoryfilebuffer[memoryfileposition++];
             buffer[4] = memoryfilebuffer[memoryfileposition++];
             buffer[5] = memoryfilebuffer[memoryfileposition++];
             buffer[6] = memoryfilebuffer[memoryfileposition++];
             buffer[7] = memoryfilebuffer[memoryfileposition++];
        }
        *l = (long(buffer[7]) << 56) | (long(buffer[6]) << 48) | (long(buffer[5]) << 40) | (long(buffer[4]) << 32) | (long(buffer[3]) << 24) | (long(buffer[2]) << 16) | (long(buffer[1]) << 8) | long(buffer[0]);
        return 1;
    }
    else if(size == 4)
    {
        unsigned char buffer[4] = { };
        if (fp != 0)
        {
            if (fread(buffer,4,1,fp) != 1) return 0;
        }
        else
        {
            if (memoryfileposition+4 > memoryfilelength) return 0;
             buffer[0] = memoryfilebuffer[memoryfileposition++];
             buffer[1] = memoryfilebuffer[memoryfileposition++];
             buffer[2] = memoryfilebuffer[memoryfileposition++];
             buffer[3] = memoryfilebuffer[memoryfileposition++];
        }
        *l = (RCLONG(buffer[3]) << 24) | (RCLONG(buffer[2]) << 16) | (RCLONG(buffer[1]) << 8) | RCLONG(buffer[0]);
        return 1;
    }
#else
    unsigned char buffer[4];
  if (fp != 0)
    {  if (fread(buffer,4,1,fp) != 1) return 0;
    }
  else
    {  if (memoryfileposition+4 > memoryfilelength) return 0;
       buffer[0] = memoryfilebuffer[memoryfileposition++];
       buffer[1] = memoryfilebuffer[memoryfileposition++];
       buffer[2] = memoryfilebuffer[memoryfileposition++];
       buffer[3] = memoryfilebuffer[memoryfileposition++];
    }
  // KMJ mod for Mac
  *i = (RCLONG(buffer[3]) << 24) | (RCLONG(buffer[2]) << 16) | (RCLONG(buffer[1]) << 8) | RCLONG(buffer[0]);
  //*i = (long(buffer[3]) << 24) | (long(buffer[2]) << 16) | (long(buffer[1]) << 8) | long(buffer[0]);
  return 1;
#endif
}

int CadFile::read(RCLONGLONG *i)
{
    unsigned char buffer[8] = { };
    if (fp != 0)
    {
        if (fread(buffer,8,1,fp) != 1) return 0;
    }
    else
    {
        if (memoryfileposition+8 > memoryfilelength) return 0;
         buffer[0] = memoryfilebuffer[memoryfileposition++];
         buffer[1] = memoryfilebuffer[memoryfileposition++];
         buffer[2] = memoryfilebuffer[memoryfileposition++];
         buffer[3] = memoryfilebuffer[memoryfileposition++];
         buffer[4] = memoryfilebuffer[memoryfileposition++];
         buffer[5] = memoryfilebuffer[memoryfileposition++];
         buffer[6] = memoryfilebuffer[memoryfileposition++];
         buffer[7] = memoryfilebuffer[memoryfileposition++];
    }
    *i = (RCLONGLONG(buffer[7]) << 56) | (RCLONGLONG(buffer[6]) << 48) | (RCLONGLONG(buffer[5]) << 40) | (RCLONGLONG(buffer[4]) << 32) | (RCLONGLONG(buffer[3]) << 24) | (RCLONGLONG(buffer[2]) << 16) | (RCLONGLONG(buffer[1]) << 8) | RCLONGLONG(buffer[0]);
    return 1;
}

int CadFile::read(RCLONG *i)
{
    unsigned char buffer[4];
  if (fp != 0)
    {  if (fread(buffer,4,1,fp) != 1) return 0;
    }
  else
    {  if (memoryfileposition+4 > memoryfilelength) return 0;
       buffer[0] = memoryfilebuffer[memoryfileposition++];
       buffer[1] = memoryfilebuffer[memoryfileposition++];
       buffer[2] = memoryfilebuffer[memoryfileposition++];
       buffer[3] = memoryfilebuffer[memoryfileposition++];
    }
  *i = (RCLONG(buffer[3]) << 24) | (RCLONG(buffer[2]) << 16) | (RCLONG(buffer[1]) << 8) | RCLONG(buffer[0]);
  return 1;
}

int CadFile::read(BitMask *bitmask)
{BitMask *bm;
  if ((bm = BitMask::load(this)) != NULL)
    {
       *bitmask = *bm;
       delete bm;
       return 1;
    }
  else
    return 0;
}

int CadFile::read(char **s)
{short length;
  read(&length);
  //if (length < 0) 
  //  return 0;
  if ((*s = new char[length&0xffff]) == NULL) 
    return 0;
  if (fp != 0)
    {  if (fread(*s,length&0xffff,1,fp) != 1) 
         return 0;
    }
  else
    {  if (memoryfileposition+(length&0xffff) > memoryfilelength) 
         return 0;
       memcpy(*s,memoryfilebuffer+memoryfileposition,length&0xffff);
       memoryfileposition += (length&0xffff);
    } 
  return 1;
}

int CadFile::read(RCCHAR **s)
{
  char *cs;
  short length;
  read(&length);
  //if (length < 0) 
  //  return 0;
  if ((*s = new RCCHAR[length&0xffff]) == NULL) 
    return 0;
  if (fp != 0)
    {
       if (fread(*s,(sizeof(RCCHAR)*length)&0xffff,1,fp) != 1)
          return 0;
    }
  else
    {  if (memoryfileposition+((sizeof(RCCHAR)*length)&0xffff) > memoryfilelength)
         return 0;
       memcpy(*s,memoryfilebuffer+memoryfileposition,(sizeof(RCCHAR)*length)&0xffff);
       memoryfileposition += ((sizeof(RCCHAR)*length)&0xffff);
    } 
  return 1;
}

int CadFile::read(double *x)
{ if (fp != 0)
    {
        if (fread((char*)x,8,1,fp) != 1) return 0;
    }
  else
    {  if (memoryfileposition + 8 > memoryfilelength) return 0;
       memcpy(x,memoryfilebuffer+memoryfileposition,8);
       memoryfileposition += 8;
    } 
   return 1;
}

int CadFile::read(float *x)
{ if (fp != 0)
    {  if (fread((char *)x,4,1,fp) != 1) return 0;
    }
  else
    {  if (memoryfileposition + 4 > memoryfilelength) return 0;
       memcpy(x,memoryfilebuffer+memoryfileposition,4);
       memoryfileposition += 4;
    } 
  return 1;
}

int CadFile::read(Entity **e)
{ return read((long *)e);
}

int CadFile::write(const Byte *b)
{ if (fwrite(b,1,1,fp) != 1) return 0;
  return 1;
}

int CadFile::write(const short *i)
{unsigned char buffer[2];
  buffer[1] = (unsigned char) (((*i) >> 8) & 255);
  buffer[0] = (unsigned char) (  *i        & 255);
  if (fwrite(buffer,2,1,fp) != 1) return 0;
  return 1;
}

int CadFile::write(const long *i,unsigned int size)
{
#ifdef USING_WIDGETS
  if(size == 8)
  {
      unsigned char buffer[8];
      buffer[7] = (unsigned char) (((*i) >> 56) & 255);
      buffer[6] = (unsigned char) (((*i) >> 48) & 255);
      buffer[5] = (unsigned char) (((*i) >> 40) & 255);
      buffer[4] = (unsigned char) (((*i) >> 32) & 255);
      buffer[3] = (unsigned char) (((*i) >> 24) & 255);
      buffer[2] = (unsigned char) (((*i) >> 16) & 255);
      buffer[1] = (unsigned char) (((*i) >>  8) & 255);
      buffer[0] = (unsigned char) (  *i         & 255);
      if (fwrite(buffer,8,1,fp) != 1) return 0;
      return 1;
  }
  else if(size == 4)
  {
      unsigned char buffer[4];
      buffer[3] = (unsigned char) (((*i) >> 24) & 255);
      buffer[2] = (unsigned char) (((*i) >> 16) & 255);
      buffer[1] = (unsigned char) (((*i) >>  8) & 255);
      buffer[0] = (unsigned char) (  *i         & 255);
      if (fwrite(buffer,4,1,fp) != 1) return 0;
      return 1;
  }
#else
  unsigned char buffer[4];
  buffer[3] = (unsigned char) (((*i) >> 24) & 255);
  buffer[2] = (unsigned char) (((*i) >> 16) & 255);
  buffer[1] = (unsigned char) (((*i) >>  8) & 255);
  buffer[0] = (unsigned char) (  *i         & 255);
  clearerr(fp);
  int cnt = fwrite(buffer,4,1,fp);
  perror("file error ");
  if(cnt != 1)
      return 0;
  //if (fwrite(buffer,4,1,fp) != 1) return 0;
  return 1;
#endif
}

int CadFile::write(const RCLONGLONG *i)
{
    unsigned char buffer[8];
    buffer[7] = (unsigned char) (((*i) >> 56) & 255);
    buffer[6] = (unsigned char) (((*i) >> 48) & 255);
    buffer[5] = (unsigned char) (((*i) >> 40) & 255);
    buffer[4] = (unsigned char) (((*i) >> 32) & 255);
    buffer[3] = (unsigned char) (((*i) >> 24) & 255);
    buffer[2] = (unsigned char) (((*i) >> 16) & 255);
    buffer[1] = (unsigned char) (((*i) >>  8) & 255);
    buffer[0] = (unsigned char) (  *i         & 255);
    if (fwrite(buffer,8,1,fp) != 1) return 0;
    return 1;
}

int CadFile::write(const char *s)
{short length;
  length = short(strlen(s) + 1);
  write(&length);
  if (fwrite(s,length&0xffff,1,fp) != 1) return 0;
  return 1;
}

int CadFile::write(const RCCHAR *s)
{short length;
  length = short(RCSTRLEN(s) + 1);
  write(&length);
  //if (fwrite(s,length&0xffff,1,fp) != 1) return 0;
  if (fwrite(s,(sizeof(RCCHAR)*length)&0xffff,1,fp) != 1) return 0;
  return 1;
}

int CadFile::write(const BitMask *bitmask)
{  return bitmask->save(this);
}

int CadFile::write(const double *x) 
{ if (fwrite((char *)x,8,1,fp) != 1) return 0;
  return 1;
}

int CadFile::write(const float *x) 
{ if (fwrite((char *)x,4,1,fp) != 1) return 0;
  return 1;
}

int CadFile::write(const Entity **e)
{long v;
  if (*e == 0)
    v = 0;
  else
    v = (*e)->getindex();
  return write(&v);
}

int CadFile::blockread(void *buffer,int size)
{ 
  if (fp != 0)
    {  while (size > 0)
         {  

//long p = ftell(fp);

            if (fread(buffer,size > 1000000 ? 1000000 : size,1,fp) != 1) 
              return 0;

            size -= 1000000;
            if (size > 0)
              buffer = (char *)buffer + 1000000;
         }
    }
  else if (memoryfilebuffer != 0)
    {  if (memoryfileposition + size > memoryfilelength) return 0;
       memcpy(buffer,memoryfilebuffer+memoryfileposition,size);
       memoryfileposition += size;
    }
  return 1;
}

int CadFile::blockwrite(void *buffer,int size)
{ while (size > 0)
    {  if (fwrite(buffer,size > 1000000 ? 1000000 : size,1,fp) != 1) return 0;
       size -= 1000000;
       if (size > 0)
         buffer = (char *)buffer + 1000000;
    }
  return 1;
}

void UndoFile::clear(void)
{  if (fp != NULL) fseek(fp,0L,0);
}

int UndoFile::previous(void)
{long l,pos;
   pos = ftell(fp);
   fseek(fp,-4L,1);
   read(&l);
   fseek(fp,l,1);
   if (ftell(fp) != pos + l) return 0;
   return 1;
}

int UndoFile::uwrite(const Entity **buffer)
{ if (fwrite(buffer,sizeof(Entity *),1,fp) != 1) return 0;
  return 1;
}

int UndoFile::uread(Entity **buffer)
{ if (fread(buffer,sizeof(Entity *),1,fp) != 1) return 0;
  return 1;
}

int UndoFile::uwrite(const GeometryDatabase **buffer)
{ if (fwrite(buffer,sizeof(GeometryDatabase *),1,fp) != 1) return 0;
  return 1;
}

int UndoFile::uread(GeometryDatabase **buffer)
{ if (fread(buffer,sizeof(GeometryDatabase *),1,fp) != 1) return 0;
  return 1;
}

#ifdef USING_WIDGETS
/*
int UndoFile::write(const long *i)
{
#ifndef _MAC
    unsigned char buffer[8];
    buffer[7] = (unsigned char) (((*i) >> 56) & 255);
    buffer[6] = (unsigned char) (((*i) >> 48) & 255);
    buffer[5] = (unsigned char) (((*i) >> 40) & 255);
    buffer[4] = (unsigned char) (((*i) >> 32) & 255);
    buffer[3] = (unsigned char) (((*i) >> 24) & 255);
    buffer[2] = (unsigned char) (((*i) >> 16) & 255);
    buffer[1] = (unsigned char) (((*i) >>  8) & 255);
    buffer[0] = (unsigned char) (  *i         & 255);
    int cnt = fwrite(buffer,8,1,((UndoFile*)this)->fp);
    qDebug() << "ferror : " << ferror(fp);
    perror("file error ");
    if(cnt != 1) return 0;
    //if (fwrite(buffer,8,1,fp) != 1) return 0;
    return 1;
#else
    unsigned char buffer[4];
    buffer[3] = (unsigned char) (((*i) >> 24) & 255);
    buffer[2] = (unsigned char) (((*i) >> 16) & 255);
    buffer[1] = (unsigned char) (((*i) >>  8) & 255);
    buffer[0] = (unsigned char) (  *i         & 255);
    int cnt = fwrite(buffer,8,1,((UndoFile*)this)->fp);
    qDebug() << "ferror : " << ferror(fp);
    perror("file error ");
    if(cnt != 1) return 0;
    //if (fwrite(buffer,4,1,fp) != 1) return 0;
    return 1;
#endif
}

int UndoFile::read(long *i)
{
#ifdef _MAC
  unsigned char buffer[8];
  if (fp != 0)
  {
      if (fread(buffer,8,1,fp) != 1) return 0;
  }
  else
  {
       if (memoryfileposition+8 > memoryfilelength) return 0;
       buffer[0] = memoryfilebuffer[memoryfileposition++];
       buffer[1] = memoryfilebuffer[memoryfileposition++];
       buffer[2] = memoryfilebuffer[memoryfileposition++];
       buffer[3] = memoryfilebuffer[memoryfileposition++];
  }
  // KMJ mod for Mac
  *i = (RCLONG(buffer[3]) << 56) | (RCLONG(buffer[2]) << 48) | (RCLONG(buffer[1]) << 40) | (RCLONG(buffer[0]) << 32) | (RCLONG(buffer[3]) << 24) | (RCLONG(buffer[2]) << 16) | (RCLONG(buffer[1]) << 8) | RCLONG(buffer[0]);
  return 1;
#else
    unsigned char buffer[4];
  if (fp != 0)
    {  if (fread(buffer,4,1,fp) != 1) return 0;
    }
  else
    {  if (memoryfileposition+4 > memoryfilelength) return 0;
       buffer[0] = memoryfilebuffer[memoryfileposition++];
       buffer[1] = memoryfilebuffer[memoryfileposition++];
       buffer[2] = memoryfilebuffer[memoryfileposition++];
       buffer[3] = memoryfilebuffer[memoryfileposition++];
    }
  // KMJ mod for Mac
  *i = (RCLONG(buffer[3]) << 24) | (RCLONG(buffer[2]) << 16) | (RCLONG(buffer[1]) << 8) | RCLONG(buffer[0]);
  //*i = (long(buffer[3]) << 24) | (long(buffer[2]) << 16) | (long(buffer[1]) << 8) | long(buffer[0]);
  return 1;
#endif
}
*/
#endif

Entity *CadFile::nextentity(Database *database,Values *values)
{short type;
 Entity *e;
  if (! read(&type))
    return NULL;

  if (type < 1000 && type > 0)
    {  if (classfunctions.entityload(type) != NULL &&
           (e = classfunctions.entityload(type)(database != NULL,this)) != NULL)
         {  return e;
         }
       else
         return NULL;
    }
  else if (database != NULL && (long)database != 1)
    {  if (classfunctions.otherload(type) == NULL ||
           ! classfunctions.otherload(type)(this,database))
         return NULL;
    }
  else if (type == values_record && values != 0)
    values->load(this,0);
  return NULL;
}

#if defined(WIN32)
StorageFile::StorageFile()
{ storage = NULL;
  stream = NULL;
}

StorageFile::~StorageFile()
{ if (stream != NULL) stream->Release();
  if (storage != NULL) storage->Release();
}

int StorageFile::open(const char *name,const char *mode)
{HRESULT hr;
#if ! defined(_WIN32_WCE)
#if defined(WIN32)
 wchar_t fname[256],*ws,sname[9];
 const char *s;
  s = name;  ws = fname;
  while ((*ws++ = *s++) != 0) {}
  s = "CadModel";  ws = sname;
  while ((*ws++ = *s++) != 0) {}
#else
 const char *fname = name;
 char *sname = "CadModel";
#endif
  if (strpbrk(mode,"r") != NULL)
    {  hr = StgOpenStorage(fname,NULL,STGM_DIRECT | STGM_READ | STGM_SHARE_EXCLUSIVE,NULL,0,&storage);
       if (FAILED(hr))
         {  storage = NULL;  stream = NULL;
         }
       else
         {  hr = storage->OpenStream(sname,0,STGM_DIRECT | STGM_READ | STGM_SHARE_EXCLUSIVE,0,&stream);
            if (FAILED(hr))
              {  storage->Release();
                 storage = NULL;  stream = NULL;
              }
         }
    }
  else
    {  hr = StgCreateDocfile(fname,STGM_DIRECT | STGM_READWRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,0,&storage);
       if (FAILED(hr))
         {  storage = NULL;  stream = NULL;
         }
       else
         {  hr = storage->CreateStream(sname,STGM_DIRECT | STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,0,0,&stream);
            if (FAILED(hr))
              {  storage->Release();
                 storage = NULL;  stream = NULL;
              }
         }
    }
  return storage != NULL && stream != NULL;
#else
  return 0;
#endif
}

int StorageFile::close(void)
{ if (stream != NULL) stream->Release();
  if (storage != NULL) storage->Release();
  storage = NULL;  stream = NULL;
  return 1;
}

int StorageFile::read(Byte *b)
{HRESULT hr;
 ULONG cb;
  hr = stream->Read((void *)b,1,&cb);
  return cb == 1 && SUCCEEDED(hr);
}

int StorageFile::read(short *i)
{HRESULT hr;
 ULONG cb;
 unsigned char buffer[2];
  hr = stream->Read((void *)buffer,2,&cb);
  *i = short((short(buffer[1]) << 8) | short(buffer[0]));
  return cb == 2 && SUCCEEDED(hr);
}

int StorageFile::read(long *i)
{HRESULT hr;
 ULONG cb;
 unsigned char buffer[4];
  hr = stream->Read((void *)buffer,4,&cb);
  *i = (short(buffer[3]) << 24) | (short(buffer[2]) << 16) | (short(buffer[1]) << 8) | short(buffer[0]);
  return cb == 4 && SUCCEEDED(hr);
}

int StorageFile::read(BitMask *bitmask)
{BitMask *bm;
  if ((bm = BitMask::load(this)) != NULL)
    {  *bitmask = *bm;
       delete bm;
       return 1;
    }
  else
    return 0;
}

int StorageFile::read(char **s)
{HRESULT hr;
 ULONG cb;
 short length;
  if (! read(&length)) return 0;
  if ((*s = new char[length]) == NULL) return 0;
  hr = stream->Read((void *)*s,length,&cb);
  return cb == unsigned(length) && SUCCEEDED(hr);
}

int StorageFile::read(double *x)
{HRESULT hr;
 ULONG cb;
  hr = stream->Read((void *)x,8,&cb);
  return cb == 8 && SUCCEEDED(hr);
}

int StorageFile::read(Entity **e)
{ return read((long *)e);
}

int StorageFile::write(const Byte *b)
{HRESULT hr;
 ULONG cb;
  hr = stream->Write((void *)b,1,&cb);
  return cb == 1 && SUCCEEDED(hr);
}

int StorageFile::write(const short *i)
{HRESULT hr;
 ULONG cb;
 unsigned char buffer[2];
  buffer[1] = (unsigned char)(((*i) >> 8) & 255);
  buffer[0] = (unsigned char)(  *i        & 255);
  hr = stream->Write((void *)buffer,2,&cb);
  return cb == 2 && SUCCEEDED(hr);
}

int StorageFile::write(const long *i)
{HRESULT hr;
 ULONG cb;
 unsigned char buffer[4];
  buffer[3] = (unsigned char) (((*i) >> 24) & 255);
  buffer[2] = (unsigned char) (((*i) >> 16) & 255);
  buffer[1] = (unsigned char) (((*i) >>  8) & 255);
  buffer[0] = (unsigned char) (  *i         & 255);
  hr = stream->Write((void *)buffer,4,&cb);
  return cb == 4 && SUCCEEDED(hr);
}

int StorageFile::write(const char *s)
{HRESULT hr;
 ULONG cb;
 short length;
  length = short(strlen(s) + 1);
  write(&length);
  hr = stream->Write((void *)s,length,&cb);
  return cb == unsigned(length) && SUCCEEDED(hr);
}

int StorageFile::write(const BitMask *bitmask)
{ return bitmask->save(this);
}

int StorageFile::write(const double *x) 
{HRESULT hr;
 ULONG cb;
  hr = stream->Write((void *)x,8,&cb);
  return cb == 8 && SUCCEEDED(hr);
}

int StorageFile::write(const Entity **e)
{long v = (*e)->getindex();   return write(&v);
}

int StorageFile::blockread(void *buffer,int size)
{HRESULT hr;
 ULONG cb;
  hr = stream->Read(buffer,size,&cb);
  return cb == unsigned(size) && SUCCEEDED(hr);
}

int StorageFile::blockwrite(void *buffer,int size)
{HRESULT hr;
 ULONG cb;
  hr = stream->Write(buffer,size,&cb);
  return cb == unsigned(size) && SUCCEEDED(hr);
}

#endif

