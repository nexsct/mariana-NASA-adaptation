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
#define MAX_LINE_LENGTH 655360


#include "uciType.h"

using namespace std;

uciType::uciType()
{
}

uciType::~uciType()
{
}

void uciType::read(string fileName)
{
	// Open the input file and make sure it exists
	ifstream inFile;
	inFile.open( fileName.c_str(), ios::in);
	if( inFile.bad()) {
		cerr << "load ERROR, "
		<< "can't open input file (" << fileName << ")" << endl;
		exit( 1);
	}
	else {
		int aaaaa = 1;
//		cout << "load "
//		<< "loading input file <" << fileName << ">" << endl;
	}

	char textline[ MAX_LINE_LENGTH];

	int tmpInstance = 1000;
	//initialize the category array
	Cat = new int[tmpInstance];

	int getLineCount = 0;
	
	while(!inFile.eof()){
		++getLineCount;
		int termCount = 0;
		inFile.getline( textline, MAX_LINE_LENGTH);
		istringstream ins;
		ins.str(textline);
		//int lineStart = ins.tellg();
		while(!ins.eof())
		{
			string tmp;
			ins >> tmp;
			if(tmp.find(":", 0) == string::npos){
				// check to see if the labesl are correct
				istringstream a;
				a.str(tmp);
				int b;
				a >> b;
				if(b*b != 1)
				{
					cerr << "Line " << b
					<< " should be labeled "
					<< " as +-1" << endl;
					cerr << "error : Line"
					<< getLineCount << "\n"
					<< textline << endl;

				}
			}
			else if(ins.eof()){
				break;
			}
			else{
				termCount++;
				//cout << tmp << " ";
			}
		}
		int * ind = new int[termCount];
		double * val = new double[termCount];
		ins.clear();
		ins.str(textline);
		if(termCount != 0)
		{
			//cout << termCount << " line 79 termcount\n";
			ins >> Cat[getLineCount - 1];
			//cout << getLineCount - 1 << "\t" ;
			for(int i = 0; i < termCount; i++)
			{
				string tmp;
				ins >> tmp;
//				if(!tmp.empty())
					sscanf(tmp.c_str(),"%d:%lf",&ind[i],&val[i]);
				//cout << ind[i] << ":" << val[i] << " ";
			}
			//cout << endl;
			featureBag.insertRow(getLineCount - 1, termCount, ind, val);
			//featureBag.printRow(getLineCount - 1);
		}
		else if(inFile.peek() != EOF)
		{
			cout << "problem w/ line " << textline << " at " 
				<< getLineCount << " and has " << termCount << endl;
		}
		// expand memory if necessary
		if(getLineCount == tmpInstance){
			tmpInstance += getLineCount;
			int* tmp = new int[tmpInstance];
			for(int i = 0; i < getLineCount - 1; i++)
			{
				tmp[i] = Cat[i];
			}
			delete [] Cat;
			Cat = tmp;
		}
		//cout << featureBag.rowSize(getLineCount - 2) << "line101\n";
	}

	inFile.close();
	// reduce Cat to the exact size
	{
		int *tmp = new int[getLineCount];
		for(int i = 0; i < getLineCount; i++)
			tmp[i] = Cat[i];
		Cat = tmp;

	}
	totalInstances = getLineCount - 1;

}

void uciType::setUsage(double *theSplit)
{
	useSplit = theSplit;
	usage = new int[totalInstances];
	int *rOrder = randomOrder(totalInstances);

	//for(int i = 0; i < 3; i++) cout << useSplit[i] << " ";

	trainCount = floor(useSplit[0]*(double)totalInstances);
	valCount = floor(useSplit[1]*(double)totalInstances);
	testCount = totalInstances - trainCount - valCount;

	trainIns_ = new int[trainCount];
	valIns_ = new int[valCount];
	testIns_ = new int[testCount];

	trainInsIn = 0;
	valInsIn = 0;
	testInsIn = 0;
	
	int first = trainCount;
	int second = trainCount + valCount;
	int third = totalInstances;
	for(int i = 0; i < totalInstances; i ++)
	{
		if(i < trainCount){
			usage[rOrder[i]] = 999;
			trainIns_[i] = rOrder[i];
			if(Cat[trainIns_[i]] == 1){
				trainInsIn++;
			}
		}
		else if(i >= trainCount &&
			i < second)
		{
			usage[rOrder[i]] = -888;
			valIns_[i - trainCount ] = rOrder[i];
			if(Cat[valIns_[i - trainCount]] == 1){
				valInsIn++;
			}
		}
		else// if(i >= second &&
			//i < totalInstances)
		{
			usage[rOrder[i]] = 7357;
			testIns_[i - trainCount - valCount] = rOrder[i];
			if(Cat[testIns_[i - trainCount - valCount]] == 1){
				testInsIn++;
			}
		}
	}
//	cout << "train/val/test" << trainCount << "/"
//		<< valCount << "/" << testCount << endl;
}
	

void uciType::reduceInstance(int number, double rPer)
{
	double natPer = ((double)trainInsIn + (double)valInsIn + (double)testInsIn)/(double)totalInstances;
	double usePer;
	
	int *tmpTrainIns = new int[trainCount];
	for(int i = 0; i < trainCount; i++) tmpTrainIns[i] = trainIns_[i];
	
	int newTrainCount;
	if(number < trainCount) newTrainCount = number;
	else newTrainCount = trainCount;
	
	if(natPer < rPer) usePer = rPer;
	else usePer = natPer;
	
	int trainInCount = round(usePer*(double)newTrainCount);
	int trainOutCount;
	if(trainInCount > trainInsIn){
		trainInCount = trainInsIn;
		trainOutCount = floor((double)trainInsIn*(-1 + 1/usePer));
		newTrainCount = trainInCount + trainOutCount;
	}
	else{
		trainOutCount = newTrainCount - trainInCount;
		//newTrainCount = trainInCount + trainOutCount;

	}
	
//	cout << trainInCount << "/" << trainInsIn << " - "
//		<< trainOutCount << "/" << (trainCount - trainInsIn) << " - "
//		<< usePer << "/" << natPer << "/" << rPer << endl;
	
	int *newTrainIns = new int[newTrainCount];
	int tmpInc = trainCount;
	trainCount = newTrainCount;
	
	while(newTrainCount > 0)
	{
		tmpInc--;
		if(Cat[tmpTrainIns[tmpInc]] == 1 && trainInCount > 0){
			newTrainCount--;
			newTrainIns[newTrainCount] = tmpTrainIns[tmpInc];
			trainInCount--;
//			cout << tmpInc << " " << newTrainCount << " " << trainInCount << " " << trainOutCount << " "
//				<< newTrainIns[newTrainCount] << " "
//				<< Cat[newTrainIns[newTrainCount]] << endl;
		}
		else if(Cat[tmpTrainIns[tmpInc]] == -1 && trainOutCount > 0)
		{
			newTrainCount--;
			newTrainIns[newTrainCount] = tmpTrainIns[tmpInc];
			trainOutCount--;
//			cout << tmpInc << " " << newTrainCount << " " << trainInCount << " " << trainOutCount << " "
//				<< newTrainIns[newTrainCount] << " "
//				<< Cat[newTrainIns[newTrainCount]] << endl;
		}
//		;
//		if(newTrainCount == 0) break;
	}
	
	delete [] trainIns_;
	trainIns_ = newTrainIns;
//	trainIns_ = new int[trainCount];
//	for(int i = 0; i < trainCount; i++)
//		trainIns_[i] = newTrainIns[i];
//	for(int i = 0; i < trainCount; i++) cout << trainIns_[i] << " ";
//	cout << endl;
}

void uciType::toLibSvmFormat( svm_problem & train,
	bool norm,
	int wht)
{
	normalize = norm;
	//struct svm_node *x_space;
	// set up the svm problem
	int *docs2use;
	if(wht == 1)
	{
		train.l = trainCount;
		docs2use = trainIns_;
	}
	else if(wht == 2)
	{
		train.l = valCount;
		docs2use = valIns_;
	}
	else if(wht == 3)
	{
		train.l = testCount;
		docs2use = testIns_;
	}
	
		//cout << "toLibSvmFormat(228): " << train.l << endl;
	train.x = new svm_node*[train.l];
	train.y = new double[train.l];

	double* sum = new double[train.l];
	for(int k=0; k<train.l; k++) sum[k]=1;
	int nElements=0;
	
//	for(int i = 0; i < train.l; i++) cout << docs2use[i] << " ";
//	cout << endl;
	
	int dim = featureBag.maxCol();
	// If no normalization was requested, simply increment the number of 
	// elements and set succesive elements of the SUM vector to '1'.  (Note 
	// that this was already done during the initlaization process above.)  
	// Otherwise, normalize term frequencies.
	if( normalize) {
		//if(verbose) cout << " normalize\n";

		// Loop: Evaluate values for successive lines of the svm_problem structure
		for( int h=0; h<train.l; h++) {

			// Add an element for each line (the lines with "-1")
			nElements++;

			// Add an element for each line (the lines with "-1")
			nElements++;
			
			// Get the index and value for this row
			//cout << h << ": " << docs2use[h] << " h/d\n";
			int rowSize = featureBag.rowSize( docs2use[h]);
//			cout << featureBag.rowSize( docs2use[h]) << " rowsize\n";
			//featureBag.printRow( docs2use[h]);
			int* idx = featureBag.getRowIndex( docs2use[h]);
//			cout << featureBag.rowSize( docs2use[h]) << " bbb\n";

			double* val = featureBag.getRowValue( docs2use[h]);
//			cout << featureBag.rowSize( docs2use[h]) << " aaa\n";

//			 cout << "index:" << h <<":" << idx[0]
//			      << ":" << val[0] <<":" << rowSize << " \n";

			// Loop: Evaluate successive elements in this line
			double tmp = 0;
			int kv = 0;
			while( kv < rowSize && kv < dim){
//				 cout << "MMM:" << kv <<":" << idx[kv] << ":" 
//				       << dim << ":" << val[kv] << " \n";

				
					tmp+=val[kv];
					// cout << "=idx>dim:" << idx[kv] << ":" << dim[kd] << endl;
					nElements++;
					kv++;
					
				
			}
			//delete [] idx;
			//delete [] val;
			sum[h] = tmp;
			//cout <<  h << endl;
		}
	}
	else {
		nElements = featureBag.nElements() + train.l;
		for(int h=0; h<train.l; h++) sum[h] = 1;
	}


	// Allocate storage for global buffer, x_space, that will be used to read 
	// all of the elements for this svm_problem structure.  NOTE 2) Does this 
	// have to be global?  NOTE 2) Why doesn't this crash during successive 
	// calls to this method?  Could the new (or malloc) operation be allocating 
	// new storage while leaking the old storage, which is later deallocated 
	// when the relevant svm_problem structure is destroyed?
	x_space = new svm_node[ nElements];

	 
	// Loop: Go through and map the BOW into the X_SPACE vector
	int max_index = 0;   // Biggest term index
	int j = 0;
	for( int i=0; i<train.l; i++) {
//		cout << "assign category:" << Cat[docs2use[i]] << endl;

//		cout << train.y[i] << " " << train.x[i] << "x_i\n";
		train.y[i] = Cat[docs2use[i]];
		train.x[i] = &x_space[j];
//		cout << "category " << train.y[i]  << endl;

		// Get dimensions and sizes associated with the BOW structure 
		int* idx = featureBag.getRowIndex( docs2use[i]);
		double* val = featureBag.getRowValue( docs2use[i]);
		int rowSize = featureBag.rowSize( docs2use[i]);

		// Loop: process successive elements from the BOW structure
		int kd = 0, kv = 0;
		while( kv < rowSize && kv < dim) {

			// Go through the sparse vector

				x_space[j].index = idx[kv];
				x_space[j].value = val[kv]/sum[i];
				//cout << "toLibSvm::" << j << ":" << dim<< ": " << val[kv] << " \n";
				nElements++;
				kd++; 
				kv++;
				j++;
				
		}


		
		// If necessary, increment the value of the maximum index
		if( j>=1 && x_space[j-1].index > max_index) { 
			max_index = x_space[j-1].index;
			//cout << "\n" << max_index << "\nMMMMM\n";
		}

		// Write terminator to end this element of the x_space vector, then 
		// increment index
		x_space[j].index = -1;
		// cout << "L:" << i << "/" << train.l << " - " << j 
		//      << "/" << nElements << endl;
		j++;
	}

	// Don't deallocate x_space here because this storage is referenced by this
	// svm_problem structure!  NOTE 1) Tnis memory allocation is tricky, and it
	// is not immediately apparent how successive calls to this method can work.
	// NOTE 2) x_space must be deallocated in main() to avoid a memory leak.
	// delete x_space;

	// Report success      
	// cout << "done building problem\n";
	//delete[] x_space;
	x_space = new svm_node[1];
}

void uciType::printBag()
{
	for(int i = 0; i < totalInstances; i++)
	{
		cout << usage[i] << "\t";
		featureBag.printRow(i);
		
	}
	
	cout << trainCount << " number for training docs\n";
	cout << featureBag.maxCol() << " number of columns\n";
}

void uciType::printBag2()
{
	for(int i = 0; i < featureBag.nRows(); i++)
	{
		int *tmp = featureBag.getRowIndex(i);
		double *vl = featureBag.getRowValue(i);
		for(int j = 0; j < featureBag.maxCol(); j++)
			cout << tmp[j] << ":" << vl[j] << " ";
		cout << endl;
		
	}
	
	cout << trainCount << " number for training docs\n";
	cout << featureBag.maxCol() << " number of columns\n";
}