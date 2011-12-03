#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "MarchInfo.h"

void MarchInfo::nextCell()
{
	if (next_x <= next_y && next_x <= next_z) {
		i += sign_x;
		tmin = next_x;
		next_x += dtx;
		
		surfaceNormal.Set((-1.0f) * sign_x, 0.0f, 0.0f);
	}
	else if (next_y <= next_x && next_y <= next_z) {
		j += sign_y;
		tmin = next_y;
		next_y += dty;

		surfaceNormal.Set(0.0f, (-1.0f) * sign_y, 0.0f);
	}
	else if (next_z <= next_x && next_z <= next_y) {
		k += sign_z;
		tmin = next_z;
		next_z += dtz;

		surfaceNormal.Set(0.0f, 0.0f, (-1.0f) * sign_z);
	}
	else {
		std::cout << "Impossible Situation !!" << endl;
	}

	return;
}

std::string MarchInfo::toString()
{
	string header = "MarchInfo:\n";
		
	const int DEFAULT_CHAR_NUMBER = 70;

	char str[DEFAULT_CHAR_NUMBER];
	char str1[DEFAULT_CHAR_NUMBER], str2[DEFAULT_CHAR_NUMBER], str3[DEFAULT_CHAR_NUMBER], 
		 str4[DEFAULT_CHAR_NUMBER], str5[DEFAULT_CHAR_NUMBER], str6[DEFAULT_CHAR_NUMBER],
		 str7[DEFAULT_CHAR_NUMBER], str8[DEFAULT_CHAR_NUMBER], str9[DEFAULT_CHAR_NUMBER],
		 str10[DEFAULT_CHAR_NUMBER], str11[DEFAULT_CHAR_NUMBER], str12[DEFAULT_CHAR_NUMBER];

	sprintf_s(str, "tmin = %f", tmin);
		
	sprintf_s(str1, "index_x = %d", i);
	sprintf_s(str2, "index_y = %d", j);
	sprintf_s(str3, "index_z = %d", k);
		
	sprintf_s(str4, "sign_x = %d", sign_x);
	sprintf_s(str5, "sign_y = %d", sign_y);
	sprintf_s(str6, "sign_z = %d", sign_z);

	sprintf_s(str7, "next_x = %f", next_x);
	sprintf_s(str8, "next_y = %f", next_y);
	sprintf_s(str9, "next_z = %f", next_z);

	sprintf_s(str10, "delta_x = %f", dtx);
	sprintf_s(str11, "delta_y = %f", dty);
	sprintf_s(str12, "delta_z = %f", dtz);

	return header
		 + string("\t") + string(str)  + string("\n")
		 + string("\t") + string(str1) + string("\n")
		 + string("\t") + string(str2) + string("\n")
		 + string("\t") + string(str3) + string("\n")
		 + string("\t") + string(str4) + string("\n")
		 + string("\t") + string(str5) + string("\n")
		 + string("\t") + string(str6) + string("\n")
		 + string("\t") + string(str7) + string("\n")
		 + string("\t") + string(str8) + string("\n")
		 + string("\t") + string(str9) + string("\n")
		 + string("\t") + string(str10) + string("\n")
		 + string("\t") + string(str11) + string("\n")
		 + string("\t") + string(str12) + string("\n")
		 + string("\tSurface Normal = ") + surfaceNormal.toString() + string("\n");

}