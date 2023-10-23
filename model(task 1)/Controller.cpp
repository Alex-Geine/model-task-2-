#include "pch.h"
#include "Controller.h"
#include <algorithm>
#include <string>

using namespace std;

#define _USE_MATH_DEFINES
#include  <math.h>

//�������� ��������� ������
void Controller::UpdPar(
	int	n, // ���������� ��
	int pCount,		//���������� ����� �� ������ �������
	double	k, //����������� ��� ������������� ������� �����������
	double stepE,  //��� �� �������
	double R,		//����������
	double eps,	//�������� ������ ����������� �������
	double V,	//��������� ������ 
	double b,	//��������� ������
	double dz	//���������� ������	
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

	mod->UpdPar(n, pCount, k, stepE, R, eps,V,b,dz);
}

//��������� ������� ����� � �����
void Controller::ShowItemList() {
	int cursel = listModels->GetCurSel();
	if (cursel != LB_ERR) {
		Done = false;
		//��������� id ���������� ��������		
		int id = listModels->GetCurSel();
		mod->PointsFunc.clear();
		mod->maxF = 0;
		mod->FindFunction(id);
		maxF = mod->maxF;
		Done = true;
		MessageBoxW(NULL, L"DONE!", L"DONE!", NULL);
		
	}
}

//������� ������
void Controller::ClearList(){
	listModels->ResetContent();	
}

//��������� ������
void Controller::FillList(){
	for (int i = 0; i < mod->Roots.size(); i++) {
		const int size = 100;
		string st = to_string(mod->Roots[i].first);
		const char* stBuf = st.c_str();   // 1. string to const char *

		size_t sz;                          // save converted string's length + 1
		wchar_t output[size] = L"";          // return data, result is CString data

		mbstowcs_s(&sz, output, size, stBuf, size); // converting function

		CString cst = output;
		listModels->AddString((LPCTSTR)cst);		
	}
}

//���������� ����������
void Controller::StartSolve() {
	Done = false;
	mod->FindPointsGr();
	mod->FindIntervals();
	mod->FindRoots();
	mod->FindFunction(0);

	maxE = mod->maxE;
	maxPhase = mod->maxPhase;
	maxF = mod->maxF;
	maxU = mod->maxU;

	FillList();

	Done = true;
	MessageBoxW(NULL, L"DONE!", L"DONE!", NULL);
}

//��������� ��������� �������� �������
void Controller::DrawMainGr(LPDRAWITEMSTRUCT Item1) {
	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
	//��������� ������� � ������� ����������
	double top = (fabs(maxPhase));
	double bottom = 0;
	double left = 0;
	double right = (fabs(maxE));
	
	double steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right - left);
	double yScale = -ItemHeight / (top - bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//�����������
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//�������������� ����(������� � ����������� ������ �������
	Matrix matr;
	matr.Scale(xScale, (yScale + 1)); //��������������
	matr.Translate(0 , 0); //������� ������ ���������


	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::BlueViolet, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Colibri"), 14, FontStyle::FontStyleRegular, UnitPixel);


	//��������
	for (int i = 0; i < 11; i++)
	{
		//��������������
		gr.DrawLine(&BackGroundPen, PointF(0,  - i * steplenY), PointF(right, - i * steplenY));

		//������������
		gr.DrawLine(&BackGroundPen, PointF( i * steplenX, 0), PointF( i * steplenX, -top));
	}

	//������� ���
	//��������������
	gr.DrawLine(&pen, PointF(0, 0), PointF(right, 0));

	//������������
	gr.DrawLine(&pen, PointF(0, 0), PointF(0, -top));

	gr.ResetTransform();

	//������� � ����
	for (int i = 0; i < 11; i++)
	{
		CString str;

		//�� Y
		str.Format(L"%.2f", - i * steplenY);
		PointF strPoint(0, - i * steplenY);
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		//�� X
		str.Format(L"%.2f",  i * steplenX);
		strPoint.X =  i * steplenX;
		strPoint.Y = 0;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	gr.SetTransform(&matr);

	//��������� �������� �������� 


	Color color(Color::DarkRed);
	Pen StickPen1(color, 0.02);

	Color color2(Color::Green);
	Pen StickPen2(color2, 0.02);

	PointF p1, p2;

	for (int i = 0; i < mod->eLevels.size(); i++) {
		p1.X = 0;
		p1.Y = mod->eLevels[i];
		p2.X = right;
		p2.Y = mod->eLevels[i];
		gr.DrawLine(&StickPen2, p1, p2);
	}

	for (int i = 1; i < mod->Points.size(); i++) {
		p1.X = mod->Points[i - 1].first;
		p1.Y = mod->Points[i - 1].second;
		p2.X = mod->Points[i].first;
		p2.Y = mod->Points[i].second;
		gr.DrawLine(&StickPen1, p1, p2);
	}

	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
	//if (Models.size() != 0) {
		//��������� ��������� ���������
		//double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		//	ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
		////��������� ������� � ������� ����������
		//double top = 2 * (maxla) * (1 + 0.2);
		//double bottom = 0;
		//double left = 0;
		//double right = 2 * (maxla) * (1 + 0.2);

		////xPadding = (right - left) / 10;
		////yPadding = (top - bottom) / 10;

		//double steplenY = (top - bottom) / 10,
		//	steplenX = (right - left) / 10;

		//double xScale = ItemWidth / (right - left);
		//double yScale = -ItemHeight / (top - bottom);



		//Bitmap Image(ItemWidth, ItemHeight);
		//Graphics gr(&Image);
		//gr.Clear(Color::White);
		////�����������
		//gr.SetSmoothingMode(SmoothingModeAntiAlias);

		////�������������� ����(������� � ����������� ������ �������
		//Matrix matr;
		//matr.Scale(xScale, yScale + 1); //��������������
		//matr.Translate(right / 2, -top / 2); //������� ������ ���������


		//gr.SetTransform(&matr);


		//Pen BackGroundPen(Color::DarkGray, 0.005);
		//Pen pen(Color::BlueViolet, 0.005);

		//
		//
		//
		//Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);

		////���

		//gr.DrawLine(&pen, PointF(-l - a, 0), PointF(l + a, 0));
		//gr.DrawLine(&pen, PointF(0, l + a), PointF(0, -l - a));

		//double bollWidth = l / 5;
		//double platformWidth = l / 8;
		//double platformHeigtht = platformWidth / 2;

		//////��������� ���������
		////for (int i = 0; i < Models.size(); i++) {
		////	MINF* minf = Models[i];

		////	Color color;
		////	color.SetFromCOLORREF(minf->col);
		////	//SolidBrush br()
		////	SolidBrush brush(color);
		////	Pen StickPen(color, 0.02);

		////	double T = minf->t * minf->T0;

		////	//���������� ��������� �������
		////	double yPl = minf->a * minf->Y0 * sin(minf->w * T);
		////	double xBoll = minf->l * sin(minf->f0 * M_PI / 180);
		////	double yBoll = minf->l * cos(minf->f0 * M_PI / 180);

		////	gr.FillRectangle(&brush, RectF(-platformWidth / 2, -yPl - platformHeigtht / 2, platformWidth, platformHeigtht));
		////	gr.FillEllipse(&brush, RectF(xBoll - bollWidth / 2, -yBoll - yPl - bollWidth / 2, bollWidth, bollWidth));
		////	gr.DrawLine(&StickPen, PointF(0, -yPl), PointF(xBoll, -yBoll - yPl));
		////}
		//
		//Graphics grnew(Item1->hDC);
		//grnew.DrawImage(&Image, 0, 0);
	//}	
}

//��������� ��������� ������� � ������������ ���������
void Controller::DrawPhase(LPDRAWITEMSTRUCT Item1) {
	if (Done) {
		double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
			ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
		//��������� ������� � ������� ����������
		double top = 2 * (fabs(maxF)) * (1 + 0.2);
		double bottom = 0;
		double left = 0;
		double right = 2 * R * (1 + 0.2);

		//xPadding = (right - left) / 10;
		//yPadding = (top - bottom) / 10;

		double steplenY = (top - bottom) / 10,
			steplenX = (right - left) / 10;

		double xScale = ItemWidth / (right - left);
		double yScale = -ItemHeight / (top - bottom);



		Bitmap Image(ItemWidth, ItemHeight);
		Graphics gr(&Image);
		gr.Clear(Color::White);
		//�����������
		gr.SetSmoothingMode(SmoothingModeAntiAlias);

		//�������������� ����(������� � ����������� ������ �������
		Matrix matr;
		matr.Scale(xScale, (yScale + 1)); //��������������
		matr.Translate(right / 2, -top / 2); //������� ������ ���������


		gr.SetTransform(&matr);


		Pen BackGroundPen(Color::DarkGray, 0.005);
		Pen pen(Color::BlueViolet, 0.006);

		Pen StickPen(Color::CornflowerBlue, 0.02);
		SolidBrush brush(Color::Black);
		SolidBrush PlatformBrush(Color::Crimson);
		Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


		//��������
		for (int i = 0; i < 10; i++)
		{
			//��������������
			gr.DrawLine(&BackGroundPen, PointF(-right / 2, top / 2 - i * steplenY), PointF(right / 2, top / 2 - i * steplenY));

			//������������
			gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top / 2), PointF(-right / 2 + i * steplenX, -top / 2));
		}

		//������� ���
		//��������������
		gr.DrawLine(&pen, PointF(-right / 2, 0), PointF(right / 2, 0));

		//������������
		gr.DrawLine(&pen, PointF(0, top / 2), PointF(0, -top / 2));

		gr.ResetTransform();

		//������� � ����
		for (int i = 0; i < 11; i++)
		{
			CString str;

			//�� Y
			str.Format(L"%.2f", top / 2 - i * steplenY);
			PointF strPoint(0, top / 2 - i * steplenY);
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

			//�� X
			str.Format(L"%.2f", right / 2 - i * steplenX);
			strPoint.X = right / 2 - i * steplenX;
			strPoint.Y = 0;
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
		}

		gr.SetTransform(&matr);

		//��������� �������


		Color color(Color::IndianRed);
		Pen StickPen1(color, 0.01);

		PointF p1, p2;
		for (int i = 1; i < mod->PointsFunc.size(); i++) {
			p1.X = mod->PointsFunc[i - 1].first;
			p1.Y = mod->PointsFunc[i - 1].second;
			p2.X = mod->PointsFunc[i].first;
			p2.Y = mod->PointsFunc[i].second;
			gr.DrawLine(&StickPen1, p1, p2);
		}

		Graphics grnew(Item1->hDC);
		grnew.DrawImage(&Image, 0, 0);
	}
}

//��������� ��������� ������� ������� ����������
void Controller::DrawPhaseTr(LPDRAWITEMSTRUCT Item1) {
	if (Done) {
		double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
			ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
		//��������� ������� � ������� ����������
		double top = 2 * (fabs(maxU)) * (1 + 0.2);
		double bottom = 0;
		double left = 0;
		double right = 2 * R * (1 + 0.2);

		//xPadding = (right - left) / 10;
		//yPadding = (top - bottom) / 10;

		double steplenY = (top - bottom) / 10,
			steplenX = (right - left) / 10;

		double xScale = ItemWidth / (right - left);
		double yScale = -ItemHeight / (top - bottom);



		Bitmap Image(ItemWidth, ItemHeight);
		Graphics gr(&Image);
		gr.Clear(Color::White);
		//�����������
		gr.SetSmoothingMode(SmoothingModeAntiAlias);

		//�������������� ����(������� � ����������� ������ �������
		Matrix matr;
		matr.Scale(xScale, (yScale + 1)); //��������������
		matr.Translate(right / 2, -top / 2); //������� ������ ���������


		gr.SetTransform(&matr);


		Pen BackGroundPen(Color::DarkGray, 0.005);
		Pen pen(Color::BlueViolet, 0.006);

		Pen StickPen(Color::CornflowerBlue, 0.02);
		SolidBrush brush(Color::Black);
		SolidBrush PlatformBrush(Color::Crimson);
		Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


		//��������
		for (int i = 0; i < 10; i++)
		{
			//��������������
			gr.DrawLine(&BackGroundPen, PointF(-right / 2, top / 2 - i * steplenY), PointF(right / 2, top / 2 - i * steplenY));

			//������������
			gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top / 2), PointF(-right / 2 + i * steplenX, -top / 2));
		}

		//������� ���
		//��������������
		gr.DrawLine(&pen, PointF(-right / 2, 0), PointF(right / 2, 0));

		//������������
		gr.DrawLine(&pen, PointF(0, top / 2), PointF(0, -top / 2));

		gr.ResetTransform();

		//������� � ����
		for (int i = 0; i < 11; i++)
		{
			CString str;

			//�� Y
			str.Format(L"%.2f", top / 2 - i * steplenY);
			PointF strPoint(0, top / 2 - i * steplenY);
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

			//�� X
			str.Format(L"%.2f", right / 2 - i * steplenX);
			strPoint.X = right / 2 - i * steplenX;
			strPoint.Y = 0;
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
		}

		gr.SetTransform(&matr);

		//��������� �������


		Color color(Color::IndianRed);
		Pen StickPen1(color, 0.01);
		PointF p1, p2;	
		for (int i = 1; i < pCount; i++) {
			p1.X = mod->PointsFunc[i-1].first;
			p2.X = mod->PointsFunc[i].first;
			p1.Y = mod->UPoints[i -1];
			p2.Y = mod->UPoints[i];
			gr.DrawLine(&StickPen1, p1, p2);
		}		

		Graphics grnew(Item1->hDC);
		grnew.DrawImage(&Image, 0, 0);
	}
}

//������� ������
void Controller::Clear() {
	ClearList();
	mod->Clear();
	//maxf0 = 0; maxdf0 = 0; maxla = 0;
	xst = 0; yst = 0; scalegr = 1;
	xstTr = 0; ystTr = 0; scalegrTr = 1;
}

//������� ������
void Controller::DeleteModel(int id) {
	//������� �������
	/*EnterCriticalSection(&csModels);
	auto iter = Models.begin();
	for (int i = 0; i < Models.size(); i++) {
		
		if (Models[i]->id == id) {
			TerminateThread(TREAD, 0);
			iter += i;
			Models.erase(iter);
			i = Models.size();
		}
	}

	LeaveCriticalSection(&csModels);*/
	

}

void Controller::UpdateModel(
	int id,
	double l, //������ ��������
	double a, //��������� �������
	double w, //������� �������
	double f0, //��������� ����������
	double df0, //��������� ������� ��������	
	double niu, //����������� ������� ������
	double k,
	COLORREF col) 
{
	/*EnterCriticalSection(&csModels);
	auto iter = Models.begin();
	for (int i = 0; i < Models.size(); i++) {

		if (Models[i]->id == id) {
			EnterCriticalSection(&(Models[i]->cs));
			Models[i]->mod->UpdatePar(l, a, w, f0, df0, niu, k);
			Models[i]->col = col;
			LeaveCriticalSection(&(Models[i]->cs));
		}
	}

	LeaveCriticalSection(&csModels);*/


}

void Controller::GetParOfModel(
	int id,
	double &l, //������ ��������
	double &a, //��������� �������
	double &w, //������� �������
	double &f0, //��������� ����������
	double &df0, //��������� ������� ��������	
	double &niu, //����������� ������� ������
	double &k,
	COLORREF &col) {
	/*EnterCriticalSection(&csModels);
	auto iter = Models.begin();
	for (int i = 0; i < Models.size(); i++) {

		if (Models[i]->id == id) {
			EnterCriticalSection(&(Models[i]->cs));
			Models[i]->mod->GetPar(&(Models[i]->cs),l, a, w, f0, df0, niu, k);
			LeaveCriticalSection(&(Models[i]->cs));
			col = Models[i]->col;
		}
	}

	LeaveCriticalSection(&csModels);*/
}