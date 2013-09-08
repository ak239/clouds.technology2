// fundamental constant
uniform float kref;
uniform float gammaref;
uniform float zref;
uniform float g;

// sun direction
uniform float thetaSun;
uniform float phiSun;

// camera and cloud position
uniform vec3 cameraPos;
uniform vec3 cloudPos;

// b c r
uniform vec3 sunConstants1;
// t kc
uniform vec2 sunConstants2;

// variable for reverse transfotm to world position
uniform vec2 windowSize;
uniform mat4 invPV;

uniform vec3 width;

uniform vec3 sunColor;
uniform vec3 skyColor;
uniform vec3 groundColor;

uniform float kts[7] = float[](1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

float pi = 3.141592653589;
float scaleKt = 3.0f;
bool isEllipse = false;

vec3 CalcPosition()
{
  vec4 ndcPos;
  ndcPos.xy = ((gl_FragCoord.xy / windowSize.xy) * 2.0) - 1.0;
  ndcPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /
      (gl_DepthRange.far - gl_DepthRange.near);
  ndcPos.w = 1.0;

  vec4 clipPos = ndcPos / gl_FragCoord.w;

  return vec3(invPV * clipPos);
}

void GetT(in vec3 pos, in vec3 dir, out float d, out float t1, out float t2)
{
  mat3 M   = mat3( 1.0f / width.x, 0.0f, 0.0f, 0.0f, 1.0f / width.y, 0.0f, 0.0f, 0.0f, 1.0f / width.z);
  vec3 P1  = pos * M - cloudPos * M;
  vec3 V1  = dir * M;

  float kt1 = dot(P1, V1);
  float kt2 = dot(V1, V1);
  float kt3 = dot(P1, P1);
  
  d = (kt1 * kt1 - kt2 * (kt3 - 1.0f));
  if (d < 0.0f) return;
  
  t1 = (-2.0 * kt1 + sqrt(d)) / kt2;
  t2 = (-2.0 * kt1 - sqrt(d)) / kt2;
        
  if (t2 > t1){
    float tmp = t2;
    t2 = t1;
    t1 = tmp;
  }
}

float angle(in float theta1, in float phi1, in float theta2, in float phi2)
{
  float tmp = (sin(theta1) * cos(phi1) * sin(theta2) * cos(phi2) + sin(theta1) * sin(phi1) * sin(theta2) * sin(phi2) + cos(theta1) * cos(theta2));
  if (tmp > 1.0f)  return 0;
  if (tmp < -1.0f) return pi;
  return acos(tmp);
}

float phase(in float theta)
{
  //if (theta < 0.05f) return 2.0f; 
  if (theta < 0.0f) return 0.0f;
  float kt  = (1.0f - g * g) / 4.0f / pi;
  float tmp = pow((1.0f + g * g - 2.0f * g * cos(theta)), 1.5f);
  //if (tmp < 0.00001f) return 2.0f;
  return kt / tmp;
}

float phaseByPhase(in float theta)
{
	float delta     = 0.02f;
	float prevValue = phase(theta);

	float t         = delta;
	float sum       = 0.0f;

	while (t <= theta){
		float curValue = phase(t) * phase(theta - t);    
    
		sum += (prevValue + curValue) / 2.0f * delta;
    
		prevValue = curValue;
		t += delta;
  }

	return sum / 86.707f; // P2Normalizer
}

float dirac(in float val)
{
  float sigma = 1.0f;
	return 1.0f / sigma  / sqrt(2.0f * pi) * exp(-(val * val) / 2.0f / sigma / sigma);  
}

float phasePeak(in float Ptheta, in float theta)
{
  if (theta > 8.0f / 180.0f * pi)
    return 0.0f;
  return Ptheta - phase(8.0f / 180.0f * pi);
}

void main( void ) {
  vec3 pos = CalcPosition();
  vec3 dir = normalize(pos - cameraPos);
    
  float thetaView = acos(dir.y / length(dir));
  float phiView   = atan(dir.z, dir.x);
  float Theta     = angle(thetaSun, phiSun, thetaView, phiView);
  float P         = phase(Theta);    
  float P2        = phaseByPhase(Theta);
  float PPeak     = phasePeak(P, Theta);
  
  float lview;
  float z1;
  float lsun;
  float alpha = 1.0f;
  
  if (isEllipse){
    float d;
    float t1;
    float t2;
    
    GetT(pos, dir, d, t1, t2);
    if (d < 0.0f)//          discard;
    {
      gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
      return;
    }
    if (abs(t2) > abs(t1)) discard;
     
    lview     = (t1 - t2) * scaleKt;
    if (lview < 10.0f)
      alpha = lview / 10.0f;
      
    GetT(pos, vec3(0.0f, 1.0f, 0.0f), d, t1, t2);
    if (d < 0.0f)          discard;
    z1 = (t1 - t2) * scaleKt;
      
    vec3 sunDir = vec3(sin(thetaSun) * cos(phiSun), sin(thetaSun) * sin(phiSun), cos(thetaSun));
    GetT(pos, sunDir, d, t1, t2);
    if (d < 0.0f)          discard;
    lsun = (t1 - t2) * scaleKt;
  }
  else
  {
    //gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //return;
    //if (pos.y < cloudPos.y) discard;
    z1    = (abs(pos.y - cloudPos.y) + 0.6 * width.y) * scaleKt;
    lview = abs(z1 / cos(thetaView));
    lsun  = abs(z1 / cos(thetaSun));
  }
    
  float b  = sunConstants1.x;
  float c  = sunConstants1.y;
  float r  = sunConstants1.z;
  float t  = sunConstants2.x;
  float kc = sunConstants2.y;
  
  float F      = b + (1.0f - b) * exp(-z1 * c);
  float gamma  = F * gammaref / (z1 - (z1 - zref) * gammaref);
    
  float Mr1Inf = 1 - gamma;
  float Mt1Inf = gamma - exp(-(lsun) * kref);
  
  //gl_FragColor = vec4(vec3(Mt1Inf), 1.0f);
  //return;

  float Mt1    = t * z1 * kc * exp(-kc * z1);
  float Mr1    = 1 / 2.0f * r * (1.0f - exp(-2.0f * kc * z1));
  float Mt2    = 1 / 2.0f * (kc * t * z1) * (kc * t * z1) * exp(-2.0f * kc * z1) + 1 / 4.0f * r * r * (1 + 2 * (kc * z1 - 1) * exp(-2 * kc * z1)) * exp (-3 * kc * z1);
  float Mr2    = 1 / 2 * r * t * (1 - (2 * kc * z1 + 1) * exp(-2 * kc * z1));
  float R3     = (Mr1Inf - Mr1 - Mr2) / 2.0f / 3.1415;
  float T3     = (Mt1Inf - Mt1 - Mt2) / 2.0f / 3.1415 * 2.0f;
  
  float kt1 = 1.0f / (cos(thetaView) + cos(thetaSun)) * ( 1.0f - exp(-kref * (lview + lsun)));
  float tmp_kt2 = (cos(thetaSun) - cos(pi / 2.0f + thetaView));
  float kt2 = 1.0f / tmp_kt2 * (exp(-kref*lsun) - exp(-kref*lview));
  float R1 = P * kt1;
  float T1 = P * kt2;
  float R2 = P2 * kt1;
  float T2 = P2 * kt2;
  float T0a = dirac(thetaView - thetaSun) * dirac(abs(phiView - phiSun) - pi) * exp(-lview * kref);
  float T0b = PPeak * ( 1.0f - exp(-lview * (kref )));
  
  vec3 Lsun    = (kts[0] * T3 + kts[1] * T2 + kts[2] * T1 + kts[3] * T0a + kts[4] * T0b)* cos(thetaSun) * sunColor;
  vec3 Lsky    = kts[5] * Mt1Inf * skyColor / 2.0f / pi;
  vec3 Lground = kts[6] * Mr1Inf * groundColor / 2.0f / pi;
  
  gl_FragColor = vec4 (Lsun + Lsky + Lground, alpha);
//  gl_FragColor = vec4 ((T3 + T2 + T1 + T0a + T0b)* cos(thetaSun) * vec3(227.0f / 255.0f, 168.0f / 255.0f, 87.0f / 255.0f) * 1.0f, alpha);
}
