#include "stdafx.h"
#include "CloudFuncs.h"

GLfloat CloudFuncs::kref         = 0.0242f;
GLfloat CloudFuncs::gammaRef     = 0.9961f;
GLfloat CloudFuncs::zref         = 1.0f;
GLfloat CloudFuncs::diracSigma   = 0.01f;
GLfloat CloudFuncs::g            = 0.99f;
GLfloat CloudFuncs::thetaP       = 8.0f * glm::pi<GLfloat>() / 180.0f;
GLfloat CloudFuncs::P2Normalizer = 28.4f;

GLfloat CloudFuncs::angle(GLfloat theta1, GLfloat phi1, GLfloat theta2, GLfloat phi2)
{
	return glm::acos(glm::sin(theta1) * glm::cos(phi1) * glm::sin(theta2) * glm::cos(phi2) +
		glm::sin(theta1) * glm::sin(phi1) * glm::sin(theta2) * glm::sin(phi2) +
		glm::cos(theta1) * glm::cos(theta2));
}

GLfloat CloudFuncs::phase(GLfloat theta)
{
	const GLfloat kt = (1.0f - g * g) / 4.0f / glm::pi<GLfloat>();
	return kt / glm::pow((1.0f + g * g - 2.0f * g * glm::cos(theta)), 1.5f);
}

GLfloat CloudFuncs::phaseWave(GLfloat theta)
{
	if (theta < thetaP)
		return phase(thetaP);
	return phase(theta);
}

GLfloat CloudFuncs::phasePeak(GLfloat theta)
{
	return phase(theta) - phaseWave(theta);
}

GLfloat CloudFuncs::phaseByPhase(GLfloat theta)
{
	GLfloat delta     = 0.01f;
	GLfloat prevValue = phase(theta);

	GLfloat t         = delta;
	GLfloat sum       = 0.0f;

	while (t <= glm::pi<GLfloat>()){
		GLfloat curValue = phase(t) * phase(theta - t);

		sum += (prevValue + curValue) / 2.0f * delta;

		prevValue = curValue;
		t += delta;
	}

	return sum / P2Normalizer;
}

GLfloat CloudFuncs::dirac(GLfloat angle)
{
	return 1.0f / diracSigma / glm::sqrt(2.0f * glm::pi<GLfloat>()) * glm::exp(-(angle * angle) / 2.0f / diracSigma / diracSigma);
}

GLfloat CloudFuncs::b(GLfloat angle)
{
	GLfloat data[] = { 1.1798f, 1.1293f, 1.1382f, 1.0953f, 0.9808f, 0.9077f, 0.7987f, 0.6629f, 0.5043f, 0.3021f };
	return linterp(data, angle);
}

GLfloat CloudFuncs::c(GLfloat angle)
{
	GLfloat data[] = { 0.0138f, 0.0154f, 0.0131f, 0.0049f, 0.0012f, 0.0047f, 0.0207f, 0.0133f, 0.0280f, 0.0783f };
	return linterp(data, angle);
}

GLfloat CloudFuncs::r(GLfloat angle)
{
	GLfloat data[] = { 0.0547f, 0.0547f, 0.0552f, 0.0564f, 0.0603f, 0.0705f, 0.0984f, 0.17f, 0.3554f, 0.95f };
	return linterp(data, angle);
}

GLfloat CloudFuncs::t(GLfloat angle)
{
	GLfloat data[] = { 0.8389f, 0.8412f, 0.8334f, 0.8208f, 0.8010f, 0.7774f, 0.7506f, 0.7163f, 0.7149f, 0.1f };
	return linterp(data, angle);
}

GLfloat CloudFuncs::kc(GLfloat angle)
{
	GLfloat data[] = { 0.0265f, 0.0262f, 0.0272f, 0.0294f, 0.0326f, 0.0379f, 0.0471f, 0.0616f, 0.07f, 0.07f };
	return linterp(data, angle);
}

GLfloat CloudFuncs::F(GLfloat angle, GLfloat z1)
{
	return b(angle) + (1 - b(angle)) * glm::exp(-z1 * c(angle));
}

GLfloat CloudFuncs::gamma(GLfloat angle, GLfloat z1)
{
	return F(angle, z1) * gammaRef / (z1 - (z1 - zref) * gammaRef);
}

GLfloat CloudFuncs::mr1Inf(GLfloat angle, GLfloat z1)
{
	return 1.0f - gamma(angle, z1);
}

GLfloat CloudFuncs::mt1(GLfloat angle, GLfloat z1)
{
	return t(angle) * z1 * kc(angle) * glm::exp(-kc(angle) * z1);
}

GLfloat CloudFuncs::mr1(GLfloat angle, GLfloat z1)
{
	return 1.0f / 2.0f * r(angle) * (1.0f - glm::exp(-2.0f * kc(angle) * z1));
}

GLfloat CloudFuncs::mt2(GLfloat angle, GLfloat z1)
{
	const GLfloat kcVal = kc(angle);
	const GLfloat rVal  = r(angle);
	const GLfloat tmp1 = kcVal * t(angle) * z1;
	
	return 1.0f / 2.0f * tmp1 * tmp1 * glm::exp(-kcVal * z1) +
		1.0f / 4.0f * rVal * (1.0f + 2.0f * (kcVal * z1 - 1.0f) * 
		glm::exp(-2.0f * kcVal * z1)) * glm::exp(-3.0f * kcVal * z1);
}

GLfloat CloudFuncs::mr2(GLfloat angle, GLfloat z1)
{
	const GLfloat kcVal = kc(angle);

	return 1.0f / 2.0f * r(angle) * t(angle) * (1.0f - (2.0f * kcVal * z1 + 1.0f) *
		glm::exp(-2.0f * kcVal * z1));
}

GLfloat CloudFuncs::rSun3(GLfloat angle, GLfloat z1)
{
	return (mr1Inf(angle, z1) - mr1(angle, z1) - mr2(angle, z1)) / 2.0f / glm::pi<GLfloat>();
}

GLfloat CloudFuncs::linterp(GLfloat data[], GLfloat angle)
{
	GLfloat step = 10.0f * glm::pi<GLfloat>() / 180.0f;

	GLuint idx0 = glm::floor(angle / step);
	GLuint idx1 = idx0 + 1;
	GLfloat d0  = angle - step * idx0;
	GLfloat kt1 = d0 / step;
	GLfloat kt2 = 1 - kt1;

	return data[idx0] * kt1 + data[idx1] * kt2;
}
