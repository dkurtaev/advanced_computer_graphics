uniform vec3 u_corners_coords[3];
uniform vec3 u_corners_normals[3];

attribute vec2 a_bary_coeffs;

void main() {
  vec3 p1 = u_corners_coords[0];
  vec3 p2 = u_corners_coords[1];
  vec3 p3 = u_corners_coords[2];
  vec3 n1 = u_corners_normals[0];
  vec3 n2 = u_corners_normals[1];
  vec3 n3 = u_corners_normals[2];

  float kRatio = 1.0 / 3.0;
  vec3 b210 = (2.0 * p1 + p2 - dot(p2 - p1, n1) * n1) * kRatio;
  vec3 b120 = (2.0 * p2 + p1 - dot(p1 - p2, n2) * n2) * kRatio;
  vec3 b021 = (2.0 * p2 + p3 - dot(p3 - p2, n2) * n2) * kRatio;
  vec3 b012 = (2.0 * p3 + p2 - dot(p2 - p3, n3) * n3) * kRatio;
  vec3 b102 = (2.0 * p3 + p1 - dot(p1 - p3, n3) * n3) * kRatio;
  vec3 b201 = (2.0 * p1 + p3 - dot(p3 - p1, n1) * n1) * kRatio;
  vec3 E = 0.5 * (b210 + b120 + b021 + b012 + b102 + b201) * kRatio;
  vec3 b111 = E + 0.5 * (E - (p1 + p2 + p3) * kRatio);

  float u = a_bary_coeffs[0];
  float v = a_bary_coeffs[1];
  float w = 1.0 - u - v;
  vec3 b = p1 * w * w * w + p2 * u * u * u + p3 * v * v * v +
           b210 * 3.0 * w * w * u +
           b120 * 3.0 * w * u * u +
           b201 * 3.0 * w * w * v +
           b021 * 3.0 * u * u * v +
           b102 * 3.0 * w * v * v +
           b012 * 3.0 * u * v * v +
           b111 * 6.0 * w * u * v;
  vec3 n = normalize(n1 * w * w + n2 * u * u + n3 * v * v +
                     normalize(0.5 * (n1 + n2)) * w * u +
                     normalize(0.5 * (n2 + n3)) * u * v +
                     normalize(0.5 * (n3 + n1)) * w * v);

  gl_Position = gl_ModelViewProjectionMatrix * vec4(b, 1.0);
}
