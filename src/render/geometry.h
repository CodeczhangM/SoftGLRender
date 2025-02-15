/*
 * SoftGLRender
 * @author 	: keith@robot9.me
 *
 */

#pragma once

#include "bounding_box.h"

namespace SoftGL {

class Plane {
 public:
  enum PlaneIntersects {
    Intersects_Cross = 0,
    Intersects_Tangent = 1,
    Intersects_Front = 2,
    Intersects_Back = 3
  };

  void Set(const glm::vec3 &n, const glm::vec3 &pt) {
    normal_ = glm::normalize(n);
    D_ = -(glm::dot(normal_, pt));
  }

  float Distance(const glm::vec3 &pt) const {
    return glm::dot(normal_, pt) + D_;
  }

  inline const glm::vec3 &GetNormal() const {
    return normal_;
  }

  Plane::PlaneIntersects Intersects(const BoundingBox &box) const;

  // check intersect with point (world space)
  Plane::PlaneIntersects Intersects(const glm::vec3 &p0) const;

  // check intersect with line segment (world space)
  Plane::PlaneIntersects Intersects(const glm::vec3 &p0, const glm::vec3 &p1) const;

  // check intersect with triangle (world space)
  Plane::PlaneIntersects Intersects(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2) const;

 private:
  glm::vec3 normal_;
  float D_ = 0;
};

struct Frustum {
 public:
  bool Intersects(const BoundingBox &box) const;

  // check intersect with point (world space)
  bool Intersects(const glm::vec3 &p0) const;

  // check intersect with line segment (world space)
  bool Intersects(const glm::vec3 &p0, const glm::vec3 &p1) const;

  // check intersect with triangle (world space)
  bool Intersects(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2) const;

 public:
  /**
   * planes[0]: near;
   * planes[1]: far;
   * planes[2]: top;
   * planes[3]: bottom;
   * planes[4]: left;
   * planes[5]: right;
   */
  Plane planes[6];

  /**
   * corners[0]: nearTopLeft;
   * corners[1]: nearTopRight;
   * corners[2]: nearBottomLeft;
   * corners[3]: nearBottomRight;
   * corners[4]: farTopLeft;
   * corners[5]: farTopRight;
   * corners[6]: farBottomLeft;
   * corners[7]: farBottomRight;
   */
  glm::vec3 corners[8];

  BoundingBox bbox;
};

enum FrustumClipMask {
  POSITIVE_X = 1 << 0,
  NEGATIVE_X = 1 << 1,
  POSITIVE_Y = 1 << 2,
  NEGATIVE_Y = 1 << 3,
  POSITIVE_Z = 1 << 4,
  NEGATIVE_Z = 1 << 5,
};

const int FrustumClipMaskArray[6] = {
    FrustumClipMask::POSITIVE_X,
    FrustumClipMask::NEGATIVE_X,
    FrustumClipMask::POSITIVE_Y,
    FrustumClipMask::NEGATIVE_Y,
    FrustumClipMask::POSITIVE_Z,
    FrustumClipMask::NEGATIVE_Z,
};

const glm::vec4 FrustumClipPlane[6] = {
    {-1, 0, 0, 1},
    {1, 0, 0, 1},
    {0, -1, 0, 1},
    {0, 1, 0, 1},
    {0, 0, -1, 1},
    {0, 0, 1, 1}
};

}
