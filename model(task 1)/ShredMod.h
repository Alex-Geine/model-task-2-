#pragma once
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>


using namespace std;


class ShredingerModel {
private:
	int
		n = 0, // количество СФ
		pCount = 100;		//количество точек на втором графике

	double
		k = 1, //коэффициент для потенциальной энергии осциллятора
		stepE = 0.01,  //шаг по энергии
		R = 1,		//полудлинна
		h = 0.001,		//шаг интергирования
		eps = 0.001,	//точность метода половинного деления
		V = 0.1,
		b = 0.1,
		dz = 0;

	

	//функция phase - правое значение для фазовой функции
	double phase(double k) {
		return -(2 * k + 1) * M_PI / 2;
	}


	//потениальная энергия
	double U(double z) {
		return k * z * z + V * exp(-(z + dz) * (z + dz) / (2 * b * b));
	}

	//дифур для фазового уравнения
	double fPhase(double x, double y, double e) {
		return -((e - U(x)) * cos(y) * cos(y) + sin(y) * sin(y));
	}

	//дифур для радиального уравнения
	double fRad(double x, double y, double e, double z) {
		return -( y * (1 - e + U(z)) * cos(x) * sin(x));
	}

	//метод Рунге-Кутты решения дифур 1го порядка
	void RKMethod(double& x, double& y, double e, bool flag, double z = 0);

	//функция, находит значение phase на границе
	double FindPhase(double e, double RR = -1000, double yy = M_PI / 2, double xp = 0);

	//вспомогательная функция для половинного деления
	double F(double nn, double phase);

	//метод деления отрезка пополам
	void HalfSegmentMethod(double a, double b, double& x, double& y, int nn);
		
public:	
	vector<pair<double, double>>
		Points,		//точки на графике (x - e, y - f(R,e))
		Intervals,	//интервалы для нахождения корней методом половинного деления (x - left, y - right)
		Roots,		//Собственные значения энергии осциллятора
		PointsFunc;		//точки собственной функции

	vector<double> eLevels;		//вектор со значениями уровней энергии
	vector<double> UPoints;

	double maxE = 0;
	double maxPhase = 0;
	double maxF = 0;
	double maxU = 0;

	ShredingerModel(int n):n(n) {
	};
	~ShredingerModel() {
	};


	//функция построения точек для графика
	void FindPointsGr();

	//функция, находящая интервалы собственных значений
	void FindIntervals();

	//функция, находящая собственные значения
	void FindRoots();

	//функция, находящая собственную функцию осциллятора
	void FindFunction(int id);

	//Функция, затирающая данные
	void Clear();

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
	
};