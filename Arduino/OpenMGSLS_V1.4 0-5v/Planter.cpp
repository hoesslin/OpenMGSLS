// 
// 
// 

#include "Planter.h"

int Times_LinearLnterpolation_int_Planter;
int T_LinearLnterpolation_int;

unsigned short* DAValX_ushortPointer_Planter;
unsigned short* DAValY_ushortPointer_Planter;

bool NeedMove_XY_bool_Command;

float Goal_X_float, Goal_Y_float, Start_X_float, Start_Y_float;
float Dis_X_float, Dis_Y_float;//how far between Goal and Start
float Plus_X_float, Plus_Y_float;//how many long in once LinearLnterpolation
float Plane_X_float, Plane_Y_float;
float RadVal_X_float, RadVal_Y_float;

void Plane_XY(float arry[3])
//plan the xy monent data and turn to  DAVal
//and put the XY arry address in two pointer
{
	Goal_X_float = arry[0];
	Goal_Y_float = arry[1];
	Dis_X_float = Goal_X_float - Start_X_float;
	Dis_Y_float = Goal_Y_float - Start_Y_float;
	Times_LinearLnterpolation_int_Planter = sqrt(Dis_X_float*Dis_X_float + Dis_Y_float*Dis_Y_float) / SPEED_XY*CYCLE_LNTERPOLARION;

	if (Times_LinearLnterpolation_int_Planter != 0) { //calculate all the LinearLnterpolation
		DAValX_ushortPointer_Planter = new unsigned short[Times_LinearLnterpolation_int_Planter];
		DAValY_ushortPointer_Planter = new unsigned short[Times_LinearLnterpolation_int_Planter];

		if (Dis_X_float != 0) {
			Plus_X_float = (Dis_X_float) / Times_LinearLnterpolation_int_Planter;
		}
		else
		{
			Plus_X_float = 0;
		}
		if (Dis_Y_float != 0) {
			Plus_Y_float = (Dis_Y_float) / Times_LinearLnterpolation_int_Planter;
		}
		else {
			Plus_Y_float = 0;
		}

		Plane_X_float = Start_X_float;
		Plane_Y_float = Start_Y_float;

		while (T_LinearLnterpolation_int != Times_LinearLnterpolation_int_Planter)
		{
			Plane_X_float = Plane_X_float + Plus_X_float;
			Plane_Y_float = Plane_Y_float + Plus_Y_float;
			DAValX_ushortPointer_Planter[T_LinearLnterpolation_int] = DisToDA_X(Plane_X_float, Plane_Y_float);
			DAValY_ushortPointer_Planter[T_LinearLnterpolation_int] = DisToDA_Y(Plane_Y_float);
		/*	
			Serial.print(T_LinearLnterpolation_int);
			Serial.print(" ");
			Serial.print(DAValX_ushortPointer_Planter[T_LinearLnterpolation_int]);
			Serial.print(" ");
			Serial.println(DAValY_ushortPointer_Planter[T_LinearLnterpolation_int]);
		*/
			T_LinearLnterpolation_int++;
		}

		Start_X_float = Goal_X_float;
		Start_Y_float = Goal_Y_float;
		T_LinearLnterpolation_int = 0;
	}
	else {//when the dis is evry small
		DAValX_ushortPointer_Planter = new unsigned short[1];
		DAValY_ushortPointer_Planter = new unsigned short[1];
		Times_LinearLnterpolation_int_Planter = 1;
		DAValX_ushortPointer_Planter[0] = { DisToDA_X(Goal_X_float, Goal_Y_float) };
		DAValY_ushortPointer_Planter[0] = { DisToDA_Y(Goal_Y_float) };
		Start_X_float = Goal_X_float;
		Start_Y_float = Goal_Y_float;
	}

	NeedMove_XY_bool_Command = true;

}

inline unsigned short DisToDA_X(float x, float y) {
	RadVal_X_float = atan(x / (sqrt(DIS_F_THETA*DIS_F_THETA + y*y) + DIS_XYMOTOR));
	RadVal_Y_float = atan(y / DIS_F_THETA);
	return atan((x - (DIS_F_THETA*tan(RadVal_X_float)*(1 / cos(RadVal_Y_float) - 1))) / (sqrt(DIS_F_THETA*DIS_F_THETA + y*y) + DIS_XYMOTOR)) / RAD_MG_MAX * 32768 + 32767* 5/BASE_VOLTAGE;
}

inline unsigned short DisToDA_Y(float y) {
	return  RadVal_Y_float / RAD_MG_MAX * 32768 + 32767*5 / BASE_VOLTAGE;
}