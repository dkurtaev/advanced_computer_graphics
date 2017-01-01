varying vec3 v_normal;

void main() {
  float lum = (normalize(v_normal).y + 2.0f) / 3.0f;
  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0) * lum;
}
