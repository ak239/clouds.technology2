#version 330

float pi = 3.14159265358;
float angleVals[10] = float[](0.0, 
                              10.0 / 180.0 * pi,
                              20.0 / 180.0 * pi,
                              30.0 / 180.0 * pi,
                              40.0 / 180.0 * pi,
                              50.0 / 180.0 * pi,
                              60.0 / 180.0 * pi,
                              70.0 / 180.0 * pi,
                              80.0 / 180.0 * pi,
                              90.0 / 180.0 * pi);
float kcVals[10] = float[](0.0265, 0.0262, 0.0272, 0.0294, 0.0326, 0.0379, 0.0471, 0.0616, 0.07,   0.07);
float tVals[10]  = float[](0.8389, 0.8412, 0.8334, 0.8208, 0.8010, 0.7774, 0.7506, 0.7165, 0.7149, 0.1);
float rVals[10]  = float[](0.0547, 0.0547, 0.0552, 0.0564, 0.0603, 0.0705, 0.0984, 0.17,   0.3554, 0.95);

//in vec4 outColor;
in vec2 outTex;
in vec3 outPosition;

out vec4 FragColor;

uniform sampler2D gSampler;
uniform vec3      offset;
uniform vec3      cloudWidth;

uniform vec3  view;
uniform vec3  sun;
uniform float kref;

void main()
{
  mat3 M  = mat3 (1.0 / cloudWidth.x, 0.0, 0.0, 0.0, 1.0 / cloudWidth.y, 0.0, 0.0, 0.0, 1.0 / cloudWidth.z);
  vec3 P1 = vec3(offset.x, -abs(offset.y), offset.z) * M;
  vec3 V1 = vec3(0.0, 1.0, 0.0) * M;
  
  vec4 texel = texture2D(gSampler, outTex);
  //if (texel.a < 0.5) discard;
	FragColor = texel;
}