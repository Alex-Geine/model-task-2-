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
	
	
	//вектор со значениями для фазового портрета
	vector<PointF> phasePoints;	

	//паоаметры модели
	int
		n = 0, // количество СФ
		pCount = 100;		//количество точек на втором графике

	double
		k = 1, //коэффициент для потенциальной энергии осциллятора
		stepE = 0.01,  //шаг по энергии
		R = 1,		//полудлинна
		eps = 0.001,	//точность метода половинного деления
		V = 0.1,		//амплитуда купола
		b = 0.1,		//дисперсия купола
		dz = 0;			//отклонение ямы от 0

	double maxE = 0;
	double maxPhase = 0;
	double maxF = 0;
	double maxU = 0;

	int curMod = 0;
	bool Done = false;

	//очищает список
	void ClearList();
	//заполняет список
	void FillList();
	
public:
	CListBox* listModels;
	int drawId = 0;
	double maxf0 = 0, maxdf0 = 0;
	double xst = 0, yst = 0, scalegr = 1;
	double xstTr = 0, ystTr = 0, scalegrTr = 1;
	
	void GetParOfModel(
		int id, 
		double &l, //Длинна маятника
		double &a, //Амплитуда подвеса
		double &w, //частота подвеса
		double &f0, //начальное отклонение
		double &df0, //начальная угловая скорость	
		double &niu, //коэффициент вязкого трения
		double &k,
		COLORREF &col);

	void UpdateModel(
		int id,
		double l, //Длинна маятника
		double a, //Амплитуда подвеса
		double w, //частота подвеса
		double f0, //начальное отклонение
		double df0, //начальная угловая скорость	
		double niu, //коэффициент вязкого трения
		double k,
		COLORREF col);
	
	//удаляет модель
	void DeleteModel(int id);
	
	//очищает данные
	void Clear();
	
	//запускает отрисовку главного графика6
	void DrawMainGr(LPDRAWITEMSTRUCT Item1);

	//запускает отрисовку главного графика
	void DrawPhase(LPDRAWITEMSTRUCT Item1);

	//запускает отрисовку главного графика
	void DrawPhaseTr(LPDRAWITEMSTRUCT Item1);

	//апдейтит параметры модели
	void UpdPar(
		int	n, // количество СФ
		int pCount,		//количество точек на втором графике
		double	k, //коэффициент для потенциальной энергии осциллятора
		double stepE,  //шаг по энергии
		double R,		//полудлинна
		double eps,	//точность метода половинного деления
		double V,	//амплитуда купола 
		double b,	//дисперсия купола
		double dz	//отклонение купола	
	);	

	//запусткает вычисления
	void StartSolve();	

	Controller():mod(new ShredingerModel(5)) {
		GdiplusStartupInput si;
		GdiplusStartup(&token, &si, NULL);
	}

	//деструктор
	~Controller() {
		GdiplusShutdown(token);
	}	

	//показвает текущий айтем в листе
	void ShowItemList();

};