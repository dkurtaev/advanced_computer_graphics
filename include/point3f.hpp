#ifndef INCLUDE_POINT3F_HPP_
#define INCLUDE_POINT3F_HPP_

class Point3f {
 public:
  Point3f(float x, float y, float z, bool normed = false);

  Point3f(const Point3f& p, bool normed = false);

  static float Dot(const Point3f& p1, const Point3f& p2);

  // Determinant of matrix (p1, p2, p3), where each point as column.
  static float Det(const Point3f& p1, const Point3f& p2, const Point3f& p3);

  Point3f operator -(const Point3f& p) const;

  Point3f operator +(const Point3f& p) const;

  void GetCoords(float* dst);

  Point3f& operator =(const Point3f& p);

  Point3f operator *(float val);

  float SqDistanceTo(const Point3f& p) const;

 private:
  inline void Init(float x, float y, float z, bool normed);

  float coords_[3];
};

#endif  // INCLUDE_POINT3F_HPP_
