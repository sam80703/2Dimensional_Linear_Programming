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
	float a, b, c;//a��x���Y�ơBb��y���Y�ơBc���`��
	float slope;
	bool deletion;//�ΨӰO�O���O�����n�Q�R�����u
};

typedef class Pair {
public:
	float a1, b1, c1, index1;	//pair�Ĥ@���u���Y��
	float slope1;	//pair�Ĥ@���u���ײv
	float a2, b2, c2, index2; //pair�ĤG���u���Y��
	float slope2;	//pair�Ĥ@���u���ײv
	int type;		//�o��pair���ݩ�I+(1) or I-(0)
	float intersection_x, intersection_y;	//���constraint�����I�]������closed form, ���@�I�~�t�^
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
		//����u����i�H�����R���@�����S���p�A�R��c���j��
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
		//����u���I�b�ڭ̭��w��u1, u2�ɽu�~�]x���ݩ�u1-u2�^�A�i�H�����R���@�����S���p
		// X* < Xm
		if (temp.intersection_x > u2) {
			//���I+��pair�ӻ��A�R���C�@�襤�ײv�p������constraint
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
		//���I+��pair�ӻ��A�R���C�@�襤�ײv�j������constraint
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
		//����u����i�H�����R���@�����S���p�A�R��c���p��
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
		//����u���I�b�ڭ̭��w��u1, u2�ɽu�~�]x���ݩ�u1-u2�^�A�i�H�����R���@�����S���p
		// X* < Xm
		if (temp.intersection_x > u2) {
			//���I-��pair�ӻ��A�R���C�@�襤�ײv�j������constraint
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
			//���I-��pair�ӻ��A�R���C�@�襤�ײv�p������constraint
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
	//step2 �}�l
	vector<Pair> pair;

	//��I+, I-�U�ۨ��t���@��]�p�G���O2�����ƴN�ѤU�Ӥ��ΰt��^
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


	//�Υ��I��x�y�Ч@������ȡA��Xpair�̭����Ix�y�Ъ�median�]median���e������L�p�A�᭱����L�j�A������q�U�ۨS���Ƨǡ^
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
	//step2 ����

	//step3 �}�l
	if (pair.size() != 0) {
		float alphax = pair[pair.size() / 2].intersection_x;	//alphax�N�OXm�]median of x�^
	//alphay�w�q�OI-��constraint�̭��A(-a * Xm + c) / b �̤j������
		float alphay = -numeric_limits<float>::max();
		//�ΨӰOalphay�̤j�Ȫ�index�A�����n��o���I�`���_�Ӻ�ײvmax, min
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

		float betax = pair[pair.size() / 2].intersection_x;	//betax�N�OXm�]median of x�^
		//betay�w�q�OI+��constraint�̭��A(-a * Xm + c) / b �̤p������
		float betay = numeric_limits<float>::max();
		//�ΨӰObetay�̤p�Ȫ�index�A�����n��o���I�`���_�Ӻ�ײvmax, min
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
		//�Ҧ��Lalpha�o���I���u�A�ΤW���O��index�h�ݨ��ǽu���ײv�A��ײvmax�Mmin
		float smax = constraint_I0[alpha_index[0]].slope;
		float smin = constraint_I0[alpha_index[0]].slope;
		for (int i = 0; i < alpha_index.size(); i++) {
			if (constraint_I0[alpha_index[i]].slope > smax)	smax = constraint_I0[alpha_index[i]].slope;
			if (constraint_I0[alpha_index[i]].slope < smin)	smin = constraint_I0[alpha_index[i]].slope;
		}
		//�Ҧ��Lbeta�o���I���u�A�ΤW���O��index�h�ݨ��ǽu���ײv�A��ײvmax�Mmin
		float tmax = constraint_I1[beta_index[0]].slope;
		float tmin = constraint_I1[beta_index[0]].slope;
		for (int i = 0; i < beta_index.size(); i++) {
			if (constraint_I1[beta_index[i]].slope > tmax)	tmax = constraint_I1[beta_index[i]].slope;
			if (constraint_I1[beta_index[i]].slope < tmin)	tmin = constraint_I1[beta_index[i]].slope;
		}
//		cout << "alphay: " << alphay << "\nbetay: " << betay << "\n";
//		cout << "smax: " << smax << "\nsmin: " << smin << "\ntmax: " << tmax << "\ntmin " << tmin << "\n";
		//step3 ����

		//step4 �}�l
		int direction;	//�ΨӰO�� X* �b Xm ������]-1 �� X* < Xm, 0 �� X* = Xm, 1�� Xm < X*�^
		if (alphay <= betay) {
			if (smax < 0) {
				u1 = alphax;
				direction = 1;
			}
			else if (smin > 0) {
				u2 = alphax;
				direction = -1;
			}
			//�o�̧�� X* �F�I
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
			//�o��return�e�S����change�]��true�A�N��o�O�L�Ѫ�return
			else {
				change = true;
				result = "NA";
				return;
			}
		}
		//step4 ����

		//step5 �}�l
		// X* < Xm ���ܡA�n�����I > Xm�����Ypair�R��constraint
		if (direction == -1) {
			for (int i = pair.size() / 2; i < pair.size(); i++) {
				//���I-��pair�ӻ��A�R���C�@�襤�ײv�j������constraint
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
				//���I+��pair�ӻ��A�R���C�@�襤�ײv�p������constraint
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
				//���I-��pair�ӻ��A�R���C�@�襤�ײv�p������constraint
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
				//���I+��pair�ӻ��A�R���C�@�襤�ײv�j������constraint
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
	//step5 ����

	//step6 �}�l�]�o�O�U�����j���B�J�A�����F���P�_constraint�������N�Ԩ�W���^
	//�~�򻼰j step2-5�A����ѤU�� constraint <= 3�A�N�i�H���X�M��ɤO�ѤF
	if (constraint_I0.size() + constraint_I1.size() > 2) prune_constraints(u1, u2, constraint_I1, constraint_I0, result, change);
	return;
	//step6 ����
}

int main() {
	float u1 = -numeric_limits<float>::max();	//u1�Mu2���t���L���j�A�ɩw�ڭ̥i��Ѫ��d��A�ηN�b�󵥵��p�G�X�{�����u��constraint�A�i�H�Ψӧ�s���k�ɽuu1�Bu2
	float u2 = numeric_limits<float>::max();
	int input_number;
	scanf("%d", &input_number);

	vector<Constraint> constraint_I1 ;	//I1�N�O���q��I+
	vector<Constraint> constraint_I0 ;	//I0�N�O���q��I-
	string result;
	bool change = false;

	//step1 �}�l
	//��Ҧ�constraint����I+, I-����
	if (!change) {
		classify_I1_I0(input_number, u1, u2, constraint_I1, constraint_I0, result, change);
	}
	
	//step1 ����

	
	if (constraint_I0.size() + constraint_I1.size() > 2)	prune_constraints(u1, u2, constraint_I1, constraint_I0, result, change);

//	cout << "I0 size: " << constraint_I0.size() << "\n";
//	cout << "I1 size: " << constraint_I1.size() << "\n";
	//�p�G change = true�A�N��ڭ̻��j�̭����̨θѤF�A�N�i�H������X����
	if (change) {
		cout << result;
	}
	//�p�G change = false�A�N���j�����b constraint < 3�A�ڭ̥μɤO�ѳѤU�� constraint
	else {
		//���Ҽ{����u���OI-
		if (constraint_I0.size() == 2) {
			//�p�G������O�����u
			if (constraint_I0[0].slope == 0 && constraint_I0[1].slope == 0) {
				cout << round(max(constraint_I0[0].c / constraint_I0[0].b, constraint_I0[1].c / constraint_I0[1].b));
			}
			//�p�G����u�ײv�ۦP�Ÿ�
			else if (constraint_I0[0].slope * constraint_I0[1].slope > 0) {
				//�ײv���O�����ܡA�p�G���䪺�ɽuu1�S����-�L���j�A���̤p�ȴN�Ou1�P����u���I�̤p�����ӡA�_�h-INF
				if (constraint_I0[0].slope > 0) {
					if (u1 > -numeric_limits<float>::max()) cout << round(min(line_constraint_intersectiony(constraint_I0[0], u1), line_constraint_intersectiony(constraint_I0[1], u1)));
					else cout << "-INF";
				}
				//�ײv���O�t���ܡA�p�G�k�䪺�ɽuu2�S����+�L���j�A���̤p�ȴN�Ou2�P����u���I�̤p�����ӡA�_�h-INF
				else {
					if (u2 < numeric_limits<float>::max()) cout << round(min(line_constraint_intersectiony(constraint_I0[0], u2), line_constraint_intersectiony(constraint_I0[1], u2)));
					else cout << "-INF";
				}
			}
			//�p�G����u�ײv���P�A�Ѭ����̪����I
			else cout << round(line_line_intersectiony(constraint_I0[1], constraint_I0[0]));
		}
		//�@��I+�A�@��I-
		else if (constraint_I0.size() == 1 && constraint_I1.size() == 1) {
			//���檺���p
			if (constraint_I0[0].slope == constraint_I1[0].slope) {
				//�p�GI+�b�U���AI-�b�W���A�N���|���i���
				if (constraint_I1[0].c < constraint_I0[0].c) {
					cout << "NA";
				}
				//�p�GI+�b�W���AI-�b�U���A�B���̨�ӳ��O�����u�ɡA�Ѭ�I-
				else if (constraint_I1[0].slope == 0) {
					cout << round(constraint_I0[0].c / constraint_I0[0].b);
				}
				//�p�GI+�b�U���AI+�b�W���A�n�ݱײv��u1, u2�ӨM�w
				else {
					//�p�G�ײv�O�����A����u1�O���O-�L���j�A���O���ܡA�Ѭ�����u�Pu1���I���p�����ӡA�_�h�Ѭ�-INF
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
			//�ײv���P�����p
			// I+�ײv > I-�ײv
			else if (constraint_I1[0].slope > constraint_I0[0].slope) {
				// I+ > I- > 0
				if (constraint_I0[0].slope > 0) {
					//�p�G u1 > ����u���I�� x�A�Ѭ�u1, I-���I
					if (u1 > line_line_intersectionx(constraint_I0[0], constraint_I1[0])) {
						cout << round(line_constraint_intersectiony(constraint_I0[0], u1));
					}
					//�Ѭ�I+, I-����u���I
					else {
						cout << round(line_line_intersectiony(constraint_I0[0], constraint_I1[0]));
					}
				}
				// I+ > 0 > I-, 0 > I+ > I- ��ر��p�ۦP
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
					//�p�G u2 < ����u���I�� x�A�Ѭ�u2, I-���I
					if (u2 < line_line_intersectionx(constraint_I0[0], constraint_I1[0])) {
						cout << round(line_constraint_intersectiony(constraint_I0[0], u2));
					}
					//�Ѭ�I+, I-����u���I
					else {
						cout << round(line_line_intersectiony(constraint_I0[0], constraint_I1[0]));
					}
				}
				// I- > I+ > 0, I- > 0 > I+ ��ر��p�ۦP
				else {
					if (u1 > -numeric_limits<float>::max()) {
						cout << round(line_constraint_intersectiony(constraint_I0[0], u1));
					}
					else cout << "-INF";
				}
			}
		}
		//������OI+�A�Ѭ�-INF
		else if (constraint_I1.size() == 2) cout << "-INF";
		//�ѤU�@�������p
		else {
			//�ѤU���@���OI-
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