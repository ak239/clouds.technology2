#pragma once
class CloudFuncs
{
public:
	static GLfloat kref;
	static GLfloat gammaRef;
	static GLfloat zref;
	static GLfloat diracSigma;
	static GLfloat g;
	static GLfloat thetaP;
	static GLfloat P2Normalizer;

	static GLfloat angle(GLfloat theta1, GLfloat phi1, GLfloat theta2, GLfloat phi2);
	static GLfloat phase(GLfloat theta);
	static GLfloat phaseWave(GLfloat theta);
	static GLfloat phasePeak(GLfloat theta);
	static GLfloat phaseByPhase(GLfloat theta);
	static GLfloat dirac(GLfloat angle);
	static GLfloat b(GLfloat angle);
	static GLfloat c(GLfloat angle);
	static GLfloat r(GLfloat angle);
	static GLfloat t(GLfloat angle);
	static GLfloat kc(GLfloat angle);
	static GLfloat F(GLfloat angle, GLfloat z1);
	static GLfloat gamma(GLfloat angle, GLfloat z1);
	static GLfloat mr1Inf(GLfloat angle, GLfloat z1);
	static GLfloat mt1(GLfloat angle, GLfloat z1);
	static GLfloat mr1(GLfloat angle, GLfloat z1);
	static GLfloat mt2(GLfloat angle, GLfloat z1);
	static GLfloat mr2(GLfloat angle, GLfloat z1);
	static GLfloat rSun3(GLfloat angle, GLfloat z1);

	//static void addVarToBar(TwBar* bar);

private:
	static GLfloat linterp(GLfloat data[], GLfloat angle);

	CloudFuncs(void);
	~CloudFuncs(void);
};

