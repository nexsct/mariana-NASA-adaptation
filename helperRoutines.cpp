/*****************************************************************************
* Notices: Mariana - Copyright ‹ 2007 United States Government as
* represented by the Administrator of the National Aeronautics and Space
* Administration.Ê All Rights Reserved.
* 
* Libsvm - support vector routine library - Copyright (c) 2000-2007
* Chih-Chung Chang and Chih-Jen Lin All rights reserved.
* 
* Disclaimers
* 
* Mariana No Warranty: THE SUBJECT SOFTWARE IS PROVIDEDÊ"AS IS" WITHOUT
* ANY WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY,
* INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE
* WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM
* INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR FREE,
* OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
* SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
* ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
* RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
* RESULTING FROM USE OF THE SUBJECT SOFTWARE.Ê FURTHER, GOVERNMENT AGENCY
* DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
* IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES ITÊ"AS IS."
* 
* Waiver and Indemnity:Ê RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS
* AGAINST THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND
* SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.Ê IF RECIPIENT'S USE OF
* THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES,
* EXPENSES OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM
* PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S USE OF THE SUBJECT
* SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED STATES
* GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR
* RECIPIENT, TO THE EXTENT PERMITTED BY LAW.Ê RECIPIENT'S SOLE REMEDY FOR
* ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
* AGREEMENT.
* 
* Libsvm Redistribution and use in source and binary forms, with or
* without modification, are permitted provided that the following
* conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* 
* 3. Neither name of copyright holders nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
* 
* Ê THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. ÊIN NO EVENT SHALL THE REGENTS OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/
#include "helperRoutines.h"
//#include <iostream>
//#include <iomanip>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <map>
//#include <string>
//#include <algorithm>
//#include <string>

using namespace std;

void Tokenize( 
		const string& str,
		vector<string>& tokens,
		const string& delimiters)
{
	// Skip delimiters at beginning of the input string, then find first
	// "non-delimiter" in the input string
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);


	// Loop: Identify successive tokens
	while( string::npos != pos || string::npos != lastPos) {

		// Found a token, add it to the vector.
		tokens.push_back( str.substr( lastPos, pos - lastPos));

		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of( delimiters, pos);

		// Find next "non-delimiter"
		pos = str.find_first_of( delimiters, lastPos);
		pos = str.find_first_of( delimiters, lastPos);
	}
}

int * randomOrder(int n)
{
	// initialize two arrays
	int* a = new int[n];
	struct timeval tv; // C requires "struct timval" instead of just "timeval"
	gettimeofday( &tv, 0);
	long int m = tv.tv_usec * getpid(); 
	srand( m);
	//initialize a
	for (int i = 0; i < n; i++){
		a[i] = i;
	}
	for (int i = n-1; n >= 0; n--)
	{
		
		int newI = rand()%i;
		int tmp = a[i];
		a[i] = a[newI];
		a[newI] = tmp;
	}
	return a;
	
}

//***************************************************************************
// getKt( T[], Area, ts) -- Get array of temperatures for simulated
// annealing?  NOTE: Is this the best way to pass variables?
void getKT( double T[], double Area, int ts)
{
  double odds;
  double per=fabs( 0.01*Area);
  T[0] = 1.01;
  for( int j=1; j<ts; j++) {
    odds = (double)j/(ts-1);
    T[j] = -log(1-odds+0.000000001)/per;//for treg A is nes so no -log
    // printf( "%g\t%g\t%g\n", 1-odds+0.000000001, per, T[j]);
  }
}

//***************************************************************************
// getRand() -- Get seed from TOD in milliseconds and generate random number?
double getRand()
{
  struct timeval tv; // C requires "struct timval" instead of just "timeval"
  gettimeofday( &tv, 0);
  long int n = tv.tv_usec * getpid(); 
  srand( n);
  return  (double)rand()/((double)(RAND_MAX)+(double)(1));
} 


//***************************************************************************
// roc( truth1[], data1[], &len, sRocFileSpec) -- Perform ROC test.  NOTE 1) 
// Can all input be passed by reference rather than as pointers?  Where 
// should the storage be allocated?  This might also require modifications 
// to bubble_sort.
double roc( double truth1[], double data1[], int &len, string sRocFileSpec)
{
  // Loop: Make copies of the data.
  double* truth = new double[ len];
  double* data = new double[ len];
  for( int g=0; g<len; g++) {
    truth[g]=truth1[g];
    data[g]=data1[g];
  }
  // cout << "roc: loaded " << len << " words" << endl;

  // Bubble sort TRUTH and DATA in ascending order of TRUTH?
  bubble_sort( truth, data, len);

  // Bubblesort TRUTH and DATA in descending order of DATA
  bubble_sort( data, truth, -len);
  
  // Allocate storage and initialize buffers to get the extremes
  double* trup = new double[ len];
  double* falp = new double[ len];
  trup[ 0]=0;
  falp[ 0]=0;
  // printf( "roc: line 548\t");
  // printf( "%d len - %d k\t",len,k);

  // Loop: Accumulate statistics
  int k=0;
  double Pcount=0, Ncount=0;
  double P=0, N=0;
  for( int i=0; i<len; i++) {
    // bias[i]=bias[i-1]-smallStep;
    // while(k<len) {
      // if( (data[k]-bias[i]<-smallStep) || k>=len) { k++; break;}
      // if( (bias[i] - data[k] > smallStep)) { k++; break;}
      
      if( truth[i]==1) {
        Pcount++;
        P++;
      }
      else {
        Ncount++;
        N++;
      }
      k++;
    // }
    trup[ i] = Pcount;
    falp[ i] = Ncount;
    // printf( " %g, %g, %g, %g\n",data[i],truth[i],trup[i],falp[i]);
  }
  // printf( "%d len - %d k\n",len,k);
  // printf( "%g:%g\t%g:%g\t%d\n",Pcount,P,Ncount,N,k);

  // Open file to hold ROC data for plot.  XXX: Modify this to get filespec
  // from the control parameter file.
  // char* filename = "roc.plot.data";
  // rocFile.open( filename, ios::out);
  ofstream rocFile;
  unsigned uWriteFile = 0;
  if( sRocFileSpec.size() > 0) {
    rocFile.open( sRocFileSpec.c_str(), ios::out);
    if( rocFile.bad()) {
      cerr << "roc: ERROR, "
           << "can't open roc ouput file (" << sRocFileSpec.c_str()
           << ")" << endl;
      return -1;
    }
    uWriteFile = 1;
  }
  if( uWriteFile) rocFile << "0\t" << trup[0]/P << endl;

  // Loop: save ROC information to file
  double Area=0;
  double tpr1=0, fpr1=0, tpr0=0, fpr0=0;
  for( int j=1;j<len;j++) {
    tpr1 = trup[j]/P;
    fpr1 = falp[j]/N;
    // Area += (tpr0+tpr1)*(fpr1-fpr0);
    Area += tpr1*(fpr1-fpr0);  // As in PC's MarianaC of 27-DEC-2007
    if( uWriteFile) rocFile << fpr1 << "\t" << tpr1 << endl;
    tpr0 = tpr1;
    fpr0 = fpr1;  
  }
  // Area = Area*0.5;  // As in PC's MarianaC of 27-DEC-2007

  // Report status, close file, free storage, and return result
  if( uWriteFile) rocFile << "1\t1" << endl;
  if( uWriteFile) rocFile.close();
  delete[] truth;
  delete[] data;
  delete[] trup;
  delete[] falp;
  
  return Area;
}

//***************************************************************************
// bubble_sort( sortArray[], secondArray[], &nSizeIn) -- Brute-force method 
// to bubblesort SORTARRAY and SECONDARRAY in ascending or descending order 
// of SORTARRAY, depending on whether NSIZEIN is > or < 0.  NOTE: Could 
// inputs be passed by reference rather than as pointers?  See note for 
// roc().
void bubble_sort( double sortArray[], double secondArray[], int nSizeIn)
{
  // Outer loop: Set ISDONE to 1, then apply inner sorting loop until done.
  int nSize = abs( nSizeIn);
  for( int i = 0; i < nSize-1; i++) {
    unsigned isDone = 1;
    for( int j=0; j < nSize-1; j++) {

      // If NSIZEIN < 0, sort in descending order.  If NSIZEIN > 0, sort in
      // ascending order
      if( ( nSizeIn < 0 && (sortArray[j] < sortArray[j+1])) ||
          ( nSizeIn > 0 && (sortArray[j] > sortArray[j+1]))) {
        double tempSort = sortArray[j];
        sortArray[j] = sortArray[j+1];
        sortArray[j+1] = tempSort;
                    
        double tempSecond=secondArray[j];
        secondArray[j]=secondArray[j+1];
        secondArray[j+1]=tempSecond;
        
        isDone = 0;
     }
   }
   if( isDone > 0) break;
  }
  return;  
}

//***************************************************************************
// bubble_sort_int( sortArray[], secondArray[], &nSizeIn) -- Brute-force method 
// to bubblesort SORTARRAY and SECONDARRAY in ascending or descending order 
// of SORTARRAY, depending on whether NSIZEIN is > or < 0.  NOTE: Could 
// inputs be passed by reference rather than as pointers?  See note for 
// roc().
void bubble_sort_int( int sortArray[], double secondArray[], int nSizeIn)
{
  // Outer loop: Set ISDONE to 1, then apply inner sorting loop until done.
  int nSize = abs( nSizeIn);
  for( int i = 0; i < nSize-1; i++) {
    unsigned isDone = 1;
    for( int j=0; j < nSize-1; j++) {

      // If NSIZEIN < 0, sort in descending order.  If NSIZEIN > 0, sort in
      // ascending order
      if( ( nSizeIn < 0 && (sortArray[j] < sortArray[j+1])) ||
          ( nSizeIn > 0 && (sortArray[j] > sortArray[j+1]))) {
        int tempSort = sortArray[j];
        sortArray[j] = sortArray[j+1];
        sortArray[j+1] = tempSort;
                    
        double tempSecond=secondArray[j];
        secondArray[j]=secondArray[j+1];
        secondArray[j+1]=tempSecond;
        
        isDone = 0;
     }
   }
   if( isDone > 0) break;
  }
  return;  
}

//***************************************************************************
// getNewParam( current, Max, temp) -- Calculate and return new temperature
// parameter for simulated annealing.
double getNewParam( double current, double Max, double temp)
{
  double next = current + temp*Max*(2*getRand()-1);
  return fabs( next);
}

//***************************************************************************

double evalModel(const svm_problem *valid,
		const svm_model *model,
		const svm_parameter *param)
{
	string st;
	struct svm_node *x;
	double* v = new double[ valid->l];
	double error = 0, correct = 0;
	double A, target;
	int h;
	for(h=0; h<valid->l; h++) {
		x = valid->x[h];
		target = valid->y[h];
		v[h] = svm_predict( model, x);
		if( v[h]*valid->y[h]>=0) ++correct;
//		if( uWrite) regFile << v[h] << endl;

		// Add regression measure
		error += (v[h]-target)*(v[h]-target);
	}
//	if( uWrite) regFile.close();

	// If this is a regression (EPSILON_SVR), calculate area based on error,
	// otherwise perform an ROC test, then write information to the console.
	if( param->svm_type==EPSILON_SVR) {
		A = -sqrt( error/valid->l);  // negative for the maximization prob
	}

	else {
		//A = roc( valid->y, v, valid.l, st);
		A = roc( valid->y, v, h, st);
	}

	return A;
}

double runTest(const svm_problem *valid,
		const svm_model *model,
		const svm_parameter *param,
		string sRegOutputFileSpec)
{
	string st;
	struct svm_node *x;
	double* v = new double[ valid->l];
	double error = 0, correct = 0;
	double A, target;
	int h;
	
	// Loop: Perform sucessive predictions for validation?  XXX: This will 
	// involve successive uses of the predict() method of B_Svm_Model.  If
	// possible, the pointers should be replaced with vectors.
	// FILE *reg = fopen( "output.reg", "w");
	ofstream regFile;
	unsigned uWrite = 0;
	if( sRegOutputFileSpec.size() > 0) {
		regFile.open( sRegOutputFileSpec.c_str(), ios::out);
		if( regFile.bad()) {
			cerr << "main: ERROR, "
			<< "can't open reg ouput file (" << sRegOutputFileSpec.c_str()
			<< ")" << endl;
			return -1;
		}
		uWrite = 1;
	}

	
	for(h=0; h<valid->l; h++) {
		x = valid->x[h];
		target = valid->y[h];
		v[h] = svm_predict( model, x);
		if( v[h]*valid->y[h]>=0) ++correct;
		if( uWrite) regFile << target << "," << v[h] << endl;

		// Add regression measure
		error += (v[h]-target)*(v[h]-target);
	}
	if( uWrite) regFile.close();

	// If this is a regression (EPSILON_SVR), calculate area based on error,
	// otherwise perform an ROC test, then write information to the console.
	if( param->svm_type==EPSILON_SVR) {
		A = -sqrt( error/valid->l);  // negative for the maximization prob
	}

	else {
		//A = roc( valid->y, v, valid.l, st);
		A = roc( valid->y, v, h, st);
	}

	return A;
}

void wait ( int seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}

