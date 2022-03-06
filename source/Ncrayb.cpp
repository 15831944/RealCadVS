
#include "ncwin.h"
#include "RCView.h"
#include "RCGraphicsView.h"
//#ifndef _USING_QT
//#include "gdiplus.h"
//#endif
#if MENUS != STUDENTMENUS

BumpMap *BumpMaps::load(RCCHAR *name)
{BumpMap *bumpmap;
  for (list.start() ; (bumpmap = (BumpMap *) list.next()) != 0 ; )
    if (bumpmap->match(name))
      return bumpmap;
  if ((bumpmap = new BumpMap(name)) == 0)
    return 0;
  if (! bumpmap->match(name))
    {  delete bumpmap;
       return 0;
    }
  list.add(bumpmap);
  return bumpmap;
}

BumpMaps::~BumpMaps()
{BumpMap *bumpmap;
  for (list.start() ; (bumpmap = (BumpMap *) list.next()) != 0 ; )
    delete bumpmap;
  list.destroy();
}

BumpMap::BumpMap(RCCHAR *filename)
{FILE *infile;
 char header[20];
 short w,h;
 int i;
  name[0] = 0;
  normaldata = 0;
  if ((infile = fopen(filename,"rb")) == 0)
    return;
  if (fread(header,20,1,infile) != 1)
    {  fclose(infile);
       return;
    }
  if (strcmp(header,"Thomas-BumpMapData.") != 0)
    {  fclose(infile);
       return;
    }
  if (fread(&w,sizeof(w),1,infile) != 1)
    {  fclose(infile);
       return;
    }
  if (fread(&h,sizeof(h),1,infile) != 1)
    {  fclose(infile);
       return;
    }
  width = w;  height = h;
  if ((normaldata = new  signed char *[int(height * 3)]) == 0)
    return;
  for (i = 0 ; i < height ; i++)
    {  if ((normaldata[i] = new signed char [width*3]) == 0)
         {  while (i > 0)
              {  i--;  delete [] normaldata[i];
              }
            delete normaldata;
            fclose(infile);
            return;
         }
       fread(normaldata[i],width*3,1,infile);
    }
  fclose(infile);
  strcpy(name,filename);
}

BumpMap::~BumpMap()
{int i;
  if (normaldata != 0)
    {  for (i = 0 ; i < height ; i++)
         delete [] normaldata[i];
       delete [] normaldata;
    }
}

void BumpMap::getnormal(double x,double y,Point *normal)
{
  signed char *xa;
  xa = & normaldata[int(fmod(y,1.0) * height)][int(fmod(x,1.0) * width) * 3];
  normal->x = *xa++ / 127.0;
  normal->y = *xa++ / 127.0;
  normal->z = *xa / 127.0;
}

void SurfaceProperties::getdefaults(void)
{ shader = v.getinteger("sp::shader");
  options = *v.getbm("sp::options");
  transparency = v.getreal("sp::transparency");
  ambient = v.getreal("sp::ambient");
  diffuse = v.getreal("sp::diffuse");
  specular = v.getreal("sp::specular");
  exponent = v.getreal("sp::exponent");
  reflectivity = v.getreal("sp::reflectivity");
  smoothness = v.getreal("sp::smoothness");
  metallic = v.getreal("sp::metallic");
  refractiveindex = v.getreal("sp::refractiveindex");

  tmi.texturetype = v.getinteger("sp::texturetype");
  strcpy(tmi.filename,v.getstring("sp::texturemap"));
  tmi.x1 = v.getreal("sp::tmx1");
  tmi.y1 = v.getreal("sp::tmy1");
  tmi.x2 = v.getreal("sp::tmx2");
  tmi.y2 = v.getreal("sp::tmy2");
  tmi.transparentcolour.red = v.getreal("sp::transparentred");
  tmi.transparentcolour.green = v.getreal("sp::transparentgreen");
  tmi.transparentcolour.blue = v.getreal("sp::transparentblue");

  tmi.options = *v.getbm("sp::options");
  tmi.size = v.getreal("sp::texturesize");
  tmi.patternscale = v.getreal("sp::patternscale");
  tmi.colourscale = v.getreal("sp::colourscale");
  tmi.origin = db.workplanes.getcurrent()->getorigin();
  tmi.xaxis  = db.workplanes.getcurrent()->getxaxis();
  tmi.yaxis  = db.workplanes.getcurrent()->getyaxis();

  strcpy(bmi.filename,v.getstring("sp::bumpmap"));
  bmi.x1 = v.getreal("sp::bmx1");
  bmi.y1 = v.getreal("sp::bmy1");
  bmi.x2 = v.getreal("sp::bmx2");
  bmi.y2 = v.getreal("sp::bmy2");
  bmi.options = *v.getbm("sp::options");
}

TextureMap *Textures::load(RCCHAR *name)
{TextureMap *texture;
  for (list.start() ; (texture = (TextureMap *) list.next()) != 0 ; )
    if (texture->match(name))
      return texture;
  if ((texture = new TextureMap(name)) == 0)
    return 0;
  if (! texture->match(name))
    {  delete texture;
       return 0;
    }
  list.add(texture);
  return texture;
}

Textures::~Textures()
{TextureMap *texture;
  for (list.start() ; (texture = (TextureMap *) list.next()) != 0 ; )
    delete texture;
  list.destroy();
}

#if 1
TextureMap::TextureMap(RCCHAR *filename)
{
 int y;
 ResourceString rs28(NCRAY+28);
 ResourceString rs41(NCRAY+41);
 ResourceString rs43(NCRAY+43);

  name[0] = 0;
  rgbdata = 0;
  rgbquad = 0;

  QImage image;
  image.load(QString(filename));

  qDebug() << "texture image format : " << image.format();
  //if(image.format() != QImage::Format_RGB888)
  //    image = image.convertToFormat(QImage::Format_RGB888);

  if (image.isNull())
  {
       cadwindow->MessageBox(rs41.gets(),rs28.gets(),MB_ICONINFORMATION);
       return;
  }

  // turn it upside down
  image = image.mirrored(false,true);

  width = int(image.width());
  height = int(image.height());

  qDebug() << "texture image format : " << image.format();
  qDebug() << "texture image bytesPerLine : " << image.bytesPerLine();

  if ((rgbdata = new unsigned char *[image.height()]) == 0)
  {
       cadwindow->MessageBox(rs43.gets(),rs28.gets(),MB_ICONINFORMATION);
       return;
  }
  for (y = 0 ; y < (int)image.height() ; y++)
  {
      if ((rgbdata[y] = new unsigned char [image.bytesPerLine()]) != 0)
         memcpy(rgbdata[y],(uchar*)image.scanLine(y),image.bytesPerLine());
  }
  strcpy(name,filename);

  // debug
  /*
  unsigned char *debugbytes = new unsigned char[image.byteCount()];
  unsigned char *imgptr = debugbytes;
  for (y = 0 ; y < (int)image.height() ; y++)
  {
      memcpy(imgptr,rgbdata[y],image.bytesPerLine());
      imgptr += image.bytesPerLine();
  }
  QImage debugImg(debugbytes,image.width(),image.height(),image.bytesPerLine(),image.format());
  debugImg.save(QString(home.getpublicfilename("debugtexture.bmp")),"BMP");
  */
  // debug
}

#else
TextureMap::TextureMap(RCCHAR *filename)
{
 Gdiplus::GdiplusStartupInput gdiplusStartupInput;
 ULONG_PTR gdiplusToken;
 WCHAR wfilename[STRING_LENGTH];
 Gdiplus::Rect rect;
 Gdiplus::BitmapData bd;
 int y;
 ResourceString rs28(NCRAY+28);
 ResourceString rs41(NCRAY+41);
 ResourceString rs43(NCRAY+43);

  name[0] = 0;
  rgbdata = 0;
  rgbquad = 0;

  //MultiByteToWideChar(CP_ACP,0,filename,strlen(filename)+1,wfilename,STRING_LENGTH);

  Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(wfilename);

  if (bitmap->GetLastStatus() != Gdiplus::Ok)
    {  cadwindow->MessageBox(rs41.gets(),rs28.gets(),MB_ICONINFORMATION);
       return;
    }

  rect.X = 0;
  rect.Y = 0;
  rect.Width = bitmap->GetWidth();
  rect.Height = bitmap->GetHeight();

  width = int(bitmap->GetWidth());
  height = int(bitmap->GetHeight());

  bitmap->LockBits(&rect,Gdiplus::ImageLockModeRead,PixelFormat24bppRGB,&bd);

  if ((rgbdata = new unsigned char *[bd.Height]) == 0)
    {  cadwindow->MessageBox(rs43.gets(),rs28.gets(),MB_ICONINFORMATION);
       return;
    }
  for (y = 0 ; y < (int)bd.Height ; y++)
    {  if ((rgbdata[y] = new unsigned char [bd.Stride]) != 0)
         memcpy(rgbdata[y],((char *)bd.Scan0) + (bd.Height - 1 - y) * bd.Stride,bd.Stride);
    }
  bitmap->UnlockBits(&bd);
  delete bitmap;

  Gdiplus::GdiplusShutdown(gdiplusToken);

  strcpy(name,filename);
}
#endif

TextureMap::~TextureMap()
{int i;
  if (rgbdata != 0)
    {  for (i = 0 ; i < height ; i++)
         delete [] rgbdata[i];
       delete [] rgbdata;
    }
  delete rgbquad;
}

void TextureMap::getcolour(double x,double y,RealColour *c)
{unsigned char *red,index;
 double i;
  if (x < 0.0)
    x = modf(x,&i) + 1.0;
  if (y < 0.0)
    y = modf(y,&i) + 1.0;
  if (rgbquad == 0)
  {
       //red = & rgbdata[int(fmod(y,1.0) * height)][int(fmod(x,1.0) * width) * 3];// for 24 bit
       red = & rgbdata[int(fmod(y,1.0) * height)][int(fmod(x,1.0) * width) * 4];// for 32 bit
       c->blue = *red++ / 255.0;
       c->green = *red++ / 255.0;
       c->red = *red / 255.0;
  }
  else
  {
       index = rgbdata[int(fmod(y,1.0) * height)][int(fmod(x,1.0) * width)];
       c->red = rgbquad[index].rgbRed / 255.0;
       c->green = rgbquad[index].rgbGreen / 255.0;
       c->blue = rgbquad[index].rgbBlue / 255.0;
  }
}

Noise::Noise(int ts,int vb,int ppv)
{int i;
  tablesize = ts;  voxelbits = vb;  voxelextents = 1 << vb;
  pointspervoxel = ppv;
  if ((table = new double[tablesize]) != 0)
    {  for (i = 0 ; i < tablesize ; i++)
         table[i] = 0.5 + 0.5 * cos(M_PI * sqrt(double(i) / double(tablesize-1)));
    }
  scale = 1.4175 / pointspervoxel;
  buffer[0] = new Point[27 * pointspervoxel];
  buffer[1] = new Point[27 * pointspervoxel];
  buffer[2] = new Point[27 * pointspervoxel];
  buffer[3] = new Point[27 * pointspervoxel];
  ibuffer[0] = jbuffer[0] = kbuffer[0] = -32000;
  ibuffer[1] = jbuffer[1] = kbuffer[1] = -32000;
  ibuffer[2] = jbuffer[2] = kbuffer[2] = -32000;
  ibuffer[3] = jbuffer[3] = kbuffer[3] = -32000;
}

double Noise::getturbulence(Point pn)
{ return getnoise(0,pn) + getnoise(1,pn * 2.0) / 2.0 + getnoise(2,pn * 4.0) / 4.0 + getnoise(3,pn * 8.0) / 8.0;
}

Point Noise::getdnoise(int buf,Point pn)
{Point dn;
 double n;
  n = getnoise(buf,pn);
  pn.x += 0.001;  dn.x = (getnoise(buf,pn) - n) / 0.001;  pn.x -= 0.001;
  pn.y += 0.001;  dn.y = (getnoise(buf,pn) - n) / 0.001;  pn.y -= 0.001;
  pn.z += 0.001;  dn.z = (getnoise(buf,pn) - n) / 0.001;
  return dn;
}

double Noise::getnoise(int buf,Point pn)
{int i,j,k,i1,i2,j1,j2,k1,k2;
 double d,noise;
 Point *p1,*p2;
  p = pn * 4.0;
  p.x = fmod(fabs(p.x + 45.0),voxelextents);
  p.y = fmod(fabs(p.y + 92.0),voxelextents);
  p.z = fmod(fabs(p.z + 17.0),voxelextents);
  i = (int(p.x) + voxelextents - 1) % voxelextents;
  j = (int(p.y) + voxelextents - 1) % voxelextents;
  k = (int(p.z) + voxelextents - 1) % voxelextents;
  if (i != ibuffer[buf] || j != jbuffer[buf] || k != kbuffer[buf])
    {  i1 = i + 1;  if (i1 == voxelextents) i1 = 0;
       i2 = i1 + 1;  if (i2 == voxelextents) i2 = 0;
       j1 = j + 1;  if (j1 == voxelextents) j1 = 0;
       j2 = j1 + 1;  if (j2 == voxelextents) j2 = 0;
       k1 = k + 1;  if (k1 == voxelextents) k1 = 0;
       k2 = k1 + 1;  if (k2 == voxelextents) k2 = 0;
       p1 = buffer[buf];
       calcpointsinvoxel(i,j,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i1,j,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i2,j,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i,j1,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i1,j1,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i2,j1,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i,j2,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i1,j2,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i2,j2,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i,j,k1,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i1,j,k1,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i2,j,k1,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i,j1,k1,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i1,j1,k1,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i2,j1,k1,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i,j2,k1,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i1,j2,k1,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i2,j2,k1,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i,j,k2,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i1,j,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i2,j,k,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i,j1,k2,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i1,j1,k2,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i2,j1,k2,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i,j2,k2,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i1,j2,k2,p1);  p1 += pointspervoxel;
       calcpointsinvoxel(i2,j2,k2,p1);
       ibuffer[buf] = i;  jbuffer[buf] = j;  kbuffer[buf] = k;
    }
  p1 = buffer[buf];
  p2 = p1 + 27 * pointspervoxel;
  for (noise = 0.0 ; p1 != p2 ; p1++)
    {  d = (p1->x - p.x) * (p1->x - p.x) + (p1->y - p.y) * (p1->y - p.y) + (p1->z - p.z) * (p1->z - p.z);
       if (d < 1.0)
         noise += table[int(d * tablesize)];
    }
  noise = noise * scale - 0.625;
  if (noise > 1.0) noise = 1.0;
  if (noise < 0.0) noise = 0.0;
  return noise;
}

void Noise::calcpointsinvoxel(int i,int j,int k,Point *buffer)
{long seed,hi,lo,t;
 int l;
  seed = (long(i) << 20) | (long(j) << 10) | long(k) + 387L;
  for (l = 0 ; l < pointspervoxel ; l++)
    {  hi = seed / 127773L;
       lo = seed % 127773L;
       t = 16807L * lo - 2836L * hi;
       if (t > 0)
         seed = t;
       else
         seed = t + 2147483647L;
       buffer[l].x = double(seed) / 2147483647.0 + i;
       hi = seed / 127773L;
       lo = seed % 127773L;
       t = 16807L * lo - 2836L * hi;
       if (t > 0)
         seed = t;
       else
         seed = t + 2147483647L;
       buffer[l].y = double(seed) / 2147483647.0 + j;
       hi = seed / 127773L;
       lo = seed % 127773L;
       t = 16807 * lo - 2836 * hi;
       if (t > 0)
         seed = t;
       else
         seed = t + 2147483647L;
       buffer[l].z = double(seed) / 2147483647.0 + k;
    }
}

static int shadecallback(SurfaceEntity *se,void *rtp,int,void *,void *cbdata)
{CallBackInfo *cbi = (CallBackInfo *) cbdata;
  ((RayTracePolygon *)rtp)->setsurfaceinfo(se,cbi->si,cbi->rti->getdefaultsi());
  if (cbi->se->isa(wall_entity))
    {SurfaceProperties sp;
       sp.copyinfo(cbi->se,1,cbi->rti,&cbi->si);
       if (cbi->si != 0)
         cbi->rti->addsurfaceinfo(cbi->si);
    }
  return cbi->rti->add((RayTracePolygon *)rtp);
}

int RayTraceImage::loadpolygons(void)
{Entity *e;
 double error;
 int stopped,nhor,nver,nloops,maxloops,nverts,maxverts;
 LightInfo *li;
 BitMask opt(32);
 ResourceString rs28(NCRAY+28),rs29(NCRAY+29);
 SurfaceProperties sp;
 CallBackInfo cbi;
 RayTracePolygon *rtp;
 int i,j,k,imin,jmin,kmin,imax,jmax,kmax;
 Point clippoint;
  cbi.rti = this;
  sp.copyinfo(0,0,this,&defaultsi);
  opt.set(0,1);
  program->processpaintmessages();
  program->setbusy(1);
  for (db.geometry.start(),stopped = 0 ; (e = db.geometry.next()) != NULL ; )
    if (e->isa(light_entity) && e->isvisible(surface))
      {  li = new LightInfo;
         if (li != 0)
           {  ((Light *)e)->copyinfo(li);
              lights.add(li);
           }
      }
    else if (e->issurface() && e->isvisible(surface))
      {SurfaceEntity *se = (SurfaceEntity *) e;
         cbi.se = (SurfaceEntity *) e;
         sp.copyinfo(se,0,this,&cbi.si);
         if (cbi.si != 0)
           surfaceinfos.add(cbi.si);
         error = (surface->getumax() - surface->getumin()) / surface->getwidth() * db.getdtoler();
         se->nfacets(error,&nhor,&nver);
         se->facet(opt,error,nhor,nver,surface,shadecallback,&cbi);
         if ((stopped = program->getinterrupt()) != 0)
           {  program->setbusy(0);
              return 0;
           }
      }

  //  Create the Voxel Data
  vmin.setxyz(1.0E38,1.0E38,1.0E38);
  vmax.setxyz(-1.0E38,-1.0E38,-1.0E38);
  maxloops = maxverts = 0;
  for (polygons.start() ; (rtp = (RayTracePolygon *)polygons.next()) != 0; )
    {  rtp->getinfo(&nloops,&nverts,&vmin,&vmax);
       if (nloops > maxloops)
         maxloops = nloops;
       if (nverts > maxverts)
         maxverts = nverts;
    }
  if (vmin.x > vmax.x)
    {  vmin.setxyz(0.0,0.0,0.0);  vmax.setxyz(0.0,0.0,0.0);
    }
  vmax.x += 4.0;  vmax.y += 4.0;  vmax.z += 4.0;
  vmin.x -= 5.152;  vmin.y -= 5.152;  vmin.z -= 5.152;
  dvoxel = (vmax - vmin) / NUM_VOXELS;
  VoxelPolygon *poly = new VoxelPolygon(maxloops+1,maxverts*8);
  VoxelPolygon *poly1 = new VoxelPolygon(maxloops+1,maxverts*8);
  VoxelPolygon *poly2 = new VoxelPolygon(maxloops+1,maxverts*8);
  VoxelPolygon *poly3 = new VoxelPolygon(maxloops+1,maxverts*8);
  VoxelPolygon *poly4 = new VoxelPolygon(maxloops+1,maxverts*8);
  VoxelPolygon *poly5 = new VoxelPolygon(maxloops+1,maxverts*8);
  VoxelPolygon *poly6 = new VoxelPolygon(maxloops+1,maxverts*8);
  if (poly != 0  && poly->vertices != 0 &&
      poly1 != 0 && poly1->vertices != 0 &&
      poly2 != 0 && poly2->vertices != 0 &&
      poly3 != 0 && poly3->vertices != 0 &&
      poly4 != 0 && poly4->vertices != 0 &&
      poly5 != 0 && poly5->vertices != 0 &&
      poly6 != 0 && poly6->vertices != 0)
    for (polygons.start() ; (rtp = (RayTracePolygon *)polygons.next()) != 0; )
      {  rtp->copy(poly);
         if (poly->nloops == 0)
           continue;
         imin = int((poly->pmin.x - vmin.x) / dvoxel.x);
         jmin = int((poly->pmin.y - vmin.y) / dvoxel.y);
         kmin = int((poly->pmin.z - vmin.z) / dvoxel.z);
         imax = int((poly->pmax.x - vmin.x) / dvoxel.x);
         jmax = int((poly->pmax.y - vmin.y) / dvoxel.y);
         kmax = int((poly->pmax.z - vmin.z) / dvoxel.z);
         if (imin == imax && jmin == jmax)
           {  for (k = kmin ; k <= kmax ; k++)
                voxels[imin][jmin][k].add(rtp);
           }
         else if (jmin == jmax && kmin == kmax)
           {  for (i = imin ; i <= imax ; i++)
                voxels[i][jmin][kmin].add(rtp);
           }
         else if (imin == imax && kmin == kmax)
           {  for (j = jmin ; j <= jmax ; j++)
                voxels[imin][j][kmin].add(rtp);
           }
         else
           {  clippoint.x = vmin.x + dvoxel.x * imin;
              i = imin;
              while (poly->nloops > 0)
                {  clippoint.x += dvoxel.x;
                   clippoint.y = vmin.y + dvoxel.y * jmin;
                   j = jmin;
                   poly->clip(0,clippoint.x,poly1,poly2);
                   while (poly1->nloops > 0)
                     {  clippoint.y += dvoxel.y;
                        clippoint.z = vmin.z + dvoxel.z * kmin;
                        poly1->clip(1,clippoint.y,poly3,poly4);
                        k = kmin;
                        while (poly3->nloops > 0)
                          {  clippoint.z += dvoxel.z;
                             poly3->clip(2,clippoint.z,poly5,poly6);
                             if (poly5->nloops > 0)
                               voxels[i][j][k].add(rtp);
                             k++;
                             *poly3 = *poly6;
                          }
                        j++;
                        *poly1 = *poly4;
                     }
                   i++;
                   *poly = *poly2;
                }
           }
      }
  delete poly;  delete poly1;  delete poly2;  delete poly3;  delete poly4;  delete poly5;  delete poly6;
  program->setbusy(0);
  if (lights.empty() && options.test(31))
    cadwindow->MessageBox(rs29.gets(),rs28.gets(),MB_ICONINFORMATION);
  return ! stopped;
}

RayTraceImage::RayTraceImage(View3dSurface *s) : options(32),noise(1000,10,5)
{int i,j,k;
 GeneralList elist;
  for (i = 0 ; i < NUM_VOXELS ; i++)
    for (j = 0 ; j < NUM_VOXELS ; j++)
      for (k = 0 ; k < NUM_VOXELS ; k++)
        voxels[i][j][k] = elist;
  surface = s;
  defaultsi = 0;
  imageformat = v.getinteger("rt::imageformat");
  strcpy(filename,v.getstring("rt::filename"));
  strcpy(backgroundimage,v.getstring("rt::backimage"));
  backgroundcolour.red = v.getreal("rt::backgroundcolour.red");
  backgroundcolour.green = v.getreal("rt::backgroundcolour.green");
  backgroundcolour.blue = v.getreal("rt::backgroundcolour.blue");
  ambientcolour.red = v.getreal("rt::ambientcolour.red");
  ambientcolour.green = v.getreal("rt::ambientcolour.green");
  ambientcolour.blue = v.getreal("rt::ambientcolour.blue");
  maximumrays = v.getinteger("rt::maximumrays");
  supersamplingrays = v.getinteger("rt::supersamplingrays");
  resolution = v.getinteger("rt::resolution");
  options = *v.getbm("rt::options");
  fogcolour.red = v.getreal("rt::fogcolour.red");
  fogcolour.green = v.getreal("rt::fogcolour.green");
  fogcolour.blue = v.getreal("rt::fogcolour.blue");
  fogdistance = v.getreal("rt::fogdistance");
  ambientstrength = v.getreal("rt::ambientstrength");
  options.clear(31);
}

RayTraceImage::~RayTraceImage()
{LightInfo *li;
 RayTracePolygon *rtp;
 SurfaceInfo *si;
 int i,j,k;
   for (i = 0 ; i < NUM_VOXELS ; i++)
     for (j = 0 ; j < NUM_VOXELS ; j++)
       for (k = 0 ; k < NUM_VOXELS ; k++)
         voxels[i][j][k].destroy();
   for (lights.start() ; (li = (LightInfo *)lights.next()) != 0 ; )
     delete li;
   lights.destroy();
   for (surfaceinfos.start() ; (si = (SurfaceInfo *)surfaceinfos.next()) != 0; )
     delete si;
   surfaceinfos.destroy();
   delete defaultsi;
   for (polygons.start() ; (rtp = (RayTracePolygon *)polygons.next()) != 0; )
     if (rtp->isface())
       delete (Face *)rtp;
     else
       delete (Triangle *)rtp;
   polygons.destroy();
}

int RayTraceImage::castoneray(Point origin,Point dir,UV *uvmin,RayTracePolygon **rtpmin)
{GeneralList list;
 Point d(dir.x,dir.y,dir.z),o(origin.x,origin.y,origin.z),p1,p2,p3;
 int ix1,iy1,iz1;
 double delta,m,t,tmin;
 unsigned long xdelta,ydelta,zdelta,xdistance,ydistance,zdistance;
 int xdir,ydir,zdir;
 UV uv;
 RayTracePolygon *rtp;
 short i,j,k;
  rayid++;
  if (origin.x < vmin.x + 0.01)
    {  if (dir.x < 1.0E-5) 
         return 0;
       t = (vmin.x + 0.01 - origin.x) / dir.x;
       origin.x = vmin.x + 0.01;
       origin.y = origin.y + dir.y * t;
       origin.z = origin.z + dir.z * t;
    }
  else if (origin.x > vmax.x - 0.01)
    {  if (dir.x > -1.0E-5) 
         return 0;
       t = (vmax.x - 0.01 - origin.x) / dir.x;
       origin.x = vmax.x - 0.01;
       origin.y = origin.y + dir.y * t;
       origin.z = origin.z + dir.z * t;
    }
  if (origin.y < vmin.y + 0.01)
    {  if (dir.y < 1.0E-5) 
         return 0;
       t = (vmin.y  + 0.01 - origin.y) / dir.y;
       origin.x = origin.x + dir.x * t;
       origin.y = vmin.y + 0.01;
       origin.z = origin.z + dir.z * t;
    }
  else if (origin.y > vmax.y - 0.01)
    {  if (dir.y > -1.0E-5) 
         return 0;
       t = (vmax.y - 0.01 - origin.y) / dir.y;
       origin.x = origin.x + dir.x * t;
       origin.y = vmax.y - 0.01;
       origin.z = origin.z + dir.z * t;
    }
  if (origin.z < vmin.z + 0.01)
    {  if (dir.z < 1.0E-5) 
         return 0;
       t = (vmin.z + 0.01 - origin.z) / dir.z;
       origin.x = origin.x + dir.x * t;
       origin.y = origin.y + dir.y * t;
       origin.z = vmin.z + 0.01;
    }
  else if (origin.z > vmax.z - 0.01)
    {  if (dir.z > -1.0E-5) 
        return 0;
       t = (vmax.z - 0.01 - origin.z) / dir.z;
       origin.x = origin.x + dir.x * t;
       origin.y = origin.y + dir.y * t;
       origin.z = vmax.z - 0.01;
    }

  d.x /= dvoxel.x;
  d.y /= dvoxel.y;
  d.z /= dvoxel.z;
  if (fabs(d.x) > fabs(d.y)) m = fabs(d.x); else m = fabs(d.y);
  if (fabs(m) < fabs(d.z)) m = fabs(d.z);
  d /= m;
  p1.x = (origin.x - vmin.x) / dvoxel.x;
  p1.y = (origin.y - vmin.y) / dvoxel.y;
  p1.z = (origin.z - vmin.z) / dvoxel.z;
  ix1 = (int)floor(p1.x);
  iy1 = (int)floor(p1.y);
  iz1 = (int)floor(p1.z);
  if (ix1 < 0 || ix1 >= NUM_VOXELS ||
      iy1 < 0 || iy1 >= NUM_VOXELS ||
      iz1 < 0 || iz1 >= NUM_VOXELS)
    return 0;
  if (fabs(d.x) > 1.0E-10)
    {  delta = sqrt(1.0 + (d.y * d.y + d.z * d.z) / d.x / d.x);
       if (delta < 65535.0)
         xdelta = (unsigned long)(delta * 65536.0);
       else
         xdelta = 0xFFFFFFFFL;
       if (d.x > 0)
         {  xdistance = (unsigned long)((ix1 + 1 - p1.x) * xdelta);
            xdir = 1;
         }
       else
         {  xdistance = (unsigned long)((p1.x - ix1) * xdelta);
            xdir = -1;
         }
    }
  else
    {  xdelta = xdistance = 0xFFFFFFFFL;  xdir = 1;
    }
  if (fabs(d.y) > 1.0E-10)
    {  delta = sqrt(1.0 + (d.x * d.x + d.z * d.z) / d.y / d.y);
       if (delta < 65535.0)
         ydelta = (unsigned long)(delta * 65536.0);
       else
         ydelta = 0xFFFFFFFFL;
       if (d.y > 0)
         {  ydistance = (unsigned long)((iy1 + 1 - p1.y) * ydelta);
            ydir = 1;
         }
       else
         {  ydistance = (unsigned long)((p1.y - iy1) * ydelta);
            ydir = -1;
         }
    }
  else
    {  ydelta = ydistance = 0xFFFFFFFFL;  ydir = 1;
    }
  if (fabs(d.z) > 1.0E-10)
    {  delta = sqrt(1.0 + (d.x * d.x + d.y * d.y) / d.z / d.z);
       if (delta < 65535.0)
         zdelta = (unsigned long)(delta * 65536.0);
       else
         zdelta = 0xFFFFFFFFL;
       if (d.z > 0)
         {  zdistance = (unsigned long)((iz1 + 1 - p1.z) * zdelta);
            zdir = 1;
         }
       else
         {  zdistance =  (unsigned long)((p1.z - iz1) * zdelta);
            zdir = -1;
         }
    }
  else
    {  zdelta = zdistance = 0xFFFFFFFFL;  zdir = 1;
    }
  tmin = -1;  *rtpmin = (RayTracePolygon*)-0;
  if (! voxels[ix1][iy1][iz1].empty())
    {  list = voxels[ix1][iy1][iz1];
       for (list.start() ; (rtp = (RayTracePolygon *) list.next()) != 0 ; )
         if (rtp->intersect(o,dir,&t,&uv) != 0 && (*rtpmin == 0 || t < tmin))
           {  p3 = o + dir * t;
              i = short((p3.x - vmin.x) / dvoxel.x);
              j = short((p3.y - vmin.y) / dvoxel.y);
              k = short((p3.z - vmin.z) / dvoxel.z);
              if (i == ix1 && j == iy1 && k == iz1)
                {  tmin = t;  *uvmin = uv;  *rtpmin = rtp;
                }
              else if (*rtpmin == 0)
                {  rtp->rayid = rayid;
                   rtp->uv = uv;
                   rtp->t = t;
                   rtp->i = i;
                   rtp->j = j;
                   rtp->k = k;
                }
           }
       if (*rtpmin != 0)
         return 1;
    }
  for (;;)
    {  if (xdistance < ydistance)
         {  if (xdistance < zdistance)
              {  ix1 += xdir;
                 xdistance += xdelta;
              }
            else
              {  iz1 += zdir;
                 zdistance += zdelta;
              }
         }
       else
         {  if (ydistance < zdistance)
              {  iy1 += ydir;
                 ydistance += ydelta;
              }
            else
              {  iz1 += zdir;
                 zdistance += zdelta;
              }
         }
       if (ix1 >= NUM_VOXELS) 
         return *rtpmin != 0;
       if (ix1 < 0) 
         return  *rtpmin != 0;
	   if (iy1 >= NUM_VOXELS) 
         return *rtpmin != 0;
	   if (iy1 < 0) 
         return *rtpmin != 0;
	   if (iz1 >= NUM_VOXELS) 
         return *rtpmin != 0;
	   if (iz1 < 0) 
         return *rtpmin != 0;
       if (! voxels[ix1][iy1][iz1].empty())
         {  list = voxels[ix1][iy1][iz1];
            for (list.start() ; (rtp = (RayTracePolygon *) list.next()) != 0 ; )
              if (rtp->rayid == rayid && (*rtpmin == 0 || rtp->t < tmin) && rtp->i == ix1 && rtp->j == iy1 && rtp->k == iz1)
                {  tmin = rtp->t;  *uvmin = rtp->uv;  *rtpmin = rtp;
                }
              else if (rtp->intersect(o,dir,&t,&uv) != 0 && (*rtpmin == 0 || t < tmin))
                {  p3 = o + dir * t;
                   i = short((p3.x - vmin.x) / dvoxel.x);
                   j = short((p3.y - vmin.y) / dvoxel.y);
                   k = short((p3.z - vmin.z) / dvoxel.z);
                   if (i == ix1 && j == iy1 && k == iz1)
                     {  tmin = t;  *uvmin = uv;  *rtpmin = rtp;
                     }
                   else if (*rtpmin == 0)
                     {  rtp->rayid = rayid;
                        rtp->uv = uv;
                        rtp->t = t;
                        rtp->i = i;
                        rtp->j = j;
                        rtp->k = k;
                     }
                }
            if (*rtpmin != 0)
              return 1;
         }
    }
}

int RayTraceImage::castray(int depth,Point rorigin,Point rdirection,RealColour *colour,Point *normal)
{RayTracePolygon *rtp;
 UV uv;
 RealColour objectcolour,sobjectcolour,lcolour,reflcolour,tcolour;
 int black;
 double x,x1,x2,ldist,d,cosa1,cosa2,rn,rs,rj,f,g1,g2,rd;
 Point waxis,ldirection,refldirection,p1,p2,du,dv,interp,intern,sinterp,sintern;
 SurfaceInfo *si,*ssi;
 LightInfo *li;
  depth++;
  if (depth >= maximumrays)
    return 0;
  if (castoneray(rorigin,rdirection,&uv,&rtp) != 0)
    {  rtp->getcolour(rdirection,uv,&objectcolour,&interp,&intern,&si);
       while (objectcolour.red < 0.0)  //  Hit a transparent pixel
         {  if (castoneray(interp,rdirection,&uv,&rtp) == 0)
              return 0;
            rtp->getcolour(rdirection,uv,&objectcolour,&interp,&intern,&si);
         }

       if (normal != 0)
         *normal = intern;
#if 0
       x = ambientstrength * si->ambient;
       colour->red = x * objectcolour.red;
       colour->green = x * objectcolour.green;
       colour->blue = x * objectcolour.blue;
#else
       x = ambientstrength * si->ambient;
       colour->red = ambientcolour.red * x * objectcolour.red;
       colour->green = ambientcolour.green * x * objectcolour.green;
       colour->blue = ambientcolour.blue * x * objectcolour.blue;
#endif
       for (lights.start() ; (li = (LightInfo *) lights.next()) != 0 ; )
         {  if (li->distant)
              {  x = 1.0;
                 ldirection = li->direction;
                 ldist = 1.0E38;
              }
            else
              {  x = 1.0;
                 ldirection = interp - li->origin;
                 ldist = ldirection.length();
                 if (ldist > 1.0E-5)
                   {  ldirection /= ldist;
                      if (li->fade)
                        x /= (1.0 + pow(ldist / li->fadedistance,li->fadeexponent));
                      if (li->cone)
                        {  if ((cosa1 = ldirection.dot(li->direction)) > li->coneangle)
                             x *= pow(cosa1,li->concentration);
                           else
                             x = 0.0;
                        }
                   }
              }
            // Check for shadows and transparent surfaces
            lcolour.red = li->colour.red;
            lcolour.green = li->colour.green;
            lcolour.blue = li->colour.blue;
            black = 0;
            sinterp = interp;
            do {  if (castoneray(sinterp,-ldirection,&uv,&rtp) != 0)
                    {  rtp->getcolour(-ldirection,uv,&sobjectcolour,&sinterp,&sintern,&ssi);
                       d = (sinterp - interp).length();
                       if (d < ldist)
                         {  if (ssi->transparency == 0.0 && sobjectcolour.red >= 0.0)
                              black = 1;
                            else if (sobjectcolour.red >= 0.0)
                              {  lcolour.red *= ssi->transparency * sobjectcolour.red;
                                 lcolour.green *= ssi->transparency * sobjectcolour.green;
                                 lcolour.blue *= ssi->transparency * sobjectcolour.blue;
                              }
                         }
                    }
                  else
                    d = 2.0E38;
               } while (! black && d < ldist);
            if (black)
              continue;  // In shadow
            switch (si->shader)
              {case 0 :
                 x1 = - x * si->diffuse * intern.dot(ldirection);
                 if (x1 < 0.0)
                   x1 = 0.0;
                 else
                   {  refldirection = (rdirection - intern * (2.0 * rdirection.dot(intern))).normalize();
                      x2 = x * si->specular * li->intensity * pow(fabs(refldirection.dot(ldirection)),si->exponent);
                      colour->red += x2 * lcolour.red;
                      colour->green += x2 * lcolour.green;
                      colour->blue += x2 * lcolour.blue;
                   }
                 break;
               case 1 :
                 cosa1 = intern.dot(ldirection);
                 rd = (1.0 - si->smoothness * si->smoothness * si->smoothness) * (1.0 - si->transparency);
                 x1 = - x * (1.0 - si->metallic * si->smoothness) * rd * cosa1;
                 if (x1 < 0.0)
                   x1 = 0.0;
                 else
                   {  refldirection = (rdirection - intern * (2.0 * rdirection.dot(intern))).normalize();
                      if (cosa1 > 1.0) cosa1 = 1.0;  else if (cosa1 < -1.0) cosa1 = -1.0;
                      f = acos(fabs(cosa1)) / M_PI * 2.0 - 1.12;
                      f = 0.0144 / (f*f) - 0.0114796;
                      g1 = acos(fabs(cosa1)) / M_PI * 2.0 - 1.01;
                      g1 = 1.000098 - 0.0001000098/(g1*g1);
                      cosa2 = intern.dot(rdirection);
                      if (cosa2 < -1.0) cosa2 = -1.0; else if (cosa2 > 1.0) cosa2 = 1.0;
                      g2 = acos(fabs(cosa2)) / M_PI * 2.0 - 1.01;
                      g2 = 1.000098 - 0.0001000098/(g2*g2);
                      rn = 1.0 - si->transparency - rd;
                      rj = rn + (rn + 0.1) * f * g1 * g2;
                      if (rj > 1.0)
                        rj = 1.0;
                      if (si->smoothness < 1.0)
                        rs = rj * pow(fabs(refldirection.dot(ldirection)),3.0/(1.0 - si->smoothness));
                      else
                        rs = rj;
                      rs *= li->intensity;
                      f = si->metallic * (1.0 - f);
                      colour->red += rs * (1.0 + f * (objectcolour.red - 1.0)) * lcolour.red;
                      colour->green += rs * (1.0 + f * (objectcolour.green - 1.0)) * lcolour.green;
                      colour->blue += rs * (1.0 + f * (objectcolour.blue - 1.0)) * lcolour.blue;
                   }
                 break;
               case 2 :
                 x1 = x * si->diffuse;
                 break;
              }
            colour->red += x1 * lcolour.red * objectcolour.red;
            colour->green += x1 * lcolour.green * objectcolour.green;
            colour->blue += x1 * lcolour.blue * objectcolour.blue;
         }
       if (si->reflective)
         {  refldirection = (rdirection - intern * (2.0 * rdirection.dot(intern))).normalize();
            if (castray(depth+1,interp,refldirection,&reflcolour,0))
              {  colour->red += si->reflectivity * reflcolour.red;
                 colour->green += si->reflectivity * reflcolour.green;
                 colour->blue += si->reflectivity * reflcolour.blue;
              }
         }
       if (si->transparency > 0.0)
         {  if (castray(depth+1,interp,rdirection,&tcolour,0))
              {  colour->red += si->transparency * tcolour.red * objectcolour.red;
                 colour->green += si->transparency * tcolour.green * objectcolour.green;
                 colour->blue += si->transparency * tcolour.blue * objectcolour.blue;
              }
         }
       if (options.test(0) && fogdistance > 0.0)
         {double d,t1,t2;
            d = (interp - rorigin).length();
            t1 = d / fogdistance;
            if (t1 < 0.0) t1 = 0.0;
            if (t1 > 1.0) t1 = 1.0;
            t2 = 1.0 - t1;
            colour->red = colour->red * t2 + fogcolour.red * t1;
            colour->green = colour->green * t2 + fogcolour.green * t1;
            colour->blue = colour->blue * t2 + fogcolour.blue * t1;
         }
       return 1;
    }
  else
    return 0;
}

void RayTraceImage::startbackgroundimage(FILE **infile,BITMAPINFOHEADER *bmih,unsigned char **scanline)
{BITMAPFILEHEADER bmfh;
 int nwords,nbytes;
 ResourceString rs28(NCRAY+28);
 ResourceString rs40(NCRAY+40);
 ResourceString rs41(NCRAY+41);
 ResourceString rs42(NCRAY+42);
 ResourceString rs43(NCRAY+43);
  *infile = 0;  *scanline = 0;
  if (strlen(backgroundimage) == 0)
    return;
  if ((*infile = fopen(backgroundimage,"rb")) == 0)
    {  cadwindow->MessageBox(rs41.gets(),rs28.gets(),MB_ICONINFORMATION);
       return;
    }
  if (fread(&bmfh,sizeof(bmfh),1,*infile) != 1)
    {  cadwindow->MessageBox(rs42.gets(),rs28.gets(),MB_ICONINFORMATION);
       fclose(*infile);
       *infile = 0;  *scanline = 0;
       return;
    }
  if (bmfh.bfType != 0x4D42)
    {  cadwindow->MessageBox(rs40.gets(),rs28.gets(),MB_ICONINFORMATION);
       fclose(*infile);
       *infile = 0;  *scanline = 0;
       return;
    }
  if (fread(bmih,sizeof(*bmih),1,*infile) != 1)
    {  cadwindow->MessageBox(rs42.gets(),rs28.gets(),MB_ICONINFORMATION);
       fclose(*infile);
       *infile = 0;  *scanline = 0;
       return;
    }
  if (bmih->biBitCount != 24 || bmih->biCompression != BI_RGB)
    {  cadwindow->MessageBox(rs40.gets(),rs28.gets(),MB_ICONINFORMATION);
       fclose(*infile);
       *infile = 0;  *scanline = 0;
       return;
    }
  fseek(*infile,bmih->biClrUsed * sizeof(RGBQUAD),1);  // Skip color table
  nwords = int(bmih->biWidth * 3 + 3) / 4;
  nbytes = nwords * 4;
  if ((*scanline = new unsigned char [nbytes]) == 0)
    {  cadwindow->MessageBox(rs43.gets(),rs28.gets(),MB_ICONINFORMATION);
       *infile = 0;  *scanline = 0;
       return;
    }
  bmih->biSizeImage = nbytes;
}

#if 1
// multi platform version
// renders to an image
int RayTraceImage:: raytrace(void)
{RECT rect;
 RCHBRUSH hbrush;
 int i,j,k,l,m,nleft,p,stopped,w,h;
 short iw,ih;
 double umin,vmin,umax,vmax,focaldistance;
 Point waxis,rorigin,rdirection,p1,p2,p3,du,dv,normal,*pnormal,tnormal,duss,dvss;
 RealColour colour,tcolour;
 unsigned char *scanline,*bgscanline;
 FILE *bginfile,*outfile;
 BITMAPFILEHEADER bmfh;
 BITMAPINFOHEADER bmih,bgbmih;
 long numwords,numbytes;
 ResourceString rs28(NCRAY+28);
 ResourceString rs30(NCRAY+30);
 ResourceString rs31(NCRAY+31);
 ResourceString rs32(NCRAY+32);
 ResourceString rs42(NCRAY+42);
 //
 RCCHAR scratchPath[256]; // scratch bitmap path
 strcpy(scratchPath,home.getpublicfilename("RayTraceScratch.bmp"));
 //
  pnormal = 0;
  outfile = 0;
  rayid = 0;

  RCView *rcview = 0;
  if((rcview = static_cast<RCView*>(this->getwindow()->gethwnd())) == 0)
      return 0;

  RCGraphicsView *graphview = 0;
  if((graphview = static_cast<RCGraphicsView*>(rcview->view())) == 0)
      return 0;

  // KMJ: Note
  // bump maps are written to a file as before

  // remove the last scratch bmp file
  QFile scratchit((QString(scratchPath)));
  scratchit.remove();

  // prepare the window for painting the raytraced scene
  ((RCView*)this->getwindow()->gethwnd())->unFreezeBackImage();
  surface->clearScene();
  app->processEvents();

  // create an image to render to
  int iDPR = graphview->devicePixelRatio();
  iDPR=1;
  QImage *renderImage = new QImage(surface->getwidth()*iDPR,surface->getheight()*iDPR,QImage::Format_RGB888);
  if(renderImage->isNull())
  {
      cadwindow->MessageBox(rs30.gets(),rs28.gets(),MB_ICONINFORMATION);
      return 0;
  }
  // fill the render image with the background colour.
  // ToDo: how about using a screen grab of the current screen??
  renderImage->fill(QColor(int(backgroundcolour.red * 255.5),int(backgroundcolour.green * 255.5),int(backgroundcolour.blue * 255.5)));
  // create the painter to draw to the image
  QPainter painter(renderImage);
  // make the image the background of the view/scene
  if(graphview->backImage != 0)
      delete graphview->backImage;
  graphview->backImage = renderImage;
  //

  //if (imageformat == 1 || imageformat == 2)
  if (imageformat == 2)
  {
       /*
       if ((outfile = fopen(filename,"wb")) == 0)
         cadwindow->MessageBox(rs30.gets(),rs28.gets(),MB_ICONINFORMATION);
       else if (imageformat == 1)
         {
            numwords = long((int(surface->getwidth() * resolution) * 3L + 3L) / 4L);
            numbytes = long(numwords * 4L * int(surface->getheight() * resolution));
            bmfh.bfType = 19778;                             // BM
            bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + numbytes;
            bmfh.bfReserved1 = 0;
            bmfh.bfReserved2 = 0;
            bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
            bmih.biSize = sizeof(BITMAPINFOHEADER);
            bmih.biWidth = int(surface->getwidth() * resolution);
            bmih.biHeight = int(surface->getheight() * resolution);
            bmih.biPlanes = 1;
            bmih.biBitCount = 24;
            bmih.biCompression = BI_RGB;
            bmih.biSizeImage = 0;
            bmih.biXPelsPerMeter = 0;
            bmih.biYPelsPerMeter = 0;
            bmih.biClrUsed = 0;
            bmih.biClrImportant = 0;
            if ((scanline = new unsigned char[int(numwords * 4)]) == 0)
              {  fclose(outfile);  cadwindow->MessageBox(rs31.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
            else if (fwrite(&bmfh,sizeof(BITMAPFILEHEADER),1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
            else if (fwrite(&bmih,sizeof(BITMAPINFOHEADER),1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
         }
       else
       */
        if ((outfile = fopen(filename,"wb")) == 0)
            cadwindow->MessageBox(rs30.gets(),rs28.gets(),MB_ICONINFORMATION);
        else if (imageformat == 2)
         {  pnormal = & normal;
            iw = (short)(surface->getwidth() * resolution * iDPR);
            ih = (short)(surface->getheight() * resolution * iDPR);
            if ((scanline = new unsigned char[iw * 3]) == 0)
              {  fclose(outfile);  cadwindow->MessageBox(rs31.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
            else if (fwrite("Thomas-BumpMapData.",20,1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
            else if (fwrite(&iw,sizeof(short),1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
            else if (fwrite(&ih,sizeof(short),1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
         }
  }
  program->setbusy(1);
  rect.left = 0;  rect.top = 0;  rect.right = int(surface->getwidth()*iDPR);  rect.bottom = int(surface->getheight()*iDPR);
  //
  /*
  hbrush = CreateSolidBrush(RGB(int(backgroundcolour.red * 255.5),int(backgroundcolour.green * 255.5),int(backgroundcolour.blue * 255.5)));
  if (hbrush != 0)
    {
       FillRect(surface->gethdc(),&rect,hbrush);
       DeleteObject(hbrush);
    }
  */
  //
  // KMJ: replaced this to pre load the background image file
  // file types other than bmp are now supported
  //startbackgroundimage(&bginfile,&bgbmih,&bgscanline);
  QImage backImage;
  if(strlen(backgroundimage) > 0)
      backImage = QImage((QString(backgroundimage)));
  if(backImage.isNull() && strlen(backgroundimage) > 0)
      cadwindow->MessageBox(rs42.gets(),rs28.gets(),MB_ICONINFORMATION);
  if(!backImage.isNull() && backImage.format() != QImage::Format_RGB888 )
  {
      backImage = backImage.convertToFormat(QImage::Format_RGB888);
  }

  umin = surface->getumin();
  vmin = surface->getvmin();
  umax = surface->getumax();
  vmax = surface->getvmax();
  waxis = surface->getwaxis();
  du = surface->getuaxis() * ((umax - umin) / (surface->getwidth() * resolution * iDPR - 1));
  dv = surface->getvaxis() * ((vmax - vmin) / (surface->getheight() * resolution * iDPR - 1));
  if (supersamplingrays <= 3)
    {  duss = du / 2.0;
       dvss = dv / 2.0;
    }
  else if (supersamplingrays <= 8)
    {  duss = du / 3.0;
       dvss = dv / 3.0;
    }
  else
    {  duss = du / 4.0;
       dvss = dv / 4.0;
    }
  p1 = surface->getview()->getref() + surface->getuaxis() * umin +  surface->getvaxis() * vmin;
  w = (int)(surface->getwidth() * resolution * iDPR);
  h = (int)(surface->getheight() * resolution * iDPR);
  p = surface->getview()->getperspective();
  if (p)
    rorigin = surface->getview()->geteye();
  else
    focaldistance = (surface->getview()->geteye() - surface->getview()->geteye()).length();
  if (focaldistance < 10000.0)
    focaldistance = 10000.0;

  QGraphicsLineItem * gitem = 0;
  stopped=0;
  program->resetinterrupt();

  surface->setRaytracing(1);
  program->m_IsRaytracing = true;

  int iby = 0; // KMJ: back image scanline count
  for (j = -1 ; j < h; j++,p1 += dv)
    {
       //if (bginfile != 0 && j < bgbmih.biHeight)
       if (!backImage.isNull() && iby < backImage.height())
       {
           // getting background data from a pre loaded image
           bgscanline = backImage.scanLine(backImage.height() - 1 - iby);
           iby++;
            /*
           if (fread(bgscanline,int(bgbmih.biSizeImage),1,bginfile) != 1)
           {
                 cadwindow->MessageBox(rs42.gets(),rs28.gets(),MB_ICONINFORMATION);
                 fclose(bginfile);
                 bginfile = 0;
           }
           */
       }
       for (i = 0,p2 = p1,k = 0 ; i < w ; i++,p2 += du)
         {  tcolour.red = tcolour.green = tcolour.blue = 0.0;
            tnormal.setxyz(0.0,0.0,0.0);
            //  Supersample rays here if required
            for (m = 0 ; m < 1 + supersamplingrays ; m++)
              {  switch (m)
                   {case  0 : p3 = p2;  break;
                    case  1 : p3 = p2 + duss + dvss;  break;
                    case  2 : p3 = p2 + duss;  break;
                    case  3 : p3 = p2 + dvss;  break;
                    case  4 : p3 = p2 + duss*2.0 + dvss*2.0;  break;
                    case  5 : p3 = p2 + duss     + dvss*2.0;  break;
                    case  6 : p3 = p2 +            dvss*2.0;  break;
                    case  7 : p3 = p2 + duss*2.0 + dvss;  break;
                    case  8 : p3 = p2 + duss*2.0;  break;
                    case  9 : p3 = p2 + duss*3.0 + dvss*3.0;  break;
                    case 10 : p3 = p2 + duss*2.0 + dvss*3.0;  break;
                    case 11 : p3 = p2 + duss     + dvss*3.0;  break;
                    case 12 : p3 = p2 +            dvss*3.0;  break;
                    case 13 : p3 = p2 + duss*3.0 + dvss*2.0;  break;
                    case 14 : p3 = p2 + duss*3.0 + dvss;  break;
                    case 15 : p3 = p2 + duss*3.0;  break;
                   }
                 if (! p)
                   rorigin = p3 + waxis * focaldistance;
                 rdirection = (p3 - rorigin).normalize();
                 if (castray(0,rorigin,rdirection,&colour,pnormal))
                   {  tcolour.red += colour.red;
                      tcolour.green += colour.green;
                      tcolour.blue += colour.blue;
                      if (pnormal != 0)
                        tnormal += *pnormal;
                   }
                 else
                   {
                      //if (bginfile != 0 && j < bgbmih.biHeight && i < bgbmih.biWidth)
                      if (!backImage.isNull() && iby <= backImage.height() && i < backImage.width())
                        {
                           //tcolour.red += bgscanline[i*3+2] / 255.0;
                           //tcolour.green += bgscanline[i*3+1] / 255.0;
                           //tcolour.blue += bgscanline[i*3] / 255.0;

                           // KMJ: red and blue are reversed on the macOS
                           tcolour.red += bgscanline[i*3] / 255.0;
                           tcolour.green += bgscanline[i*3+1] / 255.0;
                           tcolour.blue += bgscanline[i*3+2] / 255.0;

                           // add one to iby or the last scanline will be repeated upto the top of the image
                           if(iby == backImage.height())
                               iby++;
                        }
                      else
                        {  tcolour.red += backgroundcolour.red;
                           tcolour.green += backgroundcolour.green;
                           tcolour.blue += backgroundcolour.blue;
                        }
                   }
              }
            tcolour.red /= (1 + supersamplingrays);
            tcolour.green /= (1 + supersamplingrays);
            tcolour.blue /= (1 + supersamplingrays);
            tnormal.normalize();
            if (tcolour.red > 1.0) tcolour.red = 1.0;
            if (tcolour.green > 1.0) tcolour.green = 1.0;
            if (tcolour.blue > 1.0) tcolour.blue = 1.0;
            //if (imageformat == 1 && outfile != 0)
            /*
            if (imageformat == 1)
              {  scanline[k++] = (unsigned char) (tcolour.blue * 255.5);
                 scanline[k++] = (unsigned char) (tcolour.green * 255.5);
                 scanline[k++] = (unsigned char) (tcolour.red * 255.5);
              }
            else
            */
            if (imageformat == 2 && outfile != 0)
            {
                 scanline[k++] = (unsigned char) ((signed char) (tnormal.x * 127.0));
                 scanline[k++] = (unsigned char) ((signed char) (tnormal.y * 127.0));
                 scanline[k++] = (unsigned char) ((signed char) (tnormal.z * 127.0));
            }

            rect.left = i/resolution;  rect.top = (h - 2 - j)/resolution;  rect.right = (i+1)/resolution;  rect.bottom = (h - 1 - j)/resolution;

            if ((stopped = program->getinterrupt()) != 0)
              break;

            // draw the scan line into the image
            painter.fillRect(rect.left,rect.top,abs(rect.right-rect.left),abs(rect.bottom-rect.top),QBrush(QColor(int(tcolour.red * 255.5),int(tcolour.green * 255.5),int(tcolour.blue * 255.5))));
            //
            /*
            hbrush = CreateSolidBrush(RGB(int(tcolour.red * 255.5),int(tcolour.green * 255.5),int(tcolour.blue * 255.5)));
            if (hbrush != 0)
              {  FillRect(surface->gethdc(),&rect,hbrush);
                 DeleteObject(hbrush);
              }
            */
         }
       /*
       if (imageformat == 1 && outfile != 0)
         {  nleft = int(numwords) * 4 - int(surface->getwidth()*resolution) * 3;
            for (l = 0 ; l < nleft ; l++) scanline[k+l] = 0;
            if (fwrite(scanline,int(numwords) * 4,1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
         }
       else
       */
        if (imageformat == 2 && outfile != 0)
         {  if (fwrite(scanline,w * 3,1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
         }
       //
       QRectF sceneRect = surface->getScene()->sceneRect();
       double scanx1 = surface->getScene()->sceneRect().left();
       double scany1 = h - 1 - j;
       double scanx2 = surface->getScene()->sceneRect().right();
       double scany2 = scany1;
       if(gitem !=0)
       {
           surface->getScene()->removeItem(gitem);
           delete gitem;
           gitem=0;
       }
       gitem = surface->getScene()->addLine(scanx1,scany1,scanx2,scany2,QPen(QBrush(QColor(0,0,0),Qt::SolidPattern),1));
       app->processEvents();
       if(gitem != 0 && surface->getScene())
       {
            surface->getScene()->removeItem(gitem);
            delete gitem;
            gitem =0;
       }
       if (stopped)
       {
         app->processEvents();
         break;
       }
    }
  surface->setRaytracing(0);
  program->m_IsRaytracing = false;
  //if ((imageformat == 1 || imageformat == 2) && outfile != 0)
  if (imageformat == 2 && outfile != 0)
  {
       fclose(outfile);
       delete [] scanline;
  }
  // KMJ not using the back ground image file like this
  //if (bginfile != 0)
  //  fclose(bginfile);
  //delete [] bgscanline;
  // save the image to the user defined bitmap or the scratch bitmap
  if(imageformat == 1)
      renderImage->save(QString(filename),"BMP");

  // debug
  //renderImage->save(QString(scratchPath),"BMP");
  program->setbusy(0);

  return 1;
}
#else
int RayTraceImage::raytrace(void)
{RECT rect;
 RCHBRUSH hbrush;
 int i,j,k,l,m,nleft,p,stopped,w,h;
 short iw,ih;
 double umin,vmin,umax,vmax,focaldistance;
 Point waxis,rorigin,rdirection,p1,p2,p3,du,dv,normal,*pnormal,tnormal,duss,dvss;
 RealColour colour,tcolour;
 unsigned char *scanline,*bgscanline;
 FILE *bginfile,*outfile;
 BITMAPFILEHEADER bmfh;
 BITMAPINFOHEADER bmih,bgbmih;
 long numwords,numbytes;
 ResourceString rs28(NCRAY+28);
 ResourceString rs30(NCRAY+30);
 ResourceString rs31(NCRAY+31);
 ResourceString rs32(NCRAY+32);
 ResourceString rs42(NCRAY+42);
  pnormal = 0;
  outfile = 0;
  rayid = 0;
  if (imageformat == 1 || imageformat == 2)
    {  if ((outfile = fopen(filename,"wb")) == 0)
         cadwindow->MessageBox(rs30.gets(),rs28.gets(),MB_ICONINFORMATION);
       else if (imageformat == 1)
         {
            numwords = long((int(surface->getwidth() * resolution) * 3L + 3L) / 4L);
            numbytes = long(numwords * 4L * int(surface->getheight() * resolution));
            bmfh.bfType = 19778;                             // BM
            bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + numbytes;
            bmfh.bfReserved1 = 0;
            bmfh.bfReserved2 = 0;
            bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
            bmih.biSize = sizeof(BITMAPINFOHEADER);
            bmih.biWidth = int(surface->getwidth() * resolution);
            bmih.biHeight = int(surface->getheight() * resolution);
            bmih.biPlanes = 1;
            bmih.biBitCount = 24;
            bmih.biCompression = BI_RGB;
            bmih.biSizeImage = 0;
            bmih.biXPelsPerMeter = 0;
            bmih.biYPelsPerMeter = 0;
            bmih.biClrUsed = 0;
            bmih.biClrImportant = 0;
            if ((scanline = new unsigned char[int(numwords * 4)]) == 0)
              {  fclose(outfile);  cadwindow->MessageBox(rs31.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
            else if (fwrite(&bmfh,sizeof(BITMAPFILEHEADER),1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
            else if (fwrite(&bmih,sizeof(BITMAPINFOHEADER),1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
         }
       else if (imageformat == 2)
         {  pnormal = & normal;
            iw = (short)(surface->getwidth() * resolution);
            ih = (short)(surface->getheight() * resolution);
            if ((scanline = new unsigned char[iw * 3]) == 0)
              {  fclose(outfile);  cadwindow->MessageBox(rs31.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
            else if (fwrite("Thomas-BumpMapData.",20,1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
            else if (fwrite(&iw,sizeof(short),1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
            else if (fwrite(&ih,sizeof(short),1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
         }
    }
  program->setbusy(1);
  rect.left = 0;  rect.top = 0;  rect.right = int(surface->getwidth());  rect.bottom = int(surface->getheight());
  hbrush = CreateSolidBrush(RGB(int(backgroundcolour.red * 255.5),int(backgroundcolour.green * 255.5),int(backgroundcolour.blue * 255.5)));
  if (hbrush != 0)
    {
       FillRect(surface->gethdc(),&rect,hbrush);
       DeleteObject(hbrush);
    }
  startbackgroundimage(&bginfile,&bgbmih,&bgscanline);
  umin = surface->getumin();
  vmin = surface->getvmin();
  umax = surface->getumax();
  vmax = surface->getvmax();
  waxis = surface->getwaxis();
  du = surface->getuaxis() * ((umax - umin) / (surface->getwidth() * resolution - 1));
  dv = surface->getvaxis() * ((vmax - vmin) / (surface->getheight() * resolution - 1));
  if (supersamplingrays <= 3)
    {  duss = du / 2.0;
       dvss = dv / 2.0;
    }
  else if (supersamplingrays <= 8)
    {  duss = du / 3.0;
       dvss = dv / 3.0;
    }
  else
    {  duss = du / 4.0;
       dvss = dv / 4.0;
    }
  p1 = surface->getview()->getref() + surface->getuaxis() * umin +  surface->getvaxis() * vmin;
  w = (int)(surface->getwidth() * resolution);
  h = (int)(surface->getheight() * resolution);
  p = surface->getview()->getperspective();
  if (p)
    rorigin = surface->getview()->geteye();
  else
    focaldistance = (surface->getview()->geteye() - surface->getview()->geteye()).length();
  if (focaldistance < 10000.0) 
    focaldistance = 10000.0;
  for (j = 0 ; j < h ; j++,p1 += dv)
    {  if (bginfile != 0 && j < bgbmih.biHeight)
         {  if (fread(bgscanline,int(bgbmih.biSizeImage),1,bginfile) != 1)
              {  cadwindow->MessageBox(rs42.gets(),rs28.gets(),MB_ICONINFORMATION);
                 fclose(bginfile);
                 bginfile = 0;
              }
         }
       for (i = 0,p2 = p1,k = 0 ; i < w ; i++,p2 += du)
         {  tcolour.red = tcolour.green = tcolour.blue = 0.0;
            tnormal.setxyz(0.0,0.0,0.0);
            //  Supersample rays here if required
            for (m = 0 ; m < 1 + supersamplingrays ; m++)
              {  switch (m)  
                   {case  0 : p3 = p2;  break;
                    case  1 : p3 = p2 + duss + dvss;  break;
                    case  2 : p3 = p2 + duss;  break;
                    case  3 : p3 = p2 + dvss;  break;
                    case  4 : p3 = p2 + duss*2.0 + dvss*2.0;  break;
                    case  5 : p3 = p2 + duss     + dvss*2.0;  break;
                    case  6 : p3 = p2 +            dvss*2.0;  break;
                    case  7 : p3 = p2 + duss*2.0 + dvss;  break;
                    case  8 : p3 = p2 + duss*2.0;  break;
                    case  9 : p3 = p2 + duss*3.0 + dvss*3.0;  break;
                    case 10 : p3 = p2 + duss*2.0 + dvss*3.0;  break;
                    case 11 : p3 = p2 + duss     + dvss*3.0;  break;
                    case 12 : p3 = p2 +            dvss*3.0;  break;
                    case 13 : p3 = p2 + duss*3.0 + dvss*2.0;  break;
                    case 14 : p3 = p2 + duss*3.0 + dvss;  break;
                    case 15 : p3 = p2 + duss*3.0;  break;
                   }
                 if (! p)
                   rorigin = p3 + waxis * focaldistance;
                 rdirection = (p3 - rorigin).normalize();
                 if (castray(0,rorigin,rdirection,&colour,pnormal))
                   {  tcolour.red += colour.red;
                      tcolour.green += colour.green;
                      tcolour.blue += colour.blue;
                      if (pnormal != 0)
                        tnormal += *pnormal;
                   }
                 else
                   {  if (bginfile != 0 && j < bgbmih.biHeight && i < bgbmih.biWidth)
                        {  tcolour.red += bgscanline[i*3+2] / 255.0;
                           tcolour.green += bgscanline[i*3+1] / 255.0;
                           tcolour.blue += bgscanline[i*3] / 255.0;
                        }
                      else
                        {  tcolour.red += backgroundcolour.red;
                           tcolour.green += backgroundcolour.green;
                           tcolour.blue += backgroundcolour.blue;
                        }
                   }
              }
            tcolour.red /= (1 + supersamplingrays);
            tcolour.green /= (1 + supersamplingrays);
            tcolour.blue /= (1 + supersamplingrays);
            tnormal.normalize();
            if (tcolour.red > 1.0) tcolour.red = 1.0;
            if (tcolour.green > 1.0) tcolour.green = 1.0;
            if (tcolour.blue > 1.0) tcolour.blue = 1.0;
            if (imageformat == 1 && outfile != 0)
              {  scanline[k++] = (unsigned char) (tcolour.blue * 255.5);
                 scanline[k++] = (unsigned char) (tcolour.green * 255.5);
                 scanline[k++] = (unsigned char) (tcolour.red * 255.5);
              }
            else if (imageformat == 2 && outfile != 0)
              {  scanline[k++] = (unsigned char) ((signed char) (tnormal.x * 127.0));
                 scanline[k++] = (unsigned char) ((signed char) (tnormal.y * 127.0));
                 scanline[k++] = (unsigned char) ((signed char) (tnormal.z * 127.0));
              }
            rect.left = i/resolution;  rect.top = (h - 2 - j)/resolution;  rect.right = (i+1)/resolution;  rect.bottom = (h - 1 - j)/resolution;
            hbrush = CreateSolidBrush(RGB(int(tcolour.red * 255.5),int(tcolour.green * 255.5),int(tcolour.blue * 255.5)));
            if (hbrush != 0)
              {  FillRect(surface->gethdc(),&rect,hbrush);
                 DeleteObject(hbrush);
              }
            if ((stopped = program->getinterrupt()) != 0)
              break;
         }
       if (imageformat == 1 && outfile != 0)
         {  nleft = int(numwords) * 4 - int(surface->getwidth()*resolution) * 3;
            for (l = 0 ; l < nleft ; l++) scanline[k+l] = 0;
            if (fwrite(scanline,int(numwords) * 4,1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
         }
       else if (imageformat == 2 && outfile != 0)
         {  if (fwrite(scanline,w * 3,1,outfile) != 1)
              {  fclose(outfile);  cadwindow->MessageBox(rs32.gets(),rs28.gets(),MB_ICONINFORMATION);
                 outfile = 0;
              }
         }
       if (stopped)
         break;
    }
  if ((imageformat == 1 || imageformat == 2) && outfile != 0)
    {  fclose(outfile);
       delete [] scanline;
    }
  if (bginfile != 0)
    fclose(bginfile);
  delete [] bgscanline;
  program->setbusy(0);
  return 1;
}
#endif

/**
 * @brief raytrace_command4
 * @param cstate
 *
 * raytracing setup and execute command
 *
 */
void raytrace_command4(int *cstate,HighLevelEvent *,void **)
{
  ResourceString rs27(NCRAY+27),rs28(NCRAY+28);
  static RayTraceImage *rti=0;
  *cstate = ExitState;
  if (cadwindow->getcurrentwindow() == NULL)
  {
       cadwindow->MessageBox(rs27.gets(),rs28.gets(),MB_ICONINFORMATION);
       return;
  }
  // KMJ: TODO: do we need to set the repaint tyle here?
  // draw the raytraced image to the screen??
  if(rti != 0)
      return;
  cadwindow->getcurrentsurface()->setrepaintstyle(RayTraced);
  rti = new RayTraceImage(cadwindow->getcurrentwindow());
  if (rti != 0)
  {
       if (rti->setup() && rti->loadpolygons())
         rti->raytrace();
       delete rti;
       rti=0;

       // KMJ: TODO look at this
       // maybe copy the scratch image to the screen?
       //  Update the offscreen image, so that the raytraced image is not overwritten
       //if (cadwindow->getcurrentwindow()->getoffscreenbitmap() != 0)
       //  BitBlt(cadwindow->getcurrentwindow()->getoffscreenbitmap()->gethdc(),0,0,(int)cadwindow->getcurrentwindow()->getwidth(),(int)cadwindow->getcurrentwindow()->getheight(),cadwindow->getcurrentwindow()->gethdc(),0,0,SRCCOPY);

  }
}

class RaytraceActive
  {private:
     int active;
     ButtonMenu *buttonmenu;
   public:
     RaytraceActive() {  active = 0;  buttonmenu = NULL;  }
     ~RaytraceActive()  {  delete buttonmenu;  }
     int getactive(void)  {  return active;  }
     void setactive(int a)  {  active = a;  }
     void setbuttonmenu(ButtonMenu *bm)  {  buttonmenu = bm;  }
     ButtonMenu *getbuttonmenu(void)  {  return buttonmenu;  }
  };

class RaytraceMenuWindow : public MenuWindow
  {private:
     ButtonMenu *buttonmenu;
     RaytraceActive *raytraceactive;
   public:
     RaytraceMenuWindow(Window *,int,int,int,int,RaytraceActive *);
     ~RaytraceMenuWindow();
     void timerevent(void) {  if (buttonmenu != 0) buttonmenu->timerevent();  };
  };

RaytraceMenuWindow::RaytraceMenuWindow(Window *win,int x,int y,int w,int h,RaytraceActive *va) :
  MenuWindow(win,x,y,w,h)
{ raytraceactive = va;
  buttonmenu = raytraceactive->getbuttonmenu();
  addcontrol(buttonmenu);
  buttonmenu->move(this,0,0,2);
  raytraceactive->setactive(1);
}

RaytraceMenuWindow::~RaytraceMenuWindow()
{ raytraceactive->setactive(0);
}

RaytraceActive raytraceactive;

void raytrace_command5(int *cstate,HighLevelEvent *,void **)
{ResourceString rs152(NCVIEWB+152),rs153(NCVIEWB+153);
  *cstate = ExitState;
  if (raytraceactive.getactive()) return;
  if (raytraceactive.getbuttonmenu() == 0)
    {ButtonMenu *menu = new ButtonMenu(0,0,14,14,15);
       if (menu != NULL)
         {  menu->add(new Button(-1,-1, (char*)"Raytrace1",537));
            menu->add(new Button(-1,-1, (char*)"Raytrace2",538));
            menu->add(new Button(-1,-1, (char*)"Raytrace3",539));
            menu->add(new Button(-1,-1, (char*)"Raytrace4",540));
            raytraceactive.setbuttonmenu(menu);
         }
    }
  if (raytraceactive.getbuttonmenu() == 0)
    {  cadwindow->MessageBox("Menu could not be loaded.","Raytrace Menu",MB_ICONSTOP);
       return;
    }
  new RaytraceMenuWindow(cadwindow->getsubwindow(),
                         int(cadwindow->getsubwindow()->getwidth())-60,90,24*2,24*2,&raytraceactive);
  *cstate = ExitState;
}

#endif
/*
class MaterialTextureDisplayDialogControl : public DisplayDialogControl
  {private:
     char lastdirectoryname[300];
     char **filenamelist;
     int exists,lastbase,*base;
     //View3dOffScreenBitmap *output;
   public:
     //MaterialTextureDisplayDialogControl(int id,int *b,View3dOffScreenBitmap *op) : DisplayDialogControl(id) {   strcpy(lastdirectoryname,"InvalidDirectoryName");  filenamelist = 0;  exists = 0;   base = b;  lastbase = -1; output = op;  }
     MaterialTextureDisplayDialogControl(int id,int *b) : DisplayDialogControl(id) {   strcpy(lastdirectoryname,"InvalidDirectoryName");  filenamelist = 0;  exists = 0;   base = b;  lastbase = -1;   }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/
class MTScrollBarDialogControl : public ScrollBarDialogControl
  {private:
     int *base,n;
   public:
     MTScrollBarDialogControl(int i,int *b) : ScrollBarDialogControl(i,0) {  base = b; }
     void vscrollevent(Dialog *,int,int,int);
     void setrange(Dialog *,int);
     void load(Dialog *dialog);
  };

class MTListDialogControl : public ListDialogControl
  {private:
   public:
     MTListDialogControl(int id,RCCHAR *material) :
         ListDialogControl(id,0,0,material,300)  {    }
     void load(Dialog *);
  };

void MTListDialogControl::load(Dialog *dialog)
{RCCHAR directory[300];
 int i;
 long handle;
 struct RC_finddata_t fd;

  removeallitems(dialog);

  strcpy(directory,home.getpublicfilename("Textures"));

  QString qdirectory(directory);
  QDir dir(qdirectory);
  dir.setFilter(QDir::Dirs | QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

  QFileInfoList list = dir.entryInfoList();
  for (int i = 0; i < list.size(); ++i)
  {
      QFileInfo fileInfo = list.at(i);
      SendDlgItemMessage(dialog->gethdlg(),id,CB_ADDSTRING,0,(RCLPARAM)fileInfo.fileName().data());
  }
  dialog->SetDlgItemText(id,v.getstring("mt::subdirectory"));
#if 0
  RCfindfilter filter;
  filter.mfilter = QDir::AllDirs | QDir::NoDotAndDotDot;
  fd.filter =  filter;

  handle = _findfirst(directory,&fd);
  i = 0;
  while (handle != -1)
    {  if ((fd.attrib & _A_SUBDIR) && fd.name[0] != '.')
         SendDlgItemMessage(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)fd.name);
       if (_findnext(handle,&fd) != 0)
         {  _findclose(handle);
             handle = -1;
         } 
    }
  dialog->SetDlgItemText(id,v.getstring("mt::subdirectory"));
#endif
}

class MTButtonDialogControl : public ButtonDialogControl
  {private:
     int *base;
   public:
     MTButtonDialogControl(int id,int *bs) : ButtonDialogControl(id)  {   base = bs;   }
     int select(Dialog *);
  };

void MTScrollBarDialogControl::vscrollevent(Dialog *dialog,int scrollid,int scrollcode,int pos)
{DisplayDialogControl *ddc;
 int start,end;
 GeneralList list;

  GetScrollRange(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,&start,&end);
  switch (scrollcode)
    {  case SB_BOTTOM : *base = n - 3;  break;
       case SB_ENDSCROLL : break;
       case SB_LINEDOWN : (*base)++;  break;
       case SB_LINEUP : (*base)--;  break;
       case SB_PAGEDOWN : (*base) += 4;  break;
       case SB_PAGEUP : (*base) -= 4;  break;
       case SB_THUMBPOSITION :
       case SB_THUMBTRACK :
         *base = pos;
         break;
    }
  if (*base > n) *base = n;
  if (*base < 0) *base = 0;
  SetScrollPos(GetDlgItem(dialog->gethdlg(),scrollid),SB_CTL,*base,TRUE);

  list = dialog->getdisplaylist();
  for (list.start() ; (ddc = (DisplayDialogControl *)list.next()) != NULL ; )
      ddc->draw(dialog,0);
    //InvalidateRect(GetDlgItem(dialog->gethdlg(),ddc->getid()),0,TRUE);

}

void MTScrollBarDialogControl::load(Dialog *dialog)
{ n = 20;
  SetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,0,12,TRUE);
}

void MTScrollBarDialogControl::setrange(Dialog *dialog,int n1)
{ n = (n1-1) / 4;
  SetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,0,n,TRUE);
}


int MTButtonDialogControl::select(Dialog *dialog)
{RCCHAR filename[300];
 FILE *outfile,*infile;
 double width,height,angle,transparency;
 int flipx,flipy;
 
   if (id >= 110 && id < 122)
     {  GetDlgItemText(dialog->gethdlg(),id+200,filename,300);
        SetDlgItemText(dialog->gethdlg(),100,filename);
        ((StringDialogControl *)dialog->getcontrol(100))->change(dialog,filename);

        if (strrchr(filename,'.') != 0)
          strcpy(strrchr(filename,'.'),".rct");
        else
          strcat(filename,".rct");
        width = 200.0;  height = 200.0;  angle = 0.0;
        flipx = flipy = 0;
        transparency = 0.0;
        if ((infile = fopen(filename,"r")) != 0)
          {  fscanf(infile,"%lf %lf %lf %d %d %lf\n",&width,&height,&angle,&flipx,&flipy,&transparency);
             fclose(infile);
          } 

        ((RealDialogControl *)dialog->getcontrol(104))->change(dialog,width);
        ((RealDialogControl *)dialog->getcontrol(105))->change(dialog,height);
        //((RealDialogControl *)dialog->getcontrol(106))->currentvalue(dialog->gethdlg(),&angle);
        //((CheckBoxDialogControl *)dialog->getcontrol(107))->currentvalue(dialog->gethdlg(),&flipx);
        //((CheckBoxDialogControl *)dialog->getcontrol(107))->currentvalue(dialog->gethdlg(),&flipy);
        ((RealDialogControl *)dialog->getcontrol(108))->change(dialog,transparency);

        ((DisplayDialogControl *)dialog->getcontrol(109))->draw(dialog,0);
        //InvalidateRect(GetDlgItem(dialog->gethdlg(),109),0,TRUE);
     }
   else if (id == 124)
     {  if (cadwindow->MessageBox("Set the default values for the current texture map to the current values?","Set Default Texture Settings",MB_YESNO) == IDYES)
          {  angle = 0.0;
             flipx = flipy = 0;
             ((StringDialogControl *)dialog->getcontrol(100))->currentvalue(dialog,filename,300);
             ((RealDialogControl *)dialog->getcontrol(104))->currentvalue(dialog,&width);
             ((RealDialogControl *)dialog->getcontrol(105))->currentvalue(dialog,&height);
             //((RealDialogControl *)dialog->getcontrol(106))->currentvalue(dialog->gethdlg(),&angle);
             //((CheckBoxDialogControl *)dialog->getcontrol(107))->currentvalue(dialog->gethdlg(),&flipx);
             //((CheckBoxDialogControl *)dialog->getcontrol(107))->currentvalue(dialog->gethdlg(),&flipy);
             ((RealDialogControl *)dialog->getcontrol(108))->currentvalue(dialog,&transparency);
             if (strrchr(filename,'.') != 0)
               strcpy(strrchr(filename,'.'),".rct");
             else
               strcat(filename,".rct");
             if ((outfile = fopen(filename,"w")) != 0)
               {  fprintf(outfile,"%lf %lf %lf %d %d %lf\n",width,height,angle,flipx,flipy,transparency);
                  fclose(outfile);
               } 

          }   

     }

  return 0;
}

void MaterialTextureDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
    View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
    View3dOutput output(hWnd,hWnd,&top,1.0);

 RECT rect;
 RCHPEN hpen,oldhpen;
 EntityHeader header(0,0,0,0);
 BitMask options(32);
 FILE *infile;
 RCCHAR bitmapfilename[300],directoryname[300],directory[300];
 long handle;
 struct RC_finddata_t fd;
 int i;
 GeneralList list;
 DisplayDialogControl *ddc;
 
 if(hWnd == 0)
    return;

 QGraphicsView *ctl = 0;
 if(dialog->gethdlg())
     ctl = (QGraphicsView*)this->getQW();
 QGraphicsScene *scene = 0;
 if(ctl)
     scene = ctl->scene();

#if 0
  GetClientRect(hWnd,&rect);
  output->sizeevent(rect.right - rect.left,rect.bottom - rect.top);
  output->zoom(-10.0,-10.0,10.0,10.0);
#endif

  if(ctl && scene)
  //if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
    {
       //oldhpen = (RCHPEN)SelectObject(output->gethdc(),hpen);
       //SelectClipRgn(output->gethdc(),0);
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

      if (dialog->currentvalue(100,bitmapfilename,300))
         {  if (strlen(bitmapfilename) > 0)
              {  infile = fopen(bitmapfilename,"r");
                 if (infile != 0)
                   {  fclose(infile);
                      exists = 1;
                   }
                 else
                   exists = 0;
              }
            if (exists)
              {  Point org(output.getumin(),output.getvmin(),0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
                 //options.set(9);
                 options.set(31);  //  Do not to invalidate the window when drawing
                 BitMapE bitmape(bitmapfilename,org,xaxis,yaxis,output.getumax()-output.getumin(),output.getvmax()-output.getvmin(),1.0,1.0,1.0,0.004,1.0,options);
                 bitmape.draw(DM_NORMAL,NULL,&output);
              }
            //else
            //  FillRect(output->gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
         }

#if 0
       output->moveto(0,0);
       output->lineto(rect.right-1,0);
       output->lineto(rect.right-1,rect.bottom-1);
       output->lineto(0,rect.bottom-1);
       output->lineto(0,0);
       SelectObject(output->gethdc(),oldhpen);
       DeleteObject(hpen);

       BitBlt(hDC,0,0,rect.right - rect.left,rect.bottom - rect.top,output->gethdc(),0,0,SRCCOPY);
#endif
    }

  //  Update the list of available textures as well
  if (dialog->currentvalue(123,directoryname,300) && strcmp(directoryname,lastdirectoryname) != 0 || lastbase != *base)
    {  if (strcmp(directoryname,lastdirectoryname) != 0)
         *base = 0;
       if (strlen(directoryname) > 0)
         {  strcpy(directory,home.getpublicfilename("Textures/"));
            strcat(directory,directoryname);
            strcat(directory,"/*.*");
         }
       else
         strcpy(directory,home.getpublicfilename("Textures/*.*"));

       handle = _findfirst(directory,&fd);
       for (i = 310 ; i < 322 ; i++)
         dialog->SetDlgItemText(i, (char*)"");
       i = 0;
       while (handle != -1)
         {  if (strlen(fd.name) > 4 && (_stricmp(fd.name+strlen(fd.name)-4,".bmp") == 0 || _stricmp(fd.name+strlen(fd.name)-4,".jpg") == 0 ||
                                        _stricmp(fd.name+strlen(fd.name)-4,".png") == 0))
              {  strcpy(directory,home.getpublicfilename("Textures/"));
                 strcat(directory,directoryname);
                 strcat(directory,"/");
                 strcat(directory,fd.name);

                 if (i >= *base * 4 && i <= *base * 4 + 11)
                   dialog->SetDlgItemText(310+(i-*base*4),directory); 
                 i++;
              }

            if (_findnext(handle,&fd) != 0)
              {  _findclose(handle);
                 handle = -1;
              } 

         }

       ((MTScrollBarDialogControl *)dialog->getcontrol(122))->setrange(dialog,i);
       strcpy(lastdirectoryname,directoryname);
       lastbase = *base;

       list = dialog->getdisplaylist();
       for (list.start() ; (ddc = (DisplayDialogControl *)list.next()) != NULL ; )
         InvalidateRect(GetDlgItem(dialog->gethdlg(),ddc->getid()),0,TRUE);

    }

}

class MaterialButtonDialogControl : public ButtonDialogControl
  {private:
   public:
     MaterialButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int MaterialButtonDialogControl::select(Dialog *dialog)
{int i;
 OPENFILENAME ofn;
 RCCHAR szDirName[256],szFile[256],szFileTitle[256],szFilter[256];
 
  //_getcwd(szDirName, sizeof(szDirName));
  //strcpy(szDirName,home.getpublicfilename("Textures"));
  strcpy(szDirName,QDir::toNativeSeparators(QString(v.getstring("db::textures-path"))).data());
  szFile[0] = '\0';
  _tcscpy(szFilter,"Bitmap files (*.bmp);;JPEG files (*.jpg);;PNG files (*.png)");

  //for (i = 0; szFilter[i] != '\0'; i++)
  //  if (szFilter[i] == '|') szFilter[i] = '\0';

  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = szFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = sizeof(szFileTitle);
  ofn.lpstrInitialDir = szDirName;
  ofn.lpstrDefExt = _RCT("BMP");
  ofn.lpstrTitle = _RCT("Select the texture map file");
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  if (GetOpenFileName(&ofn))
    {  ((StringDialogControl *)dialog->getcontrol(100))->change(dialog,ofn.lpstrFile);
        ((DisplayDialogControl*)dialog->getcontrol(109))->draw(dialog,0);
        //InvalidateRect(GetDlgItem(dialog->gethdlg(),109),0,TRUE);
    }
  return 0;
}
/*
 * now defined in dialog.h
class MTDisplayDialogControl : public DisplayDialogControl
  {private:
     int *base;
     char lastdirectory[300];
     View3dOffScreenBitmap *output;
   public:
     MTDisplayDialogControl(int id,int *bs,View3dOffScreenBitmap *op) : DisplayDialogControl(id) {  base = bs;  strcpy(lastdirectory,"");  output = op;  }
     void draw(Dialog *dialog,RCHDC hDC);
  };
*/
void MTDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{
 View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,1E10,-1E10);
 View3dOutput output(hWnd,hWnd,&top,1.0);

 RECT rect;
 RCHPEN hpen,oldhpen;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale,rotation,pmin,pmax;
 BitMask options(32);
 RCCHAR filename[300];
 int exists;
 FILE *infile;

 if(hWnd == 0)
    return;

 QGraphicsView *ctl = 0;
 if(dialog->gethdlg())
     ctl = (QGraphicsView*)this->getQW();
 QGraphicsScene *scene = 0;
 if(ctl)
     scene = ctl->scene();

#if 0
  GetClientRect(hWnd,&rect);
  output->sizeevent(rect.right - rect.left,rect.bottom - rect.top);
  output->zoom(-10.0,-10.0,10.0,10.0);
#endif

  if(ctl && scene)
  //if ((hpen  = CreatePen(PS_SOLID,1,RGB(0,0,0))) != NULL)
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

      GetDlgItemText(dialog->gethdlg(),id + 200,filename,300);
       exists = 0;
       if (strlen(filename) > 0)
         {  infile = fopen(filename,"r");
            if (infile != 0)
              {  fclose(infile);
                 exists = 1;
              }
         }
       if (exists)
         {  BitMask bo(32);
            //bo.set(9);
            bo.set(31);  //  Do not to invalidate the window when drawing
            BitMapE bitmap(filename,Point(output.getumin(),output.getvmin(),0.0),
                                    Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),
                                     output.getumax()-output.getumin(),output.getvmax()-output.getvmin(),0.0,0.0,0.0,0.0,1.0,bo);
            bitmap.draw(DM_NORMAL,NULL,&output);

         }
#if 0
       else
         FillRect(output->gethdc(),&rect,(RCHBRUSH)GetStockObject(WHITE_BRUSH));
       oldhpen = (RCHPEN) SelectObject(output->gethdc(),hpen);
       output->moveto(0,0);
       output->lineto(rect.right-1,0);
       output->lineto(rect.right-1,rect.bottom-1);
       output->lineto(0,rect.bottom-1);
       output->lineto(0,0);
       SelectObject(output->gethdc(),oldhpen);
       DeleteObject(hpen);


       BitBlt(hDC,0,0,rect.right - rect.left,rect.bottom - rect.top,output->gethdc(),0,0,SRCCOPY);
#endif
    }

}

extern Entity *panel_get_face(Entity *e);

void material_command(int *cstate,HighLevelEvent *,void **)
{View top(_RCT("Top"),0,0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,-100.0,100.0);
 Dialog dialog("Material_Dialog");
 double xoffset,yoffset,width,height,lastwidth,lastheight,angle,transparency,ambient,diffuse;
 int i,j,flip;
 RCCHAR filename[300],material[300];
 Entity *e;
 SurfaceProperties sp;
 DialogControl *dc;
 EntityList list;
 BitMask change(32);
 double u,v,umin=0.0,vmin=0.0,umax=1.0,vmax=1.0;
 Point **verts;
 int nloops,*nvert,base;


  View3dOutput vtemp(cadwindow->gethwnd(),cadwindow->gethdc(),&top,1.0);
  //View3dOffScreenBitmap output(&vtemp,200,200);


  list = state.getselection();
  list.start();
  if ((e = list.next()) != 0)
    {  if (! sp.copyinfo(e,0,0,0))
         sp.getdefaults();


       if (e->isa(group_entity))
         e = panel_get_face(e);

       if (e == 0)
         {  *cstate = ExitState;
            return;
         }

       //  Get the extents of the entity
       if (e->isa(ruledsurface_entity))
         {  umin = 0.0;  umax = ((RuledSurface *)e)->gete1()->length();
            vmin = 0.0;  vmax = (((RuledSurface *)e)->gete1()->position(0.0) - ((RuledSurface *)e)->gete2()->position(0.0)).length();
         }
       else if (e->isa(surfaceofrev_entity))
         {  umin = 0.0;  umax = ((SurfaceOfRev *)e)->gete()->length();
            vmin = 0.0;  vmax = ((SurfaceOfRev *)e)->gete()->length();
         }
       else if(e->isa(patch_entity))
       {
           // KMJ: TODO think about getting the model space extents
           umin = 0.0;  umax = 1.0;
           vmin = 0.0;  vmax = 1.0;
       }
       else if (e->isa(plane_entity))
         {  Polygon3d polygon3d(((Plane *)e)->getorigin(),((Plane *)e)->getxaxis(),((Plane *)e)->getyaxis(),((Plane *)e)->getlist(),0,1.0);

            nloops = polygon3d.getnloops();
            nvert = polygon3d.getnvert();
            verts = polygon3d.getverts();

            if (nloops > 0 && nvert[0] > 0)
              {  

                 for (i = 0 ; i < nloops ; i++)
                   for (j = 0 ; j < nvert[i] ; j++)
                     {  u = (polygon3d.getxaxis().dot(verts[i][j] - polygon3d.getorigin()));
                        v = (polygon3d.getyaxis().dot(verts[i][j] - polygon3d.getorigin()));
 

                        if (i == 0 && j == 0)
                          {  umin = umax = u;
                             vmin = vmax = v;
                          }
                        else
                          {  if (u < umin) umin = u; 
                             if (v < vmin) vmin = v; 
                             if (u > umax) umax = u; 
                             if (v > vmax) vmax = v; 
                          }

                     }
 
              }
         }

       strcpy(filename,sp.tmi.filename);
       transparency = sp.transparency;

       if (sp.tmi.texturetype == 1 && sp.tmi.x1 == sp.tmi.x2 && sp.tmi.y1 == sp.tmi.y2 && sp.tmi.x1 < 0 && sp.tmi.y1 < 0.0)
         {  xoffset = sp.tmi.xaxis.z;
            yoffset = sp.tmi.yaxis.z;
            width = (umax - umin) * sp.tmi.xaxis.x + (vmax - vmin) * sp.tmi.xaxis.y;
            height = (umax - umin) * sp.tmi.yaxis.x + (vmax - vmin) * sp.tmi.yaxis.y;
            angle = 0.0;     
            flip = 0;
         }
       else
         {  xoffset = sp.tmi.x1 * (umax - umin);
            yoffset = sp.tmi.y1 * (vmax - vmin);
            width = (umax - umin) / (sp.tmi.x2 - sp.tmi.x1);
            height = (vmax - vmin) / (sp.tmi.y2 - sp.tmi.y1);
            angle = 0.0;     
            flip = 0;
         }

       lastwidth = width;
       lastheight = height;

       base = 0;

       dialog.add(new StringDialogControl(100,filename,256));
       dialog.add(new MaterialButtonDialogControl(101));
       dialog.add(dc = new RealDialogControl(102,&xoffset,1.0,ScaleByLength));
       dialog.add(dc = new RealDialogControl(103,&yoffset,1.0,ScaleByLength));
       dialog.add(dc = new RealDialogControl(104,&width,1.0,ScaleByLength));
       dialog.add(dc = new RealDialogControl(105,&height,1.0,ScaleByLength));
       //dialog.add(dc = new RealDialogControl(106,&angle,0.1));
       //dialog.add(new CheckBoxDialogControl(107,&flip));
       dialog.add(dc = new RealDialogControl(108,&transparency,0.1));

       dialog.add(new MTScrollBarDialogControl(122,&base));

       for (i = 110 ; i <= 121 ; i++)
       {
            dialog.add(new MTDisplayDialogControl(i,&base));
            dialog.add(dc = new MTButtonDialogControl(i,&base));
       }

       dialog.add(new MTListDialogControl(123,material));
       dialog.add(new MTButtonDialogControl(124,&base));

       dialog.add((DisplayDialogControl *)(dc = new MaterialTextureDisplayDialogControl(109,&base)));

       if (dialog.process() == TRUE)
         {
            sp.transparency = transparency;
            strcpy(sp.tmi.filename,filename);
            sp.tmi.texturetype = strlen(filename) > 0 ? 1 : 0;

            if (sp.tmi.texturetype == 1 && sp.tmi.x1 == sp.tmi.x2 && sp.tmi.y1 == sp.tmi.y2 && sp.tmi.x1 < 0 && sp.tmi.y1 < 0.0)
              {  sp.tmi.xaxis.z = xoffset;
                 sp.tmi.yaxis.z = yoffset;
                 sp.tmi.xaxis.x *= height / lastheight;
                 sp.tmi.xaxis.y *= height / lastheight;
                 sp.tmi.yaxis.x *= width / lastwidth;
                 sp.tmi.yaxis.y *= width / lastwidth;
              }
            else
              {  sp.tmi.x1 = xoffset / (umax - umin);
                 sp.tmi.y1 = yoffset / (vmax - vmin);
                 sp.tmi.x2 = (umax - umin) / width + sp.tmi.x1;
                 sp.tmi.y2 = (vmax - vmin) / height + sp.tmi.y1;

              }

            sp.tmi.options.set(0,flip);
            sp.chang.set(1,1);
            sp.chang.set(16,1);
            sp.chang.set(17,1);
            sp.chang.set(18,1);
            sp.chang.set(19,1);
            sp.chang.set(20,1);
            sp.change(e,0);
            ((SurfaceEntity *)e)->cleartriangles(); 

            ::v.setstring("mt::subdirectory",material);

         }

       *cstate = ExitState;

    }
}

