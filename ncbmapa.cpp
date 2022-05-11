//
//  Implementation of the 'Point' and 'Line' class
//

#include "ncwin.h"
#include "ncdialog.h"
#include "bitmap_dialog.h"
#include "RCFigureBrowser.h"
#include "RCGraphicsItems.h"
#include "RCPlatform.h"
#ifndef _USING_QT
#include <gdiplus.h>
#endif

extern QPixmap createTranspBackTexture(QSize size, QColor foreColour, QColor backColour,int style=0);

BitMapE::BitMapE(const EntityHeader &header,RCCHAR *name,Point org,Point xa,Point ya,double w,double h,double tr,double tg,double tb,double trange,double op,BitMask &o)  : Entity(header),options(32)
{
#ifdef USING_WIDGETS
    usedByPlane=false;
    usedByFigure=false;
    for(int i=0; i<4; i++)
        qhndl[i]=0;
#endif

    if (name == 0)
    {  filename = new RCCHAR[1];
       strcpy(filename,"");
    }
  else if ((filename = new RCCHAR[strlen(name)+1]) != NULL)
    strcpy(filename,name);
    
  qbmi=0;
  origin = org;
  width = w;
  height = h;
  red = tr;
  green = tg;
  blue = tb;
  opacity = op;
  transparencyrange = trange;
  options = o;
  xaxis = xa;
  yaxis = ya;
  originalbuffer = 0;
  originallength = 0;
  options.clear(8);
  defined = (Byte) load(name);

  if (defined && options.test(9))
    db.setrequiresopengl(1);

  // debug testing
#if 0
  if (defined && options.test(9))
  {
      QBitmap mask(width,height);

      QPainter pt;
      pt.begin(&mask);
      pt.setBrush(QBrush(QColor(0,0,0)));
      pt.drawEllipse(QPointF(min(width,height)/2,min(width,height)),min(width,height)/4.0,min(width,height)/4.0);
      pt.end();

      QPixmap modimg = QPixmap::fromImage(*qbmi);
      modimg.setMask(mask);
      delete qbmi;
      qbmi = new QImage(modimg.toImage());
      pbuffer = qbmi->bits();
  }
#endif
}

BitMapE::BitMapE(RCCHAR *name,Point org,Point xa,Point ya,double w,double h,double tr,double tg,double tb,double trange,double op,BitMask &o) : options(32)
{ 
#ifdef USING_WIDGETS
    usedByPlane=false;
    usedByFigure=false;
    for(int i=0; i<4; i++)
        qhndl[i]=0;
#endif

  if (name == 0)
    {  filename = new RCCHAR[1];
       strcpy(filename,"");
    }
  else if ((filename = new RCCHAR[strlen(name)+1]) != NULL)
    strcpy(filename,name);
  qbmi=0;
  origin = org;
  xaxis = xa;
  yaxis = ya;
  width = w;
  height = h;
  red = tr;
  green = tg;
  blue = tb;
  opacity = op;
  transparencyrange = trange;
  options = o;
  pbuffer = 0;
  originalbuffer = 0;
  originallength = 0;
  options.clear(8);
  defined = (Byte) load(name);

  /*
   // debug
  int scanLength = long(((bmi->bmiHeader.biWidth * 3 + 3) / 4) * 4);
  for(int i=0; i<scanLength; i++)
  {
      unsigned char pixel = pbuffer[i];
      qDebug() << "pixel data [" << i << "]: " << pixel;
  }
  unsigned int *ipixel = (unsigned int*)pbuffer;
  for(int i=0; i<bmi->bmiHeader.biWidth; i++)
  {
      qDebug() << "pixel data [" << i << "]: " << *ipixel;
      ipixel ++;
  }
  for(int i=0; i<bmi->bmiHeader.biWidth; i++)
  {
      QRgb col = qbmi->pixel(i, 0);
      qDebug() << "pixel ["<< i <<"] qR : " << qRed(col);
      qDebug() << "pixel ["<< i <<"] qG : " << qGreen(col);
      qDebug() << "pixel ["<< i <<"] qB : " << qBlue(col);
  }
  */

  if (defined && options.test(9))
    db.setrequiresopengl(1);

  // debug testing
#if 0
  if (defined && options.test(9))
  {
      QBitmap mask(bmi->bmiHeader.biWidth,bmi->bmiHeader.biHeight);
      QPainter pt;
      pt.begin(&mask);
      pt.setBrush(QBrush(Qt::color1));
      pt.drawRect(mask.rect());
      pt.setPen(QPen(Qt::color0));
      int j,k;
      int ired=red*255,igreen=green*255,iblue=blue*255,irange=trange*255;
      for(k=0; k < bmi->bmiHeader.biHeight; k++)
      {
          for(j=0; j < bmi->bmiHeader.biWidth; j++)
          {
              QRgb pxrgb = qbmi->pixel(j,k);
              //if(abs(qRed(pxrgb)   - ired)   <= irange &&
              //   abs(qGreen(pxrgb) - igreen) <= irange &&
              //   abs(qBlue(pxrgb)  - iblue)  <= irange)
              //    pt.drawPoint(j,k);
              if(abs((qRed(pxrgb)&0xff)   - (ired&0xff))   <= irange &&
                 abs((qGreen(pxrgb)&0xff) - (igreen&0xff)) <= irange &&
                 abs((qBlue(pxrgb)&0xff)  - (iblue&0xff))  <= irange)
                  pt.drawPoint(j,k);
          }
      }
      pt.end();

      QPixmap modimg = QPixmap::fromImage(*qbmi);
      modimg.setMask(mask);
      delete qbmi;
      qbmi = new QImage(modimg.toImage());
      pbuffer = qbmi->bits();
  }
#endif
}

BitMapE::BitMapE(const EntityHeader &header, RCCHAR *name, BITMAPINFO *bi, PixelBuffer buffer, Point org, Point xa, Point ya, double w, double h, double tr, double tg, double tb, double trange, double op, BitMask &o, int version)  : Entity(header),options(32)
{ 
#ifdef USING_WIDGETS
    usedByPlane=false;
    usedByFigure=false;
    for(int i=0; i<4; i++)
        qhndl[i]=0;
#endif

  if (name == 0)
    {  filename = new RCCHAR[1];
       strcpy(filename,"");
    }
  else if ((filename = new RCCHAR[strlen(name)+1]) != NULL)
    strcpy(filename,name);
  qgi=0;
  bmi = bi;
  pbuffer = buffer;

  int stride = ((bmi->bmiHeader.biWidth*3 + 3)/4)*4; // 32 bit words per scane line
  qbmi = new QImage(pbuffer,bmi->bmiHeader.biWidth,bmi->bmiHeader.biHeight,stride, QImage::Format_RGB888);

  if(version == 2)
  {
      // turn the image upside down
      *qbmi = qbmi->mirrored(false,true);
      // swap the red and blue bytes
      *qbmi = qbmi->rgbSwapped();
      pbuffer = qbmi->bits();
  }

  defined =0;
  if(qbmi)
      defined =1;
  originalbuffer = 0;
  originallength = 0;
  origin = org;
  xaxis = xa;
  yaxis = ya;
  width = w;
  height = h;
  red = tr;
  green = tg;
  blue = tb;
  opacity = op;
  transparencyrange = trange;
  options = o;
  //defined = 1;
  //defined = (Byte) load(name);

  if (defined && options.test(9))
    db.setrequiresopengl(1);

}

/**
 * @brief BitMapE::BitMapE
 * @param header
 * @param name
 * @param bi
 * @param buffer
 * @param org
 * @param xa
 * @param ya
 * @param w
 * @param h
 * @param tr
 * @param tg
 * @param tb
 * @param trange
 * @param op
 * @param o
 *
 * this constructor uses a Qimage to create a BitMapE object
 *
 */
BitMapE::BitMapE(const EntityHeader &header,RCCHAR *name,RCIMAGE *bi,PixelBuffer buffer,Point org,Point xa,Point ya,double w,double h,double tr,double tg,double tb,double trange,double op,BitMask &o)  : Entity(header),options(32)
//BitMapE::BitMapE(const EntityHeader &header,RCCHAR *name,QPixmap *bi,PixelBuffer buffer,Point org,Point xa,Point ya,double w,double h,double tr,double tg,double tb,double trange,double op,BitMask &o)  : Entity(header),options(32)
{
#ifdef USING_WIDGETS
    usedByPlane=false;
    usedByFigure=false;
    for(int i=0; i<4; i++)
        qhndl[i]=0;
#endif

  defined = 0;
  bmi=0;
  if (name == 0)
  {
      filename = new RCCHAR[5];
      strcpy(filename,"");
  }
  else if ((filename = new RCCHAR[strlen(name)+1]) != NULL)
    strcpy(filename,name);
  qbmi = bi;
  pbuffer = buffer;

  if ((bmi = (LPBITMAPINFO) new RCCHAR[sizeofbmi()]) == 0)
    return;

  //qDebug() << "image format : " << qbmi->
  bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi->bmiHeader.biPlanes = 1;
  //bmi->bmiHeader.biBitCount = 24;
  bmi->bmiHeader.biBitCount = qbmi->bitPlaneCount();
  //bmi->bmiHeader.biCompression = BI_RGB;
  bmi->bmiHeader.biCompression = qbmi->format();
  bmi->bmiHeader.biSizeImage = qbmi->byteCount();
  bmi->bmiHeader.biXPelsPerMeter = 10000;
  bmi->bmiHeader.biYPelsPerMeter = 10000;
  bmi->bmiHeader.biClrUsed = 0;
  bmi->bmiHeader.biClrImportant = 0;
  bmi->bmiHeader.biWidth = qbmi->width();
  bmi->bmiHeader.biHeight = qbmi->height();

  originalbuffer = 0;
  originallength = 0;
  origin = org;
  xaxis = xa;
  yaxis = ya;
  width = w;
  height = h;
  red = tr;
  green = tg;
  blue = tb;
  opacity = op;
  transparencyrange = trange;
  options = o;
  defined=0;
  if(qbmi)
    defined = 1;

  if (defined && options.test(9))
    db.setrequiresopengl(1);

}

BitMapE::~BitMapE()
{
  delete qbmi;
  pbuffer =0;
  delete [] bmi;
  bmi=0;
  delete [] originalbuffer;
  originalbuffer=0;
  delete [] filename;
  filename=0;
  // delete the graphics item??
  //if(qgi)
  //    if(cadwindow->getcurrentsurface()&&cadwindow->getcurrentsurface()->getScene())
  //      cadwindow->getcurrentsurface()->getScene()->removeItem(qgi);
  //delete qgi;
  //qgi=0;

}

int BitMapE::load(RCCHAR *name)
{
#ifdef USING_WIDGETS
    originalbuffer=0;
    bmi=0;
    pbuffer=0;

    int i;

    // get the native path seperators

    // something other than a bitmap file usually a jpeg
    if(strlen(name) < 1)
        return 0;
   QString qfname = RCFixupPathSeperator(name);
   if (qfname.length() > 4 && qfname.endsWith(".bmp",Qt::CaseInsensitive) != true)
   {
       FILE *infile;
       if ((infile = fopen(qfname.toStdString().c_str(),"rb")) != 0)
       {
           fseek(infile,0L,SEEK_END);
           originallength = ftell(infile);
           fseek(infile,0L,SEEK_SET);
           originalbuffer = new unsigned char[originallength];
           fread(originalbuffer,1,originallength,infile);
           options.set(8);  //  This is a JPEG file that has been loaded
           fclose(infile);
       }
   }


    qbmi = new QImage(qfname);
    if(qbmi == 0)
        return 0;

    // always using Qt RGB888 24 bit format
    //if(qbmi->format() < 4)
    *qbmi = qbmi->convertToFormat(QImage::Format_RGB888);

    // KMJ : TODO : change this over to pure QT??
    // construct the bmiÅ@from the QImage for now

    if ((bmi = (LPBITMAPINFO) new RCCHAR[sizeofbmi()]) == 0)
      return 0;

    //qDebug() << "image format : " << qbmi->
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biPlanes = 1;
    //bmi->bmiHeader.biBitCount = 24;
    bmi->bmiHeader.biBitCount = qbmi->bitPlaneCount();
    //bmi->bmiHeader.biCompression = BI_RGB;
    bmi->bmiHeader.biCompression = qbmi->format(); // be carefull with this??
    bmi->bmiHeader.biSizeImage = qbmi->byteCount();
    bmi->bmiHeader.biXPelsPerMeter = 10000;
    bmi->bmiHeader.biYPelsPerMeter = 10000;
    bmi->bmiHeader.biClrUsed = 0;
    bmi->bmiHeader.biClrImportant = 0;
    bmi->bmiHeader.biWidth = qbmi->width();
    bmi->bmiHeader.biHeight = qbmi->height();

    pbuffer = qbmi->bits();
    //pbuffer = new unsigned char[bd.Height * bd.Stride];
    //pbuffer = new unsigned char[qbmi->byteCount()];
    /*
    for (i = 0 ; i < (int)qbmi->height() ; i++)
      memcpy(pbuffer + i * bd.Stride,((RCCHAR *)bd.Scan0) + (bd.Height - 1 - i) * bd.Stride,bd.Stride);
    */
    return 1;
#else
 Gdiplus::GdiplusStartupInput gdiplusStartupInput;
 ULONG_PTR gdiplusToken;
 WCHAR wname[STRING_LENGTH];
 Gdiplus::Rect rect;
 Gdiplus::BitmapData bd;
 int i;

    if (strlen(name) > 4 && _stricmp(name+strlen(name)-4,".bmp") != 0)
    {FILE *infile;
        if ((infile = fopen(name,"rb")) != 0)
        {   fseek(infile,0L,SEEK_END);
            originallength = ftell(infile);
            fseek(infile,0L,SEEK_SET);
            originalbuffer = new unsigned char[originallength];
            fread(originalbuffer,1,originallength,infile);
            options.set(8);  //  This is a JPEG file that has been loaded
            fclose(infile);
        }
    }


  bmi = 0;
  pbuffer = 0;

  if ((bmi = (LPBITMAPINFO) new RCCHAR[sizeofbmi()]) == 0)
    return 0;

  //MultiByteToWideChar(CP_ACP,0,name,strlen(name)+1,wname,STRING_LENGTH);

  Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(wname);

  if (bitmap->GetLastStatus() != Gdiplus::Ok)
    {  bmi = 0;  pbuffer = 0;
       delete bitmap;
       return 0;
    }

  rect.X = 0;
  rect.Y = 0;
  rect.Width = bitmap->GetWidth();
  rect.Height = bitmap->GetHeight();

  bitmap->LockBits(&rect,Gdiplus::ImageLockModeRead,PixelFormat24bppRGB,&bd);

  bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi->bmiHeader.biPlanes = 1;
  bmi->bmiHeader.biBitCount = 24;
  bmi->bmiHeader.biCompression = BI_RGB;
  bmi->bmiHeader.biSizeImage = 0;
  bmi->bmiHeader.biXPelsPerMeter = 10000;
  bmi->bmiHeader.biYPelsPerMeter = 10000;
  bmi->bmiHeader.biClrUsed = 0;
  bmi->bmiHeader.biClrImportant = 0;
  bmi->bmiHeader.biWidth = bitmap->GetWidth();
  bmi->bmiHeader.biHeight = bitmap->GetHeight();

  pbuffer = new unsigned char[bd.Height * bd.Stride];
  for (i = 0 ; i < (int)bd.Height ; i++)
    memcpy(pbuffer + i * bd.Stride,((RCCHAR *)bd.Scan0) + (bd.Height - 1 - i) * bd.Stride,bd.Stride);
    
  bitmap->UnlockBits(&bd);

  delete bitmap;

  Gdiplus::GdiplusShutdown(gdiplusToken);
  return 1;
#endif
}


int BitMapE::getshapejpeg(FILE *infile,long *w,long *h)
{int c,c1,c2;
  for (;;)
    {  c = getc(infile);
       if (c == EOF)
         return 0;
       while (c != 0xFF)
         c = getc(infile);
       while (c == 0xFF) c = getc(infile);
       if (c == EOF)
         return 0;
       if (c == 0xC0)
         {  c1 = getc(infile); c2 = getc(infile);
            if (c1 == EOF || c2 == EOF)  return 0;
            c = getc(infile);
            if (c == EOF)  return 0;
            c1 = getc(infile); c2 = getc(infile);
            if (c1 == EOF || c2 == EOF)  return 0;
            *h = (c1 << 8) | c2;
            c1 = getc(infile); c2 = getc(infile);
            if (c1 == EOF || c2 == EOF)  return 0;
            *w = (c1 << 8) | c2;
            if (*h != 0)
              return 1;
         }
       else if (c == 0xDC)
         {  c1 = getc(infile); c2 = getc(infile);
            if (c1 == EOF || c2 == EOF)  return 0;
            c1 = getc(infile); c2 = getc(infile);
            if (c1 == EOF || c2 == EOF)  return 0;
            *h = (c1 << 8) | c2;
            return 1;
         }
    }
}

int BitMapE::getshape(RCCHAR *name,long *w,long *h)
{
#ifdef USING_WIDGETS
    QImage qimg;
    if(qimg.load(QString(name)))
    {
        *w = qimg.width();
        *h = qimg.height();
        return 1;
    }
    else
        return 0;
#else
 FILE *infile;
 BITMAPFILEHEADER bmfh;
 BITMAPINFOHEADER bmih;
 int loadstatus;

  if ((infile = fopen(name,_RCT("rb"))) == 0)
    return 0;

  if (fread(&bmfh,sizeof(bmfh),1,infile) != 1)
    {  fclose(infile);
       return 0;
    }

  if (bmfh.bfType != 0x4D42)
    {  fseek(infile,0L,0);
       loadstatus = getshapejpeg(infile,w,h);
       fclose(infile);
       return loadstatus;
    }

  if (fread(&bmih,sizeof(BITMAPINFOHEADER),1,infile) != 1)
    {  fclose(infile);
       return 0;
    }

  *w  = bmih.biWidth;
  *h  = bmih.biHeight;

  fclose(infile);
  return 1;
#endif
}

int BitMapE::sizeofbmi(void)
{ return sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
}

long BitMapE::sizeofbuffer(void)
{
#ifndef USING_WIDGETS
    return bmi->bmiHeader.biSizeImage;
    //return bmi->bmiHeader.biWidth * bmi->bmiHeader.biHeight * bmi->bmiHeader.biBitCount;
#else
    if (bmi->bmiHeader.biBitCount == 1)
    return long((((bmi->bmiHeader.biWidth+7) / 8 + 3) / 4) * 4) * long(bmi->bmiHeader.biHeight);
  else if (bmi->bmiHeader.biBitCount == 8)
    return long(((bmi->bmiHeader.biWidth + 3) / 4) * 4) * long(bmi->bmiHeader.biHeight);
  else
    return long(((bmi->bmiHeader.biWidth * 3 + 3) / 4) * 4) * long(bmi->bmiHeader.biHeight);
#endif
}

void BitMapE::transpose(void)
{int i,j,w1,w2;
 long w,h;
 PixelBuffer pnew;
  w = bmi->bmiHeader.biHeight;
  h = bmi->bmiHeader.biWidth;
  if (bmi->bmiHeader.biBitCount == 1)
    {  w1 = int((((bmi->bmiHeader.biWidth+7)/8 + 3) / 4) * 4);
       w2 = (((w+7)/8 + 3) / 4) * 4;
    }
  else if (bmi->bmiHeader.biBitCount == 8)
    {  w1 = int(((bmi->bmiHeader.biWidth + 3) / 4) * 4);
       w2 = ((w + 3) / 4) * 4;
    }
  else
    {  w1 = int(((bmi->bmiHeader.biWidth * 3 + 3) / 4) * 4);
       w2 = ((w*3 + 3) / 4) * 4;
    }
  pnew = (unsigned char *)new char[w2 * h];
  if (pnew == 0) return;
  memset(pnew,255,w2 * h);
  for (i = 0 ; i < bmi->bmiHeader.biWidth ; i++)
    for (j = 0 ; j < bmi->bmiHeader.biHeight ; j++)
      if (bmi->bmiHeader.biBitCount == 1)
        pnew[i * w2 + (j >> 3)] |= ((pbuffer[j * w1 + (i>>3)])>>(i&7))<<(j&7);
      else if (bmi->bmiHeader.biBitCount == 8)
        pnew[i * w2 + j] = pbuffer[j * w1 + i];
      else
        {  pnew[i * w2 + j*3] = pbuffer[j * w1 + i*3];
           pnew[i * w2 + j*3+1] = pbuffer[j * w1 + i*3+1];
           pnew[i * w2 + j*3+2] = pbuffer[j * w1 + i*3+2];
        }
  bmi->bmiHeader.biWidth = w;
  bmi->bmiHeader.biHeight = h;
  delete [] pbuffer;
  pbuffer = pnew;
}

void BitMapE::reverse(int)
{int i,j,w1,w2;
 unsigned char t,*scanline;
  w2 = bmi->bmiHeader.biWidth - 1;
  scanline = 0;
  if (bmi->bmiHeader.biBitCount == 1)
    {  w1 = int((((bmi->bmiHeader.biWidth+7)/8 + 3) / 4) * 4);
       scanline = new unsigned char [w1];
    }
  else if (bmi->bmiHeader.biBitCount == 8)
    w1 = int(((bmi->bmiHeader.biWidth + 3) / 4) * 4);
  else
    w1 = int(((bmi->bmiHeader.biWidth * 3 + 3) / 4) * 4);
  for (i = 0 ; i < bmi->bmiHeader.biHeight ; i++)
    for (j = 0 ; j < bmi->bmiHeader.biWidth / 2 ; j++)
      if (bmi->bmiHeader.biBitCount == 1)
        {  if (j == 0)
             memset(scanline,0,w1);
           scanline[j>>3] |= (pbuffer[i * w1 + ((w2-j)>>3)]>>((w2-j)&7))<<(j&7);
           if (j == bmi->bmiHeader.biWidth / 2 - 1)
             memcpy(pbuffer + i * w1,scanline,w1);
        }    
      else if (bmi->bmiHeader.biBitCount == 8)
        {  t = pbuffer[i * w1 + j];
           pbuffer[i * w1 + j] = pbuffer[i * w1 + (w2-j)];
           pbuffer[i * w1 + (w2-j)] = t;
        }
      else
        {  t = pbuffer[i * w1 + j*3];
           pbuffer[i * w1 + j*3] = pbuffer[i * w1 + (w2-j)*3];
           pbuffer[i * w1 + (w2-j)*3] = t;
           t = pbuffer[i * w1 + j*3+1];
           pbuffer[i * w1 + j*3+1] = pbuffer[i * w1 + (w2-j)*3+1];
           pbuffer[i * w1 + (w2-j)*3+1] = t;
           t = pbuffer[i * w1 + j*3+2];
           pbuffer[i * w1 + j*3+2] = pbuffer[i * w1 + (w2-j)*3+2];
           pbuffer[i * w1 + (w2-j)*3+2] = t;
        }
  delete [] scanline;
}

void BitMapE::addtodisplaylist(View3dSurface *,int)
{
//  For now, don't support displaylists - Means figures dot not support bitmaps.
//  Point pt1;
//  if (! child && ! vs->displaylist.start(this,25)) return;
//  pt1 = vs->modeltoview(origin);
//  if (vs->inside(pt1))
//    {  pt1 = vs->viewtoscreen(pt1);
//       if (! vs->displaylist.moveto(pt1.x,pt1.y)) return;
//    }
//  if (! child && ! vs->displaylist.end()) return;
}

#ifdef USING_WIDGETS
void BitMapE::drawGL(int drawmode,Transform *t,View3dSurface *vs)
{
    Point p1,p2,p3,p4,p5,p6,p7,p8,xa,ya,xa1,ya1,pt1;
    int i,hor,ver,styl,ix,iy,iw,ih;
    DWORD rop;

    Entity *e;
    int texture,nlights;
    Point p[9];
    Point eye,pmin,pmax;
    double ambient,diffuse;

    if (! defined)
      return;

    draw(DM_ERASE,t,vs);

    //qDebug() << "drawGL : " << "width" << width << "height" << height;

    // no special action for scrolling yet
    if( drawmode == DM_SCROLLING) drawmode = DM_NORMAL;

    if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;

    hor = options.test(2) * 2 + options.test(1);
    ver = options.test(4) * 2 + options.test(3);
    styl = options.test(7) * 4 + options.test(6) * 2 + options.test(5);

    p1 = origin;
    /*
    if (t == NULL)
      p1 = vs->modeltransform.transform(origin);
    else
      p1 = t->transform(vs->modeltransform.transform(origin));
    */

    if( getdefined() &&
        //getfilename() != 0 &&
        //strlen(getfilename()) > 0 &&
        getpbuffer() != 0)
    {

        //View *view;
        //view = cadwindow->getcurrentsurface()->getview();
        //db.geometry.extents(cadwindow->getcurrentwindow(),&pmin,&pmax);

        //OpenOpenGl(((RCView*)cadwindow->getcurrentwindow()->gethdc())->view(),3,0,0,widget->width(),widget->height());

        if(vs !=0 && vs->getrepaintstyle() == Shaded)
        {
            ambient = v.getreal("sh::ambient");
            diffuse = v.getreal("sh::diffuse");

            //if (view->getperspective())
            //  eye = view->geteye();
            //else
            //  eye = (view->geteye() - view->getref()).normalize() * (pmax.z - pmin.z + 2.0) + view->getref();

            //if (view->getperspective())
            //  SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),pmax.z+1 > 10.0 ? pmax.z+1 : 10.0,view->getperspective());
            //else
            //  SetView(eye.x,eye.y,eye.z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,pmin.z-1,pmax.z+1,view->getperspective());

            //SetViewport(cadwindow->getcurrentwindow()->getumin(),cadwindow->getcurrentwindow()->getvmin(),cadwindow->getcurrentwindow()->getumax(),cadwindow->getcurrentwindow()->getvmax());
            //SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
            //BeginScene(0/*BS_NOCLEAR*/ /*| BS_FRONTBUFFERONLY */);
            //SetLight(view->geteye().x*1000000.0,view->geteye().y*1000000.0,view->geteye().z*1000000.0);
            nlights = 0;
            for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
              if (e->isa(light_entity) && e->isvisible(vs))
                {double red,green,blue;
                   if (((Light *)e)->getoptions().test(1))
                     {  red = green = blue = 1.0;
                     }
                   else
                     {  red = db.colourtable[e->getcolour()].red/255.0;
                        green = db.colourtable[e->getcolour()].green/255.0;
                        blue = db.colourtable[e->getcolour()].blue/255.0;
                     }

                   SetLightEx(nlights,((Light *)e)->getorigin().x,((Light *)e)->getorigin().y,((Light *)e)->getorigin().z,red,green,blue);
                   nlights++;
            }

            if (nlights == 0)
                SetLight(eye.x,eye.y,eye.z);
        }
        else
        {
            //SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
            //SetViewport(cadwindow->getcurrentsurface()->getumin(),cadwindow->getcurrentsurface()->getvmin(),cadwindow->getcurrentsurface()->getumax(),cadwindow->getcurrentsurface()->getvmax());
            //SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
            //BeginScene(/*BS_FRONTBUFFERONLY |*/ BS_IGNOREDEPTH | BS_NOCLEAR/* //* rcbitmap->getoptions().test(31)*/);
            SetLight(vs->getview()->geteye().x*100000.0,vs->getview()->geteye().y*100000.0,vs->getview()->geteye().z*100000.0);
        }
         int hor = getoptions().test(2) * 2 + getoptions().test(1);
         int ver = getoptions().test(4) * 2 + getoptions().test(3);

         if (drawmode == DM_NORMAL)
         {
             //qDebug() << "drawGL : DM_NORMAL";
             SetMaterial(1.0,1.0,1.0,1.0,1.0,0.0);
             if(options.test(9)) // tansparency switch
             {
                 LoadMemoryTexture(getfilename(),(char *)getpbuffer(),
                                   getbmi()->bmiHeader.biWidth,getbmi()->bmiHeader.biHeight,
                                   getred(),getgreen(),getblue(),
                                   gettransparencyrange(), getopacity(),&texture);
             }
             else // transparency not used
             {
                 LoadMemoryTexture(getfilename(),(char *)getpbuffer(),
                                   getbmi()->bmiHeader.biWidth,getbmi()->bmiHeader.biHeight,
                                   getred(),getgreen(),getblue(),
                                   0,1,&texture);
             }
             SetTexture(texture);
         }
         else if (drawmode == DM_SELECT)
         {
             //qDebug() << "drawGL : DM_SELECT";
            SetMaterial(1.0,0.0,1.0,1.0,1.0,0.0);
            LoadMemoryTexture(getfilename(),(char *)getpbuffer(),
                               getbmi()->bmiHeader.biWidth, getbmi()->bmiHeader.biHeight,
                               getred(), getgreen(), getblue(),
                               gettransparencyrange(), 0.5,&texture);
                             // gettransparencyrange(), getopacity(),&texture);
            SetTexture(texture);
         }
         else if (drawmode == DM_INVERT)
         {
             //qDebug() << "drawGL : DM_INVERT";
            SetMaterial(0.0,0.0,1.0,1.0,1.0,0.0);
            LoadMemoryTexture(getfilename(),(char *)getpbuffer(),
                               getbmi()->bmiHeader.biWidth, getbmi()->bmiHeader.biHeight,
                               getred(), getgreen(), getblue(),
                               gettransparencyrange(), 0.5,&texture);
                               //gettransparencyrange(), getopacity(),&texture);
            SetTexture(texture);
         }
         /*
         else
         {
             SetMaterial(v.getreal("wn::backgroundcolour.red"),v.getreal("wn::backgroundcolour.green"),v.getreal("wn::backgroundcolour.blue"),1.0,1.0,0.0);
             SetTexture(0);
         }
         */
            //qDebug() << "drawing bitmap with GL";
            // draw the bitmap
            //bp1 = vs->modeltransform.transform(getorigin());

            p1 = p1 - getxaxis() * (hor* getwidth()/2.0) -  getyaxis() * ver* getheight()/2.0;
            p2 = p1 + getxaxis() * getwidth();
            p3 = p2 + getyaxis() * getheight();
            p4 = p3 - getxaxis() * getwidth();

            //p1 = vs->modeltransform.transform(p1);
            //p2 = vs->modeltransform.transform(p2);
            //p3 = vs->modeltransform.transform(p3);
            //p4 = vs->modeltransform.transform(p4);
            if (t == NULL)
            {
              p1 = vs->modeltransform.transform(p1);
              p2 = vs->modeltransform.transform(p2);
              p3 = vs->modeltransform.transform(p3);
              p4 = vs->modeltransform.transform(p4);
            }
            else
            {
              p1 = t->transform(vs->modeltransform.transform(p1));
              p2 = t->transform(vs->modeltransform.transform(p2));
              p3 = t->transform(vs->modeltransform.transform(p3));
              p4 = t->transform(vs->modeltransform.transform(p4));
            }

            p[0] = p1;
            p[1] = p2;
            p[2] = p3;
            p[3] = getxaxis().cross(getyaxis());
            p[4] = p[3];
            p[5] = p[3];
            p[6] = Point(0.001,0.001,0.0);
            p[7] = Point(0.999,0.001,0.0);
            p[8] = Point(0.999,0.999,0.0);
            DrawTexturedTriangle(p);

            p[0] = p1;
            p[1] = p3;
            p[2] = p4;
            p[3] = getxaxis().cross(getyaxis());
            p[4] = p[3];
            p[5] = p[3];
            p[6] = Point(0.001,0.001,0.0);
            p[7] = Point(0.999,0.999,0.0);
            p[8] = Point(0.001,0.999,0.0);
            DrawTexturedTriangle(p);

            //Flush();
    }

    vs->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);

    RCCOLORREF pcol;
    pcol = vs->getcolour();
    //glColor3ub(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
    SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
    GLfloat lwidth = vs->curPen.widthF()<1.0 ? 1.0 : vs->curPen.widthF();
    glLineWidth(lwidth);

    if (styl == 1 || styl == 2 )
    //if (styl == 1 || styl == 2 || SetLight != 0 && options.test(9) && drawmode == DM_INVERT)
    {
        //qDebug() << "drawGL : drawing frame";
        p1 = origin;
        /*
         if (t == 0)
           p1 = origin;
         else
           p1 = t->transform(origin);
         */

         // we are using 3d OpenGL always a 3d bitmap
         xa = xaxis * width;
         ya = yaxis * height;

         // KMJ : To Do : add a normal to view option like a figure??
#if 0
         if (SetLight != 0 && options.test(9))
         {
             // normal to screen?
              xa =  xaxis * width;
              ya =  yaxis * height;
         }
         else
         {
             // rotated in 3d
              xa =  vs->getuaxis() * width;
              ya =  vs->getvaxis() * height;
         }
#endif

         //p1 = p1 - xa * (hor/2.0) - ya * ver / 2.0;

         p1 = p1 - xa * (hor/2.0) - ya * ver / 2.0;
         p2 = p1 + xa;
         p3 = p2 + ya;
         p4 = p3 - xa;

         //p1 = vs->modeltransform.transform(p1);
         //p2 = vs->modeltransform.transform(p2);
         //p3 = vs->modeltransform.transform(p3);
         //p4 = vs->modeltransform.transform(p4);
         if (t == NULL)
         {
             p1 = vs->modeltransform.transform(p1);
             p2 = vs->modeltransform.transform(p2);
             p3 = vs->modeltransform.transform(p3);
             p4 = vs->modeltransform.transform(p4);
         }
         else
         {
             p1 = t->transform(vs->modeltransform.transform(p1));
             p2 = t->transform(vs->modeltransform.transform(p2));
             p3 = t->transform(vs->modeltransform.transform(p3));
             p4 = t->transform(vs->modeltransform.transform(p4));
         }

         vs->line3dGL(p1,p2);
         vs->line3dGL(p2,p3);
         vs->line3dGL(p3,p4);
         vs->line3dGL(p4,p1);

#if 1
         if (styl == 2)
         {
               p1 = origin;
               /*
               if (t == 0)
                 p1 = origin;
               else
                 p1 = t->transform(origin);
               */

               p1 = p1 - xa * (hor/2.0) - ya * ver / 2.0;
               p2 = p1 + xa;
               p3 = p2 + ya;
               p4 = p3 - xa;

               xa1 = xa / 20.0;
               ya1 = ya / height * width / 20.0;

               p5 = p1 - xa1 - ya1;
               p6 = p2 + xa1 - ya1;
               p7 = p3 + xa1 + ya1;
               p8 = p4 - xa1 + ya1;

               //p1 = vs->modeltransform.transform(p1);
               //p2 = vs->modeltransform.transform(p2);
               //p3 = vs->modeltransform.transform(p3);
               //p4 = vs->modeltransform.transform(p4);

               //p5 = vs->modeltransform.transform(p5);
               //p6 = vs->modeltransform.transform(p6);
               //p7 = vs->modeltransform.transform(p7);
               //p8 = vs->modeltransform.transform(p8);
               if (t == NULL)
               {
                   p1 = vs->modeltransform.transform(p1);
                   p2 = vs->modeltransform.transform(p2);
                   p3 = vs->modeltransform.transform(p3);
                   p4 = vs->modeltransform.transform(p4);

                   p5 = vs->modeltransform.transform(p5);
                   p6 = vs->modeltransform.transform(p6);
                   p7 = vs->modeltransform.transform(p7);
                   p8 = vs->modeltransform.transform(p8);
               }
               else
               {
                   p1 = t->transform(vs->modeltransform.transform(p1));
                   p2 = t->transform(vs->modeltransform.transform(p2));
                   p3 = t->transform(vs->modeltransform.transform(p3));
                   p4 = t->transform(vs->modeltransform.transform(p4));

                   p5 = t->transform(vs->modeltransform.transform(p5));
                   p6 = t->transform(vs->modeltransform.transform(p6));
                   p7 = t->transform(vs->modeltransform.transform(p7));
                   p8 = t->transform(vs->modeltransform.transform(p8));
               }

               vs->line3dGL( p5,p6);
               vs->line3dGL( p6,p7);
               vs->line3dGL( p7,p8);
               vs->line3dGL( p8,p5);

               vs->line3dGL( p1,p5);
               vs->line3dGL( p2,p6);
               vs->line3dGL( p3,p7);
               vs->line3dGL( p4,p8);

         }
#endif
         /*
         xa1 = xa / 20.0;
         ya1 = ya / height * width / 20.0;

         vs->line3dGL(p1,p1 + xa);
         vs->line3dGL(p1 + xa,p1 + xa + ya);
         vs->line3dGL(p1 + xa + ya,p1 + ya);
         vs->line3dGL(p1 + ya,p1);

         if (styl == 2)
         {
               vs->line3dGL( p1 - xa1 - ya1,p1 + xa + xa1 - ya1);
               vs->line3dGL( p1 + xa + xa1 - ya1,p1 + xa + ya + xa1 + ya1);
               vs->line3dGL( p1 + xa + ya + xa1 + ya1,p1 + ya - xa1 + ya1);
               vs->line3dGL( p1 + ya - xa1 + ya1,p1 - xa1 - ya1);
               vs->line3dGL( p1,p1 - xa1 - ya1);
               vs->line3dGL( p1 + xa,p1 + xa + xa1 - ya1);
               vs->line3dGL( p1 + xa + ya,p1 + xa + ya + xa1 + ya1);
               vs->line3dGL( p1 + ya,p1 + ya - xa1 + ya1);
         }
        */
    }

    if (gethandles())
    {
         vs->setupGL(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL ,colour,0,0,0);
         //vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);

         RCCOLORREF pcol;
         pcol = vs->getcolour();
         SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
         GLfloat lwidth = vs->curPen.widthF();
         glLineWidth(lwidth<1.0?1.0:lwidth);


         p1 = origin;

         // we are using 3d OpenGL always a 3d bitmap
         xa = xaxis * width;
         ya = yaxis * height;

         // KMJ : To Do : add a normal to view option like a figure??
#if 0
         if (SetLight != 0 && options.test(9))
         {
              xa = xaxis * width;
              ya = yaxis * height;
         }
         else
         {
              xa = vs->getuaxis() * width;
              ya = vs->getvaxis() * height;
         }
#endif

         p1 = p1 - xa * (hor/2.0) - ya * ver / 2.0;

         Point pright = vs->getuaxis();
         Point pup = vs->getvaxis();

         double sc = vs->getpixelscale() * 6.0;

         for (i = 0 ; i < 4 ; i++)
         {
              //if (i == 0)
              //  pt1 = vs->modeltoscreen(p1);
              //else if (i == 1)
              //  pt1 = vs->modeltoscreen(p1+xa) + Point(2,0,0);
              //else if (i == 2)
              //  pt1 = vs->modeltoscreen(p1+xa+ya) + Point(2,2,0);
              //else if (i == 3)
              //  pt1 = vs->modeltoscreen(p1+ya) + Point(0,2,0);

              if (i == 0)
                pt1 = p1 - (pright*sc/2.) - (pup*sc/2.);
              else if (i == 1)
                pt1 = (p1+xa) + (pright*sc/2.) - (pup*sc/2.);
              else if (i == 2)
                pt1 = (p1+xa+ya) + (pup*sc/2.) + (pright * sc/2.);
              else if (i == 3)
                pt1 = (p1+ya) - (pright*sc/2.) + (pup * sc/2.);

              vs->cbox2dGL(pt1,pright,pup,sc,sc);
         }
    }
}
#endif

void BitMapE::draw(int drawmode,Transform *t,View3dSurface *vs)
{
#ifdef USING_WIDGETS
    Point p0,p1,p2,p3,p4,p5,xa,ya,xa1,ya1,pt1;
    int i,hor,ver,styl,ix,iy,iw,ih;
    DWORD rop;
    QGraphicsItem *qframe=0; // for the frame
    QGraphicsPixmapItem *qpxm=0; // for the pixmap
    RCGraphicsBitmapItem *rcpxm=0; // for OpenGL rendering

    //qDebug() << "filename : " << QString(filename);

     if (! defined)
       return;

     //qDebug() << "draw : " << "width" << width << "height" << height;

     if(drawmode != DM_PRINT)
     {
         if(qgi && qgi != qgi->parentItem() )
             qgi->setParentItem(0);

         if(qgi && qgi->scene() != vs->getScene())
             qgi->scene()->removeItem(qgi);
         else if(qgi && vs->getScene())
             vs->getScene()->removeItem(qgi);

         delete qgi;
         qgi=0;

         for(int ih=0; ih<4; ih++)
         {
             if(qhndl[ih] && qhndl[ih]->scene() != vs->getScene())
                 qhndl[ih]->scene()->removeItem(qhndl[ih]);
             delete qhndl[ih];
             qhndl[ih]=0;
         }

         if(dynamic_cast<View3dSubSurface*>(vs) != 0)
             ((View3dSubSurface*)vs)->qgicp=0;
     }

     if(drawmode == DM_ERASE)
         return;

     if( !usedByFigure )
     {
         // bounding box check against the view
         Point bmin,bmax;
         extents(vs,&bmin,&bmax);
         //vs->modeltoscreen(bmin);
         //vs->modeltoscreen(bmax);
         if(bmax.x < vs->getumin() && bmax.y < vs->getvmin() && bmin.x > vs->getumax() && bmin.y > vs->getvmax())
             return;
     }
     // end of bounding box test

     if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;

     hor = options.test(2) * 2 + options.test(1);
     ver = options.test(4) * 2 + options.test(3);
     styl = options.test(7) * 4 + options.test(6) * 2 + options.test(5);
     if (t == 0)
       p1 = origin;
     else
       p1 = t->transform(origin);

     //Point zaxis = xaxis.cross(yaxis);
     //bool isRotated = false;
     //if( fabs(xaxis.x - 1.0) > 1.0E-10 && fabs(yaxis.y - 1.0) > 1.0E-10 || fabs(zaxis.z - 1.0) > 1.0E-10)
     //    isRotated = true;

         if(qbmi != 0 && drawmode != DM_INVERT)
         {
             // this is in a composed view
             if (vs->gettransform2d() && drawmode != DM_INVERT)
             {
                 double angle=0;
                 if (t == 0)
                   p1 = origin;
                 else
                   p1 = t->transform(origin);

                 xa =  xaxis * width;
                 ya =  yaxis * height;

                 p0 = p1 + xaxis * width/2.0 + yaxis * height/2.0;
                 p1 = p1 - xaxis * (hor*width/2.0) - yaxis * ver*height/2.0;
                 p2 = p1 + xaxis * width;
                 p3 = p2 + yaxis * height;
                 p4 = p3 - xaxis * width;

                 p0 = vs->modeltoscreen(p0);
                 p1 = vs->modeltoscreen(p1);
                 p2 = vs->modeltoscreen(p2);
                 p3 = vs->modeltoscreen(p3);
                 p4 = vs->modeltoscreen(p4);

                 double a1,b1,c1,a2,b2,c2,tt,x0,y0,x1,y1,x2,y2,x3,y3,x4,y4;
                 vs->gettransform2d(&a1,&b1,&c1,&a2,&b2,&c2);

                 x0 = a1 * p0.x + b1 * p0.y + c1;
                 y0 = a2 * p0.x + b2 * p0.y + c2;

                 p0.x = x0; p0.y = y0;

                 x1 = a1 * p1.x + b1 * p1.y + c1;
                 y1 = a2 * p1.x + b2 * p1.y + c2;
                 x2 = a1 * p2.x + b1 * p2.y + c1;
                 y2 = a2 * p2.x + b2 * p2.y + c2;

                 p1.x = x1; p1.y = y1;
                 p2.x = x2; p2.y = y2;

                 x3 = a1 * p3.x + b1 * p3.y + c1;
                 y3 = a2 * p3.x + b2 * p3.y + c2;
                 x4 = a1 * p4.x + b1 * p4.y + c1;
                 y4 = a2 * p4.x + b2 * p4.y + c2;

                 p3.x = x3; p3.y = y3;
                 p4.x = x4; p4.y = y4;
#if 0
                 // Pixmap output method #1
                 // fast but low res images
                 // transforms the pixmap before putting it in the scene
                 // only offset the item in the scene
                 // this is faster that doing all the images transforms in the scene
                 // but the resulting images are lower quality
                 //
                 // note this method cannot output bitmaps which are clipped against the printer borders
                 //
                 if(qbmi != 0)
                 {
                     QPixmap lbm = QPixmap::fromImage(*qbmi);

                     if(options.test(BitmapETransparent) && transparencyrange > 0.0)
                     {
                         QImage qimg1 = lbm.toImage();
                         QImage qimg = qimg1.convertToFormat(QImage::Format_RGBA8888);
                         int irange = (int)(transparencyrange * 255);
                         int iopacity = (int)(opacity * 255);
                         int ir = (int)(red * 255);
                         int ig = (int)(green * 255);
                         int ib = (int)(blue * 255);
                         for(int ipy=0; ipy<qimg.height(); ipy++)
                         {
                             for(int ipx=0; ipx<qimg.width(); ipx++)
                             {
                                 QRgb pcol = qimg.pixel(ipx, ipy);
                                 //qDebug() << "pixel before : " << QColor(pcol).rgba();
                                 int pr = qRed(pcol);
                                 int pg = qGreen(pcol);
                                 int pb = qBlue(pcol);
                                 if( irange > 0 &&
                                         abs((pr&0xff) - (ir&0xff)) <= irange &&
                                         abs((pg&0xff) - (ig&0xff)) <= irange &&
                                         abs((pb&0xff) - (ib&0xff)) <= irange)
                                 {
                                     QColor ncol(pr,pg,pb,0);
                                     //qDebug() << "trange alpha : " << ncol.rgba();
                                     qimg.setPixel(ipx,ipy,ncol.rgba());
                                 }
                             }
                         }
                         lbm = QPixmap::fromImage(qimg);
                     }

                     if(vs->getblackandwhite() == 1) // black and white
                     {
                         QImage qimg = lbm.toImage();
                         lbm = QBitmap::fromImage(qimg/*,Qt::MonoOnly | Qt::OrderedDither*/);
                     }
                     else if(vs->getblackandwhite() == 2) // grey scale
                     {
                         QImage qimg = lbm.toImage();
                         lbm = QBitmap::fromImage(qimg/*,Qt::MonoOnly | Qt::OrderedDither*/);
                     }
                     if(drawmode == DM_SELECT)
                     {
                         QImage qimg = lbm.toImage();
                         qimg.invertPixels();
                         lbm = QPixmap::fromImage(qimg);
                     }

                     qpxm = new QGraphicsPixmapItem();
                     if(qpxm)
                     {

                         // get the bitmaps rect
                         QRectF brect(lbm.rect());

                         // set the local quad (usually a rectangle)
                         // we need to invert the y coordinates
                         QPolygonF poly1,poly2,poly3;
                         //poly1 << brect.topLeft() << brect.topRight() << brect.bottomRight() << brect.bottomLeft();
                         poly1 << brect.bottomLeft() << brect.bottomRight() << brect.topRight() << brect.topLeft();

                         poly2 << QPointF(p1.x,p1.y)
                               << QPointF(p2.x,p2.y)
                               << QPointF(p3.x,p3.y)
                               << QPointF(p4.x,p4.y);

                         poly3 << QPointF(0,0)
                               << QPointF(poly2.boundingRect().height(),0)
                               << QPointF(poly2.boundingRect().width(),poly2.boundingRect().height())
                               << QPointF(0,poly2.boundingRect().height());

                         // make the transform
                         QTransform transf;
                         QTransform::quadToQuad(poly1,poly3,transf);

                         // note: if the images is too big this does not work
                         // it is better to scale th graphics item
                         // see the other method

                         // transform the bit map and put it in the gitem
                         QImage timg = lbm.transformed(transf,Qt::SmoothTransformation).toImage();
                         //QPixmap tlbm(poly3.boundingRect().width(),poly3.boundingRect().height());
                         QPixmap tlbm = QPixmap::fromImage(timg);
                         qpxm->setPixmap(tlbm);
                         // offet it to the correct screen position
                         qpxm->setOffset(QPointF(p0.x-qpxm->boundingRect().width()/2.0,p0.y-qpxm->boundingRect().height()/2.0));

                         qpxm->setFlag(QGraphicsItem::ItemIsSelectable);
                         if(drawmode == DM_INVERT)
                         {
                             qpxm->setSelected(true);
                             qpxm->setOpacity(0.5);
                         }
                         else if(options.test(BitmapETransparent))
                             qpxm->setOpacity(opacity);

                         if(!usedByPlane && dynamic_cast<View3dSubSurface*>(vs) != 0)
                         {
                             //vs->getScene()->removeItem(qpxm);
                             qpxm->setParentItem(((View3dSubSurface*)vs)->qgicp);
                         }
                         else if(vs->getScene())
                             vs->getScene()->addItem(qpxm);

                     }
                }
#endif
#if 1
                 // pixmap output method #2
                 // slow but high res images
                 // transforms the graphic pixmap item in the scene
                 // this is slow but the images are high quality
                 //
                 if(qbmi != 0)
                 {
                     QPixmap lbm = QPixmap::fromImage(*qbmi);
                     //if(options.test(BitmapETransparent) && transparencyrange > 0.0)
                     if(options.test(BitmapETransparent))
                     {
                         QImage qimg1 = lbm.toImage();
                         QImage qimg = qimg1.convertToFormat(QImage::Format_RGBA8888);
                         int irange = (int)(transparencyrange * 255);
                         int iopacity = (int)(opacity * 255);
                         int ir = (int)(red * 255);
                         int ig = (int)(green * 255);
                         int ib = (int)(blue * 255);
                         for(int ipy=0; ipy<qimg.height(); ipy++)
                         {
                             for(int ipx=0; ipx<qimg.width(); ipx++)
                             {
                                 QRgb pcol = qimg.pixel(ipx, ipy);
                                 //qDebug() << "pixel before : " << QColor(pcol).rgba();
                                 int pr = qRed(pcol);
                                 int pg = qGreen(pcol);
                                 int pb = qBlue(pcol);
                                 if( irange > 0 &&
                                         abs((pr&0xff) - (ir&0xff)) <= irange &&
                                         abs((pg&0xff) - (ig&0xff)) <= irange &&
                                         abs((pb&0xff) - (ib&0xff)) <= irange)
                                 {
#if 0
                                     QColor ncol(pr,pg,pb,0);
                                     //qDebug() << "trange alpha : " << ncol.rgba();
                                     qimg.setPixel(ipx,ipy,ncol.rgba());
#endif
                                     QRgb newcol = qRgba(pr,pg,pb,0);
                                     qimg.setPixel(ipx,ipy,newcol);
                                 }
                                 else
                                 {
                                     QRgb newcol = qRgba(pr,pg,pb,iopacity);
                                     qimg.setPixel(ipx,ipy,newcol);
                                 }
                             }
                         }
                         lbm = QPixmap::fromImage(qimg);
                     }

                     if(vs->getblackandwhite() == 1) // black and white
                     {
                         QImage qimg = lbm.toImage();
                         lbm = QBitmap::fromImage(qimg/*,Qt::MonoOnly | Qt::OrderedDither*/);
                     }
                     else if(vs->getblackandwhite() == 2) // grey scale
                     {
                         QImage qimg = lbm.toImage();
                         lbm = QBitmap::fromImage(qimg/*,Qt::MonoOnly | Qt::OrderedDither*/);
                     }
                     if(drawmode == DM_SELECT)
                     {
                         QImage qimg = lbm.toImage();
                         qimg.invertPixels();
                         lbm = QPixmap::fromImage(qimg);
                     }

                     //qpxm = vs->getScene()->addPixmap(lbm);
                     qpxm = new QGraphicsPixmapItem(lbm);
                     if(qpxm)
                     {

                         // the bounding rect of the pixmap in its local coodinates
                         QRectF brect = qpxm->boundingRect();

                         // new method using Qt quad to quad transform
                         // make the quads for the transform
                         QPolygonF poly1,poly2;

                         // set the local quad (usually a rectangle)
                         // we need to invert the y coordinates
                         //poly1 << brect.topLeft() << brect.topRight() << brect.bottomRight() << brect.bottomLeft();
                         poly1 << brect.bottomLeft() << brect.bottomRight() << brect.topRight() << brect.topLeft();

                         poly2 << QPointF(p1.x,p1.y)
                               << QPointF(p2.x,p2.y)
                               << QPointF(p3.x,p3.y)
                               << QPointF(p4.x,p4.y);

                         // make the transform
                         QTransform transf;
                         QTransform::quadToQuad(poly1,poly2,transf);

                         // transform the pixmap
                         qpxm->setTransform(transf);
                         //
                         ///////////////////////////////////////////////

                         qpxm->setFlag(QGraphicsItem::ItemIsSelectable);
                         if(drawmode == DM_INVERT)
                         {
                             qpxm->setSelected(true);
                             qpxm->setOpacity(0.5);
                         }
#if 0
                         else if(options.test(BitmapETransparent))
                             qpxm->setOpacity(opacity);
#endif

                         if(!usedByPlane && dynamic_cast<View3dSubSurface*>(vs) != 0)
                         {
                             //vs->getScene()->removeItem(qpxm);
                             qpxm->setParentItem(((View3dSubSurface*)vs)->qgicp);
                         }
                         //else if(vs->getScene())
                         //    vs->getScene()->addItem(qpxm);
                         //qDebug() << "bm this pos : " << qpxm->scenePos();
                         //qDebug() << "width  : " << qpxm->sceneBoundingRect().width();
                         //qDebug() << "height : " << qpxm->sceneBoundingRect().height();
                     }
                 }
#endif
             }
             else
             {
                 // new method
                 // kmj: changed to use the Qt quad to quad transformation
                 //
                 // the bitmap is put in it local coordintate space by Qt
                 // we want to get the rotated scaled and projected rectangle in the realcad screen coodinate space
                 // then we map the bitmap quad to the quad in realcad screen space
                 // this wil give us a bitmap that appears to be 3D when in a 3d view

                 // p1 is the origin point
                 // p2 is the diagonal of the bounding box when scaled to screen coorindates
                 // this give us the basic rectangle of the bitmap
                 // p3 and p4 are copies of
                 double angle=0;
                 Point ps1,ps2,ps3,ps4;
                 ps1 = p1 - vs->getuaxis() * (hor*width/2.0) + vs->getvaxis() * ((2.0 - ver)/2.0*height);
                 ps2 = ps1 + vs->getuaxis() * width;
                 ps3 = ps2 - vs->getvaxis() * height;
                 ps4 = ps1 + vs->getvaxis() * height;
                 ps1 = vs->modeltoscreen(ps1);
                 ps2 = vs->modeltoscreen(ps2);
                 ps3 = vs->modeltoscreen(ps3);
                 ps4 = vs->modeltoscreen(ps4);
                 ps1.setxyz(floor(ps1.x+0.5),floor(ps1.y+0.5),0.0);
                 ps2.setxyz(floor(ps2.x+0.5),floor(ps2.y+0.5),0.0);
                 ps3.setxyz(floor(ps3.x+0.5),floor(ps3.y+0.5),0.0);
                 ps4.setxyz(floor(ps4.x+0.5),floor(ps4.y+0.5),0.0);

                 p1 = p1 - vs->getuaxis() * (hor*width/2.0) + vs->getvaxis() * ((2.0 - ver)/2.0*height);
                 p2 = p1 + vs->getuaxis() * width - vs->getvaxis() * height;
                 p1 = vs->modeltoscreen(p1);
                 p2 = vs->modeltoscreen(p2);
                 p1.setxyz(floor(p1.x+0.5),floor(p1.y+0.5),0.0);
                 p2.setxyz(floor(p2.x+0.5),floor(p2.y+0.5),0.0);
                 p3 = p1;
                 p4 = p2;

                 ix = int((p3.x - p1.x) / (p2.x - p1.x) * (bmi->bmiHeader.biWidth-1));
                 iy = int((p4.y - p2.y) / (p1.y - p2.y) * (bmi->bmiHeader.biHeight-1));
                 iw = int((p4.x - p1.x) / (p2.x - p1.x) * (bmi->bmiHeader.biWidth-1) - ix + 1);
                 ih = int((p3.y - p2.y) / (p1.y - p2.y) * (bmi->bmiHeader.biHeight-1) - iy + 1);

                 //Point p0; // origin point
                 Point txaxis = xaxis;// * xaxis.cross(yaxis).z;
                 Point tyaxis = yaxis;// * xaxis.cross(yaxis).z;

                 if (t == 0)
                     p1 = origin;
                 else
                     p1 = t->transform(origin);


                 xa =  txaxis * width;// * xaxis.cross(yaxis).z;
                 ya =  tyaxis * height;// * xaxis.cross(yaxis).z;

                 p0 = p1 + txaxis * width/2.0 + tyaxis * height/2.0;
                 p1 = p1 - txaxis * (hor*width/2.0) - txaxis * ver*height/2.0;
                 p2 = p1 + txaxis * width;
                 p3 = p2 + tyaxis * height;
                 p4 = p3 - txaxis * width;

                 Point temp = p1;
                 p0 = vs->modeltoscreen(p0);
                 p1 = vs->modeltoscreen(p1);
                 p2 = vs->modeltoscreen(temp + xa);
                 p3 = vs->modeltoscreen(temp + xa + ya);
                 p4 = vs->modeltoscreen(temp + ya);
#if 0
                 // Pixmap output method #1
                 // fast but low res images
                 // transforms the pixmap before putting it in the scene
                 // only offset the item in the scene
                 // this is faster that doing all the images transforms in the scene
                 // but the resulting images are lower quality
                 //
                 if(qbmi != 0)
                 {
                     QPixmap lbm = QPixmap::fromImage(*qbmi);
                     //QPixmap lbm = QPixmap::fromImage(qbmi->mirrored(signbit(xaxis.x),signbit(yaxis.y)));

                     if(options.test(BitmapETransparent) && transparencyrange > 0.0)
                     {
                         QImage qimg1 = lbm.toImage();
                         QImage qimg = qimg1.convertToFormat(QImage::Format_RGBA8888);
                         int irange = (int)(transparencyrange * 255);
                         int iopacity = (int)(opacity * 255);
                         int ir = (int)(red * 255);
                         int ig = (int)(green * 255);
                         int ib = (int)(blue * 255);
                         for(int ipy=0; ipy<qimg.height(); ipy++)
                         {
                             for(int ipx=0; ipx<qimg.width(); ipx++)
                             {
                                 QRgb pcol = qimg.pixel(ipx, ipy);
                                 //qDebug() << "pixel before : " << QColor(pcol).rgba();
                                 int pr = qRed(pcol);
                                 int pg = qGreen(pcol);
                                 int pb = qBlue(pcol);
                                 if( irange > 0 &&
                                         abs((pr&0xff) - (ir&0xff)) <= irange &&
                                         abs((pg&0xff) - (ig&0xff)) <= irange &&
                                         abs((pb&0xff) - (ib&0xff)) <= irange)
                                 {
                                     QColor ncol(pr,pg,pb,0);
                                     //qDebug() << "trange alpha : " << ncol.rgba();
                                     qimg.setPixel(ipx,ipy,ncol.rgba());
                                 }
                             }
                         }
                         lbm = QPixmap::fromImage(qimg);
                     }

                     if(vs->getblackandwhite() == 1) // black and white
                     {
                         QImage qimg = lbm.toImage();
                         lbm = QBitmap::fromImage(qimg/*,Qt::MonoOnly | Qt::OrderedDither*/);
                     }
                     else if(vs->getblackandwhite() == 2) // grey scale
                     {
                         QImage qimg = lbm.toImage();
                         lbm = QBitmap::fromImage(qimg/*,Qt::MonoOnly | Qt::OrderedDither*/);
                     }
                     if(drawmode == DM_SELECT)
                     {
                         QImage qimg = lbm.toImage();
                         qimg.invertPixels();
                         lbm = QPixmap::fromImage(qimg);
                     }

                     qpxm = new QGraphicsPixmapItem();

                     if(qpxm)
                     {

                         // get the bitmaps rect
                         QRectF brect(lbm.rect());

                         // set the local quad (usually a rectangle)
                         // we need to invert the y coordinates
                         QPolygonF poly1,poly2;
                         poly1 << brect.bottomLeft() << brect.bottomRight() << brect.topRight() << brect.topLeft();

                         // debug
                         //QGraphicsPolygonItem *gpoly1 = vs->getScene()->addPolygon(poly1);
                         //gpoly1->setPen(QPen(QColor(0,0,255)));

                         poly2 << QPointF(p1.x,vs->getheight()-1.0-p1.y)
                               << QPointF(p2.x,vs->getheight()-1.0-p2.y)
                               << QPointF(p3.x,vs->getheight()-1.0-p3.y)
                               << QPointF(p4.x,vs->getheight()-1.0-p4.y);

                         // debug
                         //QGraphicsPolygonItem *gpoly2 = vs->getScene()->addPolygon(poly2);
                         //gpoly2->setPen(QPen(QColor(255,0,0)));

                         // make the transform
                         QTransform transf;
                         QTransform::quadToQuad(poly1,poly2,transf);

                         // transform the bit map and put it in the gitem
                         QPixmap tlbm = lbm.transformed(transf,Qt::SmoothTransformation);
                         qpxm->setPixmap(tlbm);
                         // offset it to the correct screen position
                         qpxm->setOffset(QPointF(p0.x,vs->getheight()-1.0-p0.y)-qpxm->boundingRect().center());

                         // debug
                         //vs->getScene()->addEllipse(p0.x-100,vs->getheight()-1.0-p0.y-100,200,200);

                         qpxm->setFlag(QGraphicsItem::ItemIsSelectable);
                         if(drawmode == DM_INVERT)
                         {
                             qpxm->setSelected(true);
                             qpxm->setOpacity(0.5);
                         }
                         else if(options.test(BitmapETransparent))
                             qpxm->setOpacity(opacity);

                         if(!usedByPlane && dynamic_cast<View3dSubSurface*>(vs) != 0)
                         {
                             //vs->getScene()->removeItem(qpxm);
                             qpxm->setParentItem(((View3dSubSurface*)vs)->qgicp);
                         }
                         else if(vs->getScene())
                             vs->getScene()->addItem(qpxm);
                     }
                }
#endif
#if 1
                 // pixmap output method #2
                 // slow but high res images
                 // transforms the graphic pixmap item in thr scene
                 // this is slow but the images are high quality
                 //
                 QPixmap lbm = QPixmap::fromImage(*qbmi);
                 //if(options.test(BitmapETransparent) && transparencyrange > 0.0)
                 if(options.test(BitmapETransparent))
                 {
                     QImage qimg1 = lbm.toImage();
                     QImage qimg = qimg1.convertToFormat(QImage::Format_RGBA8888);
                     int irange = (int)(transparencyrange * 255);
                     int iopacity = (int)(opacity * 255);
                     int ir = (int)(red * 255);
                     int ig = (int)(green * 255);
                     int ib = (int)(blue * 255);
                     for(int ipy=0; ipy<qimg.height(); ipy++)
                     {
                         for(int ipx=0; ipx<qimg.width(); ipx++)
                         {
                             QRgb pcol = qimg.pixel(ipx, ipy);
                             //qDebug() << "pixel before : " << QColor(pcol).rgba();
                             int pr = qRed(pcol);
                             int pg = qGreen(pcol);
                             int pb = qBlue(pcol);
                             if( irange > 0 &&
                                     abs((pr&0xff) - (ir&0xff)) <= irange &&
                                     abs((pg&0xff) - (ig&0xff)) <= irange &&
                                     abs((pb&0xff) - (ib&0xff)) <= irange)
                             {
#if 0
                                 QColor ncol(pr,pg,pb,0);
                                 //qDebug() << "trange alpha : " << ncol.rgba();
                                 qimg.setPixel(ipx,ipy,ncol.rgba());
#endif
                                 QRgb newcol = qRgba(pr,pg,pb,0);
                                 qimg.setPixel(ipx,ipy,newcol);
                             }
                             else
                             {
                                 QRgb newcol = qRgba(pr,pg,pb,iopacity);
                                 qimg.setPixel(ipx,ipy,newcol);
                             }
                         }
                     }
                     lbm = QPixmap::fromImage(qimg);
                 }
                 if(vs->getblackandwhite() == 1) // black and white
                 {
                     QImage qimg = lbm.toImage();
                     lbm = QBitmap::fromImage(qimg/*,Qt::MonoOnly | Qt::OrderedDither*/);
                 }
                 else if(vs->getblackandwhite() == 2) // grey scale
                 {
                     QImage qimg = lbm.toImage();
                     lbm = QBitmap::fromImage(qimg/*,Qt::MonoOnly | Qt::OrderedDither*/);
                 }
                 if(drawmode == DM_SELECT)
                 {
                     QImage qimg = lbm.toImage();
                     qimg.invertPixels();
                     lbm = QPixmap::fromImage(qimg);
                 }
                 qpxm = vs->getScene()->addPixmap(lbm);
                 if(qpxm)
                 {
                     //////////////////////////////////////////////////////////
                     // quad to quad transform method
                     // this is the bounding rectangle of the pixmap in it local coords
                     QRectF brect = qpxm->boundingRect();
                     // make the quads for the transform
                     QPolygonF poly1,poly2;

                     // set the local quad (usually a rectangle)
                     // we need to invert the y coordinates
                     poly1 << brect.bottomLeft() << brect.bottomRight() << brect.topRight() << brect.topLeft();

                     // debug
                     //QGraphicsPolygonItem *gpoly1 = vs->getScene()->addPolygon(poly1);
                     //gpoly1->setPen(QPen(QColor(0,0,255)));

                     // set the realcad screen quad (may not be a rectangle in screen space)
                     poly2 << QPointF(p1.x,vs->getheight()-1.0-p1.y)
                           << QPointF(p2.x,vs->getheight()-1.0-p2.y)
                           << QPointF(p3.x,vs->getheight()-1.0-p3.y)
                           << QPointF(p4.x,vs->getheight()-1.0-p4.y);

                     //qDebug() << "poly1 : " << poly1.boundingRect();
                     //qDebug() << "poly2 : " << poly2.boundingRect();

                     // debug
                     //QGraphicsPolygonItem *gpoly2 = vs->getScene()->addPolygon(poly2);
                     //QTransform dbt1;
                     //dbt1.scale(0.5,0.5);
                     //dbt1.translate(gpoly2->boundingRect().center().x(),gpoly2->boundingRect().center().y());
                     //gpoly2->setTransform(dbt1);
                     //gpoly2->setPen(QPen(QColor(255,0,0)));
                     //

                     // make the transform
                     QTransform transf;
                     QTransform::quadToQuad(poly1,poly2,transf);

                     // transform the pixmap
                     qpxm->setTransform(transf);

                     // end quad to quad transform method
                     ///////////////////////////////////////////////////////////
                     // debug
                     //QRectF pxmbrect = qpxm->boundingRect();
                     //vs->getScene()->addEllipse(p0.x-100,vs->getheight()-1.0-p0.y-100,200,200);
                     //QGraphicsEllipseItem *gellipt = vs->getScene()->addEllipse(pxmbrect.center().x()-100,pxmbrect.center().y()-50,200,100);
                     //gellipt->setTransform(transf);
                     //QPolygonF poly1t(poly1);
                     //QGraphicsPolygonItem *gpoly1t = vs->getScene()->addPolygon(poly1t);
                     //gpoly1t->setTransform(transf);
                     //QGraphicsRectItem *gpxmbrect = vs->getScene()->addRect(pxmbrect);
                     //gpxmbrect->setTransform(transf);
                     //QPen polytpen(QColor(255,0,255));
                     //polytpen.setWidth(20);
                     //gpoly1t->setPen(polytpen);
                     //gpxmbrect->setPen(polytpen);
                     //

                     qpxm->setFlag(QGraphicsItem::ItemIsSelectable);
                     if(drawmode == DM_INVERT)
                     {
                         qpxm->setSelected(true);
                         qpxm->setOpacity(0.5);
                     }
#if 0
                     else
                         if(options.test(BitmapETransparent))
                             qpxm->setOpacity(opacity);
#endif

                     if(!usedByPlane && dynamic_cast<View3dSubSurface*>(vs) != 0)
                         qpxm->setParentItem(((View3dSubSurface*)vs)->qgicp);
                     //else if(vs->getScene())
                     //    vs->getScene()->addItem(qpxm);

                 }
#endif
             }
         }

     vs->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

     if (styl == 1 || styl == 2 || SetLight != 0  && drawmode == DM_INVERT)
     {
          QGraphicsItem *qline;
          QList<QGraphicsItem *> glist;

          if (t == 0)
            p1 = origin;
          else
            p1 = t->transform(origin);

          // always 3d
#if 1
          xa =  xaxis * width;
          ya =  yaxis * height;
#else
          //if (SetLight != 0)
          if (SetLight != 0 && options.test(9))
          {
              // 3d
              xa =  xaxis * width;
               ya =  yaxis * height;
          }
          else
          {
              // 2d
              xa =   vs->getuaxis() * width;
               ya =  vs->getvaxis() * height;
          }
#endif
          p1 = p1 - xa * (hor/2.0) - ya * ver / 2.0;


          xa1 = xa / 20.0;
          ya1 = ya / height * width / 20.0;

          qline = vs->line3d((QGraphicsItem*)0,p1,p1 + xa); if(qline) glist.append(qline);
          qline = vs->line3d((QGraphicsItem*)0,p1 + xa,p1 + xa + ya); if(qline) glist.append(qline);
          qline = vs->line3d((QGraphicsItem*)0,p1 + xa + ya,p1 + ya); if(qline) glist.append(qline);
          qline = vs->line3d((QGraphicsItem*)0,p1 + ya,p1); if(qline) glist.append(qline);

          if (styl == 2)
          {
               qline = vs->line3d((QGraphicsItem*)0,p1 - xa1 - ya1,p1 + xa + xa1 - ya1);if(qline) glist.append(qline);
               qline = vs->line3d((QGraphicsItem*)0,p1 + xa + xa1 - ya1,p1 + xa + ya + xa1 + ya1);if(qline) glist.append(qline);
               qline = vs->line3d((QGraphicsItem*)0,p1 + xa + ya + xa1 + ya1,p1 + ya - xa1 + ya1);if(qline) glist.append(qline);
               qline = vs->line3d((QGraphicsItem*)0,p1 + ya - xa1 + ya1,p1 - xa1 - ya1);if(qline) glist.append(qline);
               qline = vs->line3d((QGraphicsItem*)0,p1,p1 - xa1 - ya1);if(qline) glist.append(qline);
               qline = vs->line3d((QGraphicsItem*)0,p1 + xa,p1 + xa + xa1 - ya1);if(qline) glist.append(qline);
               qline = vs->line3d((QGraphicsItem*)0,p1 + xa + ya,p1 + xa + ya + xa1 + ya1);if(qline) glist.append(qline);
               qline = vs->line3d((QGraphicsItem*)0,p1 + ya,p1 + ya - xa1 + ya1);if(qline) glist.append(qline);
          }

          if(glist.size() > 0)
              qframe = vs->getScene()->createItemGroup(glist);
          glist.clear();
     }

     QGraphicsItemGroup *qgroup = new QGraphicsItemGroup();

     if(qgroup)
     {
         vs->getScene()->addItem(qgroup);
         if(rcpxm)
             qgroup->addToGroup(rcpxm);
         //else if(!usedByPlane && ((View3dSubSurface*)vs)->qgicp && dynamic_cast<View3dSubSurface*>(vs) != 0)
         //    qgroup->setParentItem(((View3dSubSurface*)vs)->qgicp);
         else if(qpxm)
             qgroup->addToGroup(qpxm);
         if(qframe)
             qgroup->addToGroup(qframe);
         qgi = qgroup;

         if(!usedByPlane && ((View3dSubSurface*)vs)->qgicp && dynamic_cast<View3dSubSurface*>(vs) != 0)
             qgi->setParentItem(((View3dSubSurface*)vs)->qgicp);

         if(getselected())
             qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
         else
            qgi->setZValue(zValue);


         if(qgi)
         {
             if(drawmode == DM_INVERT)
             {
                 if(vs->qgirb == 0)
                 {
                     vs->qgirb = new QGraphicsItemGroup();
                     vs->getScene()->addItem(vs->qgirb);
                     ((QGraphicsItemGroup*)vs->qgirb)->addToGroup(qgi);
                 }
                 else
                     ((QGraphicsItemGroup*)vs->qgirb)->addToGroup(qgi);

                 qgi=0;
             }
         }

         if(drawmode == DM_PRINT)
             qgi->setZValue(zValue);

     }

#else
 Point p1,p2,p3,p4,xa,ya,xa1,ya1,pt1;
 int i,hor,ver,styl,ix,iy,iw,ih;
 DWORD rop;

  if (! defined)
    return;
  if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;


//  if (t == NULL)
//    {  if (vs->displaylist.draw(this,drawmode)) return;
//       if (! vs->displaylist.geterrorstatus())
//         addtodisplaylist(vs,0);
//       if (vs->displaylist.draw(this,drawmode)) return;
//    }

  hor = options.test(2) * 2 + options.test(1);
  ver = options.test(4) * 2 + options.test(3);
  styl = options.test(7) * 4 + options.test(6) * 2 + options.test(5);
  if (t == 0)
    p1 = origin;
  else
    p1 = t->transform(origin);

  if (SetLight != 0 && options.test(9) && bmi->bmiHeader.biBitCount > 1)
    {//  This is a transparent (3d) bitmap
     int texture;
     Point p[9];

       db.setrequiresopengl(1);

       if (OpenOpenGl(vs->gethdc(),vs->gethwnd() == 0 ? 2 : 3,0,0,(int)vs->getwidth(),(int)vs->getheight()) == 1 || options.test(31))
         {//  This is a new OpenGL Device context
          View *view;
            view = vs->getview();
            SetView(view->geteye().x,view->geteye().y,view->geteye().z,view->getref().x,view->getref().y,view->getref().z,view->getup().x,view->getup().y,view->getup().z,view->getfront(),view->getback(),view->getperspective());
            SetViewport(vs->getumin(),vs->getvmin(),vs->getumax(),vs->getvmax());
            SetBackground(::v.getreal("wn::backgroundcolour.red"),::v.getreal("wn::backgroundcolour.green"),::v.getreal("wn::backgroundcolour.blue"));
            BeginScene(BS_FRONTBUFFERONLY | BS_IGNOREDEPTH | BS_NOCLEAR * options.test(31));
            SetLight(view->geteye().x*100000.0,view->geteye().y*100000.0,view->geteye().z*100000.0);

            if (! options.test(31))  //  Use options 31 as an indictator not to invalidate
              InvalidateRect(vs->gethwnd(),0,TRUE);

         }

       if (drawmode != DM_INVERT)
         {  if (drawmode == DM_NORMAL)
              {  SetMaterial(1.0,1.0,1.0,1.0,1.0,0.0);
                 LoadMemoryTexture(filename,(RCCHAR *)pbuffer,bmi->bmiHeader.biWidth,bmi->bmiHeader.biHeight,red,green,blue,transparencyrange,opacity,&texture);
                 SetTexture(texture);
              }
            else if (drawmode == DM_SELECT)
              {  SetMaterial(0.0,0.0,1.0,1.0,1.0,0.0);
                 LoadMemoryTexture(filename,(RCCHAR *)pbuffer,bmi->bmiHeader.biWidth,bmi->bmiHeader.biHeight,red,green,blue,transparencyrange,opacity,&texture);
                 SetTexture(texture);
              }
            else 
              {  SetMaterial(v.getreal("wn::backgroundcolour.red"),v.getreal("wn::backgroundcolour.green"),v.getreal("wn::backgroundcolour.blue"),1.0,1.0,0.0);
                 SetTexture(0);
              }

            p1 = p1 - xaxis * (hor*width/2.0) - yaxis * ver*height/2.0;
            p2 = p1 + xaxis * width;
            p3 = p2 + yaxis * height;
            p4 = p3 - xaxis * width;

            p1 = vs->modeltransform.transform(p1);
            p2 = vs->modeltransform.transform(p2);
            p3 = vs->modeltransform.transform(p3);
            p4 = vs->modeltransform.transform(p4);

            p[0] = p1;
            p[1] = p2; 
            p[2] = p3; 
            p[3] = xaxis.cross(yaxis);
            p[4] = p[3];
            p[5] = p[3];
            p[6] = Point(0.001,0.001,0.0);
            p[7] = Point(0.999,0.001,0.0);
            p[8] = Point(0.999,0.999,0.0);
            DrawTexturedTriangle(p);

            p[0] = p1;
            p[1] = p3; 
            p[2] = p4; 
            p[3] = xaxis.cross(yaxis);
            p[4] = p[3];
            p[5] = p[3];
            p[6] = Point(0.001,0.001,0.0);
            p[7] = Point(0.999,0.999,0.0);
            p[8] = Point(0.001,0.999,0.0);
            DrawTexturedTriangle(p);

            Flush();
         }

    }
  else
    {  p1 = p1 - vs->getuaxis() * (hor*width/2.0) + vs->getvaxis() * ((2.0 - ver)/2.0*height);
       p2 = p1 + vs->getuaxis() * width - vs->getvaxis() * height;
       p1 = vs->modeltoscreen(p1);
       p2 = vs->modeltoscreen(p2);
       p1.setxyz(floor(p1.x+0.5),floor(p1.y+0.5),0.0);
       p2.setxyz(floor(p2.x+0.5),floor(p2.y+0.5),0.0);
       p3 = p1;
       p4 = p2;
       if (p3.x > vs->getwidth() - 1 || p4.x < 0.0 ||
           p3.y < 0.0 || p4.y > vs->getheight() - 1 ||
           p4.x - p3.x < 1.0 || p3.y - p4.y < 1.0)
         return;

       if (p3.x < 0.0) p3.x = 0.0;
       if (p4.x >= vs->getwidth()-1) p4.x = vs->getwidth()-1;
       if (p4.y < 0.0) p4.y = 0.0;
       if (p3.y >= vs->getheight()-1) p3.y = vs->getheight()-1;

       ix = int((p3.x - p1.x) / (p2.x - p1.x) * (bmi->bmiHeader.biWidth-1));
       iy = int((p4.y - p2.y) / (p1.y - p2.y) * (bmi->bmiHeader.biHeight-1));
       iw = int((p4.x - p1.x) / (p2.x - p1.x) * (bmi->bmiHeader.biWidth-1) - ix + 1);
       ih = int((p3.y - p2.y) / (p1.y - p2.y) * (bmi->bmiHeader.biHeight-1) - iy + 1);


       SetStretchBltMode(vs->gethdc(),options.test(31) ? HALFTONE : COLORONCOLOR);

       if (drawmode == DM_INVERT)
         rop = SRCINVERT;
       else if (drawmode == DM_ERASE)
         rop = WHITENESS;
       else if (drawmode == DM_SELECT)
         rop = NOTSRCCOPY;
       else
         rop = SRCCOPY;

       if (vs->gettransform2d())
         {double a1,b1,c1,a2,b2,c2,t,x3,y3,x4,y4;
            vs->gettransform2d(&a1,&b1,&c1,&a2,&b2,&c2);
            x3 = a1 * p3.x + b1 * p3.y + c1;
            y3 = a2 * p3.x + b2 * p3.y + c2;
            x4 = a1 * p4.x + b1 * p4.y + c1;
            y4 = a2 * p4.x + b2 * p4.y + c2;
            if (x4 < x3) {  t = x3;  x3 = x4;  x4 = t;  }
            if (y4 < y3) {  t = y3;  y3 = y4;  y4 = t;  }
            if (drawmode == DM_ERASE)
              {RECT rect;
               RCHBRUSH hbrush;
                 rect.left = (int)x3;  rect.top = (int)y3;  rect.right = (int)x4;  rect.bottom = (int)y4;
                 hbrush = CreateSolidBrush(vs->getbackground());
                 FillRect(vs->gethdc(),&rect,hbrush);
                 DeleteObject(hbrush);
              }
            else if (fabs(a2) < 0.0001 && fabs(b1) < 0.0001)
              StretchDIBits(vs->gethdc(),int(x3),int(y3+1.0),int(x4 - x3 + 1.0),int(y4 - y3 + 1.0),
                            ix,iy,iw,ih,pbuffer,bmi,DIB_RGB_COLORS,rop);
            else
              {  transpose();
                 reverse(0);
                 StretchDIBits(vs->gethdc(),int(x3),int(y3+1.0),int(x4 - x3 + 1.0),int(y4 - y3 + 1.0),
                               iy,ix,ih,iw,pbuffer,bmi,DIB_RGB_COLORS,rop);
                 reverse(0);
                 transpose();  
              }
         }
       else if (drawmode == DM_ERASE)
         {RECT rect;
          RCHBRUSH hbrush;
          int ix3,iy3,ix4,iy4,t;
            ix3 = (int)(p3.x);
            ix4 = ix3 + int(p4.x - p3.x + 1.0);
            iy3 = (int)(vs->getheight() - p3.y);
            iy4 = ((int)iy3) + int(p3.y - p4.y + 1.0);
            if (ix4 < ix3) {  t = ix3;  ix3 = ix4;  ix4 = t;  }
            if (iy4 < iy3) {  t = iy3;  iy3 = iy4;  iy4 = t;  }
            rect.left = ix3-1;  rect.top = iy3-1;  rect.right = ix4+1;  rect.bottom = iy4+1;
            hbrush = CreateSolidBrush(vs->getbackground());
            FillRect(vs->gethdc(),&rect,hbrush);
            DeleteObject(hbrush);
         }
       else
       {
           QGraphicsPixmapItem *pxm = vs->getScene()->addPixmap(qbmi->scaled(int(p4.x-p3.x+1.0),int(p3.y - p4.y + 1.0)));
           pxm->setPos(int(p3.x),int(vs->getheight()-1.0-p3.y));
           //StretchDIBits(vs->gethdc(),int(p3.x),int(vs->getheight()-1.0-p3.y),int(p4.x-p3.x+1.0),int(p3.y - p4.y + 1.0),ix,iy,iw,ih,pbuffer,bmi,DIB_RGB_COLORS,rop);
       }
  }
 
  vs->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

  if (styl == 1 || styl == 2 || SetLight != 0 && options.test(9) && drawmode == DM_INVERT)
    {  if (t == 0)
         p1 = origin;
       else
         p1 = t->transform(origin);

       if (SetLight != 0 && options.test(9))
         {  xa =  xaxis * width;
            ya =  yaxis * height;
         }
       else
         {  xa =   vs->getuaxis() * width;
            ya =  vs->getvaxis() * height;
         }


       p1 = p1 - xa * (hor/2.0) - ya * ver / 2.0;
       xa1 = xa / 20.0;
       ya1 = ya / height * width / 20.0;

       vs->line3d(p1,p1 + xa);
       vs->line3d(p1 + xa,p1 + xa + ya);
       vs->line3d(p1 + xa + ya,p1 + ya);
       vs->line3d(p1 + ya,p1);

       if (styl == 2)
         {  vs->line3d(p1 - xa1 - ya1,p1 + xa + xa1 - ya1);
            vs->line3d(p1 + xa + xa1 - ya1,p1 + xa + ya + xa1 + ya1);
            vs->line3d(p1 + xa + ya + xa1 + ya1,p1 + ya - xa1 + ya1);
            vs->line3d(p1 + ya - xa1 + ya1,p1 - xa1 - ya1);
            vs->line3d(p1,p1 - xa1 - ya1);
            vs->line3d(p1 + xa,p1 + xa + xa1 - ya1);
            vs->line3d(p1 + xa + ya,p1 + xa + ya + xa1 + ya1);
            vs->line3d(p1 + ya,p1 + ya - xa1 + ya1);
         }
    }

  if (gethandles())
    {  vs->setup(linestylescale,0.0,drawmode == DM_ERASE ? DM_ERASE : DM_NORMAL,colour,0,0,0);
       p1 = origin;

       if (SetLight != 0 && options.test(9))
         {  xa = xaxis * width;
            ya = yaxis * height;
         }
       else
         {  xa = vs->getuaxis() * width;
            ya = vs->getvaxis() * height;
         }


       p1 = p1 - xa * (hor/2.0) - ya * ver / 2.0;

       for (i = 0 ; i < 4 ; i++)
         {  if (i == 0)
              pt1 = vs->modeltoscreen(p1);
            else if (i == 1)
              pt1 = vs->modeltoscreen(p1+xa);
            else if (i == 2)
              pt1 = vs->modeltoscreen(p1+xa+ya);
            else if (i == 3)
              pt1 = vs->modeltoscreen(p1+ya);
            vs->line(int(pt1.x)-2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)-2);
            vs->line(int(pt1.x)+2,int(pt1.y)-2,int(pt1.x)+2,int(pt1.y)+2);
            vs->line(int(pt1.x)+2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)+2);
            vs->line(int(pt1.x)-2,int(pt1.y)+2,int(pt1.x)-2,int(pt1.y)-2);
         }
   }

/*
  if (t == NULL)
    pt1 = vs->modeltoview(p1);
  else
    {  pt1 = vs->modeltoview(t->transform(p1));
       pt2 = vs->modeltoview(t->transform(p2));
    }
  if (vs->clip3d(&pt1,&pt2))
    {  pt1 = vs->viewtoscreen(pt1);
       pt2 = vs->viewtoscreen(pt2);
       vs->line(int(pt1.x),int(pt1.y),int(pt2.x),int(pt2.y));
    }
*/
#endif
}

int BitMapE::pickhandlepoint(View3dSurface *vs,int distance,int x,int y,Point *pos)
{Point p1,p2,pt1,xa,ya;
 double d,dmin;
 int handle,i,hor,ver;

  if (! defined)
    return 0;

  hor = options.test(2) * 2 + options.test(1);
  ver = options.test(4) * 2 + options.test(3);

  p1 = origin;
  //if (SetLight != 0)
#if 1
  // KMJ: always 3d
  xa = xaxis * width;
  ya = yaxis * height;
#else
  if (SetLight != 0 && options.test(9))
    {  xa = xaxis * width;
       ya = yaxis * height;
    }
  else
    {  xa = vs->getuaxis() * width;
       ya = vs->getvaxis() * height;
    }
#endif
  p1 = p1 - xa * (hor/2.0) - ya * ver/2.0;

  dmin = -1.0;
  handle = 0;
  for (i = 0 ; i < 4 ; i++)
    {  
       if (i == 0)
         pt1 = p1;
       else if (i == 1)
         pt1 = p1 + xa;
       else if (i == 2)
         pt1 = p1 + xa + ya;
       else if (i == 3)
         pt1 = p1 + ya;

       if ((d = vs->pick(pt1,distance,x,y)) >= 0.0 && (handle == 0 || d < dmin))
         {  handle = i+1;  
            *pos = pt1;
            dmin = d;
         }

    }

  return handle;
}

void BitMapE::movehandlepoint(View3dSurface *vs,int drawentity,int handle,Point p)
{Point p1,p2,pt1,pt2,xa,ya;
 int hor,ver;

  if (GetLockedLayerTable()->test(layer)) return;  

  if (! defined)
    return;

  if (drawentity) draw(DM_ERASE);
  db.saveundo(UD_MOVED,this);
  hor = options.test(2) * 2 + options.test(1);
  ver = options.test(4) * 2 + options.test(3);

  p1 = origin;
  //if (SetLight != 0)
#if 1
   // KMJ: always 3d now
    xa = xaxis * width;
    ya = yaxis * height;
#else
  if (SetLight != 0 && options.test(9))
    {  xa = xaxis * width;
       ya = yaxis * height;
    }
  else
    {  xa = vs->getuaxis() * width;
       ya = vs->getvaxis() * height;
    }
#endif

  p1 = p1 - xa * (hor/2.0) - ya * ver/2.0;

  if (handle == 1)
    {  pt1 = p;
       pt2 = p1 + xa + ya;
    }
  else if (handle == 2)
    {  pt1 = p;
       pt2 = p1 + ya;
    }
  else if (handle == 3)
    {  pt1 = p1;
       pt2 = p;
    }
  else if (handle == 4)
    {  pt1 = p1 + xa;
       pt2 = p;
    }
  
  if ((pt2 - pt1).length() > db.getptoler())
    {  width = fabs((pt2 - pt1).dot(xa.normalize()));
       height = fabs((pt2 - pt1).dot(ya.normalize()));
       origin = (pt1 + pt2) / 2.0 + xa.normalize() * ((hor-1)*width/2.0) - ya.normalize() * ((1.0 - ver)/2.0*height);
    }
  if (drawentity) draw(DM_NORMAL);
}

int BitMapE::hide(HideImage *)
{ return 1;
}

int BitMapE::hideGL(HideImage *)
{ return 1;
}

double BitMapE::pick(View3dSurface *vs,double x,double y,Point *pos)
{Point p1,p2,p3,p4,xa,ya;
 int hor,ver;
 double d,dmin;
 View3dWindow *window;

  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  if (! defined)
    return -1;

//if (pos == NULL && vs->displaylist.defined(this))
//  return vs->displaylist.pick(this,x,y);

  hor = options.test(2) * 2 + options.test(1);
  ver = options.test(4) * 2 + options.test(3);

  p1 = origin;
  //if (SetLight != 0)
#if 1
   // KMJ: always 3d
   xa = xaxis * width;
   ya = yaxis * height;
#else
  if (SetLight != 0 && options.test(9))
    {  xa = xaxis * width;
       ya = yaxis * height;
    }
  else
    {  xa = vs->getuaxis() * width;
       ya = vs->getvaxis() * height;
    }
#endif

  p1 = p1 - xa * (hor/2.0) + ya * ((2.0 - ver)/2.0);
  p2 = p1 + xa;
  p3 = p2 - ya;
  p4 = p3 - xa;
  dmin = -1.0;
  if ((d = vs->pick(p1,p2,x,y,pos)) >= 0.0 && (dmin < 0.0 || d < dmin))
    dmin = d;
  if ((d = vs->pick(p2,p3,x,y,pos)) >= 0.0 && (dmin < 0.0 || d < dmin))
    dmin = d;
  if ((d = vs->pick(p3,p4,x,y,pos)) >= 0.0 && (dmin < 0.0 || d < dmin))
    dmin = d;
  if ((d = vs->pick(p4,p1,x,y,pos)) >= 0.0 && (dmin < 0.0 || d < dmin))
    dmin = d;
  return dmin;
}

int BitMapE::pick(View3dSurface *vs,int inside,int x1,int y1,int x2,int y2)
{Point p1,p2,p3,p4,xa,ya;
 int hor,ver;
 View3dWindow *window;
  
  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  if (! defined)
    return 0;

// return (vs->pick(p1,inside,x1,y1,x2,y2) +
//         vs->pick(p2,inside,x1,y1,x2,y2)) == 2;

  hor = options.test(2) * 2 + options.test(1);
  ver = options.test(4) * 2 + options.test(3);

  p1 = origin;
  //if (SetLight != 0)
#if 1
  // KMJ: always 3d
  xa = xaxis * width;
  ya = yaxis * height;
#else
  if (SetLight != 0 && options.test(9))
  {  xa = xaxis * width;
     ya = yaxis * height;
  }
else
  {  xa = vs->getuaxis() * width;
     ya = vs->getvaxis() * height;
  }
#endif
  p1 = p1 - xa * (hor/2.0) + ya * ((2.0 - ver)/2.0);
  p2 = p1 + xa;
  p3 = p2 - ya;
  p4 = p3 - xa;

  return (vs->pick(p1,inside,x1,y1,x2,y2) +
          vs->pick(p2,inside,x1,y1,x2,y2) +
          vs->pick(p3,inside,x1,y1,x2,y2) +
          vs->pick(p4,inside,x1,y1,x2,y2)) == 4;
}

int BitMapE::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{Point p1,p2,p3,p4,xa,ya;
 int hor,ver,picked;
 View3dWindow *window;

  window = dynamic_cast<View3dWindow *>(vs);
  if (window != 0 && window->getoffscreenbitmap() != 0)
    vs = window->getoffscreenbitmap();
 
  if (! defined)
    return 0;

  hor = options.test(2) * 2 + options.test(1);
  ver = options.test(4) * 2 + options.test(3);
  p1 = origin;
  //if (SetLight != 0)
#if 1
  xa = xaxis * width;
  ya = yaxis * height;
#else
  if (SetLight != 0 && options.test(9))
    {  xa = xaxis * width;
       ya = yaxis * height;
    }
  else
    {  xa = vs->getuaxis() * width;
       ya = vs->getvaxis() * height;
    }
#endif
  p1 = p1 - xa * (hor/2.0) + ya * ((2.0 - ver)/2.0);
  p2 = p1 + xa;
  p3 = p2 - ya;
  p4 = p3 - xa;


  picked = inside != 2;

  if (vs->pick(nvertices,pwvertices,p1,p2,inside,&picked))
    return picked;
  if (vs->pick(nvertices,pwvertices,p2,p3,inside,&picked))
    return picked;
  if (vs->pick(nvertices,pwvertices,p3,p4,inside,&picked))
    return picked;
  if (vs->pick(nvertices,pwvertices,p4,p1,inside,&picked))
    return picked;

  return picked;

}

int BitMapE::npoints(void)
{  return 1;
}

Point BitMapE::point(int)
{ return origin;
}

Point BitMapE::nearp(Point)
{ return origin;
}

int BitMapE::stretchent(Database *db,Transform &t,SortedPointTable *,int erase)
{ if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  return 1;
}

int BitMapE::move(Database *db,Transform &t,int erase,int save)
{Entity *e;
 double xs,ys;
 Point o,xa,ya,za;

  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return db == 0 ? clone(t) != 0 : db->geometry.add(clone(t));
  if (db != 0)
    { 
	  if (save)
		db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  if (erase) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 1;

  o = t.transform(origin);
  xa = t.transform(xaxis+origin) - o;
  ya = t.transform(yaxis+origin) - o;
  za = xa.cross(ya);
  za.normalize();
  ya = za.cross(xa);
  xs = xa.length();
  ys = ya.length();
  xa.normalize();
  ya.normalize();

  origin = o;
  xaxis = xa;
  yaxis = ya;

  width *= xs;
  height *= ys;

  copye = this;

  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);

 return 1;
}

Entity *BitMapE::clone(Transform &t)
{unsigned char *pb;
 BITMAPINFO *bmi1;
 Point o,xa,ya,za;
 double xs,ys;

  if (copye != NULL) return copye;
  pb = new unsigned char [sizeofbuffer()];
  memcpy(pb,pbuffer,sizeofbuffer());
  bmi1 = (BITMAPINFO *) new char [sizeofbmi()];
  memcpy(bmi1,bmi,sizeofbmi());

  o = t.transform(origin);
  xa = t.transform(xaxis+origin) - o;
  ya = t.transform(yaxis+origin) - o;
  za = xa.cross(ya);
  za.normalize();
  ya = za.cross(xa);
  xs = xa.length();
  ys = ya.length();
  xa.normalize();
  ya.normalize();

  copye = new BitMapE(*this,filename,bmi1,pb,o,xa,ya,width * xs,height * ys,red,green,blue,transparencyrange,opacity,options);

  ((BitMapE *)copye)->originallength = originallength;
  if (originalbuffer == 0)
    ((BitMapE *)copye)->originalbuffer = 0;
  else
    {  ((BitMapE *)copye)->originalbuffer = new unsigned char[originallength];
       memcpy(((BitMapE *)copye)->originalbuffer,originalbuffer,originallength);
    }
  
  copye->cloneattributes(this);

  return copye;
}

int BitMapE::project(Database *db,Transform &t,int)
{ if ((status & 2) == 2) return 1;
  if (copye == 0 && ! db->geometry.add(clone(t))) return 0;
  return 1;
}

int BitMapE::save(CadFile *outfile)
{short type,sl;
 long ll;
 RCCHAR exedir[600],fn[600],fncmp[600];

  type = bitmap_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  if (! outfile->write(&width)) return 0;
  if (! outfile->write(&height)) return 0;
  options.set(BitmapENewFormat,1);
  if (! options.save(outfile)) return 0;

  //qDebug() << "bitmap filename : " << QString(filename);
  strcpy(exedir,home.getpublicfilename(_RCT("")));  _strupr(exedir);
  //qDebug() << "bitmap exedir : " << QString(exedir);
  strcpy(fn,filename);
  strcpy(fncmp,QDir::toNativeSeparators(QString(filename)));  _strupr(fncmp);
  //qDebug() << "bitmap fn : " << QString(fn);
  //qDebug() << "bitmap fncmp : " << QString(fncmp);
  if (strncmp(exedir,fncmp,strlen(exedir)) == 0)
  {
      strcpy(fn,"::CADHD::");  strcat(fn,filename+strlen(exedir));
  }
  //qDebug() << "bitmap fn 2 : " << QString(fn);
  if (! outfile->write(fn)) return 0;
  if (! outfile->write(&red)) return 0;
  if (! outfile->write(&green)) return 0;
  if (! outfile->write(&blue)) return 0;
  if (! outfile->write(&transparencyrange)) return 0;
  if (! outfile->write(&opacity)) return 0;

  if (! options.test(BitmapELinkFile))
    {  sl = (short)sizeofbmi();
       if (! outfile->write(&sl)) return 0;
       if (! outfile->blockwrite(bmi,sl)) return 0;
       if (options.test(8))
         {  ll = originallength;
            if (! outfile->write(&ll)) return 0;
            if (! outfile->blockwrite(originalbuffer,ll)) return 0;
         }
       else
         {  ll = sizeofbuffer();
            if (! outfile->write(&ll)) return 0;
            if (! outfile->blockwrite(pbuffer,ll)) return 0;
         }
     }
  return 1;
}

Entity *BitMapE::load(int,CadFile *infile)
{BitMapE *bitmap;
 unsigned char *pbuffer;
 BITMAPINFO *bmi;
 EntityHeader *header;
 Point origin,xaxis,yaxis;
 double width,height,red,green,blue,transparencyrange,opacity;
 BitMask options(32);
 short sl;
 long ll;
 RCCHAR *fn,filename[600];

  if ((header = loadheader(infile)) == NULL) 
     return 0;
  if (! origin.load(infile)) 
    return 0;
  if (! xaxis.load(infile)) 
    return 0;
  if (! yaxis.load(infile)) 
    return 0;
  if (! infile->read(&width)) 
    return 0;
  if (! infile->read(&height))  
    return 0;
  if (! infile->read(&options)) 
    return 0;

  if (options.test(BitmapENewFormat))
    {  //  Load all the new options as well
      if(infile->getversion() == 2)
      {
          char *cfname=0;
          if (! infile->read(&cfname)) return 0;
          fn = new RCCHAR[strlen(cfname)+1];
          strcpy(fn,cfname);
          delete [] cfname;
      }
      else
          if (! infile->read(&fn)) return 0;
      //qDebug() << "bitmap filename 1 : " << QString(fn);
       if (strncmp(fn,"::CADHD::",9) == 0)
         strcpy(filename,home.getpublicfilename(fn+9));
       else
         {  strncpy(filename,fn,600);
            filename[599] = 0;
         }
       //qDebug() << "bitmap filename 1 : " << QString(filename);
       if (! infile->read(&red))
         return 0;
       if (! infile->read(&green)) 
         return 0;
       if (! infile->read(&blue)) 
         return 0;
       if (! infile->read(&transparencyrange)) 
         return 0;
       if (! infile->read(&opacity)) 
         return 0;
    }
  else
    {  strcpy(filename,"");
       red = green = blue = 1.0;
       transparencyrange = 0.004;
       opacity = 1.0;
    }

  if (! options.test(BitmapELinkFile))
    {  if (! infile->read(&sl)) 
         return 0;
       bmi = (BITMAPINFO *) new char[sl];
       if (! infile->blockread(bmi,sl)) 
         return 0;
       if (! infile->read(&ll)) return 0;

       if (ll < (int)bmi->bmiHeader.biSizeImage)
         {  pbuffer = new unsigned char[bmi->bmiHeader.biSizeImage];
            memset(pbuffer,0,bmi->bmiHeader.biSizeImage);
         }
       else
         pbuffer = new unsigned char[ll];
       if (! infile->blockread(pbuffer,ll)) 
         return 0;
       //  If this is a jpeg file, we need to use the other version to reload the file.
       if (options.test(8))
         {RCCHAR temppath[600],tempfilename[600];
          FILE *outfile;
            GetTempPath(600,temppath);
            //qDebug() << "temppath : " << QString(temppath);
            GetTempFileName(temppath,_RCT("JPG"),0,tempfilename);
            //qDebug() << "tempfilename  : " << QString(tempfilename);
            if ((outfile = fopen(tempfilename,_RCT("wb"))) != 0)
            {
                 fwrite(pbuffer,1,ll,outfile);
                 fclose(outfile);
            }
            bitmap = new BitMapE(*header,tempfilename,origin,xaxis,yaxis,width,height,red,green,blue,transparencyrange,opacity,options);
            DeleteFile(tempfilename);

           delete [] bitmap->filename;
           if ((bitmap->filename = new RCCHAR[strlen(filename)+1]) != NULL)
           strcpy(bitmap->filename,filename);

         }  
       else
         {
           bitmap = new BitMapE(*header,filename,bmi,pbuffer,origin,xaxis,yaxis,width,height,red,green,blue,transparencyrange,opacity,options,infile->getversion());
         }
       bitmap->atlist = header->getatlist();
       delete header;
    }
  else
    bitmap = new BitMapE(*header,filename,origin,xaxis,yaxis,width,height,red,green,blue,transparencyrange,opacity,options);

  return bitmap;
}

int BitMapE::save(UndoFile *outfile)
{short type,sl;
 long ll;
  type = bitmap_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! outfile->write(filename)) return 0;
#ifdef USING_WIDGETS
  // kmj fix for image bug
  if (! outfile->write(&options)) return 0;
    sl = (short)sizeofbmi();
    if (! outfile->write(&sl)) return 0;
    if (! outfile->blockwrite(bmi,sl)) return 0;
    if (options.test(8))
      {  ll = originallength;
         if (! outfile->write(&ll)) return 0;
         if (! outfile->blockwrite(originalbuffer,ll)) return 0;
      }
    // save the pbuffer as well
      {  ll = sizeofbuffer();
         if (! outfile->write(&ll)) return 0;
         if (! outfile->blockwrite(pbuffer,ll)) return 0;
      }
    if (! atlist.save(outfile)) return 0;
#else
  sl = (short) sizeofbmi();
  if (! outfile->write(&sl)) return 0;
  if (! outfile->blockwrite(bmi,sl)) return 0;
  ll = sizeofbuffer();
  if (! outfile->write(&ll)) return 0;
  if (! outfile->blockwrite(pbuffer,ll)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! outfile->write(&options)) return 0;
#endif
  return 1;
}

Entity *BitMapE::load(UndoFile *infile)
{BitMapE *bitmap;
 short sl;
 long ll;
 char bm[sizeof(BitMask)];
  if ((bitmap = new BitMapE) != 0)
  {
      AttributeList al;
       memcpy(bm,&bitmap->visible,sizeof(bm));
       if (! infile->blockread(bitmap,sizeof(*bitmap))) return 0;
       memcpy(&bitmap->visible,bm,sizeof(bm));
       if (! infile->read(&bitmap->visible)) return 0;
       if (! infile->read(&bitmap->filename)) return 0;
#ifdef USING_WIDGETS
       if (! infile->read(&bitmap->options)) return 0;
       if (! infile->read(&sl)) return 0;
       bitmap->bmi = (BITMAPINFO *) new char[sl];
       if (! infile->blockread(bitmap->bmi,sl)) return 0;
       if (bitmap->options.test(8))
         {
           if (! infile->read(&ll)) return 0;
           bitmap->originallength = ll;
            bitmap->originalbuffer = new unsigned char[ll];
            if (! infile->blockread(bitmap->originalbuffer,ll)) return 0;
         }
       // read the pbuffer as well
         {  //ll = sizeofbuffer();
           if (! infile->read(&ll)) return 0;
           bitmap->pbuffer = new unsigned char[ll];
           if (! infile->blockread(bitmap->pbuffer,ll)) return 0;
         }
       if (! al.load(infile)) return 0;   bitmap->atlist = al;
#else
       if (! infile->read(&sl)) return 0;
       bitmap->bmi = (BITMAPINFO *) new char[sl];
       if (! infile->blockread(bitmap->bmi,sl)) return 0;
       if (! infile->read(&ll)) return 0;
       bitmap->pbuffer = new unsigned char[ll];
       if (! infile->blockread(bitmap->pbuffer,ll)) return 0;       
       if (! al.load(infile)) return 0;   bitmap->atlist = al;
       if (! infile->read(&bitmap->options)) return 0;
#endif
#ifdef USING_WIDGETS
       // create a new QPixmap using the pbuffer
       bitmap->qbmi = new QImage(bitmap->pbuffer,bitmap->bmi->bmiHeader.biWidth,bitmap->bmi->bmiHeader.biHeight,
                                 (QImage::Format)bitmap->bmi->bmiHeader.biCompression);
#endif
  }
  return bitmap;
}

int BitMapE::saveattributes(CadFile *outfile)
{ if (! outfile->write(&width)) return 0;
  if (! outfile->write(&height)) return 0;
  if (! outfile->write(&red)) return 0;
  if (! outfile->write(&green)) return 0;
  if (! outfile->write(&blue)) return 0;
  if (! outfile->write(&transparencyrange)) return 0;
  if (! outfile->write(&opacity)) return 0;
  if (! options.save(outfile)) return 0;
  return 1;
}

int BitMapE::loadattributes(CadFile *infile)
{ if (! infile->read(&width)) return 0;
  if (! infile->read(&height)) return 0;
  if (! infile->read(&red)) return 0;
  if (! infile->read(&green)) return 0;
  if (! infile->read(&blue)) return 0;
  if (! infile->read(&transparencyrange)) return 0;
  if (! infile->read(&opacity)) return 0;
  if (! infile->read(&options)) return 0;
  return 1;
}

int BitMapE::savedxf(int,DXFOut *)
{ return 1;
}

int BitMapE::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}

int BitMapE::savegeometry(CadFile *outfile)
{ if (! origin.save(outfile)) return 0;
  if (! xaxis.save(outfile)) return 0;
  if (! yaxis.save(outfile)) return 0;
  if (! outfile->write(&width)) return 0;
  if (! outfile->write(&height)) return 0;
  return 1;
}

int BitMapE::loadgeometry(CadFile *infile)
{ if (! origin.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  if (! infile->read(&width)) return 0;
  if (! infile->read(&height)) return 0;
  return 1;
}

void BitMapE::verify(void)
{
#ifdef USING_WIDGETS
    // this function now just pops up the text options dialog
    state.sendevent(new NewCommandEvent(428,0));
#else
 Dialog dialog("BitMapVerify_Dialog");
 int c,l,s,h,v,st;
 double w;
 RCCHAR start[300],*horizontall[3],*verticall[3],*stylel[3];
 ResourceString rs0(NCBITMAP + 0),rs1(NCBITMAP + 1),rs2(NCBITMAP + 2);
 ResourceString rs3(NCBITMAP + 3),rs4(NCBITMAP + 4),rs5(NCBITMAP + 5);
 ResourceString rs12(NCBITMAP + 12),rs13(NCBITMAP + 13),rs14(NCBITMAP + 14);
 Point p2;
 Workplane *wp;
  horizontall[0] = rs0.gets();
  horizontall[1] = rs1.gets();
  horizontall[2] = rs2.gets();
  verticall[0] = rs3.gets();
  verticall[1] = rs4.gets();
  verticall[2] = rs5.gets();
  stylel[0] = rs12.gets();
  stylel[1] = rs13.gets();
  stylel[2] = rs14.gets();
  if ((wp = db.workplanes.getcurrent()) != 0)
    p2 = wp->modeltoworkplane(origin);
  else
    p2 = origin;
  h = options.test(2) * 2 + options.test(1);
  v = options.test(4) * 2 + options.test(3);
  st = options.test(7) * 4 + options.test(6) * 2 + options.test(5);
  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  dialog.add(new CheckBoxDialogControl(104,&options,0));
  dialog.add(new CheckBoxDialogControl(119,&options,BitmapELinkFile));
  dialog.add(new StringDialogControl(105,filename,strlen(filename)+1));
  dialog.add(new StringDialogControl(106,p2.string(start),128));
  dialog.add(new RealDialogControl(107,&width,ScaleByLength));
  dialog.add(new RealDialogControl(108,&height,ScaleByLength));
  dialog.add(new StringDialogControl(109,horizontall[h],strlen(horizontall[h])));
  dialog.add(new StringDialogControl(110,verticall[v],strlen(verticall[v])));
  dialog.add(new StringDialogControl(111,stylel[st],strlen(stylel[st])));
  dialog.add(new CheckBoxDialogControl(112,&options,BitmapETransparent));
  dialog.add(new RealDialogControl(113,&red,DontScale));
  dialog.add(new RealDialogControl(114,&green,DontScale));
  dialog.add(new RealDialogControl(115,&blue,DontScale));
  dialog.add(new RealDialogControl(116,&transparencyrange,DontScale));
  dialog.add(new RealDialogControl(117,&opacity,DontScale));
  dialog.add(new ButtonDialogControl(118));
#if (VERSION != FLEXICAD)
  if (dialog.process() == 118)
    ::state.sendevent(new NewCommandEvent(428,0));
#endif
#endif
}

void BitMapE::extents(View3dSurface *vs,Point *pmin,Point *pmax)
{
#ifdef USING_WIDGETS
  Point p1,p2,p3,p4,pmid,xa,ya;
  int hor,ver;
  double frame=0.0;

  hor = options.test(2) * 2 + options.test(1);
  ver = options.test(4) * 2 + options.test(3);

  int styl = options.test(7) * 4 + options.test(6) * 2 + options.test(5);

  if(styl == 2)
  {
      frame = width / 20.0 * 0.8;
      if(height > width)
          frame = height / 20.0 * 0.8;
  }

  p1 = origin - Point(frame,frame,0.0);
  //if (SetLight != 0)
#if 1
  // KMJ: always 3d
  xa = xaxis * (width + frame * 2.0);
  ya = yaxis * (height + frame * 2.0);
#else
  if (SetLight != 0 && options.test(9))
  {
       xa = xaxis * (width + frame * 2.0);
       ya = yaxis * (height + frame * 2.0);
  }
  else
  {
      if (vs == NULL)
      {
          *pmin = *pmax = origin;
          //*pmin = *pmax = pmid;
          xa = xaxis * (width + frame * 2.0);
          ya = yaxis * (height + frame * 2.0);
      }
      else
      {
          xa = vs->getuaxis() * (width + frame * 2.0);
          ya = vs->getvaxis() * (height + frame * 2.0);
      }
  }
#endif
  p1 = p1 - xa * (hor/2.0) + ya * ((2.0 - ver)/2.0);
  p2 = p1 + xa;
  p3 = p2 - ya;
  p4 = p3 - xa;

  pmid = p4 + (p2 - p4) / 2.0;

  if (vs == NULL)
  {
      // the figure entity calls this function with vs as zero
      // so only one point is used for a bitmap handle
      // this behaviour could be changed in the future by uncommenting this code
      // to get the size of the bit map
      // it looks like this works as follows
      //*pmin = origin - Point(1.0,1.0,0.0);
      //*pmax = origin + Point(1.0,1.0,0.0);

      // this does not work when used by landworks
      //*pmin = *pmax = origin;

      // this might also work ok it is used in the figure extents code
      // for bitmaps
      pmin->setxyz(-1.0,-1.0,0.0);
      pmax->setxyz( 1.0, 1.0,0.0);

      // need to test this for side effects before using it
      /*
       *pmin = *pmax = pmid;
      p3 = p4;
      if (p3.x < pmin->x) pmin->x = p3.x;
      if (p3.y < pmin->y) pmin->y = p3.y;
      if (p3.z < pmin->z) pmin->z = p3.z;
      if (p3.x > pmax->x) pmax->x = p3.x;
      if (p3.y > pmax->y) pmax->y = p3.y;
      if (p3.z > pmax->z) pmax->z = p3.z;
      p3 = p2;
      if (p3.x < pmin->x) pmin->x = p3.x;
      if (p3.y < pmin->y) pmin->y = p3.y;
      if (p3.z < pmin->z) pmin->z = p3.z;
      if (p3.x > pmax->x) pmax->x = p3.x;
      if (p3.y > pmax->y) pmax->y = p3.y;
      if (p3.z > pmax->z) pmax->z = p3.z;
      */
  }
  else
  {
      //*pmin = *pmax = vs->modeltouv(origin);
      *pmin = *pmax = vs->modeltouv(pmid);
       p3 = vs->modeltouv(p4);
       if (p3.x < pmin->x) pmin->x = p3.x;
       if (p3.y < pmin->y) pmin->y = p3.y;
       if (p3.z < pmin->z) pmin->z = p3.z;
       if (p3.x > pmax->x) pmax->x = p3.x;
       if (p3.y > pmax->y) pmax->y = p3.y;
       if (p3.z > pmax->z) pmax->z = p3.z;
       p3 = vs->modeltouv(p2);
       if (p3.x < pmin->x) pmin->x = p3.x;
       if (p3.y < pmin->y) pmin->y = p3.y;
       if (p3.z < pmin->z) pmin->z = p3.z;
       if (p3.x > pmax->x) pmax->x = p3.x;
       if (p3.y > pmax->y) pmax->y = p3.y;
       if (p3.z > pmax->z) pmax->z = p3.z;
  }
#else
    Point p3;
  if (vs == NULL)
    {  *pmin = *pmax = origin;
    }
  else
    {  *pmin = *pmax = vs->modeltouv(origin);
       p3 = vs->modeltouv(origin);
       if (p3.x < pmin->x) pmin->x = p3.x;
       if (p3.y < pmin->y) pmin->y = p3.y;
       if (p3.z < pmin->z) pmin->z = p3.z;
       if (p3.x > pmax->x) pmax->x = p3.x;
       if (p3.y > pmax->y) pmax->y = p3.y;
       if (p3.z > pmax->z) pmax->z = p3.z;
    }
#endif
}

int BitMapE::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(bitmap_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int BitMapE::explode(void)
{ return 0;
}

void BitMapE::change(const BitMask &change,const EntityHeader &eh,RCCHAR *fn,double w,double h,double tr,double tg,double tb,double trange,double op,const BitMask &o)
{ db.saveundo(UD_CHANGEATTRIBUTES,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (change.test(0)) colour = eh.getcolour();
  if (change.test(1)) layer = eh.getlayer();
  if (change.test(2)) style = eh.getstyle();
  if (change.test(3)) weight = eh.getweight();
  if (change.test(4)) options.set(0,o.test(0));
  if (change.test(5)) options.set(BitmapELinkFile,o.test(BitmapELinkFile));
  if (change.test(6)) width = w;
  if (change.test(7)) height = h;
  if (change.test(8))
    {  options.set(1,o.test(1));  options.set(2,o.test(2));
    }
  if (change.test(9))
    {  options.set(3,o.test(3));  options.set(4,o.test(4));
    }
  if (change.test(10))
    {  options.set(5,o.test(5));  options.set(6,o.test(6));  options.set(7,o.test(7));
    }
  if (change.test(11)) options.set(BitmapETransparent,o.test(BitmapETransparent));
  if (change.test(12)) red = tr;
  if (change.test(13)) green = tg;
  if (change.test(14)) blue = tb;
  if (change.test(16)) transparencyrange = trange;
  if (change.test(17)) opacity = op;
  if (change.test(15))
    {  delete [] filename;
       if ((filename = new RCCHAR[strlen(fn)+1]) != NULL)
         strcpy(filename,fn);
    }
  if (change.test(18))// set this when you need to reload the bitmap
    load(filename);

  if (options.test(0) && bmi != 0)
    width = height * double(bmi->bmiHeader.biWidth) / double(bmi->bmiHeader.biHeight);

  if (defined && options.test(9))
    db.setrequiresopengl(1);

  draw(DM_NORMAL);
}

/*
 * // defined in ncdialog.h
class BitMapEDisplayDialogControl : public DisplayDialogControl
{
   private:
     double iwidth,iheight;
   public:
     BitMapEDisplayDialogControl(int id,double iw,double ih) : DisplayDialogControl(id) {  iwidth = iw;  iheight = ih;  if (fabs(iwidth) < 1.0E-10) iwidth = 1.0;  if (fabs(iheight) < 1.0E-10) iheight = 1.0;  }
     void draw(Dialog *dialog,RCHDC hDC);
};
*/

void BitMapEDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
#ifdef USING_WIDGETS
#if MAJOR_VERSION == 8
    ResourceString rs0(NCVIEWA);
    View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
    View3dOutput output(hWnd,hDC,&top,1.0);
    RECT rect;
    RCHPEN hpen,oldhpen;
    double width,height;
    int original,hor,ver;
    EntityHeader header(0,0,0,0);
    Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
    BitMask options(32);
    RCCHAR filename[600];
    int c,l,s;
    double w;
    c=db.header.getcolour();
    l=(int)db.header.getlayer();
    s=(int)db.header.getstyle();
    w=db.lineweights.weighttomm(db.header.getweight());
    double defWidth,defHeight,defTr,defTg,defTb,defTrange,defOp;
    BitMask defOptions(32);
    int tristates[] = { 1,1,1 };
    double trange,tr,tg,tb,op;
    int transbm, backstyle ;
    long obmwidth,obmheight;

    if(hWnd == 0) return;

    QGraphicsView *ctl = 0;
    if(dialog->gethdlg())
      ctl = (QGraphicsView*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(118);
    QGraphicsScene *scene = 0;
    if(ctl)
        scene = ctl->scene();

    if(ctl && scene)
    {
        output.setScene(scene);
        scene->setSceneRect(QRectF());
        scene->clear();
        QRect qrect = ctl->rect();

        //scene->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
        //                                        ::v.getreal("wn::backgroundcolour.green")*255.0,
        //                                        ::v.getreal("wn::backgroundcolour.blue")*255.0)));
        QColor backColour(QColor(::v.getreal("wn::backgroundcolour.red")*255.0+0.5,
                                 ::v.getreal("wn::backgroundcolour.green")*255.0+.05,
                                 ::v.getreal("wn::backgroundcolour.blue")*255.0+0.5));
        scene->setBackgroundBrush(QBrush(backColour));

        output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                                 ::v.getreal("wn::backgroundcolour.green")*255.0,
                                 ::v.getreal("wn::backgroundcolour.blue")*255.0));

        output.sizeevent(qrect.width(),qrect.height());
        scene->update();

        BitMapE::getdefaults(&defWidth,&defHeight,&defTr,&defTg,&defTb,&defTrange,&defOp,&defOptions);

        options.clearall();
        if (
            dialog->currentvalue(100,&c ) &&
            dialog->currentvalue(101,&l ) &&
            dialog->currentvalue(102,&s ) &&
            dialog->currentvalue(103,&w ) &&
            dialog->currentvalue(104,&original) &&
            dialog->currentvalue(106,&width) &&
            dialog->currentvalue(107,&height) &&
            dialog->currentvalue(108,&hor) &&
            dialog->currentvalue(109,&ver) &&
            dialog->currentvalue(200,filename,600) &&
            dialog->currentvalue(117,&op) &&
            dialog->currentvalue(111,&transbm) &&
            dialog->currentvalue(1011,&backstyle) &&
            ((BitMapETransparencyRangeDialogControl*)dialog->getcontrol(1006))->currentvalue(&trange,&tr,&tg,&tb) &&
            dialog->currentvalue(126,&trange))
          {
            if(((QCheckBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(104))->isTristate())
                original == 1 ? options.set(0,defOptions.test(0)) : options.set(0,original);
            else
                options.set(0,original);

            if(((QCheckBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(111))->isTristate())
                transbm == 1 ? options.set(BitmapETransparent,defOptions.test(BitmapETransparent)) : options.set(BitmapETransparent,transbm);
            else
                options.set(BitmapETransparent,transbm);

            if(!((QDoubleSpinBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(106))->specialValueText().isEmpty() &&
               width == ((QDoubleSpinBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(106))->minimum() )
                width = defWidth;

            if(!((QDoubleSpinBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(107))->specialValueText().isEmpty() &&
               height == ((QDoubleSpinBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(107))->minimum() )
                height = defHeight;

            if(((QComboBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(1008))->isVisible())
                hor = defOptions.test(2) * 2 + defOptions.test(1);

            if(((QComboBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(1009))->isVisible())
                ver = defOptions.test(4) * 2 + defOptions.test(3);

            if(((QDoubleSpinBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(126))->specialValueText() == "*Varies*" &&
               trange == ((QDoubleSpinBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(126))->minimum() )
                trange = defTrange;

            if(transbm)
            {
                QColor foreColour(255,255,255);
                QSize bsize;
                double sfactor = ctl->height();
                QPixmap backpix;
                switch(backstyle)
                {
                case 0:
                default: // checker board
                    if(backColour == foreColour)
                        backColour = QColor(190,190,190);
                    bsize = QSize(100,100);
                    backpix = createTranspBackTexture(bsize,foreColour,backColour,backstyle);
                    sfactor = ctl->height() / 2.9;
                    scene->setBackgroundBrush(QBrush(backpix.scaledToHeight(sfactor)));
                    break;
                case 1: // single colour
                    bsize = QSize(ctl->width(),ctl->height());
                    backpix = createTranspBackTexture(bsize,foreColour,backColour,backstyle);
                    sfactor = ctl->height() / 2.9;
                    scene->setBackgroundBrush(backpix);
                break;
                case 2: // half and half
                    bsize = QSize(ctl->width(),ctl->height());
                    if(backColour == foreColour)
                        backColour = QColor(0,0,0);
                    backpix = createTranspBackTexture(bsize,foreColour,backColour,backstyle);
                    sfactor = ctl->height() / 2.9;
                    scene->setBackgroundBrush(backpix);
                break;
                };
            }

            Point p1,origin,xaxis,yaxis;
            if (fabs(width) < 1.0E-10) width = 1.0E-10;
            if (fabs(height) < 1.0E-10) height = 1.0E-10;
            //if (fabs(width) < 0.0) width = 0.0;
            //if (fabs(height) < 0.0) height = 0.0;

            // adjust the width for the original size if set
            BitMapE::getshape(filename,&obmwidth,&obmheight);
            double bmwidth = width;
            double bmheight = height;
            if(original)
            {
                bmwidth = double(obmwidth);
                bmheight = double(obmheight);
            }

            // get the preview viewport size
            origin.setxyz((output.getumin() + output.getumax())/2.0,(output.getvmin() + output.getvmax())/2.0,0.0);
            xaxis.setxyz(output.getumax() - output.getumin(),0.0,0.0);
            yaxis.setxyz(0.0,output.getvmax() - output.getvmin(),0.0);

            // fit the bitmap rectangle to the preview viewport
            // use that for the bitmap height and width
            if(bmheight >= bmwidth)
            {
                xaxis.x = bmwidth * yaxis.y / bmheight;
            }
            else
            {
                double w = bmwidth * xaxis.x / bmwidth;
                double h = bmheight * xaxis.x / bmwidth;
                if(h > yaxis.y)
                {
                    xaxis.x = w * yaxis.y / h;
                }
                else
                {
                    yaxis.y = h;
                    xaxis.x = w;
                }
            }
            xaxis.x *= 0.95;
            yaxis.y *= 0.95;

            // get the base point for the bitmap
            p1 = origin - xaxis * (1.0 / 2.0) - yaxis * (1.0 / 2.0);

            // get the h/v lignment point
            Point pc;
            pc = p1 + xaxis * (hor / 2.0) + yaxis * (ver / 2.0);
            PointE point(pc);

            EntityHeader lheader(c,l,s,db.lineweights.mmtoweight(w));
            BitMapE bitmap(lheader,filename,p1,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),xaxis.x,yaxis.y,tr,tg,tb,trange,op/100.0,options);

            bitmap.draw(DM_NORMAL,NULL,&output);
            point.draw(DM_NORMAL,NULL,&output);
          }
    }
#else
    ResourceString rs0(NCVIEWA);
    View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
    View3dOutput output(hWnd,hDC,&top,1.0);
    RECT rect;
    RCHPEN hpen,oldhpen;
    double width,height;
    int original,hor,ver;
    EntityHeader header(0,0,0,0);
    Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
    BitMask options(32);
    int c,l,s;
    double w;
    c=db.header.getcolour();
    l=(int)db.header.getlayer();
    s=(int)db.header.getstyle();
    w=db.lineweights.weighttomm(db.header.getweight());
    double defWidth,defHeight,defTr,defTg,defTb,defTrange,defOp;
    BitMask defOptions(32);
    int tristates[] = { 1,1,1 };

    if(hWnd == 0) return;

    QGraphicsView *ctl = 0;
    if(dialog->gethdlg())
      ctl = (QGraphicsView*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(118);
    QGraphicsScene *scene = 0;
    if(ctl)
        scene = ctl->scene();

    if(ctl && scene)
    {
        output.setScene(scene);
        scene->setSceneRect(QRectF());
        scene->clear();
        QRect qrect = ctl->rect();

        scene->setBackgroundBrush(QBrush(QColor(::v.getreal("wn::backgroundcolour.red")*255.0,
                                                ::v.getreal("wn::backgroundcolour.green")*255.0,
                                                ::v.getreal("wn::backgroundcolour.blue")*255.0)));

        output.setbackground(RGB(::v.getreal("wn::backgroundcolour.red")*255.0,
                                 ::v.getreal("wn::backgroundcolour.green")*255.0,
                                 ::v.getreal("wn::backgroundcolour.blue")*255.0));

        output.sizeevent(qrect.width(),qrect.height());
        scene->update();

        //output.moveto(0,0);
        //output.lineto(rect.right-1,0);
        //output.lineto(rect.right-1,rect.bottom-1);
        //output.lineto(0,rect.bottom-1);
        //output.lineto(0,0);
        //SelectObject(output.gethdc(),oldhpen);
        //DeleteObject(hpen);

        BitMapE::getdefaults(&defWidth,&defHeight,&defTr,&defTg,&defTb,&defTrange,&defOp,&defOptions);

        options.clearall();
        if (
            dialog->currentvalue(100,&c ) &&
            dialog->currentvalue(101,&l ) &&
            dialog->currentvalue(102,&s ) &&
            dialog->currentvalue(103,&w ) &&
            dialog->currentvalue(104,&original) &&
            dialog->currentvalue(106,&width) &&
            dialog->currentvalue(107,&height) &&
            dialog->currentvalue(108,&hor) &&
            dialog->currentvalue(109,&ver))
          {
            if(((QCheckBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(104))->isTristate())
                tristates[0] == 1 ? options.set(0,defOptions.test(0)) : options.set(0,tristates[0]);
            else
                options.set(0,tristates[0]);

            if(!((QDoubleSpinBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(106))->specialValueText().isEmpty() &&
               width == ((QDoubleSpinBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(106))->minimum() )
                width = defWidth;

            if(!((QDoubleSpinBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(107))->specialValueText().isEmpty() &&
               height == ((QDoubleSpinBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(107))->minimum() )
                height = defHeight;

            if(((QComboBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(1008))->isVisible())
                hor = defOptions.test(2) * 2 + defOptions.test(1);

            if(((QComboBox*)((Bitmap_Dialog*)dialog->gethdlg())->getControl(1009))->isVisible())
                ver = defOptions.test(4) * 2 + defOptions.test(3);

             Point p1,origin,xaxis,yaxis;
             //BitMask options(32);
             //if (fabs(width) < 1.0E-10) width = 1.0E-10;
             //if (fabs(height) < 1.0E-10) height = 1.0E-10;
             if (fabs(width) < 0.0) width = 0.0;
             if (fabs(height) < 0.0) height = 0.0;
             origin.setxyz((output.getumin() + output.getumax())/2.0,(output.getvmin() + output.getvmax())/2.0,0.0);
             xaxis.setxyz(output.getumax() - output.getumin(),0.0,0.0);
             yaxis.setxyz(0.0,output.getvmax() - output.getvmin(),0.0);
             PointE point(origin);
             point.draw(DM_NORMAL,0,&output);
             yaxis /= 3.0;
             yaxis *= height / iheight;
             xaxis = xaxis.normalize();
             if (original)
               xaxis *= 1.3 * yaxis.length();
             else
               xaxis *= width / height * yaxis.length();
             p1 = origin - xaxis * (hor / 2.0) - yaxis * (ver / 2.0);

             //  just drawing a box with lines
             // KMJ: ToDo: render the actual bitmap
             EntityHeader lheader(c,l,s,db.lineweights.mmtoweight(w));
             // don't use a line pattern when drawing the lines
             lheader.setpatternlinestyle(0);

             Line line1(lheader,p1,p1+xaxis);  line1.draw(DM_NORMAL,0,&output);
             Line line2(lheader,p1+xaxis,p1+xaxis+yaxis);  line2.draw(DM_NORMAL,0,&output);
             Line line3(lheader,p1+xaxis+yaxis,p1+yaxis);  line3.draw(DM_NORMAL,0,&output);
             Line line4(lheader,p1+yaxis,p1);  line4.draw(DM_NORMAL,0,&output);
             Line line5(lheader,p1,p1+xaxis+yaxis);  line5.draw(DM_NORMAL,0,&output);
             Line line6(lheader,p1+yaxis,p1+xaxis);  line6.draw(DM_NORMAL,0,&output);
          }
    }
#endif
#else
 ResourceString rs0(NCVIEWA);
 View top(rs0.gets(),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hDC,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 double width,height;
 int original,hor,ver;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 BitMask options(32);

 if(hWnd == 0) return;

   if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
    {  oldhpen = (RCHPEN)SelectObject(output.gethdc(),hpen);
       rect.left = 0;  rect.top = 0;
       rect.right = int(output.getwidth());  rect.bottom = int(output.getheight());
       FillRect(output.gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
#if ! defined(_WIN32_WCE)
       MoveToEx(output.gethdc(),0,0,0);
       LineTo(output.gethdc(),rect.right-1,0);
       LineTo(output.gethdc(),rect.right-1,rect.bottom-1);
       LineTo(output.gethdc(),0,rect.bottom-1);
       LineTo(output.gethdc(),0,0);
//WCEFIX
#endif
       SelectObject(output.gethdc(),oldhpen);
       DeleteObject(hpen);
       if (dialog->currentvalue(100,&original) &&
           dialog->currentvalue(102,&width) &&
           dialog->currentvalue(103,&height) &&
           dialog->currentvalue(104,&hor) &&
           dialog->currentvalue(105,&ver))
         {Point p1,origin,xaxis,yaxis;
          BitMask options(32);
            if (fabs(width) < 1.0E-10) width = 1.0E-10;
            if (fabs(height) < 1.0E-10) height = 1.0E-10;
            origin.setxyz((output.getumin() + output.getumax())/2.0,(output.getvmin() + output.getvmax())/2.0,0.0);
            xaxis.setxyz(output.getumax() - output.getumin(),0.0,0.0);
            yaxis.setxyz(0.0,output.getvmax() - output.getvmin(),0.0);
            PointE point(origin);
            point.draw(DM_NORMAL,0,&output);
            yaxis /= 3.0;
            yaxis *= height / iheight;
            xaxis = xaxis.normalize();
            if (original)
              xaxis *= 1.3 * yaxis.length();
            else
              xaxis *= width / height * yaxis.length();
            p1 = origin - xaxis * (hor / 2.0) - yaxis * (ver / 2.0);
            Line line1(p1,p1+xaxis);  line1.draw(DM_NORMAL,0,&output);
            Line line2(p1+xaxis,p1+xaxis+yaxis);  line2.draw(DM_NORMAL,0,&output);
            Line line3(p1+xaxis+yaxis,p1+yaxis);  line3.draw(DM_NORMAL,0,&output);
            Line line4(p1+yaxis,p1);  line4.draw(DM_NORMAL,0,&output);
            Line line5(p1,p1+xaxis+yaxis);  line5.draw(DM_NORMAL,0,&output);
            Line line6(p1+yaxis,p1+xaxis);  line6.draw(DM_NORMAL,0,&output);
         }
    }
#endif
}

class BitMapEButtonDialogControl : public ButtonDialogControl
{
protected:
    int baseid;
public:
     BitMapEButtonDialogControl(int id,int bid) : ButtonDialogControl(id) { baseid = bid;  }
     int select(Dialog *);
};

int BitMapEButtonDialogControl::select(Dialog *parentdialog)
{
#ifdef USING_WIDGETS
    // puts up the a color selector dialog
    Dialog dialog("DefineColour_Dialog");
    DialogControl *dc;
    int i,index;
    double red,green,blue;
    int lastIndex;
    double lastRed,lastGreen,lastBlue;
    double hue,lightness,saturation;

    //index = db.header.getcolour();
    index = -1;
    ((RealDialogControl*)parentdialog->getcontrol(baseid))->currentvalue(parentdialog,&red);
    ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->currentvalue(parentdialog,&green);
    ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->currentvalue(parentdialog,&blue);

    Colour c;
    c.set(red,green,blue);
    c.gethls(&hue,&lightness,&saturation);

    dialog.title(app->tr("Define Transparent Colour").data());
    dialog.add(dc = new IntegerDialogControl(100,&index,0,255));
    dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(104,dc));
    dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(105,dc));
    dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(106,dc));

    dialog.add(dc = new RealDialogControl(107,&hue,1.0,0.0,360.0));
    dialog.add(new ColourScrollBarDialogControl(110,dc));
    dialog.add(dc = new RealDialogControl(108,&lightness,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(111,dc));
    dialog.add(dc = new RealDialogControl(109,&saturation,0.1,0.0,1.0));
    dialog.add(new ColourScrollBarDialogControl(112,dc));

    OtherColourDisplayDialogControl *pddc;
    OtherColourSampleDialogControl *sddc;
    dialog.add((DisplayDialogControl *)(pddc = new OtherColourDisplayDialogControl(120,&dialog,c)));
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(113,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(115,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);
    dialog.add((DisplayDialogControl *)(sddc = new OtherColourSampleDialogControl(116,pddc,&dialog,c)));
    ((OtherColourDisplayDialogControl *)pddc)->add(sddc);

    dialog.add(new ColourApplyButtonDialogControl(114,3));
    int n = db.getnumericdisplaytolerance();
    db.setnumericdisplaytolerance(3);

    lastIndex = db.header.getcolour();
    lastRed   = red;
    lastGreen = green;
    lastBlue  = blue;

    if(dialog.process() == TRUE)
    {
        ((RealDialogControl*)parentdialog->getcontrol(baseid))->change(parentdialog,red);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->change(parentdialog,green);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->change(parentdialog,blue);
    }
    else
    {
        // reset the state
        ((RealDialogControl*)parentdialog->getcontrol(baseid))->change(parentdialog,lastRed) ;
        ((RealDialogControl*)parentdialog->getcontrol(baseid+1))->change(parentdialog,lastGreen);
        ((RealDialogControl*)parentdialog->getcontrol(baseid+2))->change(parentdialog,lastBlue);
    }

    db.setnumericdisplaytolerance(n);

    return 0;

#else
 Dialog dialog("Selectcolour_dialog");
 DialogControl *dc;
 double red,green,blue;
  if (parentdialog->currentvalue(108,&red) &&
      parentdialog->currentvalue(109,&green) &&
      parentdialog->currentvalue(110,&blue))
    {  dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
       dialog.add(new ScrollBarDialogControl(1101,dc));
       dialog.add(new RTColourScrollBarDialogControl(104,dc));
       dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
       dialog.add(new ScrollBarDialogControl(1102,dc));
       dialog.add(new RTColourScrollBarDialogControl(105,dc));
       dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
       dialog.add(new ScrollBarDialogControl(1103,dc));
       dialog.add(new RTColourScrollBarDialogControl(106,dc));
       dialog.add((DisplayDialogControl *) new RTColourDisplayDialogControl(107,parentdialog));
       if (dialog.process(parentdialog->gethdlg()) == TRUE)
         {  ((RealDialogControl *) parentdialog->getcontrol(108))->change(parentdialog,red);
            ((RealDialogControl *) parentdialog->getcontrol(109))->change(parentdialog,green);
            ((RealDialogControl *) parentdialog->getcontrol(110))->change(parentdialog,blue);
         }
    }
  return 0;
#endif
}

class BitMapEVerifyButtonControl : public ButtonDialogControl
  {public:
     BitMapEVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int BitMapEVerifyButtonControl::select(Dialog *dialog)
{
    int state,colour,halocolour,layer,style,iweight;
    double weight;

    state = 1001;
    if (id == 1000)
    { // colour
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(100))->currentvalue(dialog,&colour);
        general_property_command(&state,0,(void **)&colour);
        ((IntegerDialogControl *)dialog->getcontrol(100))->change(dialog,colour);
    }
    else if(id == 1001)
    { // layer
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("0"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(101))->currentvalue(dialog,&layer);
        general_property_command(&state,0,(void **)&layer);
        ((IntegerDialogControl *)dialog->getcontrol(101))->change(dialog,layer);
    }
    else if(id == 1002)
    { // style
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("3"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(102))->currentvalue(dialog,&style);
        general_property_command(&state,0,(void **)&style);
        ((IntegerDialogControl *)dialog->getcontrol(102))->change(dialog,style);
    }
    else if(id == 1003)
    { // weight
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->currentvalue(dialog,&weight);
        general_property_command(&state,0,(void **)&weight);
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->change(dialog,weight);
    }
    return 0;
}

int BitMapE::setup(BitMask *change,double *w,double *h,double *tr,double *tg,double *tb,double *trange,double *op,BitMask *o,RCCHAR *fn,Point *origin,EntityHeader *header)
{
 int i,status,hor,ver,style;
 Dialog dialog("Bitmap_Dialog");
 DialogControl *dc;
 RCCHAR *horizontall[3],*verticall[3],*stylel[3],start[300];
 RCCHAR *horizontallv[4],*verticallv[4],*stylelv[4];
 ResourceString rs0(NCBITMAP + 0),rs1(NCBITMAP + 1),rs2(NCBITMAP + 2);
 ResourceString rs3(NCBITMAP + 3),rs4(NCBITMAP + 4),rs5(NCBITMAP + 5);
 ResourceString rs6(NCBITMAP + 6),rs12(NCBITMAP + 12),rs13(NCBITMAP + 13),rs14(NCBITMAP + 14);
 ResourceString rsVaries(4107);
 Point p2;
 Workplane *wp;
 double opacityPcnt = *op * 100.0; // transparency as percenatage spinbox

 //
 int c,l,s;
 double wt,defWeight,defWidth,defHeight,deftRange,deftRed,deftGreen,deftBlue,defOpacity;
 int defColour,defLayer,defStyle,dummyhor,dummyver,dummystyle;
 BitMask defOptions(32),changeIn(32);
 RCCHAR defFname[300],nullString[1];
 int tristates[] = { 1,1,1 };
 int backstyle = v.getinteger("bi::backstyle"); // default preview background style

  int changecount = v.getinteger("op::changecount");

  EntityHeader entheader;
  if(header !=0)
     header->verify(&dialog,&c,&l,&s,&wt);
  else
     entheader.verify(&dialog,&c,&l,&s,&wt);


  // get the default values
  strcpy(nullString,"");
  if(changecount == 1)
      strcpy(defFname,fn);
  else
      strcpy(defFname,nullString);

  defColour = db.header.getcolour();
  defLayer  = db.header.getlayer();
  defStyle  = db.header.getstyle();
  defWeight = db.lineweights.weighttomm(db.header.getweight());
  BitMapE::getdefaults(&defWidth,&defHeight,&deftRed,&deftGreen,&deftBlue,&deftRange,&defOpacity,&defOptions);
    Colour deftc(deftRed,deftGreen,deftBlue);

  horizontall[0] = rs0.gets();
  horizontall[1] = rs1.gets();
  horizontall[2] = rs2.gets();

  horizontallv[0] = rsVaries.gets();
  horizontallv[1] = rs0.gets();
  horizontallv[2] = rs1.gets();
  horizontallv[3] = rs2.gets();

  verticall[0] = rs3.gets();
  verticall[1] = rs4.gets();
  verticall[2] = rs5.gets();

  verticallv[0] = rsVaries.gets();
  verticallv[1] = rs3.gets();
  verticallv[2] = rs4.gets();
  verticallv[3] = rs5.gets();

  stylel[0] = rs12.gets();
  stylel[1] = rs13.gets();
  stylel[2] = rs14.gets();

  stylelv[0] = rsVaries.gets();
  stylelv[1] = rs12.gets();
  stylelv[2] = rs13.gets();
  stylelv[3] = rs14.gets();


  // look at the change bits and see what value vary among any pre-slected entities
  if(change != NULL)
  {
      // if test is true the values dont vary ie. the dialog will show the incoming values
      // otherwise false ie. the values vary so the dialog will show the *Varies* message
      // the entity values will not be modified if the dialog values are not changed
      //
      // here we are setting the controls to there default settings
      // text boxes and combo boxes will show *Varies*
      // check boxes will be in tri state mode

      if(!change->test(0))
          c = 0;
      if(!change->test(1))
          l = 0;
      if(!change->test(2))
          s = 0;
      if(!change->test(3))
          wt = 0.10;

      if(change->test(4)) // original shape
          o->test(0)==0  ? tristates[0] = 0 : tristates[0] = 2;
      if(change->test(5)) // link to file
          o->test(BitmapELinkFile)==0 ? tristates[1] = 0 : tristates[1] = 2;
      if(!change->test(6)) // width
          *w = 0.0;
      if(!change->test(7)) // height
          *h = 0.0;
      if(!change->test(8)) // horiz just
      {
          hor = 0;
          dummyhor = 0;
      }
      else
      {
          hor = o->test(2) * 2 + o->test(1);
          dummyhor = 0;
      }
      if(!change->test(9)) // vert just
      {
          ver = 0;
          dummyver = 0;
      }
      else
      {
          ver = o->test(4) * 2 + o->test(3);
          dummyver = 0;
      }
      if(!change->test(10)) // frame style
      {
          style = 0;
          dummystyle = 0;
      }
      else
      {
          style = o->test(7) * 4 + o->test(6) * 2 + o->test(5);
          dummystyle = 0;
      }
      if(change->test(11)) // transparent flag
          o->test(BitmapETransparent)==0 ? tristates[2] = 0 : tristates[2] = 2;
      if(!change->test(12)) // tred
          *tr = 0.0;
      if(!change->test(13)) // tgreen
          *tg = 0.0;
      if(!change->test(14)) // tblue
          *tb = 0.0;
      if(!change->test(15)) // trange
          *trange = 0.0;
      if(!change->test(16)) // topacity
          opacityPcnt = 100.0;

  }
  else
  {

      hor = o->test(2) * 2 + o->test(1);
      ver = o->test(4) * 2 + o->test(3);
      style = o->test(7) * 4 + o->test(6) * 2 + o->test(5);
      o->test(0)==0  ? tristates[0] = 0 : tristates[0] = 1;
      o->test(BitmapELinkFile)==0 ? tristates[1] = 0 : tristates[1] = 1;
      o->test(BitmapETransparent)==0 ? tristates[2] = 0 : tristates[2] = 1;
  }

  //dialog.add(dc = new CheckBoxDialogControl(104,o,0));
  //dialog.add(dc = new CheckBoxDialogControl(105,o,BitmapELinkFile));

  // origin point
  if (origin != 0 && (wp = db.workplanes.getcurrent()) != 0)
    p2 = wp->modeltoworkplane(*origin);
  else if(origin != 0)
    p2 = *origin;


  dialog.add(dc = new RealDialogControl(106,w,10.0,0.0,1.0E+10,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(107,h,10.0,0.0,1.0E+10,ScaleByLength));
  //dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(new ListDialogControl(108,3,horizontall,&hor));
  dialog.add(new ListDialogControl(109,3,verticall,&ver));
  dialog.add(new ListDialogControl(110,3,stylel,&style));
  //dialog.add(dc = new CheckBoxDialogControl(111,o,BitmapETransparent));

  // transparency colour
  //dialog.add(dc = new RealDialogControl(112,tr,0.01,0.0,1.0,DontScale));
  //dialog.add(new ScrollBarDialogControl(1108,dc));
  //dialog.add(dc = new RealDialogControl(113,tg,0.01,0.0,1.0,DontScale));
  //dialog.add(new ScrollBarDialogControl(1109,dc));
  //dialog.add(dc = new RealDialogControl(114,tb,0.01,0.0,1.0,DontScale));
  dialog.add(dc = new RealDialogControl(126,trange,0.01,0.0,1.0,DontScale));
  dialog.add(new BitMapEPlaneButtonDialogControl(1007,205,tr,tg,tb));
  Colour tc(*tr,*tg,*tb);
  dialog.add(new ColourSampleControl(205,&dialog,&tc));
  dialog.add(new BitMapETransparencyRangeDialogControl(1006,126,205,200,trange,tr,tg,tb,106,107));


  //dialog.add(new ScrollBarDialogControl(1110,dc));
  //dialog.add(new BitMapEButtonDialogControl(1005,112));
  //dialog.add(dc = new RealDialogControl(116,trange,0.01,0.0,1.0,DontScale));
  //dialog.add(dc = new RealDialogControl(126,trange,0.01,0.0,1.0,DontScale));
  //dialog.add(new ScrollBarDialogControl(1112,dc));

  // opacity/transparency
  dialog.add(dc = new RealDialogControl(117,&opacityPcnt,1.0,0.0,100.0,DontScale));
  //dialog.add(new ScrollBarDialogControl(1113,dc));

  dialog.add((DisplayDialogControl *) new BitMapEDisplayDialogControl(118,*w,*h));
  dialog.add(new ButtonDialogControl(119));
  dialog.add(new ButtonDialogControl(120));

  // preview background style
  dialog.add(new RadioButtonDialogControl(1011,1011,1013,&backstyle));
  dialog.add(new RadioButtonDialogControl(1012,1011,1012,&backstyle));
  dialog.add(new RadioButtonDialogControl(1013,1011,1012,&backstyle));


  if (change != NULL)
  {
       if(changecount == 1)
           dialog.title(_RCT("Bitmap Verify"));
       else
           dialog.title(rs6.gets());

       v.setinteger("op::change",1);
       v.setbm("op::changemask",*change);
       //change->clearall();
       // add the verify features
       dialog.add(new BitMapEVerifyButtonControl(1000));
       dialog.add(new BitMapEVerifyButtonControl(1001));
       dialog.add(new BitMapEVerifyButtonControl(1002));
       dialog.add(new BitMapEVerifyButtonControl(1003));

       if(changecount > 1 && !change->test(0)) // colour
       {
           ((IntegerDialogControl*)dialog.getcontrol(100))->setSpecialValueText(_RCT("*Varies*"));
           ((IntegerDialogControl*)dialog.getcontrol(100))->setDefaultValue(defColour);
       }
       if(changecount > 1 && !change->test(1)) // layer
       {
           ((IntegerDialogControl*)dialog.getcontrol(101))->setSpecialValueText(_RCT("*Varies*"));
           ((IntegerDialogControl*)dialog.getcontrol(101))->setDefaultValue(defLayer);
       }
       if(changecount > 1 && !change->test(2)) // style
       {
           ((IntegerDialogControl*)dialog.getcontrol(102))->setSpecialValueText(_RCT("*Varies*"));
           ((IntegerDialogControl*)dialog.getcontrol(102))->setDefaultValue(defStyle);
       }
       if(changecount > 1 && !change->test(3)) // weight
       {
           ((RealDialogControl*)dialog.getcontrol(103))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(103))->setDefaultValue(defWeight);
       }
       // set up the tri state check boxes
       dialog.add(new TriStateCheckBoxDialogControl(104,&tristates[0],changecount>=1^change->test(4),defOptions.test(0))); // original shape
       dialog.add(new TriStateCheckBoxDialogControl(105,&tristates[1],changecount>=1^change->test(5),defOptions.test(BitmapELinkFile))); // link to file
       if(changecount > 1 && !change->test(6)) // width
       {
           ((RealDialogControl*)dialog.getcontrol(106))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(106))->setDefaultValue(defWidth);
       }
       if(changecount > 1 && !change->test(7)) // Height
       {
           ((RealDialogControl*)dialog.getcontrol(107))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(107))->setDefaultValue(defHeight);
       }
       if(changecount > 1 && !change->test(8)) // horiz just
           dialog.add(new ListDialogControl(1008,2,horizontallv,&dummyhor));
       if(changecount > 1 && !change->test(9)) // vert just
           dialog.add(new ListDialogControl(1009,2,verticallv,&dummyver));
       if(changecount > 1 && !change->test(10)) // frame style
           dialog.add(new ListDialogControl(1010,2,stylelv,&dummystyle));
       // set up the tri state check boxes
       dialog.add(new TriStateCheckBoxDialogControl(111,&tristates[2],changecount>=1^change->test(11),defOptions.test(BitmapETransparent))); // transparent flag

       if(changecount > 1 && (!change->test(12) || !change->test(13) || !change->test(14))) // tr, tg, tb
       {
           ((BitMapEPlaneButtonDialogControl*)dialog.getcontrol(1007))->setSpecialValueText(_RCT("*Varies*"));
           ((BitMapEPlaneButtonDialogControl*)dialog.getcontrol(1007))->setDefaultValue(deftRed,deftGreen,deftBlue);

           ((ColourSampleControl*)dialog.getcontrol(205))->setSpecialValueText(_RCT("*Varies*"));
           ((ColourSampleControl*)dialog.getcontrol(205))->setDefaultValue(deftc);
       }
       /*
       if(changecount > 1 && !change->test(12)) // tr
       {
           ((RealDialogControl*)dialog.getcontrol(112))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(112))->setDefaultValue(deftRed);
       }
       if(changecount > 1 && !change->test(13)) // tg
       {
           ((RealDialogControl*)dialog.getcontrol(113))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(113))->setDefaultValue(deftGreen);
       }
       if(changecount > 1 && !change->test(14)) // tb
       {
           ((RealDialogControl*)dialog.getcontrol(114))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(114))->setDefaultValue(deftBlue);
       }
       */
       if(changecount > 1 && !change->test(15)) // file name
       {
           if(fn != 0)
             dialog.add(new StringDialogControl(200,rsVaries.gets(),strlen(rsVaries.gets())+1));
           else
             dialog.add(new StringDialogControl(200,nullString,strlen(nullString)+1));
       }
       else if(changecount == 1 && change->test(15))
       {
           if(fn != 0)
             dialog.add(new StringDialogControl(200,fn,strlen(fn)+1));
           else
             dialog.add(new StringDialogControl(200,nullString,strlen(nullString)+1));
       }
       if(changecount > 1 && !change->test(15)) // trange
       {
           //((RealDialogControl*)dialog.getcontrol(116))->setSpecialValueText(_RCT("*Varies*"));
           //((RealDialogControl*)dialog.getcontrol(116))->setDefaultValue(deftRange);

           ((RealDialogControl*)dialog.getcontrol(126))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(126))->setDefaultValue(deftRange);
       }
       if(changecount > 1 && !change->test(16)) // opacity
       {
           ((RealDialogControl*)dialog.getcontrol(117))->setSpecialValueText(_RCT("*Varies*"));
           ((RealDialogControl*)dialog.getcontrol(117))->setDefaultValue(defOpacity);
       }
       // origin points are not compared yet so change is always false for now
       if(changecount > 1 && !change->test(19)) // origin point
       {
           if(origin != 0)
             dialog.add(new StringDialogControl(201,rsVaries.gets(),strlen(rsVaries.gets())+1));
           else
             dialog.add(new StringDialogControl(201,nullString,strlen(nullString)+1));
       }
       else if(changecount == 1 && !change->test(19)) // origin point
       {
           if(origin != 0)
             dialog.add(new StringDialogControl(201,p2.string(start),128));
           else
             dialog.add(new StringDialogControl(201,nullString,strlen(nullString)+1));
       }
  }
  else
  {
      v.setinteger("op::change",0);
      v.setbm("op::changemask",defOptions);
      // file path
      if(fn != 0)
        dialog.add(new StringDialogControl(200,fn,strlen(fn)+1));
      else
        dialog.add(new StringDialogControl(200,nullString,strlen(nullString)+1));
      // origin point
      if(origin != 0)
        dialog.add(new StringDialogControl(201,p2.string(start),128));
      else
        dialog.add(new StringDialogControl(201,nullString,strlen(nullString)+1));
      // set up the bi state check boxes
      dialog.add(new TriStateCheckBoxDialogControl(104,&tristates[0],false,defOptions.test(0))); // original shape
      dialog.add(new TriStateCheckBoxDialogControl(105,&tristates[1],false,defOptions.test(BitmapELinkFile))); // link to file
      dialog.add(new TriStateCheckBoxDialogControl(111,&tristates[2],false,defOptions.test(BitmapETransparent))); // transparent flag
  }

  status = dialog.process();
  if (status == TRUE)
  //if ((status = dialog.process()) == TRUE)
  {
      if(change != NULL)
      {
          // reset the outgoing change values
          changeIn = *change;
          change->clearall();

          if(changecount == 1)
          {
              for (int i = 0 ; i < 4 ; i++)
                change->set(i,1);

              header->setcolour(c);
              header->setstyle(s);
              header->setlayer(l);
              header->setweight(db.lineweights.mmtoweight(wt));

          }

          if( changecount > 1 && !changeIn.test(0) &&
              strlen(((IntegerDialogControl*)dialog.getcontrol(100))->getSpecialValueText()) != 0 )
              change->clear(0);
          else
          {
              change->set(0);
              header->setcolour(c);
          }
          if( changecount > 1 && !changeIn.test(1) &&
              strlen(((IntegerDialogControl*)dialog.getcontrol(101))->getSpecialValueText()) != 0 )
              change->clear(1);
          else
          {
              change->set(1);
              header->setlayer(l);
          }
          if( changecount > 1 && !changeIn.test(2) &&
              strlen(((IntegerDialogControl*)dialog.getcontrol(102))->getSpecialValueText()) != 0 )
              change->clear(2);
          else
          {
              change->set(2);
              header->setstyle(s);
          }
          if( changecount > 1 && !changeIn.test(3) &&
              strlen(((WeightAssocRealDialogControl*)dialog.getcontrol(103))->getSpecialValueText()) != 0 )
              change->clear(3);
          else
          {
              change->set(3);
              header->setweight(db.lineweights.mmtoweight(wt));
          }
          // orignal shape
          if(tristates[0] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(104))->isTriState())
              change->clear(4);
          else
          {
              change->set(4);
              o->set(0, tristates[0]);
          }
          // link to file
          if(tristates[2] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(105))->isTriState())
              change->clear(5);
          else
          {
              change->set(5);
              o->set(BitmapELinkFile, tristates[1]);
          }
          // width
          if( changecount > 1 && !changeIn.test(6) &&
              strlen(((RealDialogControl*)dialog.getcontrol(106))->getSpecialValueText()) != 0 )
              change->clear(6);
          else
              change->set(6);
          // height
          if( changecount > 1 && !changeIn.test(7) &&
              strlen(((RealDialogControl*)dialog.getcontrol(107))->getSpecialValueText()) != 0 )
              change->clear(7);
          else
              change->set(7);
          // horiz just
          if( changecount > 1 && !changeIn.test(8) && dummyhor == 0)
              change->clear(8);
          else
          {
              change->set(8);
              o->set(1,hor&1);
              o->set(2,(hor>>1)&1);
          }
          // verti just
          if( changecount > 1 && !changeIn.test(9) && dummyver == 0)
              change->clear(9);
          else
          {
              change->set(9);
              o->set(3,ver&1);
              o->set(4,(ver>>1)&1);
          }
          // frame style
          if( changecount > 1 && !changeIn.test(10) && dummystyle == 0)
              change->clear(10);
          else
          {
              change->set(10);
              o->set(5,style&1);
              o->set(6,(style>>1)&1);
              o->set(7,(style>>2)&1);
          }
          // tansparency flag
          if(tristates[2] == 1 && ((TriStateCheckBoxDialogControl*)dialog.getcontrol(111))->isTriState())
              change->clear(11);
          else
          {
              change->set(11);
              o->set(BitmapETransparent, tristates[2]);
          }
          // tr, tg, tb

          // tr
          if( changecount > 1 && !changeIn.test(12) &&
              strlen(((RealDialogControl*)dialog.getcontrol(205))->getSpecialValueText()) != 0 )
              change->clear(12);
          else
              change->set(12);
          /*
          if( changecount > 1 && !changeIn.test(12) &&
              strlen(((RealDialogControl*)dialog.getcontrol(112))->getSpecialValueText()) != 0 )
              change->clear(12);
          else
              change->set(12);
          */
          // tg
          if( changecount > 1 && !changeIn.test(13) &&
              strlen(((RealDialogControl*)dialog.getcontrol(205))->getSpecialValueText()) != 0 )
              change->clear(13);
          else
              change->set(13);
          /*
          if( changecount > 1 && !changeIn.test(13) &&
              strlen(((RealDialogControl*)dialog.getcontrol(113))->getSpecialValueText()) != 0 )
              change->clear(13);
          else
              change->set(13);
          */
          // tb
          if( changecount > 1 && !changeIn.test(14) &&
              strlen(((RealDialogControl*)dialog.getcontrol(205))->getSpecialValueText()) != 0 )
              change->clear(14);
          else
              change->set(14);
          /*
          if( changecount > 1 && !changeIn.test(14) &&
              strlen(((RealDialogControl*)dialog.getcontrol(114))->getSpecialValueText()) != 0 )
              change->clear(14);
          else
              change->set(14);
          */
          //
          // change->test(15) changing file name??
          //
          // trange
          //if( changecount > 1 && !changeIn.test(16) &&
          //    strlen(((RealDialogControl*)dialog.getcontrol(116))->getSpecialValueText()) != 0 )
          //    change->clear(16);
          //else
          //    change->set(16);
          if( changecount > 1 && !changeIn.test(16) &&
              strlen(((RealDialogControl*)dialog.getcontrol(126))->getSpecialValueText()) != 0 )
              change->clear(16);
          else
              change->set(16);
          // opacity
          if( changecount > 1 && !changeIn.test(17) &&
              strlen(((RealDialogControl*)dialog.getcontrol(117))->getSpecialValueText()) != 0 )
              change->clear(17);
          else
          {
              change->set(17);
              *op = opacityPcnt / 100.0;
          }

          return TRUE;
      }
      else
      {
           o->set(0, tristates[0]);
           o->set(BitmapELinkFile, tristates[1]);
           o->set(BitmapETransparent, tristates[2]);

           o->set(1,hor&1);
           o->set(2,(hor>>1)&1);
           o->set(3,ver&1);
           o->set(4,(ver>>1)&1);
           o->set(5,style&1);
           o->set(6,(style>>1)&1);
           o->set(7,(style>>2)&1);
           return TRUE;
      }
  }
  else
  {
      if(change != NULL)
         change->clearall();
        /*
      else if (status == 119)
        return 119;
      else if (status == 120)
        return 120;
      else
        return FALSE;
        */
  }
  return status;
}

void BitMapE::getdefaults(double *w,double *h,double *tr,double *tg,double *tb,double *trange,double *op,BitMask *o)
{ *o = *v.getbm("bi::options");
  *w = v.getreal("bi::width");
  *h = v.getreal("bi::height");
  *tr= v.getreal("bi::red");
  *tg = v.getreal("bi::green");
  *tb = v.getreal("bi::blue");
  *trange = v.getreal("bi::transparencyrange");
  *op = v.getreal("bi::opacity");
}

void BitMapE::setdefaults(double w,double h,double tr,double tg,double tb,double trange,double op,const BitMask &o)
{ v.setbm("bi::options",o);
  v.setreal("bi::width",w);
  v.setreal("bi::height",h);
  v.setreal("bi::red",tr);
  v.setreal("bi::green",tg);
  v.setreal("bi::blue",tb);
  v.setreal("bi::transparencyrange",trange);
  v.setreal("bi::opacity",op);
}

void BitMapE::setdefaults(void)
{ v.setbm("bi::options",options);
  v.setreal("bi::width",width);
  v.setreal("bi::height",height);
  v.setreal("bi::red",red);
  v.setreal("bi::green",green);
  v.setreal("bi::blue",blue);
  v.setreal("bi::transparencyrange",transparencyrange);
  v.setreal("bi::opacity",opacity);
}

int BitMapE::select(RCCHAR *name)
{
 ResourceString rs7(NCBITMAP+7),rs8(NCBITMAP+8);
 RCCHAR dirname[300],file[300],title[256],filter[256],ext[4];
 RCCHAR *e;
 int index;
 RCOPENFILENAME ofn;
 RCUINT  i,j;
 CadFile infile;
 QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

  _tcscpy(ext,v.getstring("bm::ext"));  _strlwr(ext);

  // set default path
  strcpy(dirname,home.getpublicfilename(""));

#if 1 // using the figure browser
  file[0] = '\0';

  memset(filter,0,256);
  _tcscpy(filter,rs7.getws());

  // look for last path and file
  QString lastFile = settings.value("FigureBrowser/Bitmap_Filename","").toString();
  QString lastDir = settings.value("FigureBrowser/Bitmap_Filepath",QString(dirname)).toString();

  // v8 if no path use the default value
  if(lastDir.isEmpty())
      lastDir = QString(v.getstring("db::textures-path"));

  // last resort go to the public folder
  if(lastDir.isEmpty())
      lastDir = QString(home.getpublicfilename(_RCT("Images")));
  // v8

  BitMask options(32);
  options = *v.getbm("bi::options");

  RCFigureBrowser filedialog(0);
  filedialog.SetFilter(filter);
  filedialog.SetRootDir(lastDir.data());
  filedialog.SetTitle(rs8.gets());
  filedialog.SetPrefix(_RCT("Bitmap"));
  filedialog.ShowSettingsGroup(RCFigureBrowser::bitmaps);

  filedialog.SetBitmapWidth(v.getreal("bi::width"));
  filedialog.SetBitmapHeight(v.getreal("bi::height"));

  filedialog.SetBitmapOriginalShape(options.test(BitmapESameShape));
  filedialog.SetBitmapLinkFile(options.test(BitmapELinkFile));

  filedialog.OnInitDialog();

  QString selFilename;
  if(filedialog.exec())
  {
      selFilename = QString(filedialog.GetFileName());
      strcpy(name,QDir::fromNativeSeparators(selFilename).data());

      if (strrchr(name,'.') != 0)
         v.setstring("bm::ext",strrchr(name,'.')+1);

      v.setreal("bi::width",filedialog.BitmapWidth());
      v.setreal("bi::height",filedialog.BitmapHeight());

      options.set(BitmapESameShape,(int)filedialog.GetBitmapOriginalShape());
      options.set(BitmapELinkFile,(int)filedialog.GetBitmapLinkFile());
      v.setbm("bi::options",options);

      settings.setValue("FigureBrowser/Bitmap_Filename",selFilename.mid(selFilename.lastIndexOf(QDir::separator())+1));
      settings.setValue("FigureBrowser/Bitmap_Filepath",selFilename.left(selFilename.lastIndexOf(QDir::separator())));
      return 1;
  }
  else
      return 0;

#else
  file[0] = '\0';

  memset(filter,0,256);
  _tcscpy(filter,rs7.getws());
  /*
  e = filter;  j = 1;
  for (i = 0; filter[i] != '\0'; i++)
    {  if (filter[i] == '|')
          {  filter[i] = '\0';
             strcpy(name,e);  _strlwr(name);
             if (strstr(name,ext) != 0)
               index = j;
             e = filter + i + 1;
             j++;
          }
    }
  for (i = 0 ; i < 256 ; i++)
    if (filter[i] == '^')
      filter[i] = 0;
    */

  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = cadwindow->gethwnd();
  ofn.lpstrFilter = filter;
  ofn.lpstrTitle = rs8.gets();
  ofn.nFilterIndex = index;
  ofn.lpstrFile= file;
  ofn.nMaxFile = sizeof(file);
  ofn.lpstrFileTitle = title;
  ofn.nMaxFileTitle = sizeof(title);
  ofn.lpstrInitialDir = dirname;
  ofn.lpstrDefExt = ext;
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  if (GetOpenFileName(&ofn))
  {
       strcpy(name,ofn.lpstrFile);
       if (strrchr(name,'.') != 0)
         v.setstring("bm::ext",strrchr(name,'.')+1);
       return 1;
  }
  else
    return 0;
#endif
}

int BitMapE::process_property(int action,int id,void *data,RCCHAR *text)
{BitMask change_mask(32),new_options(32);
 int i,result,cstate,horizontal_justification,vertical_justification,frame_style;
 double x,wt;
 const char *horizontal_justification_l[] = {  "Left", "Centre", "Right"  };
 const char *vertical_justification_l[] = {  "Bottom", "Centre", "Top"  };
 const char *frame_style_l[] = {  "None" , "Single", "Double"  };

    horizontal_justification = options.test(2) * 2 + options.test(1);
    vertical_justification = options.test(4) * 2 + options.test(3);
    frame_style = options.test(7) * 4 + options.test(6) * 2 + options.test(5);

    if (action == PP_GETNUMPROPERTIES)
        id = 10000;

    if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Entity");
            return 1;
        }
    }
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Colour");
        else if (action == PP_CLICK)
        {   result = ((Entity *)data)->getcolour();
            cstate = 1001;
            colour_command(&cstate,0,(void **)&result);
            change_mask.set(0,1);
            if (result != ((Entity *)data)->getcolour())
                ((Entity *)data)->change(change_mask,result,0,0,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getcolour());
    }
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Layer");
        else if (action == PP_CLICK)
        {   result = ((Entity *)data)->getlayer();
            cstate = 1001;
            layer_command(&cstate,0,(void **)&result);
            change_mask.set(1,1);
            if (result != ((Entity *)data)->getlayer())
                ((Entity *)data)->change(change_mask,0,result,0,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getlayer());
    }
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Style");
        else if (action == PP_CLICK)
        {   result = ((Entity *)data)->getstyle();
            cstate = 1001;
            style_command(&cstate,0,(void **)&result);
            change_mask.set(2,1);
            if (result != ((Entity *)data)->getstyle())
                ((Entity *)data)->change(change_mask,0,0,result,0);
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%d",getstyle());
    }
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
            strcpy(text,"Weight");
        else if (action == PP_CLICK)
        {   wt = db.lineweights.weighttomm(((Entity *)data)->getweight());
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            change_mask.set(3,1);
            if (wt != db.lineweights.weighttomm(((Entity *)data)->getweight()))
                ((Entity *)data)->change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
            return 0;
        }
        else if (action == PP_GETVALUE)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(getweight()));
    }
    else if (id-- == 0)
    {   if (action == PP_GETLABEL) 
        {   strcpy(text,"Bitmap");
            return 1;
        }
    }
    else if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Original Shape");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(BitmapESameShape) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(4,1);
            new_options.set(BitmapESameShape,_stricmp(text,"Yes") == 0);
            //void BitMapE::change(const BitMask &change,const EntityHeader &eh,char *fn,double w,double h,double tr,double tg,double tb,double trange,double op,const BitMask &o)
            change(change_mask,*this,0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,new_options);
            return 1;
        }
    }
    else if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Link to file");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(BitmapELinkFile) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(5,1);
            new_options.set(BitmapESameShape,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,new_options);
            return 1;
        }
    }
    else if (options.test(BitmapELinkFile) && id-- == 0)
    {   if (action == PP_CLICK)
        {RCCHAR fn[300];
            strcpy(fn,filename);
            if (BitMapE::select(fn))
            {   change_mask.set(15,1);
                change_mask.set(18,1);
                change(change_mask,*this,fn,0.0,0.0,0.0,0.0,0.0,0.0,0.0,new_options);
            }
            return 1;
        }
        else if (action == PP_GETLABEL) 
                strcpy(text,"Filename");
        else if (action == PP_GETVALUE) 
        {   if (strrchr(filename,'\\') != 0)
                strcpy(text,strrchr(filename,'\\')+1);
            else
                strcpy(text,filename);
        }
    }    
    else if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Width");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,width,FormatForEditField);
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;
            change_mask.set(6,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,0.0,new_options);
        }
    }
    else if (id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Height");
        else if (action == PP_GETVALUE)
            db.formatnumber(text,height,FormatForEditField);
        else if (action == PP_SETVALUE)
        {Expression e(text);
         double x;
            change_mask.set(7,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0,0.0,x * db.getmainunitscale(),0.0,0.0,0.0,0.0,0.0,new_options);
        }
    }
    else if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"Left\tCentre\tRight");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Horz Justification");
        else if (action == PP_GETVALUE)
            strcpy(text,horizontal_justification_l[horizontal_justification]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; i < 3 ; i++) 
                if (strcmp(text,horizontal_justification_l[i]) == 0)
                {   change_mask.set(8,1);
                    new_options.set(BitmapEHJust1,(i&1)!=0);
                    new_options.set(BitmapEHJust2,(i&2)!=0);
                    change(change_mask,*this,0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,new_options);
                }
        }
    }
    else if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"Bottom\tCentre\tTop");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Vert Justification");
        else if (action == PP_GETVALUE)
            strcpy(text,vertical_justification_l[vertical_justification]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; i < 3 ; i++) 
                if (strcmp(text,vertical_justification_l[i]) == 0)
                {   change_mask.set(9,1);
                    new_options.set(BitmapEVJust1,(i&1)!=0);
                    new_options.set(BitmapEVJust2,(i&2)!=0);
                    change(change_mask,*this,0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,new_options);
                }
        }
    }
    else if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"None\tSingle\tDouble");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Frame Style");
        else if (action == PP_GETVALUE)
            strcpy(text,frame_style_l[frame_style]);
        else if (action == PP_SETVALUE)
        {   for (i = 0 ; i < 3 ; i++) 
                if (strcmp(text,frame_style_l[i]) == 0)
                {   change_mask.set(10,1);
                    new_options.set(BitmapEStyle1,(i&1)!=0);
                    new_options.set(BitmapEStyle2,(i&2)!=0);
                    new_options.set(BitmapEStyle3,(i&4)!=0);
                    change(change_mask,*this,0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,new_options);
                }
        }
    }
    else if (id-- == 0)
    {   if (action == PP_CLICK)
        {   strcpy(text,"No\tYes");
            return 2;
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Transparent Bitmap");
        else if (action == PP_GETVALUE)
            strcpy(text,options.test(BitmapETransparent) ? "Yes" : "No");
        else if (action == PP_SETVALUE)
        {   change_mask.set(11,1);
            new_options.set(BitmapETransparent,_stricmp(text,"Yes") == 0);
            change(change_mask,*this,0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,new_options);
            return 1;
        }
    }
    else if (options.test(BitmapETransparent) && id-- == 0)
    {   if (action == PP_CLICK)
        {Dialog dialog("Selectcolour_Dialog");
         DialogControl *dc;
         double r,g,b;

            r = red;
            g = green;
            b = blue;
            dialog.add(dc = new RealDialogControl(101,&red,0.1,0.0,1.0));
            dialog.add(new ScrollBarDialogControl(1101,dc));
            dialog.add(new RTColourScrollBarDialogControl(104,dc));
            dialog.add(dc = new RealDialogControl(102,&green,0.1,0.0,1.0));
            dialog.add(new ScrollBarDialogControl(1102,dc));
            dialog.add(new RTColourScrollBarDialogControl(105,dc));
            dialog.add(dc = new RealDialogControl(103,&blue,0.1,0.0,1.0));
            dialog.add(new ScrollBarDialogControl(1103,dc));
            dialog.add(new RTColourScrollBarDialogControl(106,dc));
            dialog.add((DisplayDialogControl *) new RTColourDisplayDialogControl(107,0));
            if (dialog.process() == TRUE)
            {   change_mask.set(12); 
                change_mask.set(13); 
                change_mask.set(14); 
                change(change_mask,*this,0,0.0,0.0,red,green,blue,0.0,0.0,new_options);
            }
            return 1; 
        }
        else if (action == PP_GETLABEL) 
            strcpy(text,"Transparent Colour");
        else if (action == PP_GETVALUE) 
        {   sprintf(text,"R%.2lf G%.2lf B%.2lf",red,green,blue);
        }

    }
    else if (options.test(BitmapETransparent) && id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Transparency Range");
        else if (action == PP_GETVALUE)
            sprintf(text,"%.3lf",transparencyrange);
        else if (action == PP_SETVALUE)
        {   change_mask.set(16,1);
            change(change_mask,*this,0,0.0,0.0,0.0,0.0,0.0,atof(text),0.0,new_options);
        }
    }
    else if (options.test(BitmapETransparent) && id-- == 0)
    {   if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETLABEL) 
            strcpy(text,"Opacity");
        else if (action == PP_GETVALUE)
            sprintf(text,"%.3lf",opacity);
        else if (action == PP_SETVALUE)
        {   change_mask.set(17,1);
            x = atof(text);
            if (x < 0.0)
                x = 0.0; 
            if (x > 1.0)
                x = 1.0;
            change(change_mask,*this,0,0.0,0.0,0.0,0.0,0.0,0.0,x,new_options);
        }
    }
    else if (action == PP_GETNUMPROPERTIES)
        return 10000 - id;
    
    return 0;
}





