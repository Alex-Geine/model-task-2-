#pragma once
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>


using namespace std;


class ShredingerModel {
private:
	int
		n = 0, // ���������� ��
		pCount = 100;		//���������� ����� �� ������ �������

	double
		k = 1, //����������� ��� ������������� ������� �����������
		stepE = 0.01,  //��� �� �������
		R = 1,		//����������
		h = 0.001,		//��� ��������������
		eps = 0.001,	//�������� ������ ����������� �������
		V = 0.1,
		b = 0.1,
		dz = 0;

	

	//������� phase - ������ �������� ��� ������� �������
	double phase(double k) {
		return -(2 * k + 1) * M_PI / 2;
	}


	//������������ �������
	double U(double z) {
		return k * z * z + V * exp(-(z + dz) * (z + dz) / (2 * b * b));
	}

	//����� ��� �������� ���������
	double fPhase(double x, double y, double e) {
		return -((e - U(x)) * cos(y) * cos(y) + sin(y) * sin(y));
	}

	//����� ��� ����������� ���������
	double fRad(double x, double y, double e, double z) {
		return -( y * (1 - e + U(z)) * cos(x) * sin(x));
	}

	//����� �����-����� ������� ����� 1�� �������
	void RKMethod(double& x, double& y, double e, bool flag, double z = 0);

	//�������, ������� �������� phase �� �������
	double FindPhase(double e, double RR = -1000, double yy = M_PI / 2, double xp = 0);

	//��������������� ������� ��� ����������� �������
	double F(double nn, double phase);

	//����� ������� ������� �������
	void HalfSegmentMethod(double a, double b, double& x, double& y, int nn);
		
public:	
	vector<pair<double, double>>
		Points,		//����� �� ������� (x - e, y - f(R,e))
		Intervals,	//��������� ��� ���������� ������ ������� ����������� ������� (x - left, y - right)
		Roots,		//����������� �������� ������� �����������
		PointsFunc;		//����� ����������� �������

	vector<double> eLevels;		//������ �� ���������� ������� �������
	vector<double> UPoints;

	double maxE = 0;
	double maxPhase = 0;
	double maxF = 0;
	double maxU = 0;

	ShredingerModel(int n):n(n) {
	};
	~ShredingerModel() {
	};


	//������� ���������� ����� ��� �������
	void FindPointsGr();

	//�������, ��������� ��������� ����������� ��������
	void FindIntervals();

	//�������, ��������� ����������� ��������
	void FindRoots();

	//�������, ��������� ����������� ������� �����������
	void FindFunction(int id);

	//�������, ���������� ������
	void Clear();

	//�������� ��������� ������
	void UpdPar(
		int	n, // ���������� ��
		int pCount,		//���������� ����� �� ������ �������
		double	k, //����������� ��� ������������� ������� �����������
		double stepE,  //��� �� �������
		double R,		//����������
		double eps,	//�������� ������ ����������� �������
		double V,	//��������� ������ 
		double b,	//��������� ������
		double dz	//���������� ������	
	);
	
};