//---------------------------------------------------------------------------
#include <windows.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
struct Pint
{
	int x;
	int y;
	String color;
};
int NUM_CL = 10;
int NUM_PO = 4500;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

Pint maxRast(int num, Pint clusters[], Pint mass[])
{
	int max = 0;
	int nX =0;
	int nY =0;
	for (int i = 0; i < num; i++) {
		int dist = round(sqrt( (clusters[i].x-mass[i].x)*(clusters[i].x-mass[i].x) + (clusters[i].y-mass[i].y)*(clusters[i].y-mass[i].y) ));
		if (dist > max) {
			max = dist;
			nX = mass[i].x;
			nY = mass[i].y;
		}
	}

	Pint toc;
	toc.x = nX;
	toc.y = nY;
								  
	bool flag = true;

	for (int i = 0; i < num; i++) {
		for (int j = 0; i < num; i++) {
			int dist = round(sqrt( (clusters[i].x-clusters[j].x)*(clusters[i].x-clusters[j].x) + (clusters[i].y-clusters[j].y)*(clusters[i].y-clusters[j].y) )/2);
			if (dist > max) {
				flag = false;
				
			}
		}
	} 

	if (flag) {
		return toc;
	}
	else{
		toc.x = -1;
		return toc;
	}

}

struct Pint* searchNext(int num, Pint clusters[], Pint points[])
{
	struct Pint* mass = new Pint[num];
	for (int i = 0 ; i < num; i++)
	{
		int neX = 0;
		int neY = 0;
		//int maxDist = round(sqrt( (clusters[i].x-points[0].x)*(clusters[i].x-points[0].x) + (clusters[i].y-points[0].y)*(clusters[i].y-points[0].y) ));
		int maxDist = 0;
		int numC = 0;
		for (int j = 1; j < NUM_PO; j++) {
			if (clusters[i].color == points[j].color) {


				int rast = round(sqrt( (clusters[i].x-points[j].x)*(clusters[i].x-points[j].x) + (clusters[i].y-points[j].y)*(clusters[i].y-points[j].y)) );
				if (rast > maxDist ) {
					maxDist = rast;
					numC = j;
					neX = points[j].x;
					neY = points[j].y;
				}
			}
		}

		mass[i].x = neX;
		mass[i].y = neY;
	}

	return mass;      // массив возможных следующих

}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	Form1->Repaint();

	srand(time(NULL));

    Canvas->Pen->Color=clRed;
	Canvas->Pen->Width=5;

	Pint points[NUM_PO];

	for (int i = 0 ; i < NUM_PO; i++) {
		int x = rand() % 501;
		int y = rand() % 501;
		Canvas->MoveTo(x,y);
		Canvas->LineTo(x,y);
		points[i].x = x;
		points[i].y = y;
		points[i].color = "Red";

	}

	Pint clusters[NUM_CL];
	int k = 0;

	Canvas->Pen->Width=10;

	Canvas->Pen->Color= clBlack;

	int x = rand() % 501;
	int y = rand() % 501;
	Canvas->MoveTo(x,y);
	Canvas->LineTo(x,y);
	clusters[k].x = x;
	clusters[k].y = y;
	clusters[k].color = "Black";
	k++;

	Sleep(1000);

	for (int i = 0 ; i < k; i++) {
		int neX =0;
		int neY = 0;
		int maxDist = round(sqrt( (clusters[i].x-points[0].x)*(clusters[i].x-points[0].x) + (clusters[i].y-points[0].y)*(clusters[i].y-points[0].y) ));
		int numC = 0;
		for (int j = 1; j < NUM_PO; j++) {
			int rast = round(sqrt( (clusters[i].x-points[j].x)*(clusters[i].x-points[j].x) + (clusters[i].y-points[j].y)*(clusters[i].y-points[j].y)) );
			if (rast > maxDist ) {
				maxDist = rast;
				numC = j;
				neX = points[j].x;
				neY = points[j].y;
			}
		}


		clusters[k].x = neX;
		clusters[k].y = neY;
		clusters[k].color = "Green";



		Canvas->Pen->Width=10;
		Canvas->Pen->Color= clGreen;
		Canvas->MoveTo(clusters[k].x,clusters[k].y);
		Canvas->LineTo(clusters[k].x,clusters[k].y);

	}

	Canvas->Pen->Width=5;

	for (int i = 0 ; i < NUM_PO; i++) {
		int minDist = round(sqrt( (clusters[0].x-points[i].x)*(clusters[0].x-points[i].x) + (clusters[0].y-points[i].y)*(clusters[0].y-points[i].y) ));
		int numC = 0;
		for (int j = 1; j <= k; j++) {
			int rast = round(sqrt( (clusters[j].x-points[i].x)*(clusters[j].x-points[i].x) + (clusters[j].y-points[i].y)*(clusters[j].y-points[i].y)) );
			if (rast < minDist ) {
				minDist = rast;
				numC = j;
			}
		}

		switch (numC) {
		case 0:
			Canvas->Pen->Color=clBlack;
			points[i].color = "Black";
			break;
		case 1:
			Canvas->Pen->Color=clGreen;
			points[i].color = "Green";
			break;
		}

		Canvas->MoveTo(points[i].x,points[i].y);
		Canvas->LineTo(points[i].x,points[i].y);


	}

	Sleep(2500);

	bool flg = true;

	while (flg)
	{
		Pint tck = maxRast(k, clusters, searchNext(k, clusters, points));

		if ((tck.x == -1) || (k>8)) {
			flg = false;
			ShowMessage("оптимальное количество кластеров достигнуто");
		}
		else{

			k++;
			clusters[k].x = tck.x;
			clusters[k].y = tck.y;
			switch (k) {
			case 2:
				Canvas->Pen->Color=clLime;
				clusters[k].color = "Lime";
				break;
			case 3:
				Canvas->Pen->Color=clBlue;
				clusters[k].color = "Blue";
				break;
			case 4:
				Canvas->Pen->Color=clYellow;
				clusters[k].color = "Yellow";
				break;
			case 5:
				Canvas->Pen->Color=clPurple;
				clusters[k].color = "Purple";
				break;
			case 6:
				Canvas->Pen->Color=clAqua;
				clusters[k].color = "Aqua";
				break;
			case 7:
				Canvas->Pen->Color=clOlive;
				clusters[k].color = "Olive";
				break;
			case 8:
				Canvas->Pen->Color=clMaroon;
				clusters[k].color = "Maroon";
				break;
			}

			Canvas->Pen->Width=10;
			//Canvas->Pen->Color= clRed;
			Canvas->MoveTo(clusters[k].x,clusters[k].y);
			Canvas->LineTo(clusters[k].x,clusters[k].y);

			Canvas->Pen->Width=5;

			for (int i = 0 ; i < NUM_PO; i++) {
				int minDist = round(sqrt( (clusters[0].x-points[i].x)*(clusters[0].x-points[i].x) + (clusters[0].y-points[i].y)*(clusters[0].y-points[i].y) ));
				int numC = 0;
				for (int j = 1; j <= k; j++) {
					int rast = round(sqrt( (clusters[j].x-points[i].x)*(clusters[j].x-points[i].x) + (clusters[j].y-points[i].y)*(clusters[j].y-points[i].y)) );
					if (rast < minDist ) {
						minDist = rast;
						numC = j;
					}
				}

				switch (numC) {
				case 0:
					Canvas->Pen->Color=clBlack;
					points[i].color = "Black";
					break;
				case 1:
					Canvas->Pen->Color=clGreen;
					points[i].color = "Green";
					break;
				case 2:
					Canvas->Pen->Color=clLime;
					points[i].color = "Lime";
					break;
				case 3:
					Canvas->Pen->Color=clBlue;
					points[i].color = "Blue";
					break;
				case 4:
					Canvas->Pen->Color=clYellow;
					points[i].color = "Yellow";
					break;
				case 5:
					Canvas->Pen->Color=clPurple;
					points[i].color = "Purple";
					break;
				case 6:
					Canvas->Pen->Color=clAqua;
					points[i].color = "Aqua";
					break;
				case 7:
					Canvas->Pen->Color=clOlive;
					points[i].color = "Olive";
					break;
				case 8:
					Canvas->Pen->Color=clMaroon;
					points[i].color = "Maroon";
					break;
				}

				Canvas->MoveTo(points[i].x,points[i].y);
				Canvas->LineTo(points[i].x,points[i].y);


			}
			Sleep(1000);
		}



	}


	Sleep(2000);

	int lastx[k], lasty[k];
	for (int i = 0; i < k; i++) {
		lastx[i] = clusters[i].x;
		lasty[i] = clusters[i].y;
	}

	bool flag = true;

	do {

		Form1->Repaint();

		for (int i = 0; i <= k; i++) {
			int newX = 0;
			int newY = 0;
			int count = 0;
			for (int j = 0; j < NUM_PO; j++) {
				if (points[j].color == clusters[i].color) {
					newX += points[j].x;
					newY += points[j].y;
					count++;
				}
			}

			clusters[i].x = round(newX/count);
			clusters[i].y = round(newY/count);


			Canvas->Pen->Color=clRed;

			Canvas->Pen->Width=10;
			Canvas->MoveTo(clusters[i].x,clusters[i].y);
			Canvas->LineTo(clusters[i].x,clusters[i].y);

		}

		Canvas->Pen->Width=5;

		for (int i = 0 ; i < NUM_PO; i++) {
			int minDist = round(sqrt( (clusters[0].x-points[i].x)*(clusters[0].x-points[i].x) + (clusters[0].y-points[i].y)*(clusters[0].y-points[i].y) ));
			int numC = 0;
			for (int j = 1; j <= k; j++) {
				int rast = round(sqrt( (clusters[j].x-points[i].x)*(clusters[j].x-points[i].x) + (clusters[j].y-points[i].y)*(clusters[j].y-points[i].y)) );
				if (rast < minDist ) {
					minDist = rast;
					numC = j;
				}
			}

			switch (numC) {
			case 0:
				Canvas->Pen->Color=clBlack;
				points[i].color = "Black";
				break;
			case 1:
				Canvas->Pen->Color=clGreen;
				points[i].color = "Green";
				break;
			case 2:
				Canvas->Pen->Color=clLime;
				points[i].color = "Lime";
				break;
			case 3:
				Canvas->Pen->Color=clBlue;
				points[i].color = "Blue";
				break;
			case 4:
				Canvas->Pen->Color=clYellow;
				points[i].color = "Yellow";
				break;
			case 5:
				Canvas->Pen->Color=clPurple;
				points[i].color = "Purple";
				break;
			case 6:
				Canvas->Pen->Color=clAqua;
				points[i].color = "Aqua";
				break;
			case 7:
				Canvas->Pen->Color=clOlive;
				points[i].color = "Olive";
				break;
			case 8:
				Canvas->Pen->Color=clMaroon;
				points[i].color = "Maroon";
				break;
			}

			Canvas->MoveTo(points[i].x,points[i].y);
			Canvas->LineTo(points[i].x,points[i].y);


		}

		 flag = true;
		 for (int i = 0; i < k; i++) {
			 if (lastx[i] != clusters[i].x) {
				flag = false;
				lastx[i] = clusters[i].x;
			 }
			 if (lasty[i] != clusters[i].y) {
				flag = false;
				lasty[i] = clusters[i].y;
			 }
		 }

		 //Sleep(200);

	} while (!flag);


}
//---------------------------------------------------------------------------
