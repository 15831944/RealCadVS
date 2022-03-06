//
//  Declaration of the Matrix class
//

#if ! defined(__Matrix)
#define __Matrix

class Point;
class CadFile;

class Vector
  { public:
      const int n;
      int defined;
      double *elements;
      Vector(int);
      Vector(Vector&);
      ~Vector();
      Vector& null(void);
      double operator* (const Vector&);
      Vector& operator=(const Vector&);
      void operator*=(const double);
      void operator/=(const double);
      void operator+=(const Vector&);
      void operator-=(const Vector&);
      double& operator()(const int i)
        {  return elements[i];
        }
      void dump(const char *name);
  };

class Matrix
  { public:
      const int n;
      int defined;
      double *elements;
      Matrix(int);
      Matrix(Matrix&);
      ~Matrix();
      Matrix& identity(void);
      Matrix& null(void);
      Matrix& operator=(const Matrix&);
      Matrix& transpose(void);
      Matrix& inverse(void);
      double det(void);
      void operator*=(const Matrix&);
      void operator*=(const double);
      void operator/=(const double);
      void operator+=(const Matrix&);
      void operator-=(const Matrix&);
      double& operator()(const int i,const int j)
        {  return elements[i*n+j];
        }
      int solve(Vector *x,Vector *b,int pivot = 0);
      int solve(Point *x1,Point *b1);
      void dump(const char *name);
      int save(CadFile *);
      int load(CadFile *);
      // KMJ
      void debug();
  };

class Transform
  {public:
     Matrix mat;
     Transform() : mat(4) {}
     Transform& identity(void);
     Transform& apply(const Transform&);
     void copy(double *,double *,double *,double *,double *,double *,double *,double *,double *,double *,double *,double *);
     Transform& tocoordinatesystem(Point,Point,Point);
     Transform& fromcoordinatesystem(Point,Point,Point);
     Transform& rotate(Point,Point,double);
     Transform& rotate(Point,Point,Point);
     Transform& rotatex(double);
     Transform& rotatey(double);
     Transform& rotatez(double);
     Transform& translate(Point);
     Transform& translate(double,double,double);
     Transform& scale(Point,Point,Point,Point,Point);
     Transform& scale(double,double,double);
     Transform& shearz(double,double);
     Transform& mirror(Point,Point);
     Transform& operator=(const Transform&);
     Point transform(Point);
     void dump(const char *name);
     int save(CadFile *);
     int load(CadFile *);
     // KMJ
     void debug();
     Transform& operator*(const Transform&);
     void copy(const Matrix&);
     double& operator()(const int i,const int j)
     {
         return mat(i,j);
     }
     Point translation();
     Transform transpose(void);
     Transform inverse(void);
     double det(void);
     static void DecomposeTransformationMatrix(const Transform& matrix, Point& translation, Point& rotation, Point& scale);
     static void extractEulerAngleXYZ(const Transform& matrix, double& rotXangle, double& rotYangle, double& rotZangle);
  };

#endif
