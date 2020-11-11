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
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include<map>
#include <string>
#include <algorithm>
#include<math.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>

using namespace std;

int verbose;

#define MAX_LINE_LENGTH 655360

template <typename out_type, typename in_value>
out_type stream_cast(const in_value & t)
{
	stringstream ss;
	ss << t; 
	out_type result; 
	ss >> result; 
	return result;
}

class param
{
	public:
		string svm_type;
		double gamma;
		double rho;
		vector<int> keep;
		void setParams(vector<string>);
};
void param::setParams(vector<string> pString)
{
	if(pString[0] == "svm_type")
	{
		svm_type = pString[1];
	}
	if(pString[0] == "gamma")
	{
		gamma = stream_cast <double> (pString[1]);
	}
	if(pString[0] ==  "rho")
	{
			rho = stream_cast <double> (pString[1]);
	}
	if(pString[0] == "dimensions")
	{
		for(int j=1; j<pString.size(); j++)
		{	
			keep.push_back(stream_cast <int> (pString[j]));
			//cout << keep[j-1] << " ";
		}
	}
}

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

double rmse(double *truth, double *est, int length, string fileName)
{
	double error = 0.0;
	for(int i = 0; i < length; i++)
	{
		error += (est[i]-truth[i])*(est[i]-truth[i]);
	}
	return sqrt(error/(double)length);
}

void usage()
{
  cerr << endl;
  cerr << "Usage: MarianaPredict -i input_file -m model_file -o output_file" << endl;
  cerr << endl;
  cerr << "Optional arguments:" << endl;
  cerr << "  -V, --verbose mode              "
       << "verbose output mode" << endl;
  cerr << "  -h, --help                      "
       << "display this message and then exit" << endl;

  exit( -1);
}

void Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
int maxBowDim(vector<map<int,double> > & bow)
{
	int theMax=0, k=0;
	map<int,double>:: iterator j;
	for(int i=1; i<bow.size(); i++)
	{
		k++;
		j=bow[i].end();
		j--;
		if(j->first > theMax) theMax = j->first;
		/*cout << j->first << "\t"
			<< k << endl;*/
	}
	return theMax;
}


void parseModelLine(string & S, 
	map<int,double> & elements, 
	vector<double> & weights)
{
	vector<string> spaceSplit;
	vector<string> colonSplit;
	double d;
	
	Tokenize(S, spaceSplit, " ");
	
	for(int j=0; j<spaceSplit.size(); j++)
	{
		if(j == 0)
		{
			d=stream_cast <double> (spaceSplit[j]);
			weights.push_back(d);
		}
		else
		{
			Tokenize(spaceSplit[j], colonSplit, ":");
			elements.insert(make_pair(stream_cast <int> (colonSplit[0]),
										stream_cast <double> (colonSplit[1]))); 
				
			colonSplit.clear();
		}
	}
}

void parsePredictLine(string & S, 
	map<int,double> & elements, 
	vector<double> & weights, param & P)
{
	vector<string> spaceSplit;
	vector<string> colonSplit;
	map<int,double>::iterator normI;
	double d, value,lineTotal=0.0;
	int index, nothing=0;
	
	Tokenize(S, spaceSplit, " ");
	
	for(int j=0; j<spaceSplit.size(); j++)
	{
		if(j == 0)
		{
			d=stream_cast <double> (spaceSplit[j]);
			weights.push_back(d);

		}
		else
		{
			Tokenize(spaceSplit[j], colonSplit, ":");
			index=stream_cast <int> (colonSplit[0]);
			//if( find(P.keep.begin(), P.keep.end(), index) != P.keep.end())
			{
				value = stream_cast <double> (colonSplit[1]);
				elements.insert(make_pair(index, value));
				lineTotal+=value;
				nothing=1;
				//cout << index << "::" << lineTotal << " ";
			}	
			colonSplit.clear();
		}
		
	}
	if(nothing==0)
	{
		elements.insert(make_pair(1,0.0));
		lineTotal=1.0;
	}
	nothing=0;
	//cout << elements.begin()->first << "::" << lineTotal << " ";
	//cout << endl;
	for(normI=elements.begin(); normI!=elements.end(); normI++)
	{
		normI->second=normI->second/lineTotal;
	}
	//cout << lineTotal << endl;
}
void readModelFile(string fileName, vector<map<int,double> > & bow,
			vector<double> & weights, param & P)
{
	map<int,double>::iterator mapI;
	vector<string> paramVec;
	int vecNum=0;
	
	char charLine[MAX_LINE_LENGTH];
	string line;
	ifstream inFile;
	
	inFile.open( fileName.c_str(), ios::in);
	
	if( inFile.bad())
	{
		cerr << "readModelFile: ERROR, "
		<< "can't open input file (" << fileName << ")" << endl;
		exit( 1);
	}
	if(verbose) {
		cout << "\nreadModelFile: "
		<< "loading model file <" << fileName << ">" << endl;
	}
	int svMode=0;
	while(!inFile.eof())
	{
		inFile.getline(charLine, MAX_LINE_LENGTH);
		string line(charLine);
		if(line == "!") continue;
		if(line == "SV")
		{
			svMode = 1;
			continue;
		}
		if(svMode)
		{
			if(inFile.eof())break;
			bow.push_back(map<int,double>());
			parseModelLine(line, bow[vecNum], weights);
			vecNum++;
		}
		else
		{
			Tokenize(line,paramVec);
			P.setParams(paramVec);
		}
		paramVec.clear();
		
	}
	inFile.close();

}

void readPredictFile(string fileName, vector<map<int,double> > & bow,
			vector<double> & categ, param & P)
{
	//cout << "read predict\n";
	map<int,double>::iterator mapI;
	//vector<string> paramVec;
	int vecNum=0;
	
	char charLine[MAX_LINE_LENGTH];
	string line;
	ifstream inFile;
	
	inFile.open( fileName.c_str(), ios::in);
	//cout << "opened file\n";
	if( inFile.bad())
	{
		cerr << "readPredictFile: ERROR, "
		<< "can't open input file (" << fileName << ")" << endl;
		exit( 1);
	}
	if(verbose){
		cout << "readPredictFile: "
		<< "loading input file for prediction <" << fileName << ">" << endl;
	}
	int svMode=1;
	while(!inFile.eof())
	{
		inFile.getline(charLine, MAX_LINE_LENGTH);
		string line(charLine);
		if(charLine[0] == '!') continue;
		if(charLine[0] == 'D') continue;
		//cout << line << " - ";
		if(svMode)
		{
			if(inFile.eof())break;
			bow.push_back(map<int,double>());
			parsePredictLine(line, bow[vecNum], categ, P);
			vecNum++;
		}
		
		//paramVec.clear();
		
	}
	inFile.close();

}

void printVector(vector<int> & V, string vecName="vector=")
{
	cout << vecName << "=[";
	for(int i=0; i<V.size(); i++)
	{
		cout << V[i];
		if(i != V.size()-1) cout << ", ";
		else cout << "]\n";
	}
}

void printMap(map<int,double> & M, string mapName="map=")
{
	map<int,double>::iterator i;
	cout << mapName << "=[";
	for(i=M.begin(); i!=M.end(); i++)
	{
		cout << i->first << "\t" << i->second;
		cout << "\n";
	}
	cout << "]\n";
}

double predict(vector<map<int,double> > & bow,
	map<int,double> & doc,
	vector<double> & weights,
	param & K)
{
	vector<map<int,double> >:: iterator bowI;
	vector<double>::iterator alpha;
	map<int, double>::iterator docJ;
	map<int, double>::iterator bowJ;
	
	double sum = 0.0, d = 0.0;
	double decision = 0.0;
	int counter1 = 0, counter2 = 0;
	char s = ':';
	alpha=weights.begin();
	
	for(int i = 0; i < bow.size(); i++)
	{
		docJ = doc.begin(); 
		bowJ = bow[i].begin();
		//cout << "loop " << *alpha << endl;
		while( bowJ != bow[i].end() && docJ != doc.end())
		{
			if(bowJ->first == docJ->first)
			{
				d = (bowJ->second - docJ->second);
				//cout << bowJ->second << " equal " << docJ->second << endl;
				sum += d * d;
				bowJ++;
				docJ++;
				continue;
			}
			else if(bowJ->first > docJ->first)
			{
				//cout << bowJ->first << " bow " << docJ->first << endl;
				sum += (docJ->second * docJ->second);
				docJ++;
				continue;
			}
			else
			{
				//cout << bowJ->first << " doc " << docJ->first << endl;
				sum += (bowJ->second * bowJ->second);
				bowJ++;
				continue;
			}
			counter1++;
		}
		
		while( bowJ != bow[i].end())
		{
			sum += (bowJ->second * bowJ->second);
			bowJ++;
		}
		
		while( docJ != doc.end())
		{
			sum += (docJ->second * docJ->second);
			docJ++;
		}
		counter2++;
		bowI++;
		decision += *alpha * exp(- K.gamma * sum);
//		cout << *alpha << s << sum << s << K.gamma << s 
//			<< (K.gamma * sum) << s << exp(- K.gamma * sum) 
//			<< s << decision << "\n";
		alpha++;
		sum = 0.0;
	}
	//cout << endl << decision << endl;
	return decision - K.rho;
}
	 

int main(int argc, char **argv)
{
	verbose=0;
	vector<map<int,double> > supportV;
	vector<map<int,double> > predictV;
	vector<double> weights;
	vector<double> categ;
	char model[1024];
	char input[1024];
	char output[1024];
	string modelName;
	string fileName;
	string outputName;	
	param P;
//	double outputs;
	
	if( argc < 2) {
    	usage();
    	return 0;
	}
	// Define a structure to hold the command-line options
	static struct option long_options[] = {
	    { "input", required_argument, 0, 'i'},
	    { "model", required_argument, 0, 'm'},
	    { "output", required_argument, 0, 'o'},
	    { "help", no_argument, 0, 'h'},
	    { "verbose", no_argument, 0, 'V'},
	    { 0, 0, 0, 0}
	};
	
	int c, isParsed;
	// Loop: Invoke GETOPT_LONG to parse successive command-line arguments 
  // NOTE 1) The possible options MUST be listed in the call to GETOPT_LONG.
  // NOTE 2) Be warned, this call is tricky, and MUST reflect the presence or
  // absence of optional arguments!  NOTE 3) This method does not effect arc
  // and argv in any way.
  // XXX: Change these parameters to make them consistent with SVM_LIGHT?
  while( 
    ( c = getopt_long( 
        argc, argv, 
        "i:m:o:hV", long_options, NULL)) != -1) {
  
    // Examine command-line options and extract any optional arguments
    switch( c) {

     
      // input_file: Extract filespec for training data
      case 'i':
        strcpy( input, optarg);
        if( strlen( input) <= 0) {
          cout << "No input data was specified" << endl;
          usage();
          exit( -1);
        }
        isParsed = 1;
        break;

      // validation_file: Extract filespec for validation data
      case 'm':
        strcpy( model, optarg);
        if( strlen( model) <= 0) {
          cout << "No model data was specified" << endl;
          usage();
          exit( -1);
        }
        isParsed = 1;
        break;

      // test_file: Extract filespec for test data
      case 'o':
        strcpy( output, optarg);
        if( strlen( output) <= 0) {
          cout << "No output file was specified" << endl;
          usage();
          exit( -1);
        }
        isParsed = 1;
        break;
     
      
      // Verbose: verbose mode
      case 'V':
        verbose = 1;
        isParsed = 1;
        break;
      
      // help, or unknown option, or missing argument
      case 'h':
      case ':':
      case '?':
      default:
        usage();
        exit( -1);
        break;
    }
  }
  	modelName.assign(model);
  	fileName.assign(input);
	readModelFile(modelName, supportV, weights, P);
	readPredictFile(fileName, predictV, categ, P);
	if(verbose)
	{
		cout << "\ngamma: " << P.gamma << endl;
		cout << "rho: " << P.rho << endl;
		
		cout << "\nsv   :\t" << supportV.size()
			<< " Docs w/ " << maxBowDim(supportV)
			<< " Terms\n";
		cout << "input:\t" << predictV.size()
			<< " Docs w/ " << maxBowDim(predictV)
			<< " Terms\n\n";
	}//printVector(P.keep,"W");
	double* outputs = new double[predictV.size()];
	double * Category = new double[predictV.size()];
	//open file for output
	ofstream outputPred;
	outputPred.open(output, ios::out);
	if(outputPred.bad())
	{
		cerr << "errror opening file "
		<< outputPred << ".\n";
		exit(-1);
	}
	
	// Eval the prediction
	/* for the classification the results is the dist*category, which is the
	 * same as the dv * -1.  See svm_predict in svm.cpp
	 * */
	double multi = 1;
	if(P.svm_type == "c_svc") multi = -1;
	for(int i=0; i<predictV.size(); i++)
	{
		outputs[i] = multi*predict(supportV, predictV[i], weights, P);
		if(verbose)cout << categ[i] << ",  " << outputs[i] << endl;
		outputPred << categ[i] << ",  " << outputs[i] << endl;
		Category[i]=categ[i];
	}
	int length = predictV.size();
	if(P.svm_type == "c_svc")
	{
		double Area = roc(Category, outputs, length, "");
		cout << "ROC Area: " << Area << endl;
	}
	else if (P.svm_type == "epsilon_svr")
	{
		double error = rmse(Category, outputs, length, "");
		cout << "RMSE: " << error << endl;
	}
	outputPred.close();
	//
	
}
