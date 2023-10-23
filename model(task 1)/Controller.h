#pragma once

#include<gdiplus.h>
#include <vector>
#include "ShredMod.h"

using namespace std;
using namespace Gdiplus;



class Controller {	
private:	
	ULONG_PTR token;
	ShredingerModel *mod;
	
	
	//������ �� ���������� ��� �������� ��������
	vector<PointF> phasePoints;	

	//��������� ������
	int
		n = 0, // ���������� ��
		pCount = 100;		//���������� ����� �� ������ �������

	double
		k = 1, //����������� ��� ������������� ������� �����������
		stepE = 0.01,  //��� �� �������
		R = 1,		//����������
		eps = 0.001,	//�������� ������ ����������� �������
		V = 0.1,		//��������� ������
		b = 0.1,		//��������� ������
		dz = 0;			//���������� ��� �� 0

	double maxE = 0;
	double maxPhase = 0;
	double maxF = 0;
	double maxU = 0;

	int curMod = 0;
	bool Done = false;

	//������� ������
	void ClearList();
	//��������� ������
	void FillList();
	
public:
	CListBox* listModels;
	int drawId = 0;
	double maxf0 = 0, maxdf0 = 0;
	double xst = 0, yst = 0, scalegr = 1;
	double xstTr = 0, ystTr = 0, scalegrTr = 1;
	
	void GetParOfModel(
		int id, 
		double &l, //������ ��������
		double &a, //��������� �������
		double &w, //������� �������
		double &f0, //��������� ����������
		double &df0, //��������� ������� ��������	
		double &niu, //����������� ������� ������
		double &k,
		COLORREF &col);

	void UpdateModel(
		int id,
		double l, //������ ��������
		double a, //��������� �������
		double w, //������� �������
		double f0, //��������� ����������
		double df0, //��������� ������� ��������	
		double niu, //����������� ������� ������
		double k,
		COLORREF col);
	
	//������� ������
	void DeleteModel(int id);
	
	//������� ������
	void Clear();
	
	//��������� ��������� �������� �������6
	void DrawMainGr(LPDRAWITEMSTRUCT Item1);

	//��������� ��������� �������� �������
	void DrawPhase(LPDRAWITEMSTRUCT Item1);

	//��������� ��������� �������� �������
	void DrawPhaseTr(LPDRAWITEMSTRUCT Item1);

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

	//���������� ����������
	void StartSolve();	

	Controller():mod(new ShredingerModel(5)) {
		GdiplusStartupInput si;
		GdiplusStartup(&token, &si, NULL);
	}

	//����������
	~Controller() {
		GdiplusShutdown(token);
	}	

	//��������� ������� ����� � �����
	void ShowItemList();

};