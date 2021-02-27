#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <limits>  
#pragma warning(disable:4996) 
using namespace std;

typedef class Constraint {
public:
	float a, b, c;//a為x的係數、b為y的係數、c為常數
	float slope;
	bool deletion;//用來記是不是等等要被刪掉的線
};

typedef class Pair {
public:
	float a1, b1, c1, index1;	//pair第一條線的係數
	float slope1;	//pair第一條線的斜率
	float a2, b2, c2, index2; //pair第二條線的係數
	float slope2;	//pair第一條線的斜率
	int type;		//這個pair隸屬於I+(1) or I-(0)
	float intersection_x, intersection_y;	//兩個constraint的交點（直接用closed form, 有一點誤差）
};

void classify_I1_I0(int input_number, float& u1, float& u2, vector<Constraint>& constraint_I1, vector<Constraint>& constraint_I0, string& result, bool& change) {
	for (int i = 0; i < input_number; i++) {
		Constraint temp;
		temp.deletion = false;
		scanf("%f%f%f", &temp.a, &temp.b, &temp.c);
		if (temp.b != 0) {
			temp.slope = -temp.a / temp.b;
			if (temp.a < 0) {
				if (temp.b > 0)	constraint_I1.push_back(temp);
				else if (temp.b < 0)	constraint_I0.push_back(temp);
			}
			else if (temp.a > 0) {
				if (temp.b > 0)	constraint_I1.push_back(temp);
				else if (temp.b < 0)	constraint_I0.push_back(temp);
			}
			else {
				if (temp.b < 0)		constraint_I0.push_back(temp);
				else constraint_I1.push_back(temp);
			}
		}
		else {
			if (temp.a == 0) {
				if (temp.c < 0) {
					result = "NA";
					change = true;
					return;
				}
				else {
					continue;
				}
			}
			else {
				if (temp.a < 0) {
					u1 = temp.c / temp.a;
				}
				else {
					u2 = temp.c / temp.a;
				}
			}
		}
	}
}

bool is_odd(int x) {
	return ((x + 1) / 2 != (x / 2));
}

float line_line_intersectionx(Constraint c1, Constraint c2) {
	return (c1.c * c2.b - c2.c * c1.b) / (c1.a * c2.b - c2.a * c1.b);	// (c1b2 - c2b1) / (a1b2 - a2b1)
}

float line_line_intersectiony(Constraint c1, Constraint c2) {
	return (c2.a * c1.c - c1.a * c2.c) / (c2.a * c1.b - c1.a * c2.b);	// (a2c1 - a1c2) / (a2b1 - a1b2)
}

float line_constraint_intersectiony(Constraint c, float x) {
	return (-c.a * x + c.c) / c.b;	//(-a * Xm + c) / b
}

void pair_constraints(float u1, float u2, vector<Constraint>& constraint_I1, vector<Constraint>& constraint_I0, vector<Pair>& pair) {
	int c1 = constraint_I1.size();
	int c2 = constraint_I0.size();
	if (is_odd(constraint_I1.size()))	c1--;
	if (is_odd(constraint_I0.size()))	c2--;
	for (int i = 0; i < c1; i = i + 2) {
		//兩條線平行可以直接刪掉一條的特殊狀況，刪掉c較大的
		if (constraint_I1[i].slope == constraint_I1[i + 1].slope) {
			if (constraint_I1[i].c > constraint_I1[i + 1].c) {
				constraint_I1[i].deletion = true;
			}
			else {
				constraint_I1[i + 1].deletion = true;
			}
			continue;
		}
		Pair temp;
		temp.a1 = constraint_I1[i].a;
		temp.b1 = constraint_I1[i].b;
		temp.c1 = constraint_I1[i].c;
		temp.index1 = i;
		temp.slope1 = constraint_I1[i].slope;
		temp.a2 = constraint_I1[i + 1].a;
		temp.b2 = constraint_I1[i + 1].b;
		temp.c2 = constraint_I1[i + 1].c;
		temp.slope2 = constraint_I1[i + 1].slope;
		temp.index2 = i + 1;
		temp.type = 1;
		temp.intersection_x = (temp.c1 * temp.b2 - temp.c2 * temp.b1) / (temp.a1 * temp.b2 - temp.a2 * temp.b1);	// (c1b2 - c2b1) / (a1b2 - a2b1)
		//兩條線交點在我們限定的u1, u2界線外（x不屬於u1-u2），可以直接刪掉一條的特殊情況
		// X* < Xm
		if (temp.intersection_x > u2) {
			//對於I+的pair來說，刪掉每一對中斜率小的那個constraint
			if (temp.slope1 < temp.slope2) {
				int temp_index = temp.index1;
				constraint_I1[temp_index].deletion = true;
				continue;
			}
			else {
				int temp_index = temp.index2;
				constraint_I1[temp_index].deletion = true;
				continue;
			}
		}
		// Xm < X*
		//對於I+的pair來說，刪掉每一對中斜率大的那個constraint
		else if(temp.intersection_x < u1){
			if (temp.slope1 > temp.slope2) {
				int temp_index = temp.index1;
				constraint_I1[temp_index].deletion = true;
				continue;
			}
			else {
				int temp_index = temp.index2;
				constraint_I1[temp_index].deletion = true;
				continue;
			}
		}
		temp.intersection_y = (temp.a2 * temp.c1 - temp.a1 * temp.c2) / (temp.a2 * temp.b1 - temp.a1 * temp.b2);	// (a2c1 - a1c2) / (a2b1 - a1b2) 
		pair.push_back(temp);
	}
	for (int i = 0; i < c2; i = i + 2) {
		//兩條線平行可以直接刪掉一條的特殊狀況，刪掉c較小的
		if (constraint_I0[i].slope == constraint_I0[i + 1].slope) {
			if (constraint_I0[i].c < constraint_I0[i + 1].c) {
				constraint_I0[i].deletion = true;
			}
			else {
				constraint_I0[i + 1].deletion = true;
			}
			continue;
		}
		Pair temp;
		temp.a1 = constraint_I0[i].a;
		temp.b1 = constraint_I0[i].b;
		temp.c1 = constraint_I0[i].c;
		temp.index1 = i;
		temp.slope1 = constraint_I0[i].slope;
		temp.a2 = constraint_I0[i + 1].a;
		temp.b2 = constraint_I0[i + 1].b;
		temp.c2 = constraint_I0[i + 1].c;
		temp.index2 = i + 1;
		temp.slope2 = constraint_I0[i + 1].slope;
		temp.type = 0;
		temp.intersection_x = (temp.c1 * temp.b2 - temp.c2 * temp.b1) / (temp.a1 * temp.b2 - temp.a2 * temp.b1);
		//兩條線交點在我們限定的u1, u2界線外（x不屬於u1-u2），可以直接刪掉一條的特殊情況
		// X* < Xm
		if (temp.intersection_x > u2) {
			//對於I-的pair來說，刪掉每一對中斜率大的那個constraint
			if (temp.slope1 > temp.slope2) {
				int temp_index = temp.index1;
				constraint_I0[temp_index].deletion = true;
				continue;
			}
			else {
				int temp_index = temp.index2;
				constraint_I0[temp_index].deletion = true;
				continue;
			}
		}
		// Xm < X*
		else if(temp.intersection_x < u1){
			//對於I-的pair來說，刪掉每一對中斜率小的那個constraint
			if (temp.slope1 < temp.slope2) {
				int temp_index = temp.index1;
				constraint_I0[temp_index].deletion = true;
				continue;
			}
			else {
				int temp_index = temp.index2;
				constraint_I0[temp_index].deletion = true;
				continue;
			}
		}
		temp.intersection_y = (temp.a2 * temp.c1 - temp.a1 * temp.c2) / (temp.a2 * temp.b1 - temp.a1 * temp.b2);
		pair.push_back(temp);
	}
}

bool comp_intersectionx(Pair pair1, Pair pair2) {
	return(pair1.intersection_x < pair2.intersection_x);
}

void prune_constraints(float& u1, float& u2, vector<Constraint>& constraint_I1, vector<Constraint>& constraint_I0, string& result, bool& change) {
/*	cout << "\n\nu1: " << u1 << "\nu2: " << u2 << "\n\n";
	cout << "I+:\n";
	for (int i = 0; i < constraint_I1.size(); i++) {
		cout << constraint_I1[i].a << "x+" << constraint_I1[i].b << "y <= " << constraint_I1[i].c << "\n";
		cout << "slope: " << constraint_I1[i].slope << "\n";
	}
	cout << "I-:\n";
	for (int i = 0; i < constraint_I0.size(); i++) {
		cout << constraint_I0[i].a << "x+" << constraint_I0[i].b << "y <=" << constraint_I0[i].c << "\n";
		cout << "slope: " << constraint_I0[i].slope << "\n";
	}*/
	//step2 開始
	vector<Pair> pair;

	//把I+, I-各自兩兩配成一對（如果不是2的倍數就剩下來不用配對）
	pair_constraints(u1, u2, constraint_I1, constraint_I0, pair);
/*	cout << "\npair: \n\n";
	for (int i = 0; i < pair.size(); i++) {
		cout << "first constraint: " << pair[i].a1 << "x+" << pair[i].b1 << "y <= " << pair[i].c1 << "\n";
		cout << "slope: " << pair[i].slope1 << "\n";
		cout << "second constraint: " << pair[i].a2 << "x+" << pair[i].b2 << "y <= " << pair[i].c2 << "\n";
		cout << "slope: " << pair[i].slope2 << "\n";
		cout << "type: " << pair[i].type << "\n";
		cout << "intersection: " << pair[i].intersection_x << ", " << pair[i].intersection_y << "\n";
	}
	*/


	//用交點的x座標作為比較值，找出pair裡面交點x座標的median（median的前面都比他小，後面都比他大，但那兩段各自沒有排序）
	nth_element(pair.begin(), pair.begin() + pair.size() / 2, pair.end(), comp_intersectionx);
/*	cout << "\npair: \n\n";
	for (int i = 0; i < pair.size(); i++) {
		cout << "first constraint: " << pair[i].a1 << "x+" << pair[i].b1 << "y <= " << pair[i].c1 << "\n";
		cout << "slope: " << pair[i].slope1 << "\n";
		cout << "second constraint: " << pair[i].a2 << "x+" << pair[i].b2 << "y <= " << pair[i].c2 << "\n";
		cout << "slope: " << pair[i].slope2 << "\n";
		cout << "type: " << pair[i].type << "\n";
		cout << "intersection: " << pair[i].intersection_x << ", " << pair[i].intersection_y << "\n";
	}
	cout << "pair size: " << pair.size() << "\n";*/
	//step2 結束

	//step3 開始
	if (pair.size() != 0) {
		float alphax = pair[pair.size() / 2].intersection_x;	//alphax就是Xm（median of x）
	//alphay定義是I-的constraint裡面，(-a * Xm + c) / b 最大的那個
		float alphay = -numeric_limits<float>::max();
		//用來記alphay最大值的index，等等要把這些點蒐集起來算斜率max, min
		vector<int> alpha_index;
		for (int i = 0; i < constraint_I0.size(); i++) {
			float temp = line_constraint_intersectiony(constraint_I0[i], alphax);
			if (temp > alphay + 0.05) {
				alpha_index.clear();
				alpha_index.push_back(i);
				alphay = temp;
			}
			else if (temp < alphay + 0.05 && temp >= alphay - 0.05 ) {
				alpha_index.push_back(i);
			}
		}

		float betax = pair[pair.size() / 2].intersection_x;	//betax就是Xm（median of x）
		//betay定義是I+的constraint裡面，(-a * Xm + c) / b 最小的那個
		float betay = numeric_limits<float>::max();
		//用來記betay最小值的index，等等要把這些點蒐集起來算斜率max, min
		vector<int> beta_index;
		for (int i = 0; i < constraint_I1.size(); i++) {
			float temp = line_constraint_intersectiony(constraint_I1[i], betax);
			if (temp < betay - 0.05) {
				beta_index.clear();
				beta_index.push_back(i);
				betay = temp;
			}
			else if (temp > betay - 0.05 && temp <= betay + 0.05) {
				beta_index.push_back(i);
			}
		}
//		cout << "alpha_index: " << alpha_index.size() << "\n";
		//所有過alpha這個點的線，用上面記的index去看那些線的斜率，找斜率max和min
		float smax = constraint_I0[alpha_index[0]].slope;
		float smin = constraint_I0[alpha_index[0]].slope;
		for (int i = 0; i < alpha_index.size(); i++) {
			if (constraint_I0[alpha_index[i]].slope > smax)	smax = constraint_I0[alpha_index[i]].slope;
			if (constraint_I0[alpha_index[i]].slope < smin)	smin = constraint_I0[alpha_index[i]].slope;
		}
		//所有過beta這個點的線，用上面記的index去看那些線的斜率，找斜率max和min
		float tmax = constraint_I1[beta_index[0]].slope;
		float tmin = constraint_I1[beta_index[0]].slope;
		for (int i = 0; i < beta_index.size(); i++) {
			if (constraint_I1[beta_index[i]].slope > tmax)	tmax = constraint_I1[beta_index[i]].slope;
			if (constraint_I1[beta_index[i]].slope < tmin)	tmin = constraint_I1[beta_index[i]].slope;
		}
//		cout << "alphay: " << alphay << "\nbetay: " << betay << "\n";
//		cout << "smax: " << smax << "\nsmin: " << smin << "\ntmax: " << tmax << "\ntmin " << tmin << "\n";
		//step3 結束

		//step4 開始
		int direction;	//用來記錄 X* 在 Xm 的哪邊（-1 為 X* < Xm, 0 為 X* = Xm, 1為 Xm < X*）
		if (alphay <= betay) {
			if (smax < 0) {
				u1 = alphax;
				direction = 1;
			}
			else if (smin > 0) {
				u2 = alphax;
				direction = -1;
			}
			//這裡找到 X* 了！
			else {
				change = true;
				result = to_string(int(round(alphay)));
				return;
			}
		}
		else {
			if (smax < tmin) {
				u1 = alphax;
				direction = 1;
			}
			else if (smin > tmax) {
				u2 = alphax;
				direction = -1;
			}
			//這裡return前沒有把change設成true，代表這是無解的return
			else {
				change = true;
				result = "NA";
				return;
			}
		}
		//step4 結束

		//step5 開始
		// X* < Xm 的話，要往交點 > Xm的那頭pair刪掉constraint
		if (direction == -1) {
			for (int i = pair.size() / 2; i < pair.size(); i++) {
				//對於I-的pair來說，刪掉每一對中斜率大的那個constraint
				if (pair[i].type == 0) {
					if (pair[i].slope1 > pair[i].slope2) {
						int temp_index = pair[i].index1;
						constraint_I0[temp_index].deletion = true;
					}
					else {
						int temp_index = pair[i].index2;
						constraint_I0[temp_index].deletion = true;
					}
				}
				//對於I+的pair來說，刪掉每一對中斜率小的那個constraint
				else if (pair[i].type == 1) {
					if (pair[i].slope1 < pair[i].slope2) {
						int temp_index = pair[i].index1;
						constraint_I1[temp_index].deletion = true;
					}
					else {
						int temp_index = pair[i].index2;
						constraint_I1[temp_index].deletion = true;
					}
				}
			}
		}
		else {
			for (int i = 0; i < pair.size() / 2 + 1; i++) {
				//對於I-的pair來說，刪掉每一對中斜率小的那個constraint
				if (pair[i].type == 0) {
					if (pair[i].slope1 < pair[i].slope2) {
						int temp_index = pair[i].index1;
						constraint_I0[temp_index].deletion = true;
					}
					else {
						int temp_index = pair[i].index2;
						constraint_I0[temp_index].deletion = true;
					}
				}
				//對於I+的pair來說，刪掉每一對中斜率大的那個constraint
				else if (pair[i].type == 1) {
					if (pair[i].slope1 > pair[i].slope2) {
						int temp_index = pair[i].index1;
						constraint_I1[temp_index].deletion = true;
					}
					else {
						int temp_index = pair[i].index2;
						constraint_I1[temp_index].deletion = true;
					}
				}
			}
		}
	}
	
	for (int i = constraint_I0.size() - 1; i >= 0; i--) {
		if (constraint_I0[i].deletion == true) {
			constraint_I0.erase(constraint_I0.begin() + i);
		}
	}
	for (int i = constraint_I1.size() - 1; i >= 0; i--) {
		if (constraint_I1[i].deletion == true) {
			constraint_I1.erase(constraint_I1.begin() + i);
		}
	}
/*	cout << "\n\n\nafter erase\n";
	cout << "I+:\n";
	for (int i = 0; i < constraint_I1.size(); i++) {
		cout << constraint_I1[i].a << "x+" << constraint_I1[i].b << "y <= " << constraint_I1[i].c << "\n";
		cout << "slope: " << constraint_I1[i].slope << "\n";
	}
	cout << "I-:\n";
	for (int i = 0; i < constraint_I0.size(); i++) {
		cout << constraint_I0[i].a << "x+" << constraint_I0[i].b << "y <=" << constraint_I0[i].c << "\n";
		cout << "slope: " << constraint_I0[i].slope << "\n";
	}*/
	//step5 結束

	//step6 開始（這是下面遞迴的步驟，但為了先判斷constraint夠不夠就拉到上面）
	//繼續遞迴 step2-5，直到剩下的 constraint <= 3，就可以跳出然後暴力解了
	if (constraint_I0.size() + constraint_I1.size() > 2) prune_constraints(u1, u2, constraint_I1, constraint_I0, result, change);
	return;
	//step6 結束
}

int main() {
	float u1 = -numeric_limits<float>::max();	//u1和u2為負正無限大，界定我們可行解的範圍，用意在於等等如果出現垂直線的constraint，可以用來更新左右界線u1、u2
	float u2 = numeric_limits<float>::max();
	int input_number;
	scanf("%d", &input_number);

	vector<Constraint> constraint_I1 ;	//I1就是講義的I+
	vector<Constraint> constraint_I0 ;	//I0就是講義的I-
	string result;
	bool change = false;

	//step1 開始
	//把所有constraint分成I+, I-兩類
	if (!change) {
		classify_I1_I0(input_number, u1, u2, constraint_I1, constraint_I0, result, change);
	}
	
	//step1 結束

	
	if (constraint_I0.size() + constraint_I1.size() > 2)	prune_constraints(u1, u2, constraint_I1, constraint_I0, result, change);

//	cout << "I0 size: " << constraint_I0.size() << "\n";
//	cout << "I1 size: " << constraint_I1.size() << "\n";
	//如果 change = true，代表我們遞迴裡面找到最佳解了，就可以直接輸出答案
	if (change) {
		cout << result;
	}
	//如果 change = false，代表遞迴結束在 constraint < 3，我們用暴力解剩下的 constraint
	else {
		//先考慮兩條線都是I-
		if (constraint_I0.size() == 2) {
			//如果兩條都是水平線
			if (constraint_I0[0].slope == 0 && constraint_I0[1].slope == 0) {
				cout << round(max(constraint_I0[0].c / constraint_I0[0].b, constraint_I0[1].c / constraint_I0[1].b));
			}
			//如果兩條線斜率相同符號
			else if (constraint_I0[0].slope * constraint_I0[1].slope > 0) {
				//斜率都是正的話，如果左邊的界線u1沒有到-無限大，那最小值就是u1與兩條線交點最小的那個，否則-INF
				if (constraint_I0[0].slope > 0) {
					if (u1 > -numeric_limits<float>::max()) cout << round(min(line_constraint_intersectiony(constraint_I0[0], u1), line_constraint_intersectiony(constraint_I0[1], u1)));
					else cout << "-INF";
				}
				//斜率都是負的話，如果右邊的界線u2沒有到+無限大，那最小值就是u2與兩條線交點最小的那個，否則-INF
				else {
					if (u2 < numeric_limits<float>::max()) cout << round(min(line_constraint_intersectiony(constraint_I0[0], u2), line_constraint_intersectiony(constraint_I0[1], u2)));
					else cout << "-INF";
				}
			}
			//如果兩條線斜率不同，解為它們的交點
			else cout << round(line_line_intersectiony(constraint_I0[1], constraint_I0[0]));
		}
		//一條I+，一條I-
		else if (constraint_I0.size() == 1 && constraint_I1.size() == 1) {
			//平行的情況
			if (constraint_I0[0].slope == constraint_I1[0].slope) {
				//如果I+在下面，I-在上面，就不會有可行解
				if (constraint_I1[0].c < constraint_I0[0].c) {
					cout << "NA";
				}
				//如果I+在上面，I-在下面，且它們兩個都是水平線時，解為I-
				else if (constraint_I1[0].slope == 0) {
					cout << round(constraint_I0[0].c / constraint_I0[0].b);
				}
				//如果I+在下面，I+在上面，要看斜率跟u1, u2來決定
				else {
					//如果斜率是正的，先看u1是不是-無限大，不是的話，解為兩條線與u1交點較小的那個，否則解為-INF
					if (constraint_I0[0].slope > 0) {
						if (u1 > -numeric_limits<float>::max()) cout << round(min(line_constraint_intersectiony(constraint_I0[0], u1), line_constraint_intersectiony(constraint_I1[0], u1)));
						else cout << "-INF";
					}
					else {
						if (u2 < numeric_limits<float>::max()) cout << round(min(line_constraint_intersectiony(constraint_I0[0], u2), line_constraint_intersectiony(constraint_I1[0], u2)));
						else cout << "-INF";
					}
				}
			}
			//斜率不同的情況
			// I+斜率 > I-斜率
			else if (constraint_I1[0].slope > constraint_I0[0].slope) {
				// I+ > I- > 0
				if (constraint_I0[0].slope > 0) {
					//如果 u1 > 兩條線交點的 x，解為u1, I-交點
					if (u1 > line_line_intersectionx(constraint_I0[0], constraint_I1[0])) {
						cout << round(line_constraint_intersectiony(constraint_I0[0], u1));
					}
					//解為I+, I-兩條線交點
					else {
						cout << round(line_line_intersectiony(constraint_I0[0], constraint_I1[0]));
					}
				}
				// I+ > 0 > I-, 0 > I+ > I- 兩種情況相同
				else{
					if (u2 < numeric_limits<float>::max()) {
						cout << round(line_constraint_intersectiony(constraint_I0[0], u2));
					}
					else cout << "-INF";
				}
			}
			// I- > I+
			else {
				// 0 > I- > i+
				if (constraint_I0[0].slope < 0) {
					//如果 u2 < 兩條線交點的 x，解為u2, I-交點
					if (u2 < line_line_intersectionx(constraint_I0[0], constraint_I1[0])) {
						cout << round(line_constraint_intersectiony(constraint_I0[0], u2));
					}
					//解為I+, I-兩條線交點
					else {
						cout << round(line_line_intersectiony(constraint_I0[0], constraint_I1[0]));
					}
				}
				// I- > I+ > 0, I- > 0 > I+ 兩種情況相同
				else {
					if (u1 > -numeric_limits<float>::max()) {
						cout << round(line_constraint_intersectiony(constraint_I0[0], u1));
					}
					else cout << "-INF";
				}
			}
		}
		//兩條都是I+，解為-INF
		else if (constraint_I1.size() == 2) cout << "-INF";
		//剩下一條的情況
		else {
			//剩下的一條是I-
			if (constraint_I0.size() == 1) {
				if (constraint_I0[0].slope > 0) {
					if (u1 > -numeric_limits<float>::max()) {
						cout << round(line_constraint_intersectiony(constraint_I0[0], u1));
					}
					else cout << "-INF";
				}
				else {
					if (u2 < numeric_limits<float>::max()) {
						cout << round(line_constraint_intersectiony(constraint_I0[0], u2));
					}
					else cout << "-INF";
				}
			}
			else cout << "-INF";
		}
	}
}