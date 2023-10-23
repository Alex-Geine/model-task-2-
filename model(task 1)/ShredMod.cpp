#include "pch.h"
#include "ShredMod.h"



//метод Рунге-Кутты 4 порядка решения дифур 1го порядка 
void ShredingerModel::RKMethod(double&x, double&y, double e, bool flag, double z) {
	double K1 = 0;
	double K2 = 0;
	double K3 = 0;
	double K4 = 0;

	//если нужно решить уравнение для фазовой функции
	if (flag) {
		K1 = fPhase(x, y, e);
		K2 = fPhase(x + h / 2, y + h / 2 * K1, e);
		K3 = fPhase(x + h / 2, y + h / 2 * K2, e);
		K4 = fPhase(x + h, y + h * K3, e);
		x += h;
	}

	//если нужно решить уравнение для радиальной функции
	else {
		K1 = fRad(x, y, e, z);
		K2 = fRad(x + h / 2, y + h / 2 * K1, e, z);
		K3 = fRad(x + h / 2, y + h / 2 * K2, e, z);
		K4 = fRad(x + h, y + h * K3, e, z);
	}

	y += h / 6 * (K1, +2 * K2 + 2 * K3 + K4);	
}



//функция, находит значение phase на границе
double ShredingerModel::FindPhase(double e, double RR, double yy, double xx) {
	
	//начальное приближение
	double
		x = -R,
		y = yy;

	if(RR == - 1000)
		while (x <= R)
			RKMethod(x, y, e, 1);
	else {
		x = xx;
		while (x <= RR)
			RKMethod(x, y, e, 1);
	}
		

	return y;
}

//функция построения точек для графика
void ShredingerModel::FindPointsGr() {
	double
		FCur = 0,	//текущая фазовая функция
		e = 0;		//начальное значение энергии


	int i = 0;

	//цикл находит точки для графика фазовой функции
	while (FCur >= phase(n)) {
		e += i * stepE;
		FCur = FindPhase(e);

		pair<double, double> p;
		p.first = e;
		p.second = FCur;
		Points.push_back(p);
		cout << FCur << endl;

		i++;
	}
	maxE = e;
	maxPhase = FCur;

	for (int j = 0; j < n; j++)
		eLevels.push_back(phase(j));
}

//вспомогательная функция для половинного деления
double ShredingerModel::F(double nn, double phase) {
	return phase - this->phase(nn);
}

//функция, находящая интервалы собственных значений
void ShredingerModel::FindIntervals() {
	int nn = 0; //индекс собственной функции

	for (int i = 1; i < Points.size(); i++) {
		//F(a) * F(b) < 0
		double PHASE = phase(nn);
		double fLeft = F(nn, Points[i - 1].second);
		double fRight = F(nn, Points[i].second);
		if (fLeft * fRight < 0)
		{
			pair<double, double> p;
			p.first = Points[i - 1].first;
			p.second = Points[i].first;
			
			Intervals.push_back(p);
			nn++;
		}
	}
	
}

//метод деления отрезка пополам
void ShredingerModel::HalfSegmentMethod(double a, double b, double& x, double& y, int nn) {
	double xx;
	while (fabs(b - a) > 2 * eps) {
		xx = (a + b) / 2;
		double fa = F(nn, FindPhase(a));
		double fx = F(nn, FindPhase(xx));
		double fb = F(nn, FindPhase(b));
		if (fa * fx < 0)
			b = xx;
		else if (fx * fb < 0)
			a = xx;
	}
	x = (a + b) / 2;
	y = FindPhase(x);
}

//функция, находящая собственные значения
void ShredingerModel::FindRoots() {
	//перебираем все собственные значения
	for (int i = 0; i < n; i++) {
		pair<double, double> p;
		HalfSegmentMethod(Intervals[i].first, Intervals[i].second, p.first, p.second, i);
		Roots.push_back(p);
		
	}
}


//функция, находящая собственную функцию осциллятора
void ShredingerModel::FindFunction(int id) {
	double
		r = 1,	//начальные приближения
		x = -R,
		xPast = x,
		curPhase = M_PI /2,
		curE = Roots[id].first,
		stepX = 2 * R / pCount;

	double maxf = 0;
	double Up = 0;
	for (int i = 0; i < pCount; i++) {
		x += stepX;
		curPhase = FindPhase(curE, x, curPhase, xPast);
		RKMethod(curPhase, r, curE, 0);
		pair<double, double> p;
		p.first = x;
		p.second = r * cos(curPhase);
		PointsFunc.push_back(p);
		if (maxf < fabs(p.second))
			maxf = fabs(p.second);
		
		Up = U(x);
		if (maxU < Up)
			maxU = Up;
		UPoints.push_back(Up);
		xPast = x;
	}
	maxF = maxf;
}

//Функция, затирающая данные
void ShredingerModel::Clear() {
	Points.clear();
	Intervals.clear();
	Roots.clear();
	PointsFunc.clear();
	eLevels.clear();
	UPoints.clear();
}

//апдейтит параметры модели
void ShredingerModel::UpdPar(
	int	n, // количество СФ
	int pCount,		//количество точек на втором графике
	double	k, //коэффициент для потенциальной энергии осциллятора
	double stepE,  //шаг по энергии
	double R,		//полудлинна
	double eps,	//точность метода половинного деления
	double V,	//амплитуда купола 
	double b,	//дисперсия купола
	double dz	//отклонение купола	
)
{
	this->n = n;
	this->pCount = pCount;
	this->k = k;
	this->stepE = stepE;
	this->R = R;
	this->eps = eps;
	this->V = V;
	this->b = b;
	this->dz = dz;
}