
#ifndef _NCLIST_H
#define _NCLIST_H

#include "qtdefs.h"

class OneGeneral
  {friend class GeneralList;
   friend class DatabaseList;
   friend class SortedGeneralList;
   friend class EntityList;
   private:
     void *data;
     OneGeneral *nextg,*lastg;
   public:
     OneGeneral(void *d,OneGeneral *ng,OneGeneral *lg)
       {  data = d;  nextg = ng;  lastg = lg;
       }
  };

class GeneralList
{protected:
   OneGeneral *list;
   OneGeneral *position;
   void setlist(const GeneralList &oldlist)  {  list = oldlist.list;  }
 public:
   GeneralList()
     {  list = position = NULL;
     }
   int add(void *data);
   int addatend(void *data);
   void del(void *data);
   void destroy(void);
   void check(void);
   GeneralList copy(void);
   void reverse(void);
   int inlist(void *data) const;
   int empty(void) const {  return list == NULL;  }
   int length(void) const;
   void start(void);
   void end(void);
   int atend(void) const;
   inline void *next(void)
      {void *d;
         if (position == 0) return 0;
         d = position->data;  position = position->nextg;  return d;
      }
   void *peek(void);
   void *last(void);
};

class OrderedObject
  {public:
     virtual int operator<(OrderedObject&) = 0;
  };

class SortedGeneralList : public GeneralList
  {public:
     int add(OrderedObject *);
     void merge(SortedGeneralList *);
     void resort(void);
  };

class DoubleObject : public OrderedObject
  {public:
     double x;
     DoubleObject(double y)  {  x = y;  }
     int operator<(OrderedObject &oo)
       {  return x < ((DoubleObject *) &oo)->x;
       } 
  };

class SortedDoubleList : public SortedGeneralList
  {public:
     int add(double x)  {  return SortedGeneralList::add(new DoubleObject(x));  }
     int addunique(double x);
     SortedDoubleList copy(void)  {SortedDoubleList l;  l.setlist(GeneralList::copy());  return l;  }
     double next()  {  return ((DoubleObject *)GeneralList::next())->x;  }
     double last()  {  return ((DoubleObject *)GeneralList::last())->x;  }
     void destroy(void);
     int save(CadFile *);
     int load(CadFile *);
  };

#define MR_LISTTOL 0.0001

class Segments : public SortedDoubleList
  {public:
     void operator|=(Segments &);
     void operator&=(Segments &);
     void dump(const char *);
     Segments copy(void);
  };

class PointObject : public OrderedObject
  {public:
     Point p;
     PointObject(Point p1)  {  p = p1;  }
     int operator<(OrderedObject &oo)
       {  return p.x < ((PointObject *) &oo)->p.x;
       } 
  };

class PointNormObject : public OrderedObject
  {public:
     Point p;
     PointNormObject(Point p1) {  p = p1;  }
     int operator<(OrderedObject &oo)
       {  return fabs(p.x) + fabs(p.y) + fabs(p.z) <
                    fabs(((PointNormObject *) &oo)->p.x) +
                    fabs(((PointNormObject *) &oo)->p.y) +
                    fabs(((PointNormObject *) &oo)->p.z);
       }
  };

class PointList : public GeneralList
  {public:
     int add(Point p)  {  return GeneralList::add(new Point(p));  }
     void del(Point p);
     Point next()  {  return *((Point *) GeneralList::next());  }
     Point last()  {  return *((Point *) GeneralList::last());  }
     void destroy(void);
     PointList copy(void);
     int save(CadFile *);
     int load(CadFile *);
  };

class SortedPointList : public SortedGeneralList
  {public:
     int add(Point p)  {  return SortedGeneralList::add(new PointObject(p));  }
     void del(Point p);
     Point next()  {  return ((PointObject *)GeneralList::next())->p;  }
     Point last()  {  return ((PointObject *)GeneralList::last())->p;  }
     void destroy(void);
     SortedPointList copy(void);
     int save(CadFile *);
     int load(CadFile *);
  };

class SortedPointNormList : public SortedGeneralList
  {public:
     int add(Point p)  {  return SortedGeneralList::add(new PointNormObject(p));  }
     void del(Point p);
     Point next()  {  return ((PointNormObject *)GeneralList::next())->p;  }
     Point last()  {  return ((PointNormObject *)GeneralList::last())->p;  }
     void destroy(void);
     SortedPointNormList copy(void);
  };

class StringList : public GeneralList
  {private:
    int unique;
   public:
    StringList() {  unique = 1;  }
    StringList(int u)  {  unique = u;  }
    int add(const RCCHAR *);
    void del(const RCCHAR *);
    RCCHAR *next() {  return (RCCHAR *) GeneralList::next();   }
    void destroy(void);
    StringList copy(void);
  };

class CStringList : public GeneralList
  {private:
    int unique;
   public:
    CStringList() {  unique = 1;  }
    CStringList(int u)  {  unique = u;  }
    int add(const char*);
    void del(const char*);
    char *next() {  return (char *) GeneralList::next();   }
    void destroy(void);
    CStringList copy(void);
  };

#endif
