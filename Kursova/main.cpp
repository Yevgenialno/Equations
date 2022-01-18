#include <iostream>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <fstream>

#define ATTR1 96
#define ATTR2 97
#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_LEFT 75
#define KEY_ENTER 13
#define KEY_ESC 27

using namespace std;
char *m[] = {"Add equation", "Solve system", "Clear", "Help", "Exit"};
bool Globalvx = false, Globalvxd = false, flag = false;
HANDLE hstdout;
int button, act = 0, nEquation = 0, thisEmptyMn = 0;
const int level = 29524, level2 = 100, menu = 5;
char x[level][level2], equations[5][level2], answers[5][3][50], globalAns[3][50], temp2[50], NulMn, EmptyMn[10];

void draw(HANDLE h, COORD c, int k, int n, int len)
{
	SetConsoleTextAttribute(h, ATTR1);
	//system("cls");
	COORD cc = c;
	COORD c2 = {0, 1};
	for (int i = 0;i < n;i++)
	{
		cc.X=c.X + i*len;
		SetConsoleCursorPosition(h, cc);
		std::cout<<m[i];
	}
	cc.X = c.X + k*len;
	SetConsoleCursorPosition(h, c2);
	cout<<"Use left and right arrows to switch menu paragraph\nYour system:"<<'\n';
	for(int i = 0; i < nEquation; i++)
		std::cout<<equations[i]<<'\n';
	SetConsoleTextAttribute(h, ATTR2);
	SetConsoleCursorPosition(h, cc);
	std::cout<<m[k];
}

void getparts(int xx)
{
	int nskobok = 0;
	char *p, *end1, *end2, mn1[level2], mn2[level2], op;
	end1 = x[xx];
	p = x[xx] + 1;
	if(*end1 != '(')
	{
		mn1[0] = *end1;
		mn1[1] = '\0';
	}
	else
	{
		do
		{
			if(*end1 == '(')	nskobok++;
			if(*end1 == ')')	nskobok--;
			end1++;
		}
		while (nskobok);
		end1--;
		for (int i = 0; p != end1; p++, i++)
		{
			mn1[i] = *p;
			mn1[i + 1] = '\0';
		}
	}
	op = *(end1 + 1);
	end2 = end1 + 2;
	p = end2 + 1;
	if(*end2 != '(')
	{
		mn2[0] = *end2;
		mn2[1] = '\0';
		end2+=2;
	}
	else
	{
		do
		{
			if(*end2 == '(')	nskobok++;
			if(*end2 == ')')	nskobok--;
			end2++;
		}
		while (nskobok);
		end2--;
		for (int i = 0; p != end2; p++, i++)
		{
			mn2[i] = *p;
			mn2[i + 1] = '\0';
		}
	}
	strcpy(x[3*xx+1], mn1);
	x[3*xx+2][0] = op;
	strcpy(x[3*xx+3], mn2);
}

void fill(int h)
{
	for(int i = 1; i < level; i++)
		for(int j = 0; j < level2; j++)
			x[i][j] = '\0';
	for(int i = 0; i <= level; i++)
	{
		if(strlen(x[i]) > 1)
			getparts(i);
	}
}

void connect(int i)
{
	while(i != 0)
	{
		if(i%3 == 0) i-=2;
		for(int k = 0; k <= strlen(x[i/3]); k++)
			x[i/3][k] = '\0';
		if(strlen(x[i]) > 1) strcat(x[i/3], "(");
		strcat(x[i/3], x[i]);
		if(strlen(x[i]) > 1) strcat(x[i/3], ")");
		strcat(x[i/3], x[i+1]);
		if(strlen(x[i+2]) > 1) strcat(x[i/3], "(");
		strcat(x[i/3], x[i+2]);
		if(strlen(x[i+2]) > 1) strcat(x[i/3], ")");
		i=i/3;
	}
}

void unite(int j)
{
	for(int t = 0; x[0][t] != '\0'; t++)
		x[0][t] = '\0';
	if(strlen(answers[j][0]) > 2) strcat(x[0], answers[j][0]);
	if(strlen(answers[j][1]) > 5)
		strcat(x[0], answers[j][1]);
	else
		if(Globalvx == true) 
			if(strlen(answers[j][0]) > 2) strcat(x[0], "+X"); else strcat(x[0], "X");
		if(strlen(answers[j][2]) > 6)
			strcat(x[0], answers[j][2]);
		else
			if(Globalvxd == true)
				if((strlen(answers[j][0]) > 2)||(strlen(answers[j][2]) > 6)) strcat(x[0], "+X^"); else strcat(x[0], "X^");
	std::cout<<x[0]<<"=0\n";
}

void zakony(int j)
{
	for(int y = 0; y < 3; y++)
	{
		do
		{
			flag = false;
			if(strstr(answers[j][y], "0^"))
			{
				cout<<"Using identity 0^ = 1, we replace it\n";
				char *p = strstr(answers[j][y], "0^"), *p2;
				p2 = p+2;
				*p = '1';
				p++;
				while(*p2)
				{
					*p = *p2;
					p++;
					p2++;
				}
				*p = '\0';
				p++;
				*p = '\0';
				flag = true;
				unite(j);
			}
			if(strstr(answers[j][y], "1^"))
			{
				cout<<"Using identity 1^ = 0, we replace it\n";
				char *p = strstr(answers[j][y], "1^"), *p2;
				p2 = p+2;
				*p = '0';
				p++;
				while(*p2)
				{
					*p = *p2;
					p++;
					p2++;
				}
				*p = '\0';
				p++;
				*p = '\0';
				flag = true;
				unite(j);
			}
		} while(flag);
	}
	for(int y = 0; y < 3; y++)
	{
		do
		{
			flag = false;
			//universalni mezi
			if(strstr(answers[j][y], "0*"))
			{
				cout<<"Using law about universal boundaries 0 * (A) = 0, we replace it\n";
				char *p = strstr(answers[j][y], "0*")+1, *p2;
				p2 = p+2;
				if(*p2 == '^') p2++;
				while(*p2)
				{
					*p = *p2;
					p++;
					p2++;
				}
				*p = '\0';
				p++;
				*p = '\0';
				flag = true;
				unite(j);
			}
			if(strstr(answers[j][y], "*0"))
			{
				cout<<"Using universal boundaries (A) * 0 = 0, we replace it\n";
				char *p = strstr(answers[j][y], "*0")-1, *p2;
				p2 = p+3;
				if(*p == '^') p--;
				*p = '0';
				p++;
				while(*p2)
				{
					*p = *p2;
					p++;
					p2++;
				}
				*p = '\0';
				p++;
				*p = '\0';
				flag = true;
				unite(j);
			}
			if((strstr(answers[j][y], "+1+")) || (strstr(answers[j][y], "(1+")) || (strstr(answers[j][y], "+1)")))
			{
				cout<<"Using universal boundaries (A) + 1 = 1, we replace it\n";
				switch(y)
				{
					case 0: strcpy(answers[j][0], "(1)"); break;
					case 1: strcpy(answers[j][1], "X*(1)"); break;
					case 2: strcpy(answers[j][2], "X^*(1)"); break;
				}
				flag = true;
				unite(j);
			}
			//neitralnist
			if(strstr(answers[j][y], "0+"))
			{
				cout<<"Using neutrality 0 + (A) = (A), we replace it\n";
				char *p = strstr(answers[j][y], "0+"), *p2;
				p2 = p+2;
				while(*p2)
				{
					*p = *p2;
					p++;
					p2++;
				}
				*p = '\0';
				p++;
				*p = '\0';
				flag = true;
				unite(j);
			}
			if(strstr(answers[j][y], "+0"))
			{
				cout<<"Using neutrality (A) + 0 = (A), we replace it\n";
				char *p = strstr(answers[j][y], "+0"), *p2;
				p2 = p+2;
				while(*p2)
				{
					*p = *p2;
					p++;
					p2++;
				}
				*p = '\0';
				p++;
				*p = '\0';
				flag = true;
				unite(j);
			}
			if(strstr(answers[j][y], "1*"))
			{
				cout<<"Using neutrality 1 * (A) = (A), we replace it\n";
				char *p = strstr(answers[j][y], "1*"), *p2;
				p2 = p+2;
				while(*p2)
				{
					*p = *p2;
					p++;
					p2++;
				}
				*p = '\0';
				p++;
				*p = '\0';
				flag = true;
				unite(j);
			}
			if(strstr(answers[j][y], "*1"))
			{
				cout<<"Using neutrality (A) * 1 = (A), we replace it\n";
				char *p = strstr(answers[j][y], "*1"), *p2;
				p2 = p+2;
				while(*p2)
				{
					*p = *p2;
					p++;
					p2++;
				}
				*p = '\0';
				p++;
				*p = '\0';
				flag = true;
				unite(j);
			}
			//idempotentnist
			for(char *ch = answers[j][y]; *ch; ch++)
			{
				if((*ch == '*') && (((isalpha(*(ch-1))) && (isalpha(*(ch+1))) && ((*(ch+2)) != '^') && (*(ch+1)==*(ch-1))) || ((isalpha(*(ch-2))) && (isalpha(*(ch+1))) && ((*(ch-1)) == '^') && ((*(ch+2)) == '^') &&(*(ch-2)==*(ch+1)))))
				{
					cout<<"Using idempotence (A) * (A) = (A), we replace it\n";
					char *p = ch, *p2;
					p2 = p+2;
					if(*p2 == '^') p2++;
					while(*p2)
					{
						*p = *p2;
						p++;
						p2++;
					}
					*p = '\0';
					p++;
					*p = '\0';
					flag = true;
					unite(j);
				}
				if(((*ch == '+') && (*(ch-3) != '*') && (*(ch-2) != '*') && (*(ch+3) != '*') && (*(ch+2) != '*') && (isalpha(*(ch-1))) && (isalpha(*(ch+1))) && ((*(ch+2)) != '^') && (*(ch+1)==*(ch-1))) || ((isalpha(*(ch-2))) && (isalpha(*(ch+1))) && ((*(ch-1)) == '^') && ((*(ch+2)) == '^') &&(*(ch-2)==*(ch+1))))
				{
					cout<<"Using idempotence (A) + (A) = (A), we replace it\n";
					char *p = ch, *p2;
					p2 = p+2;
					if(*p2 == '^') p2++;
					while(*p2)
					{
						*p = *p2;
						p++;
						p2++;
					}
					*p = '\0';
					p++;
					*p = '\0';
					flag = true;
					unite(j);
				}
			}
			//dopovnenist
			for(char *q = answers[j][y]; *q; q++)
			{
				if((*q == '^') && (((*(q - 1) == *(q - 3)) && (isalpha(*(q - 1))) && (*(q - 2) == '*')) || ((*(q - 1) == *(q + 2)) && (isalpha(*(q - 1))) && (*(q + 1) == '*'))))
				{
					cout<<"Using complementarity (A) * (A)^ = 0, we replace it\n";
					char *p = q - 3;
					*p = '0';
					p++;
					q++;
					for(; *q; q++, p++)
						*p = *q;
					*p = '\0';
					*(p+1) = '\0';
					*(p+2) = '\0';
					*(p+3) = '\0';
					*(p+4) = '\0';
					unite(j);
					flag = true;
				}
			}
		} while(flag);	
	}
}

bool IsCorrect(char *q)
{
	const char operations[7] = "+*^-&=", symbols[11] = "+*^-&=()10";
	int n = 0, n0 = 0, nRavno;
	char s[2], s1[2], s0[2];
	if(strstr(operations, &*q)) return false;
	for(char *p = q; *p; p++)
	{
		s[0] = *p;
		s1[0] = *(p + 1);
		s0[0] = *(p - 1);
		if(*p == '(')
		{
			if(strstr(operations, s1)) return false;
			n++;
		}
		if(*p == ')')
		{
			if(strstr(operations, s0)) return false;
			n--;
		}
		if(n == 0) n0++;
		if(s[0] == '=')
		{
			nRavno++;
			n0 = 0;
			if(n) return false;
		}
		if((strstr(operations, s)) && (strstr(operations, s1)))
		if((nRavno == 2) || (n0 == 3) || (n < 0))
			return false;
		if(((s[0] < 'A') || (s[0] > 'Z')) && (strstr(symbols, s) == NULL)) return false;
	}
	return true;
}

int main(int argc, char** argv)
{
	system("title Solving system of equations");
	int l, kol = 0;
	const int len = 16;
	char *p, q, temp[level2], file[200];
	ifstream my1("help.txt", ios_base::in);
	hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = {0, 0}, my_pos = {0, 0};
	nEquation = 0;
	label1:
	SetConsoleTextAttribute(hstdout, ATTR1);
	system("cls");
	draw(hstdout, pos, act, menu, len);
	while(true)
	{
		if(kbhit())
		{
			SetConsoleTextAttribute(hstdout, ATTR1);
			while(act <= 0)
			{
				act = act+menu;
			}
			button = getch();
			if(button == 75)
			{
				act = (act - 1)%menu;
				draw(hstdout, pos, act, menu, len);
			}
			
			if(button == 77)
			{
				act = (act + 1)%menu;
				draw(hstdout, pos, act, menu, len);
			}
			
			if(button == 13)
			{
				act = act % menu;
				switch(act)
				{
					case 0:
						system("cls");
						cout<<"Legend:\n+ is union;\n* is intersection;\n^ is an addition;\n- is the difference;\n& is a symmetrical difference;\n0 is Empty set;\n1 is Universal set;\nX is Unknown set.\nEnter your equation:\n";
						fflush(stdin);
						cin>>equations[nEquation];
						if (IsCorrect(equations[nEquation]))
							nEquation++;
						else
						{
							for(char *p = equations[nEquation]; *p; p++)
								*p = '\0';
							cout<<"The equation was entered incorrectly.\nPress Enter to return to main menu";
							while(true)
							{
								if(kbhit())
								{
									button = getch();
									if(button == 13)
										goto label1;
								}
							}
						}
						goto label1;
						break;
					case 1:
						my_pos.Y = nEquation + 3;
						SetConsoleCursorPosition(hstdout, my_pos);
						for(int y = 0; y < 3; y++)
								for(int u = 0; u < 50; u++)
									globalAns[y][u] = '\0';
						for(int y = 0; y < 10; y++)
							EmptyMn[y] = '\0';
						for(int j = 0; j < nEquation; j++)
						{
							for(int w = 0; w < level; w++)
								for(int y = 0; y < level2; y++)
									x[w][y] = '\0';
							strcpy(x[0], equations[j]);
							for(int y = 0; y < 3; y++)
								for(int u = 0; u < 50; u++)
									answers[j][y][u] = '\0';
							answers[j][0][0] = '(';
							answers[j][1][0] = '+';
							answers[j][1][1] = 'X';
							answers[j][1][2] = '*';
							answers[j][1][3] = '(';
							answers[j][2][0] = '+';
							answers[j][2][1] = 'X';
							answers[j][2][2] = '^';
							answers[j][2][3] = '*';
							answers[j][2][4] = '(';
							cout<<"Equation number "<<j+1<<":\n";
							cout<<x[0]<<"\n";
							cout<<"Using the equivalence (A(X) = B(X)) <=> (A(X) & B(X) = 0), we transform the equation to equation, that has empty set in its right part\n";
							p = &x[0][0];
							temp[0] = '(';
							for(l = 1; *p != '='; p++, l++)
								temp[l] = *p;
							*(temp + l) = ')';
							l++;
							*(temp + l) = '&';
							l++;
							*(temp + l) = '(';
							l++;
								p++;
							for(; *p != '\0'; p++, l++)
								temp[l] = *p;
							*(temp + l) = ')';
							*(temp + l + 1) = '\0';
							l = 1;
							strcpy(x[0], temp);
							cout<<x[0]<<"=0\n";
							fill(j);
							cout<<"Using the definition of symmetric difference (A & B) = (A - B) + (B - A), we replace it\n";
							for(long int i = 2; i < level - 3; i+=3) 
							{
								if(x[i][0] == '&') //ubiraem &
								{
									for(int k = 0; k <= strlen(x[i/3]); k++)
										x[i/3][k] = '\0';
									strcat(x[i/3], "(");
									if(strlen(x[i-1]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[i-1]);
									if(strlen(x[i-1]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], "-");
									if(strlen(x[i+1]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[i+1]);
									if(strlen(x[i+1]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], ")+(");
									if(strlen(x[i+1]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[i+1]);
									if(strlen(x[i+1]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], "-");
									if(strlen(x[i-1]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[i-1]);
									if(strlen(x[i-1]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], ")");
									i=i/3;
									connect(i);
									i = 2;
									std::cout<<x[0]<<"=0\n";
									fill(j);
								}
							}
							cout<<"Using the definition of difference (A - B) = (A * (B^)), we replace it\n";
							for(long int i = 2; i < level - 3; i+=3) 
							{
								if(x[i][0] == '-') //ubiraem -
								{
									for(int k = 0; k <= strlen(x[i/3]); k++)
										x[i/3][k] = '\0';
									if(strlen(x[i-1]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[i-1]);
									if(strlen(x[i-1]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], "*");
									strcat(x[i/3], "(");
									if(strlen(x[i+1]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[i+1]);
									if(strlen(x[i+1]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], "^)");
									i=i/3;
									connect(i);
									i = 2;
									std::cout<<x[0]<<"=0\n";
									fill(j);
								}
							}
							cout<<"Using de Morgan's law (A + B)^ = ((A^) * (B^)); (A * B)^ = ((A^) + (B^)) and involutiveness (A^)^ = A, we replace external additions\n";
							for(long int i = 2; i < level - 3; i+=3) 
							{
								if((x[i][0] == '^') && (strlen(x[i-1]) > 1)) //ubiraem ()^
								{
									i--;
									if(x[3*i+2][0] == '^')
									{
										strcpy(x[i], x[3*i + 1]);
									}
									else
									{
										if(x[3*i+2][0] == '+') x[3*i+2][0] = '*';
										else x[3*i+2][0] = '+';
										x[i][0] = '\0';
										strcat(x[i], "(");
										if(strlen(x[3*i+1]) > 1) strcat(x[i], "(");
										strcat(x[i], x[3*i+1]);
										if(strlen(x[3*i+1]) > 1) strcat(x[i], ")");
										strcat(x[i], "^");
										strcat(x[i], ")");
										strcat(x[i], x[3*i+2]);
										strcat(x[i], "(");
										if(strlen(x[3*i+3]) > 1) strcat(x[i], "(");
										strcat(x[i], x[3*i+3]);
										if(strlen(x[3*i+3]) > 1) strcat(x[i], ")");
										strcat(x[i], "^");
										strcat(x[i], ")");
									}
									strcpy(x[i/3], x[i]);
									i = i/3;
									connect(i);
									i = 2;
									std::cout<<x[0]<<"=0\n";
									fill(j);
								}
							}
							cout<<"using distributive law (A * (B + C)) = ((A * B) + (A * C)), we replace external intersections\n";
							for(int i = 2; i <= (level-3)/3; i+=3) //ubiraem vneshnie peresecheniya
							{
								if((x[i][0] == '*')&&(x[3*(i-1)+2][0] == '+'))
								{
									for(int k = 0; k < level2; k++)
										x[i/3][k] = '\0';
									strcat(x[i/3], "(");
									if(strlen(x[i+1]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[i+1]);
									if(strlen(x[i+1]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], "*");
									if(strlen(x[3*i-2]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[3*i-2]);
									if(strlen(x[3*i-2]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], ")");
									strcat(x[i/3], "+");
									strcat(x[i/3], "(");
									if(strlen(x[i+1]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[i+1]);
									if(strlen(x[i+1]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], "*");
									if(strlen(x[3*i]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[3*i]);
									if(strlen(x[3*i]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], ")");
									i=i/3;
									connect(i);
									i = -1;
									std::cout<<x[0]<<"=0\n";
									fill(j);
									continue;
								}
								if((x[i][0] == '*')&&(x[3*(i+1)+2][0] == '+'))
								{
									for(int k = 0; k < level2; k++)
										x[i/3][k] = '\0';
									strcat(x[i/3], "(");
									if(strlen(x[i-1]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[i-1]);
									if(strlen(x[i-1]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], "*");
									if(strlen(x[3*i+4]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[3*i+4]);
									if(strlen(x[3*i+4]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], ")");
									strcat(x[i/3], "+");
									strcat(x[i/3], "(");
									if(strlen(x[i-1]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[i-1]);
									if(strlen(x[i-1]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], "*");
									if(strlen(x[3*i + 6]) > 1) strcat(x[i/3], "(");
									strcat(x[i/3], x[3*i + 6]);
									if(strlen(x[3*i + 6]) > 1) strcat(x[i/3], ")");
									strcat(x[i/3], ")");
									i=i/3;
									connect(i);
									i = -1;
									std::cout<<x[0]<<"=0\n";
									fill(j);
									continue;
								}
							}
							cout<<"Removing unnecessary brackets\n";
							for(int t = 0; t < level2; t++)
								x[1][t] = '\0';
							x[1][0] = '(';
							for(int t = 1, y = 0; x[0][y] != '\0'; y++)
							{
								if(x[0][y] != '+')
								{
									if((x[0][y] != '(') && (x[0][y] != ')'))
									{
										x[1][t] = x[0][y];
										t++;
									}
								}
								else
								{
									x[1][t] = ')';
									t++;
									x[1][t] = '+';
									t++;
									x[1][t] = '(';
									t++;
								}
							}
							x[1][strlen(x[1])] = ')';
							strcpy(x[0], x[1]);
							std::cout<<x[0]<<"=0\n";
							cout<<"Grouping equation to form (A1) + X * (A2) + X^ * (A3)\n";
							while(strstr(x[0], "X*X"))
							{
								int u = 0, u2 = 0;
								while(x[0][u])
								{
									if((x[0][u] == 'X') && (x[0][u+1] == '*') && (x[0][u+2] == 'X'))
										u+=2;
									x[0][u2] = x[0][u];
									u++;
									u2++;
								}
								x[0][u2] = '\0';
								x[0][u2 + 1] = '\0';
								x[0][u2 + 2] = '\0';
							}
							while(strstr(x[0], "X^*X^"))
							{
								int u = 0, u2 = 0;
								while(x[0][u])
								{
									if((x[0][u] == 'X') && (x[0][u+1] == '^') && (x[0][u+2] == '*') && (x[0][u+3] == 'X') && (x[0][u+4] == '^'))
										u+=3;
									x[0][u2] = x[0][u];
									u++;
									u2++;
								}
								x[0][u2] = '\0';
								x[0][u2 + 1] = '\0';
								x[0][u2 + 2] = '\0';
								x[0][u2 + 3] = '\0';
							}
							std::cout<<x[0]<<"=0\n";
							x[2][0] = '\0';
							for(int t = 0; t < strlen(x[0]); t++)
							{
								int poch = t, temp = t;
								bool vx = false, vxd = false;
								while((x[0][t] != '+') && (x[0][t] != '\0'))
								{
									if((x[0][t] == 'X') && (x[0][t+1] != '^'))
										{vx = true; Globalvx = true;}
									if((x[0][t] == 'X') && (x[0][t+1] == '^'))
										{vxd = true, Globalvxd = true;}
									t++;
								}
								if((vx == true) && (vxd == true))
								{
									for(int cpy = t + 1; x[0][cpy] != '\0'; cpy++)
									{
										x[0][poch] = x[0][cpy];
										poch++;
									}
									t = temp;
									for(; poch < strlen(x[0]); poch++)
										x[0][poch] = '\0';
									std::cout<<x[0]<<"=0\n";
								}
								if((vx == false) && (vxd == false))
								{
									if(strlen(answers[j][0]) > 1)
										answers[j][0][strlen(answers[j][0])] = '+';
									while(poch <= t)
									{
										if((x[0][poch] != '(') && (x[0][poch] != '+') && (x[0][poch] != ')'))
											x[2][strlen(x[2])] = x[0][poch];
										poch++;
									}
									strcat(answers[j][0], x[2]);
									strcpy(temp2, x[2]);
									for(int u = 0; x[2][u] != '\0'; u++) x[2][u] = '\0';
								}
								if((vx == true) && (vxd == false))
								{
									if(strlen(answers[j][1]) > 4)
										answers[j][1][strlen(answers[j][1])] = '+';
									if(t - poch == 3)
										x[2][0] = '1';
									else while(poch <= t)
									{
										if((x[0][poch] != '(') && (x[0][poch] != '+') && (x[0][poch] != ')') && (x[0][poch] != 'X') && (x[0][poch + 1] != 'X') && ((x[0][poch - 1] != 'X') || (x[0][poch - 2] != '(')))
											x[2][strlen(x[2])] = x[0][poch];
										poch++;
									}
									strcat(answers[j][1], x[2]);
									strcpy(temp2, x[2]);
									for(int u = 0; x[2][u] != '\0'; u++) x[2][u] = '\0';
								}
								if((vx == false) && (vxd == true))
								{
									if(strlen(answers[j][2]) > 5)
										answers[j][2][strlen(answers[j][2])] = '+';
									if(t - poch == 4)
										x[2][0] = '1';
									else while(poch <= t)
									{
										if((x[0][poch] != '(') && (x[0][poch] != '+') && (x[0][poch] != ')') && ((x[0][poch + 1] != 'X') || (x[0][poch + 3] != ')')) && (x[0][poch - 2] != 'X') && (x[0][poch] != 'X') && (x[0][poch - 1] != 'X'))
											x[2][strlen(x[2])] = x[0][poch];
										poch++;
									}
									strcat(answers[j][2], x[2]);
									strcpy(temp2, x[2]);
									for(int u = 0; x[2][u] != '\0'; u++) x[2][u] = '\0';
								}
							}
							answers[j][0][strlen(answers[j][0])] = ')';
							answers[j][1][strlen(answers[j][1])] = ')';
							answers[j][2][strlen(answers[j][2])] = ')';
							cout<<"Here we consider that the intersection is performed earlier than the union:\n";
							unite(j);
							zakony(j);
							unite(j);
							int jEmptyMn = thisEmptyMn;
							for(char *p = answers[j][0]; *p; p++)
							{
								if((isalpha(*p)) && (((*(p-1) == '(') || (*(p-1) == '+')) && ((*(p+1) == ')') || (*(p+1) == '+'))))
								{
									NulMn = *p;
									cout<<"In the bracket that does not contain X we see that "<<*p<<" is 0, so we can replace it\n";
									if(!(strstr(EmptyMn, &NulMn))) EmptyMn[strlen(EmptyMn)] = NulMn;
									for(int y = 0; y < 3; y++)
										for(char *p1 = answers[j][y]; *p1; p1++)
											if(*p1 == NulMn) *p1 = '0';
									unite(j);
									cout<<"Also we can replace it in solved equations:\n";
									for(int u = 0; u < j; u++)
									{
										cout<<"Equation number "<<u+1<<"\n";
										for(int y = 0; y < 3; y++)
											for(char *p1 = answers[u][y]; *p1; p1++)
												if(*p1 == NulMn) *p1 = '0';
										unite(u);
										zakony(u);
									}
									cout<<"Also we can replace it in not solved equations:\n";
									for(int u = j+1; u < nEquation; u++)
									{
										cout<<"Equation number "<<u+1<<"\n";
										for(char *p1 = equations[u]; *p1; p1++)
											{
												if(*p1 == NulMn) *p1 = '0';
											}
										cout<<equations[u]<<"\n";
									}
									cout<<"Returning to our equation:\n";
									unite(j);
									zakony(j);
								}
							}
							Globalvx = false; Globalvxd = false;
							cout<<"Answer to equation number"<<j+1<<":\n";
							if(strlen(answers[j][0]) > 2) cout<<"If "<<answers[j][0]<<" = 0, than\n";
							cout<<answers[j][2] + 4<<" < X < "<<answers[j][1] + 3<<"^.\nElse no solutions.\n\n";
							if(strcmp(answers[j][0], "(0)") == 0) for(int u = 0; u < 3; u++) answers[j][0][u] = '\0';
							if(strcmp(answers[j][2], "+X^*(0)") == 0) for(int u = 0; u < 7; u++) answers[j][2][u] = '\0';
							if(strcmp(answers[j][1], "+X*(0)") == 0) for(int u = 0; u < 6; u++) answers[j][1][u] = '\0';
							for(int u = 0; u < 3; u++)
							{
								if((strlen(globalAns[u]) > 0) && (strlen(answers[j][u]) > 0))
									strcat(globalAns[u], "+");
								int inc;
								if(u == 0) inc = 0;
								if(u == 1) inc = 3;
								if(u == 2) inc = 4;
								strcat(globalAns[u], answers[j][u] + inc);
								
							}
						}
						if (((strcmp(globalAns[2], "0")) && !(strcmp(globalAns[1], "1"))) || (!strcmp(globalAns[0], "(1)")))
							cout<<"The system does not have solutions\n";
						else
						{
							if(!(strlen(globalAns[0]))) globalAns[0][0] = '0';
							if(!(strlen(globalAns[1]))) globalAns[1][0] = '0';
							if(!(strlen(globalAns[2]))) globalAns[2][0] = '0';
							cout<<"Main solution:\nIf "<<globalAns[0]<<" = 0";
							for(char *p = EmptyMn; *p; p++) cout<<" and "<<*p<<" = 0";
							cout<<", than\n"<<globalAns[2]<<" < X < ("<<globalAns[1]<<")^.\nElse no solutions.";
						}
						break;
					case 4:
						exit(0);
						break;
					case 2:
						for(int i = 1; i < 5; i++)
							equations[i][0] = '\0';
							x[0][0] = '\0';
						nEquation = 0;
						draw(hstdout, pos, act, menu, len);
						goto label1;
						break;
					case 3:
						system("cls");
						while(!my1.eof())
						{
							my1.getline(file, 200);
							cout<<file<<"\n";
						}
						cout<<"\nPress Enter to return to main menu";
						while(true)
							if(kbhit())
							{
								my1.close();
								my1.open("help.txt", ios_base::in);
								button = getch();
								if(button == 13)
									goto label1;
							}
						break;
				}
			}
		}
	}
	return 0;
}
