#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define charac 8

int tut = 0;
int pixel_x, pixel_y, org_pixelx = 0, org_pixely = 0;
int number_width = 0, number_height = 0;
char type;
int cnt = 0, cnt1 = 0;


int d_number_prot[10][7] = { {1,1,1,0,1,1,1},{0,0,0,0,0,1,1,},{0,1,1,1,1,1,0},	//Digital rakamlar�n prototipi
							 {0,0,1,1,1,1,1},{1,0,0,1,0,1,1},{1,0,1,1,1,0,1},
							 {1,1,1,1,1,0,1},{0,0,1,0,0,1,1},{1,1,1,1,1,1,1},
							 {1,0,1,1,1,1,1} };
int d_number_temp[7] = { 0,0,0,0,0,0,0 };	//Digital rakamlar� 7 karede kontrol ediyorum
Mat numbers[charac];
Mat dv_pieces[charac][9];	//digital rakam� dikeyde 9'a b�l�yorum
Mat d_pieces[charac][81];	//digital rakam� yatayda 9'a b�l�yorum

char a_number_prot[10][15] = { {'b','w','b','a','a','w','b','w','a','a','b','w','b','a','a'},{'b','w','b','a','a','b','w','b','a','a','w','a','a','a','a'},
								{'b','w','b','w','a','w','b','w','b','w','b','w','b','w','a'},{'b','w','b','w','a','w','b','w','b','w','b','w','b','w','b'},
								{'b','w','b','a','a','b','w','b','w','b','w','a','a','a','a'},{'w','b','w','b','a','w','b','w','b','w','w','b','w','b','a'},
								{'b','w','b','a','a','w','b','w','b','w','b','w','b','w','a'},{'w','b','a','a','a','w','b','w','b','a','w','b','a','a','a'},
								{'b','w','b','a','a','w','b','w','b','w','b','w','b','a','a'},{'b','w','b','w','b','w','b','w','b','w','b','w','a','a','a'} };	//Analog 10 rakam var ve her bir rakam� 3e b�l�p 5 farkl� renk mi var diye bak�yorum.
Mat a_pieces[charac][3]; //analog rakamlar� dikeyde 3'e b�l�yorum.
char a_colors[charac * 3][5]; //Her bir par�ada en fazla 5 renk olabiliyor..
char color_temp[200];
int count_b = 0;
int count_w = 0;
int color_count = 0;;
int count_Apiece = 0;

Mat org_img;
char number_temp[20];
int temp, sum1, temp1;
string filepath;
int step_count = 0;


//Function Prototypes
int filters(Mat img, int pixel_x, int pixel_y);
void numberProcesses(Mat img);
void digitalNumbersDetect(Mat number, int i, int number_height, int number_width);
void analogNumbersDetect(Mat number, int i, int number_height, int number_width);
void resultNumber();

int main()
{
	cout << "Dosya Yolunu Giriniz.." << endl;
	cin >> filepath;

	cout << "Display'deki Rakamlarin Turunu Giriniz [Dijital -> D , Analog -> A]" << endl;
	cin >> type;

	//"D:/images/dene/fin/fin.jpeg"
	Mat _img = imread(filepath);

	pixel_x = (int)_img.cols;
	pixel_y = (int)_img.rows;

	org_pixelx = pixel_x;	//Filtreleme i�in tekrar g�nderdi�imde orjinal de�erler kaybolmam�� olsun diye.
	org_pixely = pixel_y;

	resize(_img, _img, Size(pixel_x * 3, pixel_y * 3));

	org_img = _img;		//Filtreleme i�in tekrar g�nderdi�imde orjinal image kaybolmam�� olsun diye.
	if (filters(_img, pixel_x, pixel_y)) {	//Filtreleme i�lemleri i�in benim olu�turdu�um fonksiyon.
		return 1;
	}
}
int filters(Mat img, int pixel_x, int pixel_y) {	//Filtreleme i�lemleri yap�l�yor.
	cvtColor(img, img, COLOR_BGR2GRAY);				//�mage renk uzay� griye �evriliyor b�ylece renk uzay� iki boyutlu olmu� oluyor.
	threshold(img, img, 50, 100000, THRESH_BINARY);	//�mage siyah beyaz yap�l�yor.

	if (tut == 0) {
		if (type == 'D') {
			erode(img, img, getStructuringElement(MORPH_ERODE, Size(2, 2)));	//A��nd�rma morfolojik i�lemi ger�ekle�tiriliyor.
		}
		else {
			erode(img, img, getStructuringElement(MORPH_ERODE, Size(7, 7)));	//A��nd�rma morfolojik i�lemi ger�ekle�tiriliyor.
		}
	}else if (tut == 1) {
		dilate(img, img, getStructuringElement(MORPH_ERODE, Size(3.5, 3.5)));	// Geni�letme morfolojik i�lemi ger�ekle�tiriliyor.
	}else if (tut == 2) {
		if (type == 'D') {
			erode(img, img, getStructuringElement(MORPH_ERODE, Size(7, 7)));	//A��nd�rma morfolojik i�lemi ger�ekle�tiriliyor.
		}
		else {
			erode(img, img, getStructuringElement(MORPH_ERODE, Size(2, 2)));	//A��nd�rma morfolojik i�lemi ger�ekle�tiriliyor.
		}
	}else if (tut == 3) {
		dilate(img, img, getStructuringElement(MORPH_ERODE, Size(2, 2)));	// Geni�letme morfolojik i�lemi ger�ekle�tiriliyor.
	}else {
		cout << "Deger tanimlanamadi" << endl;
		return 1;
	}
	count_Apiece = 0;
	numberProcesses(img);		//Crop i�lemlerini yapt���m fonksiyon.
	return 1;
}
void numberProcesses(Mat img) {		//T�m crop i�lemleri yap�l�yor.
	//Crop image
	Mat cropped_img;
	int temp_y = 0, temp_x = 0;
	for (int j = 0; j < img.rows; j++) {		//�stten itibaren ilk g�r�len beyaz pikselin x ve y koordinatlar� tutuluyor.
		for (int k = 0; k < img.cols; k++) {
			if ((int)img.at<uchar>(j, k) == 0) {
				temp_y = j;
				temp_x = k;
				break;
			}
		}
		if (temp_y)
			break;
	}
	img(Rect(Point(temp_x, temp_y), Point(pixel_x * 3, pixel_y * 3))).copyTo(cropped_img);		//Beyaz pikselin x ve y de�erlerine g�re image'e k�rpma i�lemi yap�l�yor.

	int height = (pixel_y * 3) - temp_y;		//K�rpma i�leminden sonraki image'in y�kseklik ve geni�lik de�erleri tutuluyor.
	int width = (pixel_x * 3) - temp_x;


	for (int j = 0; j < charac; j++) {	//Number_temp o anki rakam�n kontrol�n� yapt���m b�lgelerinin 1 veya 0 olmas�n� tutuyor. Burada i�ini bo�alt�yorum.
		number_temp[j] = 0;
	}

	for (int i = 0; i < charac; i++) { // Genel for d�ng�s� her bir rakam i�in t�m i�lemlerin yap�l�p sonra ikinci rakama ge�mesini sa�l�yor.
		cnt = i * (int)(width / charac);
		cnt1 = cnt + (int)(width / charac);

		cropped_img(Rect(Point(cnt, 0), Point(cnt1, height))).copyTo(numbers[i]);	//Crop i�lemi yap�lm�� image'in karakter say�s�na g�re rakam rakam ay�r�yor. 
		numbers[i](Rect(Point(10, 0), Point(width / charac, height))).copyTo(numbers[i]);

		temp_x = 0; temp_y = 0;

		for (int j = 0; j < numbers[i].rows; j++) {	//�stten ilk beyaz� al�p ona g�re sadece rakam�n �ste ve alta s�f�r olmas�n� sa�l�yorum.
			for (int k = 0; k < numbers[i].cols; k++) {
				if ((int)numbers[i].at<uchar>(j, k) != 0) {
					temp_y = j;
					break;
				}
			}
			if (temp_y)
				break;
		}
		numbers[i](Rect(Point(15, temp_y + 1), Point(width / charac - 30, height))).copyTo(numbers[i]);	//K���k bir crop i�lemi ger�ekle�tiriyorum bence hepsi i�in ge�erli oldu :)

		number_height = height - temp_y - 1;
		number_width = width / charac - 45;		//irem: oluyo oluyo 24/09/20 13:42. 3 tanede oldu

		temp_y = 0; temp_x = 0;
		for (int j = (int)numbers[i].rows - 1; j > 0; j--) {		//Alttan beyaz olana kadar k�rpma yapt�m.
			for (int k = (int)numbers[i].cols - 1; k > 0; k--) {
				if ((int)numbers[i].at<uchar>(j, k) != 0) {
					temp_y = j;
					temp_x = k;
					break;
				}
			}
			if (temp_x)
				break;
		}

		numbers[i](Rect(Point(0, 0), Point(number_width, temp_y))).copyTo(numbers[i]);
		number_height -= number_height - temp_y;

		if (type == 'D') {
			digitalNumbersDetect(numbers[i], i, number_height, number_width);	//Digital rakamlar�n detect edildi�i benim olu�turdu�um fonksiyon
		}
		else if (type == 'A') {
			temp_x = 0;
			for (int j = 0; j < numbers[i].cols; j++) {	//�stten ilk beyaz� al�p ona g�re sadece rakam�n �ste ve alta s�f�r olmas�n� sa�l�yorum.
				for (int k = 0; k < numbers[i].rows; k++) {
					if ((int)numbers[i].at<uchar>(k, j) != 0) {
						temp_x = j;
						break;
					}
				}
				if (temp_x)
					break;
			}
			numbers[i](Rect(Point(temp_x, 0), Point(number_width, number_height))).copyTo(numbers[i]);
			number_width -= temp_x;

			temp_x = 0;

			for (int j = (int)numbers[i].cols - 1; j > 0; j--) {
				for (int k = (int)numbers[i].rows - 1; k > 0; k--) {
					if ((int)numbers[i].at<uchar>(k, j) != 0) {
						temp_x = j;
						break;
					}
				}
				if (temp_x)
					break;
			}

			numbers[i](Rect(Point(0, 0), Point(temp_x, number_height))).copyTo(numbers[i]);
			number_width -= number_width - temp_x;

			analogNumbersDetect(numbers[i], i, number_height, number_width);	//Analog rakamlar�n detect edildi�i benim olu�turdu�um fonksiyon
		}
	}
}
void digitalNumbersDetect(Mat number, int i, int number_height, int number_width) {

	int sum = 0;
	cnt = 0; cnt1 = 0;
	for (int j = 0; j < 9; j++) {	//Digital rakam� dikeyde 9'a b�l�yorum
		number(Rect(Point(cnt, 0), Point(cnt + number_width / 9, number_height))).copyTo(dv_pieces[i][j]);
		cnt += number_width / 9;
	}

	cnt = 0; cnt1 = 0;
	for (int j = 0; j < 9; j++) {	//Her bir par�ay� bir de yatayda 9'a b�l�yorum
		for (int k = 0; k < 9; k++) {
			dv_pieces[i][j](Rect(Point(0, cnt), Point(number_width / 9, cnt + number_height / 9))).copyTo(d_pieces[i][cnt1]);
			cnt += number_height / 9;
			cnt1++;
		}
		cnt = 0;
	}

	//Find number
	//Sectigim alandaki pixellerin yar�s�ndan fazlas� siyahtan farkl� m� diye kontrol ediyorum ve d_number_temp dizisinde benim belirledi�im noktalardaki durumu tutuyorum.
	sum = d_pieces[i][2].rows * d_pieces[i][2].cols;
	temp = countNonZero(d_pieces[i][2]);
	sum1 = d_pieces[i][3].rows * d_pieces[i][3].cols;
	temp1 = countNonZero(d_pieces[i][3]);
	if (temp > sum * 0.5 || temp1 > sum1 * 0.5) {
		d_number_temp[0] = 1;
	}
	else {
		d_number_temp[0] = 0;
	}

	sum = d_pieces[i][6].rows * d_pieces[i][6].cols;
	temp = countNonZero(d_pieces[i][6]);
	sum1 = d_pieces[i][7].rows * d_pieces[i][7].cols;
	temp1 = countNonZero(d_pieces[i][7]);
	if (temp > sum * 0.5 || temp1 > sum1 * 0.5) {
		d_number_temp[1] = 1;
	}
	else {
		d_number_temp[1] = 0;
	}

	sum = d_pieces[i][36].rows * d_pieces[i][36].cols;
	temp = countNonZero(d_pieces[i][36]);
	sum1 = d_pieces[i][45].rows * d_pieces[i][45].cols;
	temp1 = countNonZero(d_pieces[i][45]);
	if (temp > sum * 0.5 || temp1 > sum1 * 0.5) {
		d_number_temp[2] = 1;
	}
	else {
		d_number_temp[2] = 0;
	}

	sum = d_pieces[i][40].rows * d_pieces[i][40].cols;
	temp = countNonZero(d_pieces[i][40]);
	sum1 = d_pieces[i][49].rows * d_pieces[i][49].cols;
	temp1 = countNonZero(d_pieces[i][49]);
	if (temp > sum * 0.5 || temp1 > sum1 * 0.5) {
		d_number_temp[3] = 1;
	}
	else {
		d_number_temp[3] = 0;
	}

	sum = d_pieces[i][44].rows * d_pieces[i][44].cols;
	temp = countNonZero(d_pieces[i][44]);
	sum1 = d_pieces[i][53].rows * d_pieces[i][53].cols;
	temp1 = countNonZero(d_pieces[i][53]);
	if (temp > sum * 0.5 || temp1 > sum1 * 0.5) {
		d_number_temp[4] = 1;
	}
	else {
		d_number_temp[4] = 0;
	}

	sum = d_pieces[i][74].rows * d_pieces[i][74].cols;
	temp = countNonZero(d_pieces[i][74]);
	sum1 = d_pieces[i][75].rows * d_pieces[i][75].cols;
	temp1 = countNonZero(d_pieces[i][75]);
	if (temp > sum * 0.5 || temp1 > sum1 * 0.5) {
		d_number_temp[5] = 1;
	}
	else {
		d_number_temp[5] = 0;
	}

	sum = d_pieces[i][78].rows * d_pieces[i][78].cols;
	temp = countNonZero(d_pieces[i][78]);
	sum1 = d_pieces[i][79].rows * d_pieces[i][79].cols;
	temp1 = countNonZero(d_pieces[i][79]);
	if (temp > sum * 0.5 || temp1 > sum1 * 0.5) {
		d_number_temp[6] = 1;
	}
	else {
		d_number_temp[6] = 0;
	}

	for (int j = 0; j < 10; j++) {	//d_number_temp dizisinde rakamlar�n o noktalardaki de�erini tutuyordum burada da o de�erlere sahip bir rakam var m� onu kontrol ediyorum. 
		if (d_number_prot[j][0] == d_number_temp[0] && d_number_prot[j][1] == d_number_temp[1] && d_number_prot[j][2] == d_number_temp[2] && d_number_prot[j][3] == d_number_temp[3] && d_number_prot[j][4] == d_number_temp[4] && d_number_prot[j][5] == d_number_temp[5] && d_number_prot[j][6] == d_number_temp[6]) {
			number_temp[i] = j;
			step_count++;

		}
	}
	for (int j = 0; j < 7; j++) {	//Bir sonraki rakam kontrol� i�in i�ini bo�alt�yorum.
		d_number_temp[j] = 0;
	}
	if (i == charac - 1) {
		if (step_count < charac - 1) {	//E�er karakter say�s� kadar rakam bulamad�ysam a��nd�rma i�lemi yanl�� olmu� yeniden filtreleme fonksiyonuma d�n�yorum. 
			tut++;
			filters(org_img, org_pixelx, org_pixely);	// Parametre olaran en ba�taki image ve pixel de�erlerinin bozulmam�� halini g�nderiyorum.
		}
		else {
			resultNumber();		// Benim yazd���m sonu� bulma fonksiyonum.
		}

	}
}
void analogNumbersDetect(Mat number, int i, int number_height, int number_width) {

	cnt = 0;
	for (int k = 0; k < 3; k++) {		//Rakamlar� dikeyde 3'e b�l�yor.
		numbers[i](Rect(Point(cnt, 0), Point(cnt + number_width / 3, number_height))).copyTo(a_pieces[i][k]);
		cnt += number_width / 3;
	}
	count_w = 0; count_b = 0;  color_count = 0;
	for (int j = 0; j < 3; j++) {	//Rakamlar i�in her sat�r�n �o�unluk rengini belirleyip dizide tutuyorum.
		for (int k = 0; k < a_pieces[i][j].rows; k++) {
			for (int l = 0; l < a_pieces[i][j].cols; l++) {
				if ((int)a_pieces[i][j].at<uchar>(k, l) != 0) {
					count_w++;
				}
				if ((int)a_pieces[i][j].at<uchar>(k, l) == 0) {
					count_b++;
				}
			}
			if (count_w >= a_pieces[i][j].cols * 0.5) {
				color_temp[color_count] = 'w';
				color_count++;

			}
			if (count_b > a_pieces[i][j].cols * 0.5) {
				color_temp[color_count] = 'b';
				color_count++;
			}
			if (color_count >= number_height) {
				color_count = 0;
				count_w = 0;
				count_b = 0;
				break;
			}
			count_w = 0;
			count_b = 0;
		}
		int temppp = 0;
		for (int m = 0; m < number_height; m++) {	//Blok blok hangi renklerde oldu�unu belirliyorum
			if (m == number_height - 1) {
				a_colors[count_Apiece][temppp] = color_temp[m];
				temppp++;
			}
			if (m != number_height - 1) {
				if ((color_temp[m] == 'b') && (color_temp[m + 1] == 'w')) {
					a_colors[count_Apiece][temppp] = color_temp[m];
					temppp++;
				}
				if ((color_temp[m] == 'w') && (color_temp[m + 1] == 'b')) {
					a_colors[count_Apiece][temppp] = color_temp[m];
					temppp++;
				}
			}
		}
		if (temppp < 5) {		//Her rakam en fazla 5 renkten olu�uyor daha az say�da renk blo�u var ise bo� bir harf veriyorum.
			for (int m = temppp; m < 5; m++) {
				a_colors[count_Apiece][m] = 'a';
			}

		}
		count_Apiece++;
		color_count = 0;
	}


	if (i == charac - 1) {
		resultNumber();
	}

}
void resultNumber() {
	if (type == 'A') {	//Analog rakamlarda belirledi�im renk bloklar�n� prototip ile kar��la�t�rarak rakam� buluyorum.
		int control = 0;
		step_count = 0;

		for (char i = 0; i < charac; ++i) {
			for (int j = 0; j < 10; j++) {
				for (int k = i * 3; k < 3 + i * 3; k++) {
					for (int l = 0; l < 5; l++) {
						if (a_colors[k][l] != a_number_prot[j][5 * (k % 3) + l]) {
							control++;
							break;
						}
					}
					if (control) {
						break;
					}
				}
				if (control == 0) {
					number_temp[i] = j;
					step_count++;
					break;
				}
				control = 0;
			}
		}
		if (charac == 8) {
			if ((step_count < charac - 2)) {	//E�er karakter say�s� kadar rakam bulamad�ysam a��nd�rma i�lemi yanl�� olmu� yeniden filtreleme fonksiyonuma d�n�yorum.
				tut++;
				filters(org_img, org_pixelx, org_pixely);	// Parametre olaran en ba�taki image ve pixel de�erlerinin bozulmam�� halini g�nderiyorum.
			}
			else {

				int result = 0;
				int us = charac - 1;
				for (int i = 0; i < charac; i++) {	// Number_temp dizisinde buldu�um rakamlar� tutuyordum. Burada o rakamlar� say� haline getiriyorum.
					result += number_temp[i] * pow(10, us);
					us--;
				}
				cout << "Result -> " << result << endl;	//Sonucu yazd�r�yorum.

			}
		}
		else {
			if ((step_count < charac - 1)) {	//E�er karakter say�s� kadar rakam bulamad�ysam a��nd�rma i�lemi yanl�� olmu� yeniden filtreleme fonksiyonuma d�n�yorum.
				tut++;
				filters(org_img, org_pixelx, org_pixely);	// Parametre olaran en ba�taki image ve pixel de�erlerinin bozulmam�� halini g�nderiyorum.
			}
			else {

				int result = 0;
				int us = charac - 1;
				for (int i = 0; i < charac; i++) {	// Number_temp dizisinde buldu�um rakamlar� tutuyordum. Burada o rakamlar� say� haline getiriyorum.
					result += number_temp[i] * pow(10, us);
					us--;
				}
				cout << "Result -> " << result << endl;	//Sonucu yazd�r�yorum.

			}
		}
	}
	if (type == 'D') {	//Sayac�m dijitalse direkt diziden sonucu yazd�r
		int result = 0;
		int us = charac - 1;
		for (int i = 0; i < charac; i++) {	// Number_temp dizisinde buldu�um rakamlar� tutuyordum. Burada o rakamlar� say� haline getiriyorum.
			result += number_temp[i] * pow(10, us);
			us--;
		}
		cout << "Result -> " << result << endl;	//Sonucu yazd�r�yorum.
	}

}