
//
//  Implementation of the GeneralList class
//

#include "ncwin.h"
#include "nclist.h"
/*

static int count = 0;

#define NUMTRIALLOCATED 250

void *Triangle::operator new(size_t size)
{static char *lastallocated = 0;
 static int index;
 int i;
  if (lastallocated == 0 || index == NUMTRIALLOCATED)
    {  lastallocated = (char *)GlobalAlloc(GMEM_FIXED,NUMTRIALLOCATED * (size + 4));
       for (i = 0 ; i < NUMTRIALLOCATED ; i++)
         *(long *)(lastallocated + i * (size + 4)) = i;
       index = 0;
    }
  if (lastallocated == 0)
    return 0;
  index++;
  return lastallocated + (index - 1) * (size + 4) + 4;
}

void Triangle::operator delete(void *a)
{char *add,*base;
 int i;
  if (a != 0)
    {  add = (char *)a - 4;
       base = add - (*(long *)add) * (sizeof(Triangle) + 4);
       if (*(long *)add == -1)
         (void *)*(int *)0;
       *(long *)add = -1;
       for (i = 0 ; i < NUMTRIALLOCATED ; i++)
         if (*(long *)(base + i * (sizeof(Triangle) + 4)) != -1)
           break;
       if (i == NUMTRIALLOCATED)
         GlobalFree(base);
    }
}

#define NUMEDGEALLOCATED 1000

void *Edge::operator new(size_t size)
{static char *lastallocated = 0;
 static int index;
 int i;
  if (lastallocated == 0 || index == NUMEDGEALLOCATED)
    {  lastallocated = (char *)GlobalAlloc(GMEM_FIXED,NUMEDGEALLOCATED * (size + 4));
       for (i = 0 ; i < NUMEDGEALLOCATED ; i++)
         *(long *)(lastallocated + i * (size + 4)) = i;
       index = 0;
    }
  if (lastallocated == 0)
    return 0;
  index++;
  return lastallocated + (index - 1) * (size + 4) + 4;
}

void Edge::operator delete(void *a)
{char *add,*base;
 int i;
  if (a != 0)
    {  add = (char *)a - 4;
       base = add - (*(long *)add) * (sizeof(Edge) + 4);
       if (*(long *)add == -1)
         (void *)*(int *)0;
       *(long *)add = -1;
       for (i = 0 ; i < NUMEDGEALLOCATED ; i++)
         if (*(long *)(base + i * (sizeof(Edge) + 4)) != -1)
           break;
       if (i == NUMEDGEALLOCATED)
         GlobalFree(base);
    }
}

#define NUMSHADERALLOCATED 1000

void *Shader::operator new(size_t size)
{static char *lastallocated = 0;
 static int index;
 int i;
  if (lastallocated == 0 || index == NUMSHADERALLOCATED)
    {  lastallocated = (char *)GlobalAlloc(GMEM_FIXED,NUMSHADERALLOCATED * (size + 4));
       for (i = 0 ; i < NUMSHADERALLOCATED ; i++)
         *(long *)(lastallocated + i * (size + 4)) = i;
       index = 0;
    }
  if (lastallocated == 0)
    return 0;
  index++;
  return lastallocated + (index - 1) * (size + 4) + 4;
}

void Shader::operator delete(void *a)
{char *add,*base;
 int i;
  if (a != 0)
    {  add = (char *)a - 4;
       base = add - (*(long *)add) * (sizeof(Shader) + 4);
       if (*(long *)add == -1)
         (void *)*(int *)0;
       *(long *)add = -1;
       for (i = 0 ; i < NUMSHADERALLOCATED ; i++)
         if (*(long *)(base + i * (sizeof(Shader) + 4)) != -1)
           break;
       if (i == NUMSHADERALLOCATED)
         GlobalFree(base);
    }
}


/*
void *operator new(size_t size)
{char *address = (char *) malloc(size + 12);
  *(long *)address = count++;
  *(long *)(address + 4) = size;
  *(long *)(address + 8 + size) = count++;
  return address + 8;
}

void operator delete(void *a)
{ if (a != 0)
    {  if (*((long *)((char *)a-8)) + 1 != *((long *)((char*)a+*((long *)((char *)a-4)))))
         a = (void *)*(int *)0;
       *((long *)((char *)a-8)) = -1;  //  Already deleted
       free((char *)a-8);
    }
}

void checkmem(void *a)
{  if (*((long *)((char *)a-8)) + 1 != *((long *)((char*)a+*((long *)((char *)a-4)))))
     (void *)*(int *)0;
}
*/

int GeneralList::add(void *data)
{OneGeneral *og;
  if ((og = new OneGeneral(data,list,NULL)) != NULL)
    {  if (list != NULL) list->lastg = og;
       list = og;
       return 1;
    }
  else
    return 0;
}

int GeneralList::addatend(void *data)
{OneGeneral *lastog,*og;
  if (list == NULL)
    return add(data);
  else
    {  if ((og = new OneGeneral(data,NULL,NULL)) != NULL)
         {  for (lastog = list ; lastog != NULL ; lastog = lastog->nextg)
              if (lastog->nextg == NULL) break;
            lastog->nextg = og;
            og->lastg = lastog;
            return 1;
         }
       else
         return 0;
    }
}

void GeneralList::reverse(void)
{OneGeneral *og,*lastog;
  lastog = NULL;
  while (list != NULL)
    {  og = list->nextg;  list->nextg = list->lastg;  list->lastg = og;
       lastog = list;
       list = list->lastg;
    }
  list = lastog;
}

void GeneralList::del(void *data)
{OneGeneral *og,*ognext;
  for (og = list ; og != NULL ; og = ognext)
    {  ognext = og->nextg;
       if (og->data == data)
         {  if (og->lastg == NULL && og->nextg == NULL)
              list = NULL;
            else if (og->lastg == NULL)
              {  list = og->nextg;  og->nextg->lastg = NULL;
              }
            else if (og->nextg == NULL)
              {  og->lastg->nextg = NULL;
              }
            else
              {  og->lastg->nextg = og->nextg;
                 og->nextg->lastg = og->lastg;
              }
            delete og;
            break;  //  May need to be put back later,  now deletes one only instead of all *data's ?
         }
    }
}

void GeneralList::start(void)
{  position = list;
}

void GeneralList::end(void)
{OneGeneral *og;
  for (og = list,position = 0 ; og != 0 ; position = og,og = og->nextg);
}

GeneralList GeneralList::copy(void)
{GeneralList tmp;
  for (start() ;  ! atend() ; ) tmp.add(next());
  tmp.reverse();
  return tmp;
}

void *GeneralList::peek(void)
{ if (position == NULL)
    return NULL;
  else
    return position->data;
}

int GeneralList::atend(void) const
{  return position == NULL;
}

void *GeneralList::last(void)
{void *d;
  if (position == NULL)
    return NULL;
  else
    {  d = position->data;  position = position->lastg;  return d;
    }
}

int GeneralList::inlist(void *data) const
{OneGeneral *og;
  for (og = list ; og != NULL ; og = og->nextg)
    if (og->data == data)
      return 1;
  return 0;
}

int GeneralList::length(void) const
{OneGeneral *og;
 int n;
  for (og = list,n = 0 ; og != NULL ; og = og->nextg,n++);
  return n;
}

void GeneralList::destroy(void)
{
  OneGeneral *og,*last;
  for (og = list ; og != NULL ; )
    {  last = og;  og = og->nextg;
       delete last;
    }
  list = position = NULL;
}

void GeneralList::check(void)
{OneGeneral *og,*last1,*last2,*last3;
 int c;
  last1 = last2 = last3 = 0;
  for (og = list,c = 0 ; og != 0 ; og = og->nextg,c++)
    {  //checkmem(og);
       last3 = last2;
       last2 = last1;
       last1 = og;
    }
if (last1 == last2 && last2 == last3 && last3 != 0)
  {FILE * dfile = fopen("debug.log","a");
      fprintf(dfile,"%d %d %d %d\n",last1,last2,last3,c);
      fclose(dfile);
  }
}

int SortedGeneralList::add(OrderedObject *oo)
{OneGeneral *og,*lastog,*newog;
  if (oo == NULL) return 0;
  for (og = list,lastog = NULL ; og != NULL ; lastog = og,og = og->nextg)
    if (*oo < *((OrderedObject *)og->data)) break;
  if ((newog = new OneGeneral(oo,NULL,NULL)) == NULL) return 0;
  if (og == NULL && lastog == NULL)
    {  list = newog;
       newog->lastg = NULL;
       newog->nextg = NULL;
    }
  else if (og == NULL && lastog != NULL)
    {  lastog->nextg = newog;
       newog->lastg = lastog;
       newog->nextg = NULL;
    }
  else if (og != NULL && lastog == NULL)
    {  list = newog;
       og->lastg = newog;
       newog->lastg = NULL;
       newog->nextg = og;
    }
  else
    {  lastog->nextg = newog;
       og->lastg = newog;
       newog->nextg = og;
       newog->lastg = lastog;
    }
  return 1;
}

void SortedGeneralList::merge(SortedGeneralList *list1)
{OneGeneral *og1,*lastog1,*og2,*og2next;
  for (og2 = list1->list,og1 = list,lastog1 = NULL ; og2 != NULL ; og2 = og2next)
    {  for ( ; og1 != NULL ; lastog1 = og1,og1 = og1->nextg)
         if (*((OrderedObject *)og2->data) < *((OrderedObject *)og1->data)) break;
       og2next = og2->nextg;
       if (og1 == NULL && lastog1 == NULL)
         {  lastog1 = list = og2;
            og2->lastg = NULL;
            og2->nextg = NULL;
         }
       else if (og1 == NULL && lastog1 != NULL)
         {  lastog1->nextg = og2;
            og2->lastg = lastog1;
            og2->nextg = NULL;
            lastog1 = og2;
         }
       else if (og1 != NULL && lastog1 == NULL)
         {  lastog1 = list = og2;
            og1->lastg = og2;
            og2->lastg = NULL;
            og2->nextg = og1;
         }
       else
         {  lastog1->nextg = og2;
            og1->lastg = og2;
            og2->nextg = og1;
            og2->lastg = lastog1;
            lastog1 = og2;
         }
    }
  list1->list = NULL;
}

void SortedGeneralList::resort(void)
{int i,j,k,n;
 OneGeneral *og,*lastog;
 void *data;
  if (list == NULL || list->nextg == NULL) return;
  for (og = list,lastog = NULL,n = 0 ; og != NULL ; lastog = og,og = og->nextg) n++;
  for (i = 2 ; i <= n ; i++)
    {  for (j = n,k = n+1,og = lastog ; j >= i ; j--,og = og->lastg)
         if (*((OrderedObject *)og->data) < *((OrderedObject *)og->lastg->data))
           {  k = j;
              data = og->data;
              og->data = og->lastg->data;
              og->lastg->data = data;
           }
       i = k;
    }
}

void SortedDoubleList::destroy(void)
{DoubleObject *oo;
  for (start() ;  (oo = (DoubleObject *)GeneralList::next()) != NULL ; )
    delete oo;
  GeneralList::destroy();
}

int SortedDoubleList::save(CadFile *outfile)
{double d;
 short n;
  n = short(length());
  if (! outfile->write(&n)) return 0;
  for (start() ;  ! atend() ; )
    {  d = next();
       if (! outfile->write(&d)) return 0;
    }
  return 1;
}

int SortedDoubleList::load(CadFile *infile)
{double d;
 short i,n;
  if (! infile->read(&n)) return 0;
  for (i = 0 ; i < n ; i++)
    {  if (! infile->read(&d)) return 0;
       add(d);
    }
  return 1;
}

int SortedDoubleList::addunique(double x)
{int found;

  found = 0;
  for (start() ; ! atend() ; )
    {  if (fabs(x - next()) < db.getptoler())
         {  found = 1;
            break;    
         }
    }

  if (! found)
    return add(x);
  else
    return 0;
  
}

Segments Segments::copy(void)
{double x;
 Segments s;
  for (start() ;  ! atend() ; )
    {  x = next();  s.add(x);
    }
  return s;
}  

void Segments::operator|=(Segments &s2)
{double x1,x2,x3,x4,x5;
 Segments s3;
  start();  s2.start();
  if (atend())
    {  *this = s2.copy();
       return;
    }
  else if (s2.atend())
    return;
  x1 = next();  if (atend()) return;  x2 = next();
  x3 = s2.next();  if (s2.atend()) return;  x4 = s2.next();
  if (x1 < x3) x5 = x1; else x5 = x3;
  for (;;)
    {  if (x1 < x3)
         {  if (x3 > x2 + MR_LISTTOL)
              {  s3.add(x5);  s3.add(x2);
                 if (atend())
                   {  s3.add(x3);  s3.add(x4);
                      while (! s2.atend()) s3.add(s2.next());
                      break;    
                   }
                 else
                   {  x1 = next();  if (atend()) return;  x2 = next();
                      if (x1 < x3) x5 = x1; else x5 = x3;
                   }
              }
            else if (x4 < x2 + MR_LISTTOL)
              {  if (s2.atend())
                   {  s3.add(x5);  s3.add(x2);  
                      while (! atend()) s3.add(next());
                      break;
                   }
                 else
                   {  x3 = s2.next();  if (s2.atend()) return;  x4 = s2.next();
                   }
              }
            else
               {  if (atend())
                   {  s3.add(x5);  s3.add(x4);  
                      while (! s2.atend()) s3.add(s2.next());
                      break;
                   }
                 else
                   {  x1 = next();  if (atend()) return;  x2 = next();
                   }
              }
         }
       else
         {  if (x1 > x4 + MR_LISTTOL)
              {  s3.add(x5);  s3.add(x4);
                 if (s2.atend())
                   {  s3.add(x1);  s3.add(x2);
                      while (! atend()) s3.add(next());
                      break;   
                   }
                 else
                   {  x3 = s2.next();  if (s2.atend()) return;  x4 = s2.next();
                      if (x1 < x3) x5 = x1; else x5 = x3;
                   }
              }
            else if (x2 < x4 + MR_LISTTOL)
              {  if (atend())
                   {  s3.add(x5);  s3.add(x4);  
                      while (! s2.atend()) s3.add(s2.next());
                      break;
                   }
                 else
                   {  x1 = next();  if (atend()) return;  x2 = next();
                   }
              }
            else
               {  if (s2.atend())
                   {  s3.add(x5);  s3.add(x2);  
                      while (! atend()) s3.add(next());
                      break;
                   }
                 else
                   {  x3 = next();  if (s2.atend()) return;  x4 = next();
                   }
              }
         }
    }
  destroy();
  list = s3.list;
}

void Segments::operator&=(Segments &s2)
{double x1,x2,x3,x4;
 Segments s3;
  start();  s2.start();
  if (atend() || s2.atend())
    {  destroy();
       return;
    }
  x1 = next();  if (atend()) return;  x2 = next();
  x3 = s2.next();  if (s2.atend()) return;  x4 = s2.next();
  for (;;)
    {  if (x1 < x3)
         {  if (x3 > x2 - MR_LISTTOL)
              {  if (atend())
                   break;    
                 else
                   {  x1 = next();  if (atend()) return;  x2 = next();
                   }
              }
            else if (x4 < x2 + MR_LISTTOL)
              {  s3.add(x3);  s3.add(x4);
                 if (s2.atend())
                   break;
                 else
                   {  x3 = s2.next();  if (s2.atend()) return;  x4 = s2.next();
                   }
              }
            else
              {  s3.add(x3);  s3.add(x2);
                 if (atend())
                   break;
                 else
                   {  x1 = next();  if (atend()) return;  x2 = next();
                   }
              }
         }
       else
         {  if (x1 > x4 - MR_LISTTOL)
              {  if (s2.atend())
                   break;   
                 else
                   {  x3 = s2.next();  if (s2.atend()) return;  x4 = s2.next();
                   }
              }
            else if (x2 < x4 + MR_LISTTOL)
              {  s3.add(x1);  s3.add(x2);
                 if (atend())
                   break;
                 else
                   {  x1 = next();  if (atend()) return;  x2 = next();
                   }
              }
            else
              {  s3.add(x1);  s3.add(x4);
                 if (s2.atend())
                   break;
                 else
                   {  x3 = s2.next();  if (s2.atend()) return;  x4 = s2.next();
                   }
              }
         }
    }
  destroy();
  list = s3.list;
}

void Segments::dump(const char *name)
{FILE *dfile = fopen("debug.log","a");
 int i;
 double x1,x2;
  start();
  i = 0;
  while (! atend())
    {  x1 = next();  if (atend()) break;
       x2 = next();
       fprintf(dfile,"%s[%d] %f %f\n",name,i,x1,x2);
    }
  fclose(dfile);
}

void PointList::destroy(void)
{Point *p;
  for (start() ;  (p = (Point *)GeneralList::next()) != NULL ; )
    delete p;
  GeneralList::destroy();
}

PointList PointList::copy(void)
{Point p;
 PointList l;
  for (start() ;  ! atend() ; )
    {  p = next();  l.add(p);
    }
  l.reverse();
  return l;
}  

int PointList::save(CadFile *outfile)
{Point p;
 short n;
  n = short(length());
  if (! outfile->write(&n)) return 0;
  for (start() ;  ! atend() ; )
    {  p = next();
       if (! p.save(outfile)) return 0;
    }
  return 1;
}

int PointList::load(CadFile *infile)
{Point p;
 short i,n;
  if (! infile->read(&n)) return 0;
  for (i = 0 ; i < n ; i++)
    {  if (! p.load(infile)) return 0;
       add(p);
    }
  reverse();
  return 1;
}

SortedPointList SortedPointList::copy(void)
{Point p;
 SortedPointList l;
  for (start() ;  ! atend() ; )
    {  p = next();  l.add(p);
    }
  return l;
}

void SortedPointList::destroy(void)
{PointObject *oo;
  for (start() ;  (oo = (PointObject *)GeneralList::next()) != NULL ; )
    delete oo;
  GeneralList::destroy();
}

int SortedPointList::save(CadFile *outfile)
{Point p;
 short n;
  n = short(length());
  if (! outfile->write(&n)) return 0;
  for (start() ;  ! atend() ; )
    {  p = next();
       if (! p.save(outfile)) return 0;
    }
  return 1;
}

int SortedPointList::load(CadFile *infile)
{Point p;
 short i,n;
  if (! infile->read(&n)) return 0;
  for (i = 0 ; i < n ; i++)
    {  if (! p.load(infile)) return 0;
       add(p);
    }
  return 1;
}

void SortedPointNormList::destroy(void)
{PointNormObject *oo;
  for (start() ;  (oo = (PointNormObject *)GeneralList::next()) != NULL ; )
    delete oo;
  GeneralList::destroy();
}

int StringList::add(const RCCHAR *string)
{RCCHAR *s;
 RCCHAR s1[256],s2[256];
  if (unique)
    {  //
		strcpy(s1,string);  _strupr(s1);
       for (start() ; (s = (RCCHAR *) GeneralList::next()) != 0 ; )
         {  strcpy(s2,s);  _strupr(s2);
            if (strcmp(s1,s2) == 0)
              return 1;
         }
    }
  if ((s = new RCCHAR[strlen(string)+1]) == 0)
    return 0;
  strcpy(s,string);
  return GeneralList::add(s);
}

int CStringList::add(const char *string)
{char *s;
 char s1[256],s2[256];
  if (unique)
    {  //
        strcpy(s1,string); strupr(s1);
       for (start() ; (s = (char *) GeneralList::next()) != 0 ; )
         {  strcpy(s2,s);  strupr(s2);
            if (strcpy(s1,s2) == 0)
              return 1;
         }
    }
  if ((s = new char[strlen(string)+1]) == 0)
    return 0;
  strcpy(s,string);
  return GeneralList::add(s);
}

void StringList::del(const RCCHAR *string)
{RCCHAR *s;
  for (start() ; (s = (RCCHAR *) GeneralList::next()) != 0 ; )
    if (strcmp(string,s) == 0)
      {  GeneralList::del(s);  delete [] s;
         return;
      }
}

void CStringList::del(const char *string)
{char *s;
  for (start() ; (s = (char *) GeneralList::next()) != 0 ; )
    if (strcmp(string,s) == 0)
      {  GeneralList::del(s);  delete [] s;
         return;
      }
}

void StringList::destroy(void)
{char *s;
  for (start() ;  (s = (char *)GeneralList::next()) != NULL ; )
    delete s;
  GeneralList::destroy();
}

void CStringList::destroy(void)
{char *s;
  for (start() ;  (s = (char *)GeneralList::next()) != NULL ; )
    delete s;
  GeneralList::destroy();
}

StringList StringList::copy(void)
{RCCHAR *s;
 StringList l;
  for (start() ;  (s = next()) != 0 ; )
    l.add(s);
  l.reverse();
  return l;
}

CStringList CStringList::copy(void)
{char *s;
 CStringList l;
  for (start() ;  (s = next()) != 0 ; )
    l.add(s);
  l.reverse();
  return l;
}

