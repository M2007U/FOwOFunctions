//version 2024.1205.0347

#pragma once

#include <iostream>
#include <iomanip>

#include <stdlib.h>
#include <cstdlib>

#include <functional>

#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <cstring>
#include <cctype>

#include <cmath>
#include <math.h>
#include <random>

#include <typeinfo>
#include <sys/types.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

class FOwO_generic
{
    public:

    template <typename TYPE>
    void swap(TYPE &ObjectA, TYPE &ObjectB)
    {
        TYPE ObjectC;

        ObjectC = ObjectA;
        ObjectA = ObjectB;
        ObjectB = ObjectC;
    }

    template <typename TYPE>
    string DatatypeOfVar (TYPE inBox)
    {
        //MACHINE DEPENDENT
        string outString(typeid(inBox).name());

        return outString;
    }

    template <typename TYPE>
    bool DatatypeOfVar_IsNumeric(TYPE inBox)
    {
        //"Sp" means "Sample"
        int         Sp_i;
        long        Sp_l;
        long long   Sp_ll;
        float       Sp_f;
        double      Sp_d;

        //"Dts" means "DatatypeString"
        string Dts_i(typeid(Sp_i).name());
        string Dts_l(typeid(Sp_l).name());
        string Dts_ll(typeid(Sp_ll).name());
        string Dts_f(typeid(Sp_f).name());
        string Dts_d(typeid(Sp_d).name());

        string Dts_Box(typeid(inBox).name());

        return
            (Dts_Box == Dts_i) ||
            (Dts_Box == Dts_l) ||
            (Dts_Box == Dts_ll) ||
            (Dts_Box == Dts_f) ||
            (Dts_Box == Dts_d);
    }

    template <typename TYPE>
    string DataTypeOfVar_Predef(TYPE InVar)
    {
        //"Sp" means "Sample"

        int         Sp_i;
        long        Sp_l;
        long long   Sp_ll;
        float       Sp_f;
        double      Sp_d;

        char        Sp_c;
        string      Sp_s;



        //"Dts" means "DatatypeString"

        string Dts_i(typeid(Sp_i).name());
        string Dts_l(typeid(Sp_l).name());
        string Dts_ll(typeid(Sp_ll).name());
        string Dts_f(typeid(Sp_f).name());
        string Dts_d(typeid(Sp_d).name());

        string Dts_c(typeid(Sp_c).name());
        string Dts_s(typeid(Sp_s).name());


        string Dts_Var(typeid(InVar).name());

        if      (Dts_Var == Dts_i)  {return "int";}
        else if (Dts_Var == Dts_l)  {return "long";}
        else if (Dts_Var == Dts_ll) {return "long long";}
        else if (Dts_Var == Dts_f)  {return "float";}
        else if (Dts_Var == Dts_d)  {return "double";}
        else if (Dts_Var == Dts_c)  {return "char";}
        else if (Dts_Var == Dts_s)  {return "string";}
        else {return "unknown";}

    }

};



class FOwO_vector
{
    public:

    FOwO_vector(){};

    
    template <typename TYPE, size_t SIZE>
    vector<TYPE> ArrayToVector(TYPE (&InArray)[SIZE])
    {
        //converts an array to a vector, where both the array and vector elements shares the same datatype

        vector<TYPE> outVector = {};

        for(int i = 0 ; i < SIZE ; i++)
        {
            outVector.push_back(InArray[i]);
        }

        return outVector;
    }

    template <typename TYPE>
    int FindObjPos(vector<TYPE> inVektor , TYPE Target , char StartingDirection)
    {
        //tries to return the position of the requested Object in the vector
        //if the object cannot be found, then return -1
        //direction can either be start from left 'l' or start from right 'r'

        int CurrentPos;
        bool Found = false;
        int Answer;

        if(StartingDirection == 'l')
        {
            CurrentPos = 0;
            while(CurrentPos < inVektor.size() && Found == false )
            {
                if(inVektor[CurrentPos] == Target)
                {
                    Found = true;
                }
                else
                {
                    CurrentPos++;
                }
            }
        }
        else if (StartingDirection == 'r')
        {
            CurrentPos = inVektor.size() - 1;
            while(CurrentPos > -1 && Found == false)
            {
                if(inVektor[CurrentPos] == Target)
                {
                    Found = true;
                }
                else
                {
                    CurrentPos--;
                }
            }
        }
        else
        {
            //nope
            Found = false;
        }

        if(Found == true)
        {
            Answer = CurrentPos;
        }
        else
        {
            Answer = -1;
        }

        return Answer;
    }

    template <typename TYPE>
    int FindObjPos(vector<TYPE> inVektor , vector<TYPE> TargetList , int StartingPos, char SearchDirection, int EnddingPos)
    {
        if (StartingPos < 0 || inVektor.size() <= StartingPos){return -1;} //if the starting position is out of range
        if (EnddingPos < 0 || inVektor.size() <= EnddingPos){return -1;} //if the endding position is out of range
        
        if (StartingPos == EnddingPos)
        {
            if (FindObjPos(TargetList,inVektor[StartingPos],'l') > -1)  
            {
                return StartingPos;
            }
            else
            {
                return -1;
            }
        }
        else if (StartingPos < EnddingPos && SearchDirection == 'l') {return -1;}//searching direction is wrong
        else if (StartingPos > EnddingPos && SearchDirection == 'r') {return -1;}//searching direction is wrong

        int CurrentIndex = StartingPos;
        while(true)
        {
            if (FindObjPos(TargetList, inVektor[CurrentIndex], 'l') > -1) 
            {
                return CurrentIndex;
            }
            else
            {
                //there is still room to search for it
                //keep on looking
                if (SearchDirection == 'l' && EnddingPos < CurrentIndex)
                {
                    CurrentIndex--;
                }
                else if (SearchDirection == 'r' && CurrentIndex < EnddingPos)
                {
                    CurrentIndex++;
                }
                else
                {
                    return -1;
                }
            }
        }
    }

    template <typename TYPE>
    vector<TYPE> slip_byIndex(vector<TYPE> inVektor, TYPE inContent, int inIndex)
    {
        //if the index goes out of range, it clamps it

        vector<TYPE> tempVektor = {};

        if (inIndex < 0)
        {
            tempVektor.push_back(inContent);
            for (int i = 0 ; i < inVektor.size() ; i++)
            {
                tempVektor.push_back(inVektor[i]);
            }
        }
        else if (0 <= inIndex && inIndex < inVektor.size())
        {
            for(int i = 0 ; i < inIndex ; i++)
            {
                tempVektor.push_back(inVektor[i]);
            }

            tempVektor.push_back(inContent);

            for (int i = inIndex ; i < inVektor.size() ; i++)
            {
                tempVektor.push_back(inVektor[i]);
            }
        }
        else if (inVektor.size() <= inIndex)
        {
            tempVektor = inVektor;
            tempVektor.push_back(inContent);
        }

        return tempVektor;
    }

    template <typename TYPE>
    vector<TYPE> remove_byIndex(vector<TYPE> inVektor , int Index)
    {
        //remove an item from the vektor based on the given index
        //if the given index is 0 to positive, indexing starts from front to the back, starting with 0
        //if the given index is negative, it removes the last element

        vector<TYPE> outVektor = {};
        int inVektorSize = inVektor.size();

        //-----

        if (inVektorSize == 0 || Index == inVektorSize || Index > inVektorSize ) //if the inVektor is empty, or index is out of range
        {
            outVektor = inVektor; //change nothing
        }
        else if (Index >= 0) //it is a normal acceptable index
        {
            for(int i = 0 ; i < inVektorSize ; i++)
            {
                if(i != Index)
                {
                    outVektor.push_back( inVektor[i] ); //exclude the targeted index
                }
            }
        }
        else if (Index < 0) //if the index is negative
        {
            for(int i = 0 ; i < inVektorSize - 1 ; i++)
            {
                outVektor.push_back( inVektor[i] ); //exclude the last item
            }
        }

        //-----

        return outVektor;    
    }

    template <typename TYPE>
    vector<TYPE> concat_bin(vector<TYPE> VektorL , vector<TYPE> VektorR)
    {
        //concatenate 2 vectors of the same type, VektorL comes first, then VektorR
        //then return the resulting vector

        vector<TYPE> outVektor = {};

        outVektor = VektorL;

        for(int i = 0 ; i < VektorR.size() ; i++)
        {
            outVektor.push_back(VektorR[i]);
        }

        return outVektor;
    }

    template <typename TYPE>
    vector<TYPE> cycleBelt(vector<TYPE> inVektor, int step)
    {
        //move the items in the vector like a conveyor belt
        //aka Rotate or Shift
        //directions are : 
        // 'f' : front : (0) <- (1) <- (2) <- (3) ... (n), then send (0) to behind (n)
        // 'b' : back  : (0) -> (1) -> (3) -> (3) ... (n), then send (n) in front of (0)
        //step : how many iterations to send one object to the front/back

        //another way to understand this is that "step" is the position where we put the pipe
        //then exchange what is on the left of the pipe and right of the pipe
        //eg :
        //say we have the following vector {0,1,2,3,4,5,6,7,8,9}
        //if step is 7, then vector with the pipe would be something like {0,1,2,3,4,5,6,|,7,8,9}
        //after flipping it, we will have {7,8,9,0,1,2,3,4,5,6}


        int SafeStep = step % inVektor.size();

        vector<TYPE> VekL = {};
        vector<TYPE> VekR = {};
        vector<TYPE> VekO = {};

        for(int i = 0 ; i < inVektor.size() ; i++)
        {
            if(i < SafeStep)
            {
                VekL.push_back(inVektor[i]);
            }
            else
            {
                VekR.push_back(inVektor[i]);
            }
        }

        VekO = concat_bin(VekR, VekL);

        return VekO;
    }

    template <typename TYPE0, typename TYPE1>
    TYPE1 FunctionMap(vector<TYPE0> VektorIn , vector<TYPE1> VektorOut , TYPE0 ObjIn, TYPE1 ObjOutIfNull)
    {
        //given a list : InputList, and another list : OutPutList
        //given an object, if the Object can be found in the InputList, get the Index Number
        //then use the number to get an output Item from the outPutList by using the index

        int CurrentPos = 0;
        bool Found = false;
        TYPE1 returnResult;

        while(Found == false && CurrentPos < VektorIn.size())
        {
            if(VektorIn[CurrentPos] == ObjIn)
            {
                Found = true;
            }
            else
            {
                CurrentPos++;
            }
        }

        if(Found == false)
        {
            returnResult = ObjOutIfNull;
        }
        else
        {
            returnResult = VektorOut[CurrentPos];
        }

        return returnResult;
    }

    template <typename TYPE>
    TYPE MatrixChoice(vector<TYPE> Table, int TableWidth, int inRow, int inCol, TYPE ReturnIfNull)
    {
        //given a table (in this case is a vector)
        //given the coordinate : row and col
        //given the table width
        //the function will return the choosen object

        int UserChoice = inRow * TableWidth + inCol;
        TYPE Answer;

        if (0 <= UserChoice && UserChoice < Table.size())
        {
            Answer = Table[UserChoice];
        }
        else
        {
            Answer = ReturnIfNull;
        }

        return Answer;
    }

    template <typename TYPE>
    void PushBackMultiple (vector<TYPE> &InVector, vector<TYPE> InAppendList)
    {
        for(int i = 0 ; i < InAppendList.size() ; i++)
        {
            InVector.push_back(InAppendList[i]);
        }
        
        return;
    }

    

};



class FOwO_bool
{
    public :

    FOwO_bool(){};

    int toInt(bool Given, int ResultIfFalse, int ResultIfTrue, int ResultIfNull)
    {
        //convert a bool to an integer

        int ReturnValue;

        if(Given == false)
        {
            ReturnValue = ResultIfFalse;
        }
        else if(Given == true)
        {
            ReturnValue = ResultIfTrue;
        }
        else
        {
            ReturnValue = ResultIfNull;
        }

        return ReturnValue;
    }

    bool logic_simple(string Gate, bool Input0, bool Input1)
    {
        //available gates : "not0", "not1" , "or" , "and" , "xor"

        bool Output;

        if (Gate == "not0")
        {
            Output = !Input0;
        }
        if (Gate == "not1")
        {
            Output = !Input1;
        }
        else if (Gate == "or")
        {
            Output = Input0 || Input1;
        }
        else if (Gate == "and")
        {
            Output = Input0 && Input1;
        }
        else if (Gate == "xor")
        {
            Output = Input0 ^ Input1;
        }
        else
        {
            Output = Input0;
        }

        return Output;
    }

    bool logic_matrix(bool Out00, bool Out01, bool Out10, bool Out11, bool Input0, bool Input1)
    {
        //Out00 is the result it will return if Input0 == false and Input1 == false
        //Out01 is the result it will return if Input0 == false and Input1 == true
        //Out10 is the result it will return if Input0 == true  and Input1 == false
        //Out11 is the result it will return if Input0 == true  and Input1 == true

        bool OutF;

        if      (Input0 == false && Input1 == false){OutF = Out00;}
        else if (Input0 == false && Input1 == true ){OutF = Out01;}
        else if (Input0 == true  && Input1 == false){OutF = Out10;}
        else if (Input0 == true  && Input1 == true ){OutF = Out11;}

        return OutF;
    }

    bool intToBool(int Given, int IfThisThenFalse, int IfThisThenTrue)
    {
        //convert an int to a bool
        //Given           : the bool to be tested
        //IfThisThenFalse : if Given is the same as this, then return false
        //IfThisThenTrue  : if Given is the same as this, then return true

        bool ReturnValue;

        if(Given == IfThisThenFalse)
        {
            ReturnValue = false;
        }
        else if(Given == IfThisThenTrue)
        {
            ReturnValue = true;
        }
        //else, let it be null

        return ReturnValue;
    }


};



class FOwO_math_number
{
    public:

    long double Konst(string Name)
    {
        if      (Name == "PI" || Name == "pi" || Name == "π"){return 3.141592653589;}
        else if (Name == "E" || Name == "e"){return 2.718281828459;}
        else if (Name == "gold" || Name == "φ"){return 1.61803398874;}
        else {return 0;}
    }

    int DigitSum(long input)
    {
        //sum the digits of a number
        //this is meant for experiments

        int Box = input;
        int Acu = 0;

        while (Box > 0)
        {
            Acu = Acu + Box % 10;
            Box = Box / 10;
        }

        return Acu;
    }

    int isPrime(int x)
    {
        //will try to determine if the given number is a prime number
        //CAUTION : this is slow
        //this function will return
        //0 : if x is not prime
        //1 : if x is prime
        //2 : if x == 0 or x == 1
        //9 : if x is invalid or x < 0

        double xRoot = sqrt((double)x);
        int nowTesting = 2;
        bool Quot;
        int ReturnResult = 404;
        bool FLAG_Done = false;

        if (x < 0)
        {
            ReturnResult = 9;
        }
        else if (x == 0 || x == 1)
        {
            ReturnResult = 2;
        }
        else
        {
            while(FLAG_Done == false)
            {
                if (nowTesting > xRoot)
                {
                    ReturnResult = 1;
                    FLAG_Done = true;
                }
                else
                {
                    Quot = x % nowTesting == 0;
                    if (Quot == true)
                    {
                        ReturnResult = 0;
                        FLAG_Done = true;
                    }
                    else
                    {
                        nowTesting = nowTesting + 1;
                    }
                }
            }
        }

        return ReturnResult;
    }

    long integerReverse(long x)
    {
        //reveses an integer, or long integer
        //this is meant for an experiment

        long box0 = x;
        long box1 = 0;

        while (box0 > 0)
        {
            box1 = box1 + box0 % 10;
            box0 = box0 / 10;
            box1 = box1 * 10;
        }

        box1 = box1 / 10 ;

        return box1;
    }

};

class FOwO_math_discrete
{
    public:

    long long powInt (long long base, long long pow)
    {
        //returns base^pow
        //if pow is < 0, then return -1

        if (pow < 0)
        {
            return -1;
        }

        int Box = 1;
        for(int i = 0 ; i < pow ; i++)
        {
            Box = Box * base;
        }

        return Box;
    }

    long long ModularExponent (long long base, long long pow, long long mod)
    {
        //returns (base^pow)%mod,
        //but raising something to a power will explode and go out of range,
        //so we need to use the iterative method

        long long Box = 1;

        for(long long i = 0 ; i < pow ; i++)
        {
            Box = Box * base;

            if (Box >= mod)
            {
                Box = Box % mod;
            }
        }

        return Box;
    }

    long long GreatestCommonFactor (long long InA, long long InB)
    {
        long long BoxA;
        long long BoxB;
        long long BoxR;

        if (InA > InB)
        {
            BoxA = InA;
            BoxB = InB;
        }
        else if (InA < InB)
        {
            BoxA = InB;
            BoxB = InA;
        }
        else if (InA == InB)
        {
            return InA;
        }

        //now BoxA > BoxB

        while(true)
        {
            //so for every iteration
            //BoxA = BoxB * SomeNumber + BoxR
            BoxR = BoxA % BoxB;
            if (BoxR == 0)
            {
                //we are done
                return BoxB;
            }
            else
            {
                //prepare for next iteration
                BoxA = BoxB;
                BoxB = BoxR;
            }
        }
        
        return -1;
    }

    long long ModularMultiplicativeInverse (long long InMod, long long InOneSide)
    {
        long long SafetyTest = GreatestCommonFactor(InMod, InOneSide);

        if (SafetyTest != 1)
        {
            //no multiplicative inverse
            return -1;
        }

        long long Answer = 1;
        long long Box = InOneSide;

        while(Box % InMod != 1)
        {
            Answer++;
            Box += InOneSide;
        }

        return Answer;


        /*
        ( p * q ) MOD n = 1 MOD n
        pqMODn - 1MODn = 1MODn - 1MODn
        (pqMODn - 1MODn) MODn = (1MODn - 1MODn) MONn
        (pq - 1) MOD n = 0 MON n
        so pq - 1 is a multiple of n
        */
    }


};

class FOwO_math_continuos
{
    public:

    FOwO_vector fowo_vector;

    //rounding a float to the nearest integer where
    //mode :
    //near : round near
    //down : round down
    //up   : round up
    double round (double Given, string mode)
    {
        int Result = 0;
        int PartInt = (int)Given;
        double PartFrac = Given - PartInt;

        if(mode == "near")
        {
            //round near
            if(PartFrac < 0.5)
            {
                Result = PartInt;
            }
            else if(PartFrac >= 0.5)
            {
                Result = PartInt + 1;
            }
        }
        else if (mode == "down")
        {
            //round down
            Result = PartInt;
        }
        else if (mode == "up")
        {
            //round up
            if(PartFrac > 0)
            {
                Result = PartInt + 1;
            }
            else //PartFrac is 0
            {
                Result = PartInt;
            }
        }

        return Result;
    }

    //try to guess the approximate root of a number
    //this is meant for experiments
    double rootAprox(double input, float root, int iter, double initBoxL, double initBoxR)
    {
        double boxL;
        double boxR;
        double boxC;
        double boxF;

        boxL = initBoxL;
        boxR = initBoxR;

        for (int i = 0 ; i < iter ; i++)
        {
            boxC = (boxL + boxR) / 2;
            boxF = pow(boxC,root);

            if (boxF == input)
            {
                boxL = boxC;
                boxR = boxC;
                i = iter;
            }
            else if (boxF < input)
            {
                boxL = boxC;
            }
            else if (boxF > input)
            {
                boxR = boxC;
            }
        }

        return (boxL + boxR) / 2;
    }   

    //clamping a float in between 2 floats
    long double Clamp(long double InLow, long double InHigh, long double InPass)
    {
        

        if(InLow <= InHigh)
        {
            if(InPass <= InLow)
            {
                return InLow;
            }
            else if (InHigh <= InPass)
            {
                return InHigh;
            }
        }
        else
        {
            return InPass;
        }

        return InPass;
    }

    
    //Linear Interpolate Between 2 numbers
    double LERP_single(double InA, double InB, double InT)
    {
        if(InB > InA)
        {
            double Box = InA;
            InA = InB;
            InB = Box;
        }

        return (InB-InA)*InT + InA;
    }

    //inverse Linear Interpolate Between 2 numbers
    double LERPinv_single(double InA, double InB, double InR)
    {
        if(InB > InA)
        {
            double Box = InA;
            InA = InB;
            InB = Box;
        }

        return (InR - InA) / (InB - InA);
    }

    //Linear Interpolate between 2 vectors
    vector<double> LERP_vector(vector<double> InA, vector<double> InB, double InT)
    {
        vector<double> outVector = {};

        if(InA.size() == InB.size())
        {
            for(int i = 0 ; i < InA.size() ; i++)
            {
                outVector.push_back( LERP_single(InA[i],InB[i],InT) );
            }
        }

        return outVector;
    }

    template <typename TYPE>
    TYPE VectorDistance (vector<TYPE> VectorA, vector<TYPE> VectorB)
    {
        vector<int>     DatatypeBox_i = {};
        vector<float>   DatatypeBox_f = {};
        vector<long>    DatatypeBox_l = {};
        vector<double>  DatatypeBox_d = {};

        string DatatypeBox_is = typeid(DatatypeBox_i).name();
        string DatatypeBox_ls = typeid(DatatypeBox_l).name();
        string DatatypeBox_fs = typeid(DatatypeBox_f).name();
        string DatatypeBox_ds = typeid(DatatypeBox_d).name();

        string DatatypeBox_VA = typeid(VectorA).name();
        string DatatypeBox_VB = typeid(VectorB).name();

        vector<string> Datatype_Available = {DatatypeBox_is, DatatypeBox_ls, DatatypeBox_fs, DatatypeBox_ds};

        if ( fowo_vector.FindObjPos(Datatype_Available,DatatypeBox_VA,'l') == -1 || fowo_vector.FindObjPos(Datatype_Available,DatatypeBox_VB,'l') == -1 )
        {
            TYPE result;
            return result;
        }

        if (VectorA.size() == 0 || VectorB.size() == 0)
        {
            TYPE result;
            return result;
        }

        if (VectorA.size() != VectorB.size())
        {
            TYPE result;
            return result;
        }

        double DistanceBox = 0;
        for(int i = 0 ; i < VectorA.size() ; i++)
        {
            DistanceBox += pow(((double)VectorA[i] - (double)VectorB[i]) , 2);
        }
        DistanceBox = pow(DistanceBox, 0.5);

        return (TYPE)DistanceBox ;
    }





};

class FOwO_math_geometry
{
    public:

    FOwO_math_number fowo_number;

    long double Trig_DegToRad(long double Input)
    {
        return Input / 180 * fowo_number.Konst("pi");
    }

    long double Trig_RadToDeg(long double Input)
    {
        return Input / fowo_number.Konst("pi") * 180;
    }


};

class FOwO_math_combinatory
{
    
    long long Permutation(long long InStart, long long InRange)
    {
        if (InRange <= 0){return 1;}
        if (InRange > InStart){return 0;}
        if (InStart == 0){return 1;}

        long CurrentCounter = 0;
        long CurrentMultiplier = InStart;
        long CurrentBox = 1;

        while(CurrentCounter < InRange)
        {
            CurrentBox = CurrentBox * CurrentMultiplier;
            CurrentMultiplier--;
            CurrentCounter++;
        }

        return CurrentBox;
    }

    long long Factorial(long long Input)
    {
        if (Input == 0){return 1;}
        return Permutation(Input, Input);
    }

    long long Combination(long long InChoice, long InChoose)
    {
        
        long long AnswerA = Permutation(InChoice, InChoose);
        
        long long AnswerB = Factorial(InChoose);
        
        long long Answer = AnswerA / AnswerB;
        
        return Answer ;
    }

    vector<long long> PascalTriangle(long long InRow)
    {
        
        vector<long long> outVector = {};
        
        for(int i = 0 ; i < (int)InRow + 1 ; i++)
        {
            outVector.push_back( Combination(InRow,i) );
        }
        return outVector;
    }

};

class FOwO_math_random
{
    public:

    FOwO_math_continuos fowo_math_cowon;

    std::random_device Mersenne_rd;
    std::mt19937 Mersenne_gen;
    std::uniform_real_distribution<double> Mersenne_dist;

    FOwO_math_random() : Mersenne_gen(Mersenne_rd()) , Mersenne_dist(0,1) {} //1 is exclusive

    //generate a random double between 0 and 1
    double Mersenne_norm()
    {
        return Mersenne_dist(Mersenne_gen);
    }

    //generate a random float between a min and a max
    double Mersenne_float(double InMin, double InMax)
    {
        return (InMax - InMin) * Mersenne_norm() + InMin;
    }

    //generate a random int between a min and a max
    long long Mersenne_int(long long InMin, long long InMax)
    {
        return fowo_math_cowon.round(Mersenne_float((double) InMin, (double)InMax + 1),"down");
    }
};

class FOwO_math
{
    public :
    FOwO_vector fowo_vector;

    FOwO_math_number nUwUmber;
    FOwO_math_discrete dOwOs;
    FOwO_math_continuos cOwOn;
    FOwO_math_geometry geOwO;
    FOwO_math_combinatory cOwOmbo;
    FOwO_math_random randOwOm;

    FOwO_math(){}

    template <typename TYPE>
    TYPE powTYPE(TYPE base, TYPE pow)
    {
        TYPE Box = 1;

        for(TYPE i = 0 ; i < pow ; i++)
        {
            Box = Box * base;
        }

        return Box;
    }

    template <typename T>
    T SumVector(vector<T> inVektor)
    {
        //sum all numbers in the vector
        //works for int, float, long, doubles, etc

        T returnResult = 0;
        for(int i = 0 ; i < inVektor.size() ; i++)
        {
            returnResult += inVektor[i];
        }

        return returnResult;
    }

    template <typename TYPE>
    TYPE MinOrMax (vector<TYPE> InList, string InMode , TYPE ReturnIfNull)
    {
        int DatatypeCheck_Int = 0;
        float DatatypeCheck_Float = 0;
        long DatatypeCheck_Long = 0;

        if(InList.size() == 0)
        {
            return ReturnIfNull;
        }

        string DatatypeCheck_Int_String(typeid(DatatypeCheck_Int).name());
        string DatatypeCheck_Float_String(typeid(DatatypeCheck_Float).name());
        string DatatypeCheck_Long_String(typeid(DatatypeCheck_Long).name());
        string DatatypeCheck_Input_String(typeid(InList[0]).name());

        vector<string> AcceptableList = {DatatypeCheck_Int_String, DatatypeCheck_Float_String, DatatypeCheck_Long_String};

        if ( fowo_vector.FindObjPos(AcceptableList, DatatypeCheck_Input_String, 'r') > -1 )
        {
            TYPE Container = InList[0];

            if (InMode == "min")
            {
                for(int i = 0 ; i < InList.size() ; i++)
                {
                    if (InList[i] < Container){Container = InList[i];}
                }

                return Container;
            }
            else if (InMode == "max")
            {
                for(int i = 0 ; i < InList.size() ; i++)
                {
                    if (InList[i] > Container){Container = InList[i];}
                }

                return Container;
            }
            else
            {
                return ReturnIfNull;
            }
        }
        else
        {
            return ReturnIfNull;
        }

        return ReturnIfNull;
    }

};



class FOwO_string_shorthand
{
    public :

    string ColorText (string InString)
    {
        //returns the hidden charracter for color changing the string on a console
        //with this you can change the color of your text on a console
        //d : default
        //k : black
        //r : red
        //g : green
        //y : yellow
        //b : blue
        //m : magenta
        //c : cyan
        //w : white

        vector<string> vectorSetIn  = {"d"      ,"k"         ,"r"         ,"g"         ,"y"         ,"b"         ,"m"         ,"c"         ,"w"         };
        vector<string> vectorSetout = {"\033[0m","\033[1;30m","\033[1;31m","\033[1;32m","\033[1;33m","\033[1;34m","\033[1;35m","\033[1;36m","\033[1;37m"};

        FOwO_vector fowo;
        string outString = fowo.FunctionMap<string,string>(vectorSetIn, vectorSetout, InString, "");

        return outString;
    }

    string ColorBackground (string InString)
    {
        //returns the hidden charracter for color changing the string on a console
        //with this you can change the color of your text on a console
        //d : default
        //k : black
        //r : red
        //g : green
        //y : yellow
        //b : blue
        //m : magenta
        //c : cyan
        //w : white

        vector<string> vectorSetIn  = {"d"      ,"k"         ,"r"         ,"g"         ,"y"         ,"b"         ,"m"         ,"c"         ,"w"         };
        vector<string> vectorSetout = {"\033[0m","\033[1;40m","\033[1;41m","\033[1;42m","\033[1;43m","\033[1;44m","\033[1;45m","\033[1;46m","\033[1;47m"};

        FOwO_vector fowo;
        string outString = fowo.FunctionMap<string,string>(vectorSetIn, vectorSetout, InString, "");

        return outString;
    }

};

class FOwO_string_convert
{
    public :

    string charToString(char Kar)
    {
        //turn the given charracter into a single charracter string

        string boxStr = "";
        boxStr += Kar;
        return boxStr;
    }

    string toUpper(string InString)
    {
        //convert everything in the string to Upper case

        string boxString = "";

        for(int i = 0 ; i < InString.length() ; i++)
        {
            char KarPass0 = InString.at(i);
            char KarPass1;

            if(97 <= (int)KarPass0 && (int)KarPass0 <= 122)
            {
                KarPass1 = (char)((int)KarPass0 - 32);
            }
            else
            {
                KarPass1 = KarPass0;
            }

            boxString += KarPass1;
        }

        return boxString;
    }

    string toLower(string InString)
    {
        //convert everything in the string to Lower case

        string boxString = "";

        for(int i = 0 ; i < InString.length() ; i++)
        {
            char KarPass0 = InString.at(i);
            char KarPass1;

            if(65 <= (int)KarPass0 && (int)KarPass0 <= 90)
            {
                KarPass1 = (char)((int)KarPass0 + 32);
            }
            else
            {
                KarPass1 = KarPass0;
            }

            boxString += KarPass1;
        }

        return boxString;
    }

    string forceCase(string InString, char mode)
    {
        //mode :
        //'l' : lower
        //'u' : upper
        //'f' : formal

        string boxString = "";

        for(int i = 0 ; i < InString.length() ; i++)
        {
            char KarPass0 = InString.at(i);
            char KarPass1;

            if      (65 <= (int)KarPass0 && (int)KarPass0 <= 90 && ( mode == 'l' || mode == 'f' )) //if is Upper but want to become lower
            {
                KarPass1 = (char)((int)KarPass0 + 32); //make it lower
            }
            else if (97 <= (int)KarPass0 && (int)KarPass0 <= 122 && mode == 'u') //if is lower but want to become upper
            {
                KarPass1 = (char)((int)KarPass0 - 32); //make it upper
            }
            else
            {
                KarPass1 = KarPass0;
            }

            boxString += KarPass1;
        }

        //now fix formal
        if(mode == 'f')
        {
            boxString[0] = (char)((int)boxString.at(0) - 32);
        }

        return boxString;
    }

    string toAsciiDoubleHex(string input)
    {
        //turns a given string into a string of ascii numbers,
        //each charracter will be interpreted as a 2 digit hexadicamal number
        
        stringstream Bwuffer;
        int boxInt = 0;
        string boxString = "";
        string Result = "";

        for(int i = 0 ; i < input.length() ; i++)
        {
            Bwuffer.clear();
            boxInt = (int)(input.at(i));
            Bwuffer << hex << boxInt;
            Bwuffer >> hex >> boxString >> dec;
            Bwuffer.clear();

            Result = Result + boxString;
        }

        return Result;
    }

    template <typename TYPE>
    string AnythingToString(TYPE Input)
    {
        string Result;
        stringstream buffer;
        buffer.clear();
        buffer << Input;
        buffer >> Result;
        return Result;
    }

};

class FOwO_string_manip
{
    public : 

    FOwO_vector fowo_vector_Helper;
    FOwO_math fowo_math_Helper;

    string trim(string InString, int start, int end)
    {
        //returns a trimmed string of the given string
        //given the starting position and endding position

        if
        (
            (start < end || start == end) &&
            (0 <= start) &&
            (end < InString.length())
        )
        {
            string resultStr = "";

            for(int i = start ; i < InString.length() && i <= end ; i++)
            {
                resultStr += InString.at(i);
            }

            return resultStr;
        }
        else
        {
            return "";
        }
    }

    string trim_Safe(string InString, int start, int end)
    {
        int PosS = (int)fowo_math_Helper.cOwOn.Clamp(0,(float) InString.length()-1, (float) start);
        int PosE = (int)fowo_math_Helper.cOwOn.Clamp(0,(float) InString.length()-1, (float) end);

        return trim(InString,PosS,PosE);
    }

    string vectorConcat(vector<string> inVector)
    {
        //given a vector of strings
        //concatenate every string in the given vector
        //the returns the whole resulting string

        string AnswerString = "";

        for(int i = 0 ; i < inVector.size() ; i++)
        {
            AnswerString = AnswerString + inVector[i];
        }

        return AnswerString;
    }

    string subStringReplace(string input, string target, string change)
    {
        //replace a targeted sub string to a another sub string
        //input  : the raw string to be processed
        //target : the substring to be replaced in the given raw string
        //change : the substring to be replaced with

        //here's how it works :
        //safe strings       //potential target strings, say "apple" // queue
        //PlateL             PlateM                                  PlateR
        //d7u5heappl         app              6<---                  5uf7dhejapple48r

        string output = "";
        string PlateL = "";
        string PlateM = "";
        string PlateR = input;

        if(input.length() < target.length() || input == "" || target == "")
        {
            output = "";
        }
        else
        {
            while(PlateR != "")
            {
                //flush, get from PlateR
                if(PlateR.length() == 1)
                {
                    PlateM = PlateM + PlateR;
                    PlateR = "";
                }
                else
                {
                    PlateM = PlateM + trim(PlateR,0,0);
                    PlateR = trim(PlateR,1,PlateR.length()-1);
                }

                //maintain length of PlateM
                if(PlateM.length() > target.length())
                {
                    PlateL = PlateL + trim(PlateM,0,0);
                    PlateM = trim(PlateM,1,PlateM.length()-1);
                }

                //does PlateM matches target ?
                if(PlateM == target)
                {
                    PlateL = PlateL + change;
                    PlateM = "";
                }
            
            }

            output = PlateL + PlateM; //there might be leftovers in PlateM
        }

        return output;
    }

    string HexaSignExtend(string input, int size)
    {
        //sign extends a hexadecimal number

        string Result = input;
        stringstream Bwuffer;
        int Box;
        Bwuffer.clear();
        Bwuffer << hex << input;
        Bwuffer >> Box;

        if(input.length() < size)
        {
            while( Result.length() < size )
            {
                if(Box > 0){Result = "0" + Result;}
                else if(Box < 0){Result = "f" + Result;}
                else if(Box == 0){Result = Result + "0";}
            }
        }
        else if(input.length() > size)
        {
            while(Result.length() > size && Result.length() != 0)
            {
                Result = trim(Result,1,Result.length()-1);
            }
        }
        else if(input.length() == 0)
        {
            Result = "";
            for(int i = 0 ; i < size ; i++)
            {
                Result = Result + "0";
            }
        }

        return Result;

    }

    /*
    string HexaSignExtend2(string input, int size)
    {
        //sign extend
        
        string Result = input;

        if(input == "")
        {
            while(Result.length() < size)
            {
                Result = Result + "0";
            }
        }
        else if (FOwO_string_isHexaDc(input) == true)
        {
            
            int GetSign = -1;
            char BoxChar = input.at(0);

            if( (int)('0') <= (int)BoxChar && (int)BoxChar <= (int)('7') )
            {
                //it is a possitive number
                GetSign = 0;
            }
            else
            {
                //it is negative
                GetSign = 1;
            }

            while(Result.length() < size)
            {
                if(GetSign == 0)
                {
                    Result = "0" + Result;
                }
                else
                {
                    Result = "f" + Result;
                }
            }
        }
        else
        {
            Result = input;
        }

        return Result;
    }
    */

    string cycleBelt(string InString, int Step, char Direction)
    {
        //rotates the string, or shift the string
        //InString : the string to be processed
        //Step : how many shifts will there be ?
        //Direction : Left ('l') or Right ('r') ?

        string StrL = "";
        string StrR = "";
        string StrO = "";

        for(int i = 0 ; i < InString.length() ; i++)
        {
            if(i < Step)
            {
                StrL += InString.at(i);
            }
            else
            {
                StrR += InString.at(i);
            }
        }

        StrO = StrR + StrL;

        return StrO;
    }

    string vectorToSeperatedString(vector<string> inVector, string TokenLeftSide, string TokenRightSide, string TokenBetween)
    {
        //for every string from the given vector
        //attach "TokenLeftSide" on its left
        //attach "TokenRightSide" on its right
        //then concatenate all of them, where in between in separated by "TokenBetween"
        //the return this huge string

        //turn a string like : _bp-2	:=	_bp+4	*	_bp+6
        //into something like : <_bp-2>	<:=>	<_bp+4>	<*>	<_bp+6>

        string Answer = "";

        if (inVector.size() > 0)
        {
            Answer = TokenLeftSide + inVector[0] + TokenRightSide;
            for (int i = 1 ; i < inVector.size() ; i++)
            {
                Answer = Answer + TokenBetween + TokenLeftSide + inVector[i] + TokenRightSide;
            }
        }
        else
        {
            Answer = "";
        }

        return Answer;
        
    }

    vector<string> SeparateByChar (string Blob, char SeparateChar)
    {
        //separate the string into elements in a vector, separated by the given charracter

        string Plate = Blob;
        vector<string> Answer;
        string Box = "";

        while(Plate.length() > 0)
        {
            if (Plate.at(0) != SeparateChar)
            {
                Box = Box + Plate.at(0);
                if (Plate.length() > 1)
                {
                    Plate = trim(Plate,1,Plate.length()-1);
                }
                else if (Plate.length() == 1)
                {
                    Plate = "";
                }
            }
            else
            {
                Answer.push_back(Box);
                Box = "";

                if (Plate.length() > 1)
                {
                    Plate = trim(Plate,1,Plate.length()-1);
                }
                else if (Plate.length() == 1)
                {
                    Plate = "";
                }
            }
        }

        if (Box.length())
        {
            Answer.push_back(Box);
        }

        return Answer;

    }

    string permute (vector<int> InPermuteList , string InString, char InErrorChar, int InStringRequiredLength)
    {
        //can be treated as reconstruct or permute or extend
        //based on the given list of ints, reconstruct a new string by treating the numbers as indexes refering to the given string
        //if the InStringRequiredLength is -1, it means it has no required length, else, this will only execute if the length of InString Matches

        string outString = "";

        if(InStringRequiredLength < 0 || InString.length() == InStringRequiredLength)
        {
            for(int i = 0 ; i < InPermuteList.size() ; i++)
            {
                if(InPermuteList[i] < InString.length())
                {
                    outString = outString + InString.at( InPermuteList[i] );
                }
                else
                {
                    outString = outString + InErrorChar;
                }
                
            }
        }
        else
        {
            for(int i = 0 ; i < InStringRequiredLength ; i++)
            {
                outString = outString + InErrorChar;
            }
        }

        return outString;
    }

    string padding (string InString, string InFill, int InSize, char InAlign)
    {
        //InAlign : the align direction, can be left, right, center
        //'l' : left, 'r' : right, 'c' : center

        string Box = InString;

        while(Box.length() < InSize)
        {
            if (InAlign == 'l')
            {
                Box = Box + InFill;
            }
            else if (InAlign == 'r')
            {
                Box = InFill + Box;
            }
            else if (InAlign == 'c')
            {
                Box = InFill + Box + InFill;
            }
        }

        return Box;
    }

    string spaceCleaning (string InString, vector<char> InSpaceChars)
    {
        //remove any whitespace charracters from the string
        //you can also exploit this by creating a vector of charracters (not whitespace)
        //then this becomes a charracter filter

        string outString = "";

        for(int i = 0 ; i < InString.length() ; i++)
        {
            if(fowo_vector_Helper.FindObjPos<char>(InSpaceChars, InString.at(i),'r') > -1) //if the last charracter of the InString can be found in the vector and be considered to be a space charracter
            {
                if(outString.length() == 0)
                {
                    //do nothing
                }
                else
                {
                    if(fowo_vector_Helper.FindObjPos<char>(InSpaceChars , outString.at(outString.length()-1) , 'r') > -1) //if the last charracter of outString can be found in the vector an considered to be a space charracter
                    {
                        //do nothing
                    }
                    else
                    {
                        outString = outString + InString.at(i);
                    }
                }
            }
            else
            {
                outString = outString + InString.at(i);
            }
        }

        return outString;
    }

    string slip(string InString, int InPosition, string InSubstring)
    {
        int SlipPosition = fowo_math_Helper.cOwOn.Clamp(0,InString.length(),InPosition);

        int CurrentPosition = 0;
        string Plate = "";

        //deal with the front first
        while( CurrentPosition != SlipPosition )
        {
            Plate = Plate + InString.at(CurrentPosition);
            CurrentPosition++;
        }

        Plate = Plate + InSubstring;
        
        //deal with the rest
        while( CurrentPosition < InString.length() )
        {
            Plate = Plate + InString.at(CurrentPosition);
            CurrentPosition++;
        }

        return Plate;
        
    }
};

class FOwO_string_detect
{
    public :

    FOwO_string_manip FOwO_string_manip_Helper;
    FOwO_string_convert FOwO_string_convert_Helper;
    FOwO_vector FOwO_vector_Helper;

    string charType(char Input)
    {        
        int ASCIIval = (int)Input;
        string output;

        if(0 <= ASCIIval && ASCIIval <= 31 && ASCIIval != 9) {output = "wierd";}
        else if(ASCIIval == 9 || ASCIIval == 32 || ASCIIval == 10 || ASCIIval == 13) //if is tab ot space
        {output = "space";}
        else if
        (
            (21 <= ASCIIval && ASCIIval <= 47) || 
            (58 <= ASCIIval && ASCIIval <= 64) ||
            (91 <= ASCIIval && ASCIIval <= 96) ||
            (123 <= ASCIIval && ASCIIval <= 126)
        )
        {output = "symbol";}
        else if(48 <= ASCIIval && ASCIIval <= 57){output = "number";}
        else if(65 <= ASCIIval && ASCIIval <= 90){output = "ltrUpper";}
        else if(97 <= ASCIIval && ASCIIval <= 122){output = "ltrLower";}
        else if(ASCIIval == 127){output = "DEL";}
        else {output = "idk";}

        return output;
    }

    void PrintAscii(string input)
    {
        //to print every charracter in a string as ascii numbers, except the \0 charracter
        for(int i = 0; i<input.length(); i++)
        {
            cout << "-(" << (int)(input.at(i)) << ")-";
        }
        cout << endl;
    }

    int FindCharPos(string InString, char Target, char Direction)
    {
        int CurrentPos;
        bool Found = false;
        int Answer;

        if(Direction == 'l')
        {
            CurrentPos = 0;
            while(CurrentPos < InString.length() && Found == false )
            {
                if(InString.at(CurrentPos) == Target)
                {
                    Found = true;
                }
                else
                {
                    CurrentPos++;
                }
            }
        }
        else if (Direction == 'r')
        {
            CurrentPos = InString.length() - 1;
            while(CurrentPos > -1 && Found == false)
            {
                if(InString.at(CurrentPos) == Target)
                {
                    Found = true;
                }
                else
                {
                    CurrentPos--;
                }
            }
        }
        else
        {
            //nope
            Found = false;
        }

        if(Found == true)
        {
            Answer = CurrentPos;
        }
        else
        {
            Answer = -1;
        }

        return Answer;
    }

    bool atPosIsChar(string TestSubject, int TargetPos, char TargetChar)
    {
        //check if the given string, at the given position is the requested char
        //is yes, return true, else false

        bool ReturnResult;

        if(TestSubject == "")
        {
            ReturnResult = false;
        }
        else
        {
            ReturnResult = (int)(TestSubject.at(TargetPos)) == (int)(TargetChar);
        }

        return ReturnResult;
    }

    int IslandCount(string Given, string& OutString)
    {
        //count how many islands, i.e. word count
        //output a string without leading spaces and tailing spaces

        int IslandQty = -1;
        string Plate = Given;
        bool IsEmpty_CannotProcess = false;

        //check if the string is naughtyly empty, meaning only whitespace, tabs, newline only
        if(isNaughtyEmpty(Given) == 0)
        {
            //ok, at least we have something to deal with...

            //trim away leading spaces
            while
            (
                (
                    atPosIsChar(Plate,0,' ') ||
                    atPosIsChar(Plate,0,'\t') ||
                    atPosIsChar(Plate,0,'\n')
                )
                &&
                (
                    Plate.length() > 1
                )
            )
            {
                Plate = FOwO_string_manip_Helper.trim(Plate,1,Plate.length()-1);
                //cout << "debug : cut head : \"" << Plate << "\"" << endl;
            }

            //trim away tailing spaces
            while
            (
                (
                    atPosIsChar(Plate,Plate.length()-1,' ') ||
                    atPosIsChar(Plate,Plate.length()-1,'\t') ||
                    atPosIsChar(Plate,Plate.length()-1,'\n')
                )
                &&
                (
                    Plate.length() > 1
                )
            )
            {
                Plate.pop_back();
                //cout << "debug : cut tail : \"" << Plate << "\"" << endl;
            }

            //now we can start counting islands
            //Initiallize Arrow into the first charracter, which is a land already
            int Arrow = 0;
            bool IWasOnLand = true;
            IslandQty = 1;

            while(Arrow < Plate.length())
            {
                if(IWasOnLand == true)
                {
                    if //I Hit Sea
                    (
                        atPosIsChar(Plate,Arrow,' ') ||
                        atPosIsChar(Plate,Arrow,'\t') ||
                        atPosIsChar(Plate,Arrow,'\n')
                    )
                    {
                        //Just went into sea
                        IWasOnLand = false;
                    }
                    else
                    {
                        //I am still on Land
                        //then do nothing
                    }
                }
                else //IWasOnSea
                {
                    if //I Hit Sea
                    (
                        atPosIsChar(Plate,Arrow,' ') ||
                        atPosIsChar(Plate,Arrow,'\t') ||
                        atPosIsChar(Plate,Arrow,'\n')
                    )
                    {
                        //I am still on Sea
                    }
                    else
                    {
                        //I Hit Land~, Island + 1
                        IWasOnLand = true;
                        IslandQty++;
                    }
                }

                Arrow++;
            }

            OutString = Plate;


        }
        else
        {
            //duh... like no words here, so yeah, 0
            IslandQty = 0;
        }

        return IslandQty;
    }

    int FOwO_string_TargetCharCount(string Given, char Target)
    {
        int result = 0;

        for(int i = 0 ; i < Given.length() ; i++)
        {
            if(Given.at(i) == Target)
            {
                result++;
            }
        }

        return result;
    }

    bool isNaughtyEmpty(string InString)
    {
        //if the string is entirely tabs or whitespace or newline or empty, then return true, else false
        bool Answer;

        if(InString == "")
        {
            //bruh
            Answer = true;
        }
        else
        {
            //assume it is true first
            Answer = true;

            //now check if it is false
            int CurrentPos = 0;
            while(CurrentPos < InString.length() && Answer == true)
            {
                if
                (
                    InString.at(CurrentPos) != ' ' &&
                    InString.at(CurrentPos) != '\n' &&
                    InString.at(CurrentPos) != '\t'
                )
                {
                    Answer = false;
                }
                else
                {
                    CurrentPos++;
                }
            }

            //now the Answer holds the result;
        }

        return Answer;

    }

    bool StringIsMadeFromChars(string InString, vector<char> InCharList)
    {
        for(int i = 0 ; i < InString.length() ; i++)
        {
            if(FOwO_vector_Helper.FindObjPos(InCharList,InString.at(i),'r') < 0) // if the charracter in the string is not found in the charracter vector
            {
                return false;
            }
        }

        return true;
    }

    int containsString(string input, string target, char startFrom)
    {
        //see if a string contains a another sub string
        //if yes, returns the position, else -1
        //input  : the raw string to be processed
        //target : the substring to be replaced in the given raw string
        //change : the substring to be replaced with

        //here's how it works :
        //safe strings       //potential target strings, say "apple" // queue
        //PlateL             PlateM                                  PlateR
        //d7u5heappl         app              6<---                  5uf7dhejapple48r

        string PlateL = "";
        string PlateM = "";
        string PlateR = "";

        if(input.length() < target.length() || input == "" || target == "")
        {
            return -1;
        }
        else if (startFrom == 'l')
        {
            string PlateR = input;

            while(PlateR != "")
            {
                //flush, get from PlateR
                if(PlateR.length() == 1)
                {
                    PlateM = PlateM + PlateR;
                    PlateR = "";
                }
                else
                {
                    PlateM = PlateM + PlateR.at(0);
                    PlateR = FOwO_string_manip_Helper.trim(PlateR,1,PlateR.length()-1);
                }

                //maintain length of PlateM
                if(PlateM.length() > target.length())
                {
                    PlateL = PlateL + PlateM.at(0);
                    PlateM = FOwO_string_manip_Helper.trim(PlateM,1,PlateM.length()-1);
                }

                //does PlateM matches target ?
                if(PlateM == target)
                {
                    return PlateL.length() ;
                }
            }

            return -1;
        }
        else if (startFrom == 'r')
        {
            string PlateL = input;

            while(PlateL != "")
            {
                //flush, get from PlateL
                if(PlateL.length() == 1)
                {
                    PlateM = PlateL + PlateM;
                    PlateL = "";
                }
                else
                {
                    PlateM = PlateL.at(PlateL.length()-1) + PlateM;
                    PlateL = FOwO_string_manip_Helper.trim(PlateL,0,PlateL.length()-2);
                }

                //maintain length of PlateM
                if(PlateM.length() > target.length())
                {
                    PlateR = PlateM.at(PlateM.length()-1) + PlateR;
                    PlateM = FOwO_string_manip_Helper.trim(PlateM,0,PlateM.length()-2);
                }

                //does PlateM matches target ?
                if(PlateM == target)
                {
                    return PlateL.length() ;
                }
            }

            return -1;
        }

        return false;
    }
    

};

class FOwO_string_math
{
    public :

    FOwO_vector             FOwO_vector_Helper;
    FOwO_bool               FOwO_bool_Helper;
    FOwO_string_shorthand   FOwO_string_shorthand_Helper;
    FOwO_string_manip       FOwO_string_manip_Helper;
    FOwO_string_convert     FOwO_string_convert_Helper;
    


    int isNumber_Simple(string TestSubject)
    {
        //check if a given string can be interpret as a number

        string TestSubjectPass = TestSubject;
        //-404 : syntax error
        //   0 : not a number
        //   1 : int
        //   2 : float

        int Answer = -1;

        //string is completely empty ?
        //bruh ~ reject
        if(TestSubjectPass == "")
        {
            Answer = 0;
            return Answer;
        }

        //does string has a single negative sign infront of it ?
        //if yes, remove it, it is acceptable
        if (TestSubjectPass.at(0) == '-')
        {
            TestSubjectPass = FOwO_string_manip_Helper.trim(TestSubjectPass,1,TestSubjectPass.length() - 1);
        }


        //string is completely empty ?
        //bruh ~ reject
        if(TestSubjectPass == "")
        {
            Answer = 0;
            return Answer;
        }
    
        //string has other charracters ?
            //wth ? reject
        for(int i = 0 ; i < TestSubjectPass.length() ; i++ )
        {
            if( (TestSubjectPass.at(i) < '0' || '9' < TestSubjectPass.at(i)) && TestSubjectPass.at(i) != '.' )
            {
                Answer = 0;
                return Answer;
            }
        }

        string BoxString = TestSubjectPass;
        //" ... 000 125 . 125 000 ..."
        //" ... 000 000 . 000 000 ..."
        //clean out leading zeros and backing zeros
        while(BoxString.at(0) == '0' && BoxString.length() > 1)
        {
            BoxString = FOwO_string_manip_Helper.trim(BoxString,1,BoxString.length()-1);
        }

        while(BoxString.at(BoxString.length() - 1) == '0' && BoxString.length() > 1)
        {
            BoxString = FOwO_string_manip_Helper.trim(BoxString,0,BoxString.length()-2);
        }

        //then check how many decimal points there are
        int DecimalPointCount = 0;
        for(int i = 0 ; i < BoxString.length() ; i++)
        {
            if(BoxString.at(i) == '.')
            {
                DecimalPointCount++;
            }
        }

        if(DecimalPointCount == 0) //none ? 
        {
            // string is not empty ?
                    //it is an int
            //end up with empty string ? prolly we have "000 000" to start with
                //it is integer zero
            Answer = 1;
        }
        else if (DecimalPointCount == 1)
        {
            //one ? it is a real
            //if on the left of the decimal is completely empty, it is because the zero is being cleared, add one zero back on the left
            //but if on the right of the decimal is completely empty, the zeros are redundant, but this will be treated as a real, so add a zero back
            //end up with "." ? prolly we have "000 . 000" to start with
                //it is real / float zero
            Answer = 2;
        }
        else
        {
            //more than that ? invalid
            Answer = -404;
        }
            
        return Answer;
    }

    string isNumber_Complex(string TestSubject)
    {

        //  00000                   12500       .   00064       00000
        // |----------------------|----------|---|-----------|---------------------|
        // | reduntant zero front | int part | . | frac part | redundant zero back |

        // '0'.ascii = 48, '9'.ascii = 57

        bool FLAG_isNeg = false;
        string CurrentState = "STATE_Start";
        bool Kontinue = true;
        int readCursor = 0;
        char charBox;

        string PART_Head = "";
        string PART_ReduntZeroF = "";
        string PART_IntPart = "";
        string PART_FracPart = "";
        string PART_ReduntZeroB = "";

        string ReturnResult = "";

        while( Kontinue == true && readCursor < TestSubject.length())
        {
            charBox = TestSubject.at(readCursor);

            if (CurrentState == "STATE_Start")
            {
                if (charBox == '-')
                {
                    PART_Head = "-";
                    CurrentState = "STATE_RedundantZeroF";
                    Kontinue = true;
                }
                else if (charBox == '0')
                {
                    PART_ReduntZeroF = PART_ReduntZeroF + charBox;
                    CurrentState = "STATE_RedundantZeroF";
                    Kontinue = true;
                }
                else if (49 <= (int)charBox && (int)charBox <=57) //1..9
                {
                    PART_IntPart = PART_IntPart + charBox;
                    CurrentState = "STATE_IntPart";
                    Kontinue = true;
                }
                else
                {
                    CurrentState = "EmergencyHalt" ;
                    Kontinue = false;
                }
            }
            else if (CurrentState == "STATE_RedundantZeroF")
            {
                if (charBox == '0')
                {
                    PART_ReduntZeroF = PART_ReduntZeroF + charBox;
                    CurrentState = "STATE_RedundantZeroF";
                    Kontinue = true;
                }
                else if (49 <= (int)charBox && (int)charBox <=57) //1..9
                {
                    PART_IntPart = PART_IntPart + charBox;
                    CurrentState = "STATE_IntPart" ;
                    Kontinue = true;
                }
                else if (charBox == '.')
                {
                    CurrentState = "STATE_FracPart";
                    Kontinue = true;
                }
                else
                {
                    CurrentState = "EmergencyHalt" ;
                    Kontinue = false;
                }
            }
            else if (CurrentState == "STATE_IntPart")
            {
                if (48 <= (int)charBox && (int)charBox <=57) // 0..9
                {
                    PART_IntPart = PART_IntPart + charBox;
                    CurrentState = "STATE_IntPart";
                    Kontinue = true;
                }
                else if (charBox == '.')
                {
                    CurrentState = "STATE_FracPart";
                    Kontinue = true;
                }
                else
                {
                    CurrentState = "EmergencyHalt";
                    Kontinue = false;
                }
            }
            else if (CurrentState == "STATE_FracPart")
            {
                if (charBox == '0')
                {
                    PART_ReduntZeroB = PART_ReduntZeroB + charBox;
                    CurrentState = "STATE_RedundantZeroB";
                    Kontinue = true;
                }
                else if (49 <= (int)charBox && (int)charBox <=57) // 1..9
                {
                    PART_FracPart = PART_FracPart + charBox;
                    CurrentState = "STATE_FracPart";
                    Kontinue = true;
                }
                else
                {
                    CurrentState = "EmergencyHalt";
                    Kontinue = false;
                }
            }
            else if (CurrentState == "STATE_RedundantZeroB")
            {
                if (charBox == '0')
                {
                    PART_ReduntZeroB = PART_ReduntZeroB + charBox;
                    CurrentState = "STATE_RedundantZeroB";
                    Kontinue = true;
                }
                else if (49 <= (int)charBox && (int)charBox <=57) // 1..9
                {
                    PART_ReduntZeroB = PART_ReduntZeroB + charBox;
                    PART_FracPart = PART_FracPart + PART_ReduntZeroB;
                    PART_ReduntZeroB = "";
                    CurrentState = "STATE_FracPart";
                    Kontinue = true;
                }
                else
                {
                    CurrentState = "EmergencyHalt";
                    Kontinue = false;
                }
            }
            else if (CurrentState == "EmergencyHalt")
            {
                CurrentState = "EmergencyHalt";
                Kontinue = false;
            }

            
            readCursor++;
        }

        
        if (CurrentState == "EmergencyHalt")
        {
            ReturnResult = "Undef";
        }
        else
        {
            if (PART_Head == "-" )
            {
                ReturnResult = ReturnResult + "Neg";
            }
            else
            {
                ReturnResult = ReturnResult + "Pos";
            }

            if (PART_ReduntZeroF.length() > 0)
            {
                ReturnResult = ReturnResult + "Rzero";
            }

            if (PART_IntPart.length() > 0 && PART_FracPart.length() == 0)
            {
                //is Int
                ReturnResult = ReturnResult + "Int";
            }
            else if ((PART_IntPart == "0" || PART_IntPart == "" ) && PART_FracPart.length() > 0 )
            {
                //is Frac
                ReturnResult = ReturnResult + "Frac";
            }
            else if (PART_IntPart.length() > 0 && PART_FracPart.length() > 0 )
            {
                //if Float
                ReturnResult = ReturnResult + "Float";
            }

            if (PART_ReduntZeroB.length() > 0)
            {
                ReturnResult = ReturnResult + "Rzero";
            }
        }

        //cout << "[" << PART_Head << "] [" << PART_ReduntZeroF << "] [" << PART_IntPart << "] . [" << PART_FracPart << "] [" << PART_ReduntZeroB << "]" << endl;

        return ReturnResult;
    }

    bool isNumber_HexaDc(string TestSubject)
    {
        //it will return 1 if the given string can be interpreted as a hexadecimal number, 
        //else, return 0

        string box1 =  FOwO_string_convert_Helper.forceCase(TestSubject, 'u');
        vector<char> OKchars = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
        int box2 = 404;

        bool ReturnResult = true; //assume it is true first

        for(int i = 0;i<TestSubject.length();i++)
        {
            if( FOwO_vector_Helper.FindObjPos<char>(OKchars,box1.at(i),'l') < 0 ) //it is not in the set
            {
                ReturnResult = false;
            }
        }

        return ReturnResult;
    }

    bool isNumber_BuiltInDouble (string TestSubject)
    {
        try
        {
            double Box = stod(TestSubject);
            return true;
        }
        catch(const invalid_argument& e)
        {
            return false;
        }
        catch(const out_of_range& e)
        {
            return false;
        }

        return false;
    }



    long long Base02String_to_Int (string InString)
    {
        //converts a BinaryString to an int

        int Box = 0;

        for(int i = 0 ; i < InString.length() ; i++)
        {
            if (InString.at(i) == '1')
            {
                Box++;
            }

            Box *= 2;
        }

        Box = Box / 2;

        return Box;
    }

    string Int_to_Base02String (long long InNumber)
    {
        //converts am int to a BinaryString

        //here's how it works
        /*
        [165] --> 1 + 164 --> [82] --> 0 + 82 --> [41] --> 1 + 40 --> [20] --> 0 + 20 --> [10] --> 0 + 10 --> [5] --> 1 + 4 --> [2] --> 0 + 2 --> [1] //done
                  1                    0                   1                   0                   0                  1                 0 

        [iterBefore] --> MOD2 + Quotient --> [iterAfter]
                    string outPut
        */

        long long BoxIterBefore = InNumber ;
        long long BoxIterAfter  = 0;
        string BoxString = "" ;

        if (BoxIterBefore > 0)
        {
            while(BoxIterBefore > 1)
            {
                if (BoxIterBefore % 2 == 1)
                {
                    BoxString = "1" + BoxString;
                    BoxIterAfter = (BoxIterBefore - 1) / 2;
                }
                else //BoxIterBefore % 2 == 0
                {
                    BoxString = "0" + BoxString;
                    BoxIterAfter = BoxIterBefore / 2;
                }

                BoxIterBefore = BoxIterAfter;
                BoxIterAfter = 0;
            }

            BoxString = "1" + BoxString;
        }
        else if (BoxIterBefore == 0)
        {
            BoxString = "0";
        }
        else
        {
            cout << FOwO_string_shorthand_Helper.ColorText("r") << "FQAQ : number is negative, this will only work on positive integers" << endl; 
            BoxString = "-1";
        }

        return BoxString;
        

    }
    
    string Int_to_Base16String (long long InNumber)
    {
        //converts an int to a Base16 number

        long long BoxIterBefore = InNumber ;
        long long BoxIterAfter  = 0;
        string BoxString = "" ;

        if (BoxIterBefore > 0)
        {
            while(BoxIterBefore > 15)
            {
                long long TempRemInt = BoxIterBefore % 16;
                string TempRemString = FOwO_vector_Helper.FunctionMap<long long, string>({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"},TempRemInt,"#");
                BoxString = TempRemString + BoxString;
                BoxIterAfter = (BoxIterBefore - TempRemInt) / 16;


                BoxIterBefore = BoxIterAfter;
                BoxIterAfter = 0;
            }

            BoxString = FOwO_vector_Helper.FunctionMap<long long, string>({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"},BoxIterBefore,"#") + BoxString;
        }
        else if (BoxIterBefore == 0)
        {
            BoxString = "0";
        }
        else
        {
            cout << FOwO_string_shorthand_Helper.ColorText("r") << "FQAQ : number is negative, this will only work on positive integers" << FOwO_string_shorthand_Helper.ColorText("d") << endl; 
            BoxString = "-1";
        }

        return BoxString;
    }

    long long Base16String_to_Int (string InString)
    {
        //converts a Base16 number to an int

        long long Box = 0;

        for(int i = 0 ; i < InString.length() ; i++)
        {
            Box += FOwO_vector_Helper.FunctionMap<char,long long>(vector<char>{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'},vector<long long>{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},InString.at(i),0);
            Box *= 16;
        }

        Box /= 16;

        return Box;
    }

    string Base16String_to_Base02String (string InString)
    {
        string inBox = FOwO_string_convert_Helper.toUpper(InString);
        string outBox = "";

        for(int i = 0 ; i < inBox.length() ; i++)
        {
            outBox = outBox + 
            FOwO_vector_Helper.FunctionMap<char,string>(
                {   '0',   '1',   '2',   '3',   '4',   '5',   '6',   '7',   '8',   '9',   'A',   'B',   'C',   'D',   'E',   'F'},
                {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"},
                inBox.at(i),"####"
                );
        }

        return outBox;
    }

    string Base02String_to_Base16String (string InString)
    {
        string inBox = FOwO_string_convert_Helper.toUpper(InString);
        string outBox = "";

        for(int i = 0 ; i < inBox.length() ; i++)
        {
            outBox = outBox + FOwO_vector_Helper.FunctionMap<string,char>
            (
                {"0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111"},
                {   '0',   '1',   '2',   '3',   '4',   '5',   '6',   '7',   '8',   '9',   'A',   'B',   'C',   'D',   'E',   'F'},
                FOwO_string_manip_Helper.trim_Safe(inBox,i * 4,i * 4 + 3),'#');
        }

        return outBox;
    }
    

    string StringLogic (char InChar0 , char InChar1 , string InLogic , string InStringA, string InStringB)
    {
        //treating every charracter in 2 given strings as bits
        //performing bitwise operations, then return the result as a string
        //we can refer to FOwO_bool.logic_simple() to check the allowed operations

        string outString = "";
        
        if(InStringA.length() == InStringB.length())
        {
            for(int i = 0 ; i < InStringA.length() ; i++)
            {
                bool BoolGotA;
                bool BoolGotB;

                if (InStringA.at(i) == InChar0) {BoolGotA = false;} else if (InStringA.at(i) == InChar1) {BoolGotA = true;}
                if (InStringB.at(i) == InChar0) {BoolGotB = false;} else if (InStringB.at(i) == InChar1) {BoolGotB = true;}

                bool BoolResult =  FOwO_bool_Helper.logic_simple(InLogic, BoolGotA, BoolGotB);

                if (BoolResult) { outString = outString + InChar1 ; } else { outString = outString + InChar0 ;}
            }
        }
        else
        {
            cout << FOwO_string_shorthand_Helper.ColorText("r") << "FQAQ : given string has different lengths, will not " << InLogic << FOwO_string_shorthand_Helper.ColorText("d") << endl;
            cout << FOwO_string_shorthand_Helper.ColorText("r") << "InStringA : " << InStringA << " , InStringB : " << InStringB << FOwO_string_shorthand_Helper.ColorText("d") << endl;
            outString = "";
        }

        return outString;
    }


};

class FOwO_string
{
    public :

    FOwO_string_shorthand   shOwOrthand;
    FOwO_string_convert     cOwOnvert;
    FOwO_string_manip       mOwOnip;
    FOwO_string_detect      dOwOtect;
    FOwO_string_math        mOwOth;
};



class FOwO_cout
{
    public:

    FOwO_generic fowo_gen;
    FOwO_string_shorthand fowo_string_shorthand;
    FOwO_string fowo_string;

    FOwO_cout(){}

    void clearConsole ()
    {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    int MakeSpace(int Iter, string ExtraTitle)
    {
        //making enough space on the console
        //ExtraTitle is optional

        for(int i = 0; i < Iter ; i++)
        {
            cout << endl;
        }

        cout << ExtraTitle << endl;
        return Iter;
    }

    int Debug(bool DebugSwitch, string Message, bool NextLine)
    {
        if(DebugSwitch == true)
        {
            if(NextLine == false)
            {
                cout << Message;
            }
            else
            {
                cout << Message << endl;
            }  
        }

        return 0;
    }

    int ForcePrintFileASCII (string FileName, int InRythym, int InAsciiOffset, int InCropStart, int InCropEnd)
    {
        //prints a file as numbers
        //Rythym : how many ints in a console row ?
        //InAsciiOffset : if the byte is represented as a negative number (because of 2's compliment), how much to add to make the 2's comp number to become a positive int ?
        //  in a byte, the value is 256
        //InCropStart and InCropEnd : printing the bytes in between position InCropStart and  InCropEnd, if both are <0, then it prints everything

        fstream FileOwO(FileName);
        FOwO_string_math fowo_string_math;
        FOwO_string_manip fowo_string_manip;

        if(!FileOwO)
        {
            cout << fowo_string_shorthand.ColorText("r") << "FQAQ : error opening file" << fowo_string_shorthand.ColorText("d") << endl;
            return 1;
        }
        else
        {
            char BoxChar;
            int BoxInt = 0;
            int Rym = 0;
            int CropIndex = 0;
            while(FileOwO.get(BoxChar))
            {
                if
                (
                    (InCropStart <= CropIndex && CropIndex <= InCropEnd) ||
                    (InCropStart < 0 && InCropEnd < 0)
                )
                {
                    if((int)BoxChar < 0)
                    {
                        BoxInt = (int)BoxChar + InAsciiOffset;
                    }
                    else
                    {
                        BoxInt = (int)BoxChar;
                    }
                    
                    cout << fowo_string_manip.padding(fowo_string_math.Int_to_Base16String(BoxInt),"0",2,'r') << "  ";
                    Rym++;

                    if(Rym >= InRythym)
                    {
                        cout << endl;
                        Rym = 0;
                    }
                }
                CropIndex++;
            }

            FileOwO.close();
            cout << endl << endl << "Total Bytes : " << CropIndex << endl << endl;
        }

        return 0;
    }

    string ConsoleQuick (string InType, string InString)
    {
        //printing a string in a given style

        if      (InType == "w" || InType == "rep" || InType == "report")
        {
            return fowo_string_shorthand.ColorText("d") + "FOoO : " + InString + fowo_string_shorthand.ColorText("d");
        }
        else if (InType == "r" || InType == "err" || InType == "error")
        {
            return fowo_string_shorthand.ColorText("r") + "FQAQ : " + InString + fowo_string_shorthand.ColorText("d");
        }
        else if (InType == "g" || InType == "suc" || InType == "success")
        {
            return fowo_string_shorthand.ColorText("g") + "FOwO : " + InString + fowo_string_shorthand.ColorText("d");
        }
        else if (InType == "y" || InType == "prm" || InType == "prompt")
        {
            return fowo_string_shorthand.ColorText("y") + "FOoO : " + InString + fowo_string_shorthand.ColorText("d");
        }
        else
        {
            return "";
        }
    }

    template <typename TYPE>
    string ForcePrintVector(vector<TYPE> InVector, string InVectorName, string ItemLeftSide, string ItemSeparate, string ItemRightSide, int paddingLength, char AlignDirection, string paddingString)
    {
        //print everthing in a vector to the console
        //WARNING : paddingLength cannot be negative

        if (paddingString == "")
        {
            cout << ConsoleQuick("error","padding string is empty, will not work") << endl;
            return "";
        }

        string outString = InVectorName + " = {";

        for(int i = 0 ; i < InVector.size() ; i++)
        {
            stringstream Buffer;
            Buffer.clear();
            Buffer << InVector[i];
            string VectorSegment = "";
            Buffer >> VectorSegment;

            VectorSegment = fowo_string.mOwOnip.padding(VectorSegment,paddingString,paddingLength,AlignDirection);
            outString = outString + ItemLeftSide + VectorSegment + ItemRightSide;

            if(i < InVector.size() - 1){ outString = outString + ItemSeparate;}
        }

        return outString + "}";

    }

    void PromptUserStrings(vector<string*> InStringList, vector<string> InMessages)
    {
        if (InStringList.size() != InMessages.size())
        {
            cout << ConsoleQuick("error","the size for both InStringList and InMessages do not match") << endl;
            return;
        }

        for(int i = 0 ; i < InStringList.size() ; i++)
        {
            cout << ConsoleQuick("prompt",InMessages[i]);
            getline(cin,*InStringList[i]);
        }

        return;

        /*
        here's a sample code on how to use this function :

        ```
        FOwO fowo;

        string BoxA;
        string BoxB;
        string BoxC;
        string BoxD;

        //keep in mind, when passing in the string variables, we are passing the address into it
        fowo.cOwOut.PromptUserStrings({&BoxA,&BoxB,&BoxC,&BoxD},{"now for A : ","and then B : ","now is C : ", "and finally D :"});

        cout << "entered A : " << BoxA << endl;
        cout << "entered B : " << BoxB << endl;
        cout << "entered C : " << BoxC << endl;
        cout << "entered D : " << BoxD << endl;
        ```
        */
    }

    template <typename TYPE>
    string GenerateTable (vector<string> InListHeader, vector<int> InListColumnWidth, vector<char> InListColumnAlign, vector<TYPE> InListData)
    {
        string Table;

        //Table width = InListHeader.size()

        /*
        |         | Header0 | Header1 | Header2 | ... | HeaderN |
        |---------|---------|---------|---------| ... |---------|
        | Row0    |       0 |       1 |       2 | ... | 3       |
        | Row1    |       0 |       1 |       2 | ... | 3       |
        | Row2    |       0 |       1 |       2 | ... | 3       |
        */

        if(InListHeader.size() != InListColumnWidth.size())
        {
            return "";
        }

        //deal with the header line first
        for (int i = 0 ; i < InListHeader.size() ; i++)
        {
            //for every header
            Table = Table + "| "; //add the left margin

            if (InListHeader[i].length() <= InListColumnWidth[i] )
            {
                //if the header length is shorter than the length
                Table = Table + fowo_string.mOwOnip.padding(InListHeader[i]," ",InListColumnWidth[i],'l');
            }
            else
            {
                //if the header length is too long
                //then crop it
                Table = Table + fowo_string.mOwOnip.trim(InListHeader[i],0,InListColumnWidth[i]-1);
            }

            Table = Table + " "; //add the right margin

        }
        Table = Table + "|\n";

        //then the separation lines
        for (int i = 0 ; i < InListHeader.size() ; i++)
        {
            //for every header
            Table = Table + "|";

            for (int j = 0 ; j < InListColumnWidth[i] + 2 ; j++)
            {
                Table = Table + "-";
            }
        }
        Table = Table + "|\n";

        //then the content
        for(int i = 0 ; i < InListData.size() ; i++)
        {
            Table = Table + "| ";
            string BoxStep0 = fowo_string.cOwOnvert.AnythingToString<TYPE>(InListData[i]);
            string BoxStep1 = fowo_string.mOwOnip.trim_Safe(BoxStep0,0, InListColumnWidth[ i % InListColumnWidth.size() ]);
            string BoxStep2 = fowo_string.mOwOnip.padding(BoxStep1," ", InListColumnWidth[ i % InListColumnWidth.size() ], InListColumnAlign[ i % InListColumnAlign.size()]);
            
            Table = Table + BoxStep2 + " ";

            //if the whole row is now done
            if (i % InListHeader.size() == InListHeader.size() - 1 )
            {
                Table = Table + "|\n";
            }
        }

        return Table;
    }

    template <typename TYPE>
    bool PromptForNumber(string InPromptText, TYPE& InCarrier)
    {
        string USERInsert = "";
        cout << ConsoleQuick("prompt",InPromptText);
        getline(cin,USERInsert);

        if (fowo_string.mOwOth.isNumber_Simple(USERInsert) > 0)
        {
            if (fowo_gen.DataTypeOfVar_Predef(InCarrier) == "int")
            {
                InCarrier = stoi(USERInsert);
                return true;
            }
            else if (fowo_gen.DataTypeOfVar_Predef(InCarrier) == "long")
            {
                InCarrier = stol(USERInsert);
                return true;
            }
            else if (fowo_gen.DataTypeOfVar_Predef(InCarrier) == "long long")
            {
                InCarrier = stoll(USERInsert);
                return true;
            }
            else if (fowo_gen.DataTypeOfVar_Predef(InCarrier) == "float")
            {
                InCarrier = stof(USERInsert);
                return true;
            }
            else if (fowo_gen.DataTypeOfVar_Predef(InCarrier) == "double")
            {
                InCarrier = stod(USERInsert);
                return true;
            }
            else
            {
                cout << ConsoleQuick("error","carrier's datatype is invalid") << endl;
                return false;
            }
        }
        else
        {
            cout << ConsoleQuick("error","user input cannot be interpreted as a number") << endl;
            return false;
        }

        return false;

        /*
        
        int BoxInt;
        long BoxLong;
        long long BoxLongLong;
        float BoxFloat;
        double BoxDouble;
        char BoxChar;
        string BoxString;

        string USERInput;

        fowo.cOwOut.PromptForNumber("gimme an int      : ",BoxInt);
        fowo.cOwOut.PromptForNumber("gimme a long      : ",BoxLong);
        fowo.cOwOut.PromptForNumber("gimme a long long : ",BoxLongLong);
        fowo.cOwOut.PromptForNumber("gimme a float     : ",BoxFloat);
        fowo.cOwOut.PromptForNumber("gimme a double    : ",BoxDouble);

        cout << "user provided : " << endl;
        cout << BoxInt << endl;
        cout << BoxLong << endl;
        cout << BoxLongLong << endl;
        cout << BoxFloat << endl;
        cout << BoxDouble << endl;

        */
    }

};



class FOwO_network_action
{
    public :

    FOwO_cout fowo_cout;

    int talk_send(string Given, int TargetSocket )
    {
        //attempts to send a string across the connection
        //if success, this will return the number of sent bytes
        //if fail, return -1


        int ReturnResult = 0;

        if(TargetSocket >= 0)
        {
            const char* GivenPass1 = Given.data();
            ssize_t SentBytes = send(TargetSocket, &GivenPass1, Given.length(), 0); //ssize_t send(int sockfd, const void *buf, size_t len, int flags);
            if(SentBytes == -1)
            {
                //something is wrong
                ReturnResult = -1;
                cout << fowo_cout.ConsoleQuick("error","unable to send message") << endl;
            }
            else
            {
                ReturnResult = SentBytes;
            }
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error", "invalid socket") << endl;
            ReturnResult = -1;
        }

        return ReturnResult;
    }

    string talk_recv(int InTargetSocket, int InBufferLength)
    {
        //attempt to receive a message on the network
        //if success, will return the received string
        //if fail, will return empty string

        string ResultString = "";

        if(InTargetSocket >= 0)
        {
            char buffer[InBufferLength];
            int BytesReceived = recv(InTargetSocket, buffer, InBufferLength - 1,0);

            if(BytesReceived > 0)
            {
                buffer[BytesReceived] = '\0'; // null terminate the received data
                string Result(buffer);
                ResultString = Result;
            }
            else if (BytesReceived == 0)
            {
                cout << fowo_cout.ConsoleQuick("error","connection closed by remote host") << endl;
                ResultString = "";
            }
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","invalid socket") << endl;
            ResultString = "";
        }

        return ResultString;
    }

};

class FOwO_network_machine_server
{
    public :

    FOwO_string_shorthand fowo_string_shorthand;
    FOwO_cout fowo_cout;

    int ServerSocket;
    int ClientSocket;
    struct sockaddr_in ServerAddr;
    struct sockaddr_in ClientAddr;
    socklen_t AddrSize;
    string IPAddr;
    int PortNumber;

    stringstream STR_Buffer;
    string STR_Send = "";
    string STR_Recv = "";

    bool Connection_Setup (int InPortNumber, int InPortListenBacklog)
    {
        //tries to setup a connection

        //STEP 1 : create a socket
        ServerSocket = socket(AF_INET,SOCK_STREAM,0);
        if(ServerSocket == 0)
        {
            cout << fowo_cout.ConsoleQuick("error","socket creation error");
            return false;
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("success","socket created");
        }

        //STEP 2 : setup Port Number
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_addr.s_addr = INADDR_ANY;
        ServerAddr.sin_port = htons(InPortNumber);

        //STEP 3 : bind Socket and Port
        if( ::bind(ServerSocket, (struct sockaddr*) &ServerAddr, sizeof(ServerAddr)) < 0)
        {
            cout << fowo_cout.ConsoleQuick("error","Socket bind Failed") << endl;
            return false;
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("success","Socket bind Success") << endl;
            cout << fowo_cout.ConsoleQuick("report","now attempt to listen") << endl;
        }

        //STEP 4 : Listen
        if (listen(ServerSocket, InPortListenBacklog) < 0) //refer to notes at the class FOwO_network
        {
            cout << fowo_cout.ConsoleQuick("error","listened to nothing ...") << endl;
            return false;
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("success","listened success") << endl;
        }

        //STEP 5 : Connection Setup, accept connection from client
        AddrSize = sizeof(ClientAddr);
        ClientSocket = accept(ServerSocket, (struct sockaddr*) &ClientAddr, &AddrSize);
        if(ClientSocket < 0)
        {
            cout << fowo_cout.ConsoleQuick("error","accept connection failed") << endl;
            return false;
        }

        //STEP X : Success Connection
        cout << fowo_cout.ConsoleQuick("success","Connected, yay") << endl;
        cout << fowo_cout.ConsoleQuick("report","PortNumber : " + to_string(InPortNumber)) << endl;
        return true;
    }

    bool Connection_Setup_Force (int InPortNumber, int InPortListenBacklog)
    {
        //set up a connection
        //if the set up fails, try again
        //until it success to setup a connection

        bool JobDone = false;

        while(!JobDone)
        {
            JobDone = Connection_Setup(InPortNumber, InPortListenBacklog);
        }

        return true;
    }

    int MessageSEND (string InMessage)
    {
        //send a message across the network
        const char* MessageAsCString = InMessage.c_str();
        return (int) send(ClientSocket, MessageAsCString, strlen(MessageAsCString), 0);
    }

    string MessageRECV (int InBufferLength, bool InIfFailForceExit)
    {
        //receive a message from a port or a socket
        //if it fails to receive a message, then force the program to quit

        char MessageOwO[InBufferLength];
        memset(MessageOwO, 0, InBufferLength); //clear the buffer

        int BytesReceived = recv(ClientSocket, MessageOwO, InBufferLength - 1, 0);
        if (BytesReceived < 0)
        {
            cout << fowo_cout.ConsoleQuick("error","received failed") << endl;
            if(InIfFailForceExit)
            {
                exit(1);
            }
            return "";
        }
        else if (BytesReceived == 0)
        {
            cout << fowo_cout.ConsoleQuick("error","connection closed by the client") << endl;
            if(InIfFailForceExit)
            {
                exit(1);
            }
            return "";
        }
        else
        {
            MessageOwO[BytesReceived] = '\0';
            return string (MessageOwO);
        }
    }

    void Connection_Close()
    {
        if(ServerSocket > 0){close(ServerSocket) ; }
        if(ClientSocket > 0){close(ClientSocket) ; }
        cout << fowo_cout.ConsoleQuick("y","Server Connection Closed");
    }

};

class FOwO_network_machine_client
{
    public : 

    FOwO_string_shorthand fowo_string_shorthand;
    FOwO_cout fowo_cout;

    int ServerSocket;
    int ClientSocket;
    struct sockaddr_in ServerAddr;
    struct sockaddr_in ClientAddr;
    socklen_t AddrSize;
    string IPAddr;
    int PortNumber;

    stringstream STR_Buffer;
    string STR_Send = "";
    string STR_Recv = "";

    bool Connection_Setup (string InIPAddress, int InPortNumber)
    {
        //tries to setup a connection

        //STEP 1 : Create Socket
        ClientSocket = socket(AF_INET,SOCK_STREAM,0);
        if (ClientSocket == 0)
        {
            cout << fowo_cout.ConsoleQuick("error","socket creation error") << endl;
            return false;
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("success","socket created") << endl;
        }

        //STEP 2 : Accept and configure IPAddress and PortNumber
        IPAddr = InIPAddress;
        PortNumber = InPortNumber;
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_port = htons(PortNumber);

        //STEP 3 : convert IPv4 or IPv6 from string form to binary form
        if( inet_pton(AF_INET, IPAddr.c_str(), &ServerAddr.sin_addr) <= 0)
        {
            cout << fowo_cout.ConsoleQuick("error","invalid address / address not supported") << endl;
            return false;
        }

        //STEP 4 : Connect to the server
        if( connect(ClientSocket, (struct sockaddr*) &ServerAddr, sizeof(ServerAddr)) < 0)
        {
            cout << fowo_cout.ConsoleQuick("error","connection failed") << endl;
            return false;
        }

        //STEP X : Connection success
        cout << fowo_cout.ConsoleQuick("success","connected, yay OwO") << endl;
        return true;
    }

    bool Connection_Setup_Force (string InIPAddress, int InPortNumber)
    {
        //set up a connection
        //if the set up fails, try again
        //until it success to setup a connection

        bool JobDone = false;

        while(!JobDone)
        {
            JobDone = Connection_Setup(InIPAddress, InPortNumber);
        }

        return true;
    }

    int MessageSEND (string InMessage)
    {
        //send a message across the network

        const char* MessageAsCString = InMessage.c_str();
        return (int) send(ClientSocket, MessageAsCString, strlen(MessageAsCString), 0);
    }

    string MessageRECV (int InBufferLength, bool InIfFailForceExit)
    {
        //receive a message from a port or a socket
        //if it fails to receive a message, then force the program to quit

        char MessageOwO[InBufferLength];
        memset(MessageOwO, 0, InBufferLength); //clear the buffer

        int BytesReceived = recv(ClientSocket, MessageOwO, InBufferLength - 1, 0);
        if (BytesReceived < 0)
        {
            cout << fowo_cout.ConsoleQuick("error","received failed") << endl;
            if(InIfFailForceExit)
            {
                exit(1);
            }
            return "";
        }
        else if (BytesReceived == 0)
        {
            cout << fowo_cout.ConsoleQuick("error","connection closed by the client") << endl;
            if(InIfFailForceExit)
            {
                exit(1);
            }
            return "";
        }
        else
        {
            MessageOwO[BytesReceived] = '\0';
            return string (MessageOwO);
        }

        
    }

    void Connection_Close()
    {
        if(ServerSocket > 0){close(ServerSocket) ; }
        if(ClientSocket > 0){close(ClientSocket) ; }
        cout << fowo_cout.ConsoleQuick("y","Client Connection Closed");
    }

};

class FOwO_network
{
    public :

    string MachineType;

    FOwO_network_action actiOwOns;
    FOwO_network_machine_server sOwOrver;
    FOwO_network_machine_client clOwOnt;

    FOwO_cout fowo_cout;

    /*
    when sending and receiving messages between servers and clients
    each machine has ServerSocket and ClientSocket

    on the server side, when setting up the connection, we have :

    ```
    ServerSocket = socket(AF_INET, SOCK_STREAM, 0); // Server creates a socket for listening
    ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrSize); // After binding and listening, it accepts a client connection
    ```

    so now both the server machine and client machine have both the ServerSocket and ClientSocket;
    but when sending or receiving messages, which socket to use ?
    
    On the ServerSide
    {
        ** ServerSocket **
        is used to listen for incoming connections from Clients.
        It does not send or receive messages directly with CLients

        ** ClientSocket **
        is created by the server when it accepts a connection.
        This is used to send and receive messages to/from the specific client.

        so on the server side, both send and recv should be
        ```
        send(ClientSocket, message, strlen(message), 0);
        recv(ClientSocket, message, sizeof(message) - 1, 0);
        ```
    }

    On the ClientSide
    {
        ** ClientSocket **
        This socket is used to send and receive messages to/from the server after the client connects

        ** ServerSocket **
        Clients don't quite have a ServerSocket,
        Clients only have a ClientSocket to communicate with the server

        so on the client side, both send and recv should be
        ```
        send(ClientSocket, message, strlen(message), 0);
        recv(CLientSocket, message, sizeof(message) - 1, 0);
        ```
    }

    
    */

    void FLEX_SetupForce(int IfServer_Port, int IfServer_BackLog, string IfClient_IPAddress, int IfClient_Port)
    {
        //setup a connection based on the MachineType

        if (MachineType == "server")
        {
            cout << fowo_cout.ConsoleQuick("report","attempt to setup as server") << endl;
            sOwOrver.Connection_Setup_Force(IfServer_Port, IfServer_BackLog);
            cout << fowo_cout.ConsoleQuick("success","setup successful") << endl;
        }
        else if (MachineType == "client")
        {
            cout << fowo_cout.ConsoleQuick("report","attempt to setup as server") << endl;
            clOwOnt.Connection_Setup_Force(IfClient_IPAddress, IfClient_Port);
            cout << fowo_cout.ConsoleQuick("success","setup successful") << endl;
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","MachineType unclear") << endl;
        }
    }

    string FLEX_MessageSEND(string InMessage)
    {
        //sends a message, based on the machine type

        string OutString = "";

        if(MachineType == "server")
        {
            OutString = sOwOrver.MessageSEND(InMessage);
        }
        else if (MachineType == "client")
        {
            OutString = clOwOnt.MessageSEND(InMessage);
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","MachineType is unclear, MachineType is : " + MachineType) << endl;
            OutString = "";
        }

        return OutString;
    }

    string FLEX_MessageRECV(int InBufferLength, bool IfFailForceExit)
    {
        //receives a message, based on the machine type

        string OutString = "";

        if(MachineType == "server")
        {
            OutString = sOwOrver.MessageRECV(InBufferLength, IfFailForceExit);
        }
        else if (MachineType == "client")
        {
            OutString = clOwOnt.MessageRECV(InBufferLength, IfFailForceExit);
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","MachineType is unclear, MachineType is : " + MachineType) << endl;
            OutString = "";
        }

        return OutString;
    }

    void FLEX_Close()
    {
        //closes a connection, based on the MachineType

        if(MachineType == "server")
        {
            sOwOrver.Connection_Close();
        }
        else if (MachineType == "client")
        {
            clOwOnt.Connection_Close();
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","MachineType is unclear, MachineType is : " + MachineType) << endl;
        }
    }




};



class FOwO_cryptography
{
    public :

    FOwO_vector fowo_vector;
    FOwO_bool fowo_bool;
    FOwO_math fowo_math;
    FOwO_string fowo_string;
    FOwO_cout fowo_cout;

    int Hash_2024_0229_1925 (string InString, int startNumber, int factor, int size)
    {
        int Box = startNumber;

        for(int i = 0 ; i < InString.length() ; i++)
        {
            Box += (int)InString.at(i);
            Box *= factor;
        }

        Box = Box / factor;
        Box = Box % size;

        return Box;
    }



    string SDES_Permute_P10 (string InString)
    {
        return fowo_string.mOwOnip.permute({2,4,1,6,3,9,0,8,7,5},InString,'#',10);
    }

    string SDES_Permute_P8 (string InString)
    {
        return fowo_string.mOwOnip.permute({5,2,6,3,7,4,9,8},InString,'#',10);
    }

    string SDES_Permute_P4 (string InString)
    {
        return fowo_string.mOwOnip.permute({1,3,2,0},InString,'#',4);
    }

    string SDES_Permute_IP (string InString)
    {
        return fowo_string.mOwOnip.permute({1,5,2,0,3,7,4,6},InString,'#',8);
    }

    string SDES_Permute_RP (string InString)
    {
        return fowo_string.mOwOnip.permute({3,0,2,4,6,1,7,5},InString,'#',8);
    }

    string SDES_Permute_EP (string InString)
    {
        return fowo_string.mOwOnip.permute({3,0,1,2,1,2,3,0},InString,'#',4);
    }

    string SDES_Arith_Logic (char InChar0 , char InChar1 , string InLogic , string InStringA, string InStringB)
    {
        string outString = "";

        if(InStringA.length() == InStringB.length())
        {
            for(int i = 0 ; i < InStringA.length() ; i++)
            {
                bool BoolGotA;
                bool BoolGotB;

                if (InStringA.at(i) == InChar0) {BoolGotA = false;} else if (InStringA.at(i) == InChar1) {BoolGotA = true;}
                if (InStringB.at(i) == InChar0) {BoolGotB = false;} else if (InStringB.at(i) == InChar1) {BoolGotB = true;}

                bool BoolResult =  fowo_bool.logic_simple(InLogic, BoolGotA, BoolGotB);

                if (BoolResult) { outString = outString + InChar1 ; } else { outString = outString + InChar0 ;}
            }
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","given string has different lengths, will not " + InLogic) << endl;
            cout << fowo_cout.ConsoleQuick("error","InStringA : " + InStringA + " , InStringB : " + InStringB) << endl;
            outString = "";
        }

        return outString;
    }

    string SDES_SBox (char InChar0 , char InChar1 , char Row0, char Row1, char Col0, char Col1, int BoxNumber)
    {
        int CoordX = 0;
        int CoordY = 0;

        if(Row0 == InChar1){CoordX += 2;}
        if(Row1 == InChar1){CoordX += 1;}

        if(Col0 == InChar1){CoordY += 2;}
        if(Col1 == InChar1){CoordY += 1;}

        vector<string> Table = {};
        switch (BoxNumber)
        {
            case 0  : Table = {"01","00","11","10"   ,   "11","10","01","00"   ,   "00","10","01","11"   ,   "11","01","11","10"} ; break ;
            case 1  : Table = {"00","01","10","11"   ,   "10","00","01","11"   ,   "11","00","01","00"   ,   "10","01","00","11"} ; break ;
            default : Table = {"##","##","##","##"   ,   "##","##","##","##"   ,   "##","##","##","##"   ,   "##","##","##","##"} ; break ;
        }

        return fowo_vector.MatrixChoice<string>(Table, 4, CoordX, CoordY, "##");
    }

    string SDES_Main_F(string InString, string subKey)
    {
        string outString = "";

        if (InString.length() == 8 && subKey.length() == 8)
        {
            string StringL = fowo_string.mOwOnip.trim(InString,0,3);
            string StringR = fowo_string.mOwOnip.trim(InString,4,7);

            string Phase0 = SDES_Permute_EP(StringR); //Phase0.length is 8
            string Phase1 = fowo_string.mOwOth.StringLogic('0','1',"xor",Phase0,subKey); //Phase1.length is 8
            string Phase2 = SDES_SBox('0','1',Phase1.at(0),Phase1.at(3),Phase1.at(1),Phase1.at(2),0) + SDES_SBox('0','1',Phase1.at(4),Phase1.at(7),Phase1.at(5),Phase1.at(6),1) ; //Phase2.length is 2 + 2 = 4
            string Phase3 = SDES_Permute_P4(Phase2); //Phase3.length = 4
            string Phase4 = fowo_string.mOwOth.StringLogic('0','1',"xor",Phase3,StringL); //Phase4.length is 4

            outString = Phase4 + StringR;
        }

        return outString;
    }

    string SDES_Main_Whole(string PlainText, string Key, string mode)
    {
        string KeyDrf1 = SDES_Permute_P10(Key);
        string KeyDrf2L = fowo_string.mOwOnip.trim(KeyDrf1,0,4);
        string KeyDrf2R = fowo_string.mOwOnip.trim(KeyDrf1,5,9);
        string KeyDrfA = fowo_string.mOwOnip.cycleBelt(KeyDrf2L,1,'l') + fowo_string.mOwOnip.cycleBelt(KeyDrf2R,1,'l');
        string KeyDrfB = fowo_string.mOwOnip.cycleBelt(KeyDrf2L,3,'l') + fowo_string.mOwOnip.cycleBelt(KeyDrf2R,3,'l');
        string KeySub0 = SDES_Permute_P8(KeyDrfA);
        string KeySub1 = SDES_Permute_P8(KeyDrfB);

        string Phase6 = "";

        if(mode == "enc")
        {
            string Phase1 = SDES_Permute_IP(PlainText);
            string Phase2 = SDES_Main_F(Phase1, KeySub0);
            string Phase3 = fowo_string.mOwOnip.permute({4,5,6,7,0,1,2,3},Phase2,'#',8);
            string Phase4 = SDES_Main_F(Phase3, KeySub1);
            string Phase5 = SDES_Permute_RP(Phase4);
            Phase6 = Phase5;
        }
        else if(mode == "dec")
        {
            string Phase1 = SDES_Permute_IP(PlainText);
            string Phase2 = SDES_Main_F(Phase1, KeySub1);
            string Phase3 = fowo_string.mOwOnip.permute({4,5,6,7,0,1,2,3},Phase2,'#',8);
            string Phase4 = SDES_Main_F(Phase3, KeySub0);
            string Phase5 = SDES_Permute_RP(Phase4);    
            Phase6 = Phase5;
        }
        
        return Phase6;
    }

    string SDES_Hash(string PlainText, string InitVector, string TenBitsKey)
    {
        //Plaintext can be any normal string, it doesn't need to be 0s and 1s

        string Plate_Raw = PlainText; //get the whole thing first
        string Chunk = "00000000";
        string Carry = InitVector;

        Plate_Raw = fowo_string.cOwOnvert.toAsciiDoubleHex(Plate_Raw); //convert to Base16
        Plate_Raw = fowo_string.mOwOth.Base16String_to_Base02String(Plate_Raw); //convert from Base16 to Base02

        while (Plate_Raw != "")
        {
            if (Plate_Raw.length() >= 8) //if the workload still has a size longer than a chunk
            {
                //move the first 8 bits from PlateRaw to Chunk
                Chunk = fowo_string.mOwOnip.trim(Plate_Raw,0,7); 
                Plate_Raw = fowo_string.mOwOnip.trim(Plate_Raw,8,Plate_Raw.length()-1);
            }
            else //if the workload is 8 bits long or lesser
            {
                //padd it with zeros, then move it from PlateRaw to Chunk
                Chunk = fowo_string.mOwOnip.padding(Plate_Raw,"0",8,'l'); 
                Plate_Raw = "";
            }

            //xor the thing
            Carry = fowo_string.mOwOth.StringLogic('0','1',"xor",Chunk,Carry);

            //encrypt
            Carry = SDES_Main_Whole(Carry,TenBitsKey, "enc");
        }

        return Carry;
    }



    vector<long long> DFHM_Chain (long long n, long long g)
    {
        //given the modular n and the generator, give all the possible public keys
        //where ResultList[i] = g^i mod n

        long long safeG = g % n;

        vector<long long> ResultList = {safeG};
        long long Box = safeG;

        bool JobDone = false;
        while(JobDone == false)
        {
            Box = (Box * safeG) % n;

            if (Box == safeG)
            {
                JobDone = true;
            }
            else
            {
                ResultList.push_back(Box);
            }

        }

        return fowo_vector.cycleBelt(ResultList,ResultList.size()-1);
    }

    bool DFHM_generatorTest (long long n, long long g)
    {
        //see if a given number is a legit generator in mod n

        vector<long long> GeneratorList = DFHM_Chain(n,g);

        return GeneratorList.size() == n - 1;
    }

    vector<long long> DFHM_generatorList (long long n)
    {
        //generate a list of generator based on the mod n 

        vector<long long> ResultList = {};

        for(long long i = 0 ; i <= n ; i++)
        {
            if (DFHM_generatorTest(n,i))
            {
                ResultList.push_back(i);
            }
        }
        return ResultList;
    }



    long long RSA_Generate_BaseOnPQE_Single (long long p, long long q, long long e, bool IfFailThenReport)
    {
        //find the private key, based on the given p, q, and e
        // if e is invalid, return -1;

        long long n = p * q ;
        long long totientN = (p-1) * (q-1);

        if (totientN % e == 0)
        {
            if (IfFailThenReport)
            {
                cout << fowo_cout.ConsoleQuick("error","given e = " + to_string(e) + " do not have multiplicative inverse in mod (p-1)(q-1) = " + to_string(n)) << endl;
            }
            return -1;
        }

        //stupid method:
        long long d = 1;

        while ( ((e * d) % totientN != 1) && d < totientN )
        {
            d++;
        }
        if (d == totientN)
        {
            if (IfFailThenReport)
            {
                cout << fowo_cout.ConsoleQuick("error","unable to find d for e = " + to_string(e)) << endl;
            }
            return -1;
        }



        return d;

    }

    void RSA_Generate_BaseOnPQ_List(long p, long q, bool IfFailThenReport)
    {
        long long n = p * q;
        long long totientN = (p-1)*(q-1);

        for(int i = 1 ; i < totientN ; i++)
        {
            long long Answer = RSA_Generate_BaseOnPQE_Single(p,q,i,IfFailThenReport);

            if (Answer != -1)
            {
                cout << fowo_cout.ConsoleQuick("success","found pair : p = " + to_string(p) + " , q = " + to_string(q) + " , n = " + to_string(n) + " , e = " + to_string(i) + " , d = " + to_string(Answer)) << endl;
            }
        }
    }




};



class FOwO_experimental_TOKEN
{
    public :

    string lex;
    float value;

    FOwO_experimental_TOKEN(string InLex, float InVal)
    {
        lex = InLex;
        value = InVal;
    }
};

class FOwO_experimental
{
    public :

    FOwO_vector fowo_vector;
    FOwO_math fowo_math;
    FOwO_cout fowo_cout;
    FOwO_string fowo_string;

    

   


};



class FOwO
{
    public :
    FOwO_generic gOwOn;
    FOwO_cout cOwOut;
    FOwO_vector vectOwOr;
    FOwO_bool bOwOl;
    FOwO_math mOwOth;
    FOwO_string strOwOng;
    FOwO_network netOwOrk;
    FOwO_cryptography cryptOwOgraphy;
    FOwO_experimental expOwOmental;

    FOwO()
    {
        //for now nothing
    }

    FOwO(FOwO& InNewFOwO)
    {
        //nothing
    }

    ~FOwO()
    {
        cout << strOwOng.shOwOrthand.ColorText("d") << strOwOng.shOwOrthand.ColorBackground("d");
    }

    
};


/*

WEIRD THINGYS THAT WE DO NOT KNOW :

---- ---- ---- ---- ---- ---- ---- ---- 2024-11-24

for some weird reasons, 
'0' + "."
you will get "ith A"

---- ---- ---- ---- ---- ---- ---- ---- 2024-11-25

if we have the following :

```
cout << '0' + "." << endl;
cout << '1' + "." << endl;
cout << '2' + "." << endl;
cout << '3' + "." << endl;
cout << '4' + "." << endl;
cout << '5' + "." << endl;
cout << '6' + "." << endl;
cout << '7' + "." << endl;
cout << '8' + "." << endl;
cout << '9' + "." << endl;
```

the console will print out the following :

```
ize()
ze()
e()
()
)
//and then a whole bunch of un printable charracters
```

*/
