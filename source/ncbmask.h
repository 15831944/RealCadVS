
#ifndef _BITMASK_HEADER
#define _BITMASK_HEADER

#ifdef _USING_QT
#include "qtdefs.h"
#endif

class BitMask
  {private:
     static int nbitsperlong;
     union {RCLONG *pbits;
            RCLONG   bits;
           };
     short nbits,direct;
   public:
     BitMask() { direct = 1; bits = 0; pbits = 0;}
     BitMask(int nbits);
     BitMask(const BitMask &);
     ~BitMask();
     BitMask& operator=(const BitMask &);
     void operator&=(const BitMask &);
     void operator|=(const BitMask &);
     int getdefined(void) const {  return direct || pbits != NULL;  }
     void clearandset(const int bit,...);
     void setandclear(const int bit,...);
     void set(const int bit);
     void set(const int bit,const int value);
     void clear(const int bit);
     void toggle(const int bit);
     int test(const int bit) const;
     void setall(void);
     void clearall(void);
     int save(CadFile *) const;
     char *saveascii(char *buffer) const;
     void loadascii(char *value);
     int getnbitsset(void);
     static BitMask *load(CadFile *);
     void save(char *);
     void load(char *);
#ifdef USING_WIDGETS
     void debug() {
         if(direct)
             printf("%ld\n",bits);
         else
             printf("%ld\n",*pbits);
     }
     bool operator==(const BitMask &other){
         if (direct)
             return bits & other.bits != 0;
         else
             return *pbits & *other.pbits != 0;
     }
#endif
  };

class AttributeMasks
  {public:
     BitMask entity,colour,layer,style,weight;
     AttributeMasks() : entity(32),colour(256),layer(MAX_LAYERS),style(256),weight(256) {}
     AttributeMasks &operator=(const AttributeMasks &am)
       {  entity = am.entity;
          colour = am.colour;
          layer = am.layer;
          style = am.style;
          weight = am.weight;
          return *this;
       }
  };

#endif
