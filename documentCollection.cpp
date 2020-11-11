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
#define MAX_LINE_LENGTH 1000000 //Read in a max of 1M char

#include "documentCollection.h"

using namespace std;


documentCollection::documentCollection()
{
	//this.verbose = verbose;
}

documentCollection::~documentCollection()
{
//	delete catMatrix_;
//	//delete &bow;
//	delete[] IG_;
//	delete[] catCount_;
//	delete[] trainDocs_;
//	delete[] valDocs_;
//	delete[] x_space;
//	
//	cout << "deleting documentCollection\n";
	
	
}

void documentCollection::load(const char *fileName)
{
	// Open the input file and make sure it exists
	ifstream inFile;
	inFile.open( fileName, ios::in);

		if( inFile.bad()) {
			cerr << "load ERROR, "
			<< "can't open input file (" << fileName << ")" << endl;
			exit( 1);
		}
		else {
//			cout << "load "
//			<< "loading input file <" << fileName << ">" << endl;
		}
	
	//  Start reading in lines.  They are ordered and the first lines are the
	//  Paramters that help the loading process.  This could be replaced w/
	//  an XML reading scheme.
	std::map<string,int> parameterValue;
	parameterValue["ID"] = 1;
	parameterValue["DocumentFile"] = 2;
	parameterValue["CategoryFile"] = 3;
	parameterValue["ThesaurusFile"] = 4;
	parameterValue["NoOfDocuments"] = 5;
	parameterValue["termCounts"] = 6;
	parameterValue["NoOfCats"] = 7;
	parameterValue["catCounts"] = 8;
	parameterValue["trainDocs"] = 9;
	parameterValue["valDocs"] = 10;
	parameterValue["testDocs"] = 11;
	parameterValue["thesaurus"] = 12;
	parameterValue["terms"] = 13;
	parameterValue["IG"] = 14;
	parameterValue["catMatrix"] = 15;
	parameterValue["bow"] = 16;
	
	//initialize the modelstring withthe date
	{
		time_t rawtime;
		struct tm * timeinfo;

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		modelString = asctime (timeinfo);
	}
	
	while (!inFile.eof())
	{
		// read in a line
		char textline[ MAX_LINE_LENGTH];
		inFile.getline( textline, MAX_LINE_LENGTH);
		string sTextLine = textline;

		// parse up the string
		vector<string> lineSplit;
		Tokenize(sTextLine, lineSplit, ":");


		//wait(1);
		
		switch(parameterValue[lineSplit[0]])
		{
		case 1:
			ID_ = lineSplit[1];
			modelString += "ID:";
			modelString += ID_;
			modelString += "\n";
			cout << "ID - " << ID_ << endl;
			break;
		case 2:
			
			DocumentFile_ = lineSplit[1];
			modelString += DocumentFile_;
			modelString += "\n";
			cout << "DocumentFile - " << DocumentFile_ << endl;
			break;
		case 3:
			CategoryFile_ = lineSplit[1];
			modelString += CategoryFile_;
			modelString += "\n";
			cout << "CategoryFile - " << CategoryFile_ << endl;
			break;
		case 4:
			ThesaurusFile_ = lineSplit[1];
			modelString += ThesaurusFile_;
			modelString += "\n";
			cout << "ThesaurusFile - " << ThesaurusFile_ << endl;
			break;
		case 5:
		{
			istringstream strin(lineSplit[1]);
			strin >> NoOfDocuments_;
			cout << "NoOfDocuments - " << NoOfDocuments_ << endl;
			break;
		}
		case 6:
		{
			istringstream strin(lineSplit[1]);
			strin >> termCounts_;
			cout << "termCounts - " << termCounts_ << endl;
			break;
		}
		case 7:
		{
			istringstream strin(lineSplit[1]);
			strin >> NoOfCats_;
			cout << "NoOfCats - " << NoOfCats_ << endl;
			break;
		}
		case 8:
		{
			catCount_ = new int*[3];
			for(int i = 0; i < 3; i++){
				catCount_[i] = new int[NoOfCats_];
				inFile.getline( textline, MAX_LINE_LENGTH);
				istringstream strin(textline);
				int j = 0;
				while(!strin.eof()){
					strin >> catCount_[i][j++];
				}
			}

			cout << "catCount - " << catCount_[0][1] << endl;
			break;
		}
		case 9:  // Train Docs
		{
			vector<int> tmp;
			inFile.getline(textline, MAX_LINE_LENGTH);
			istringstream strin(textline);
			while(!strin.eof()) {
				int a;
				strin >> a;
				tmp.push_back(a);
			}
			trainDocs_ = new int[tmp.size()];
			for(uint i = 0; i < tmp.size(); i++){
				trainDocs_[i] = tmp[i];
				cout << trainDocs_[i] << " ";
			}
			cout << endl;
			break;
		}
		case 10:  //Val Docs
		{
			vector<int> tmp;
			inFile.getline(textline, MAX_LINE_LENGTH);
			istringstream strin(textline);
			while(!strin.eof()) {
				int a;
				strin >> a;
				tmp.push_back(a);
			}
			valDocs_ = new int[tmp.size()];
			for(uint i = 0; i < tmp.size(); i++){
				valDocs_[i] = tmp[i];
				cout << valDocs_[i] << " ";
			}
			cout << endl;
			break;
		}
		case 11:   // Test Docs
		{
			vector<int> tmp;
			inFile.getline(textline, MAX_LINE_LENGTH);
			istringstream strin(textline);
			while(!strin.eof()) {
				int a;
				strin >> a;
				tmp.push_back(a);
			}
			testDocs_ = new int[tmp.size()];
			for(uint i = 0; i < tmp.size(); i++){
				testDocs_[i] = tmp[i];
				cout << testDocs_[i] << " ";
			}
			cout << endl;
			break;
		}
		case 12:
		{
			thesaurus_ = lineSplit[1];
			//cout << thesaurus_ << endl;
			break;
		}
		case 13:
		{
			terms_ = lineSplit[1];
			//cout << "terms - " << terms_ << endl;
			break;
		}
		case 14:
		{
			IG_ = new double*[NoOfCats_];
			for(int i = 0; i < NoOfCats_; i++){
				IG_[i] = new double[termCounts_];
				inFile.getline( textline, MAX_LINE_LENGTH);
				istringstream strin(textline);
				int j = 0;
				while(!strin.eof()){
					strin >> IG_[i][j++];
				}
			}

			cout << "IG - " << IG_[0][1] << endl;
			break;
		}
		case 15:
		{
			catMatrix_ = new double*[NoOfDocuments_];
			for(int i = 0; i < NoOfDocuments_; i++){
				catMatrix_[i] = new double[NoOfCats_];
				inFile.getline( textline, MAX_LINE_LENGTH);
				istringstream strin(textline);
				int j = 0;
				while(!strin.eof()){
					strin >> catMatrix_[i][j++];
				}
			}

			cout << "catMatrix - " << catMatrix_[0][1] << endl;
			break;
		}
		case 16:
		{
			int getLineCount = 0;
			while(!inFile.eof()){
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
						// check to see if the indecies are the same
						istringstream a;
						a.str(tmp);
						int b;
						a >> b;
						if(++getLineCount != b)
						{
							cerr << "Line " << b
							<< " should be labeled "
							<< a << endl;
							cerr << "error : " << textline << endl;
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
				int dnum;
				ins >> dnum;
				for(int i = 0; i < termCount; i++)
				{
					string tmp;
					ins >> tmp;
					sscanf(tmp.c_str(),"%d:%lf",&ind[i],&val[i]);
					//cout << ind[i] << ":" << val[i] << " ";
				}
				bow.insertRow(getLineCount-1, termCount, ind, val);
				//bow.printRow(getLineCount-1);
				//cout << endl;
				if(getLineCount%500 == 0)cout << "Document Number " << getLineCount << endl;
			}
			break;
		}
		default:
			cout << ".";
		}
	}

	inFile.close();
//	cout << bow.maxCol() << "max col line 308\n";


}
/* this finds the documents that make up the training problem 
 * cat is the input values.  Not the catmatrix column index. 
 * to get the column of cat do cat - 1
 * */
void documentCollection::findTrainDocs(int cat,
		const double percentIn,
		const int targetCount)
{
	// set up an array with the usage designation
	int * usage = new int[NoOfDocuments_];
	int trainI = 0, valI = 0, testI = 0; // The counts for each set
	for(int i = 0; i < NoOfDocuments_; i++)
	{
		if(trainDocs_[trainI] == i){
			trainI++;
			usage[i] = 999; //train
		}
		else if(valDocs_[valI] == i){
			valI++;
			usage[i] = -888;	//val
		}
		else if(testDocs_[testI] == i){
			testI++;
			usage[i] = 7357;	//test
		}
		else {
			cerr << "docCollection::something wrong at line 333\n";
		}
	}
		cout << "train/val/test: " << trainI << ":" << valI << ":" << testI << endl;

	totalVal = valI;
	totalTest = testI;
	// set up arrays that list the documents in and out 
	// of the category for each of the three sets
	int tic = catCount_[0][cat - 1];
	int toc = trainI - catCount_[0][cat - 1];
	int mintoc = toc;
	int * trainDocsIn = new int[tic];
	int * trainDocsOut = new int[toc];

	int vic = catCount_[1][cat - 1];
	int voc = valI - catCount_[1][cat - 1];
	int * valDocsIn = new int[vic];
	int * valDocsOut = new int[voc];

	int teic = catCount_[2][cat - 1];
	int teoc = testI - catCount_[2][cat - 1];
	int * testDocsIn = new int[teic];
	int * testDocsOut = new int[teoc];
	// go through all the documents in reverse order 
	// and assign it where it belongs
	for(int i = NoOfDocuments_ - 1; i >= 0; i--)
	{
		if(usage[i] == 999){
			if(catMatrix_[i][cat - 1] == 1) trainDocsIn[--tic] = i;
			else trainDocsOut[--toc] = i;
		}
		else if(usage[i] == -888){
			if(catMatrix_[i][cat - 1] == 1) valDocsIn[--vic] = i;
			else valDocsOut[--voc] = i;
		}
		else if(usage[i] == 7357){
			if(catMatrix_[i][cat - 1] == 1) testDocsIn[--teic] = i;
			else testDocsOut[--teoc] = i;
		}
		else {
			cerr << "docCollection::something wrong in findTrainDoc\n"
			<< i << ":" << usage[i] << ":" << tic << "/" << toc << ":"
			<< vic << "/" << voc << ":"
			<< teic << "/" << teoc << endl;
		}
	}

	// Calc how many document are needed for the training set
	int totalInClass = catCount_[0][cat - 1]
	                                + catCount_[1][cat - 1]
	                                               + catCount_[2][cat - 1];
	double natPer = (double)totalInClass/(double)NoOfDocuments_;
	double per;
	// add the percentages to the modelstring
	{
		std::ostringstream oss;
		modelString += "percentage in-class (minimum/natural):";
		oss<< percentIn;
		modelString += oss.str();
		modelString += "/";
		oss<< natPer;
		modelString += oss.str();
		modelString += "\n";
	}
		// is the natural percentage more than the requested percentage?
	if(natPer > percentIn)per = natPer;
	else per = percentIn;

	// how many from in the category are needed to hit the targetCount?
	int DocsIn = (int)round(per*(double)targetCount);
	int DocsOut;
	// are there enough trainDocs?
	if(DocsIn > catCount_[0][cat - 1])
	{
		// set the number of docsIn to be all in the category
		DocsIn = catCount_[0][cat - 1];
		// calc how many docs not in the class are needed for training
		// If catCount_ makes up natPer of the total, what is the total?
		DocsOut = (int)round((double)DocsIn*(-1.0 + 1.0/per));
		if(DocsOut > toc) DocsOut = mintoc;
		//		cout << DocsOut << " DocsOut\n";
	}
	else
	{
		DocsOut = targetCount - DocsIn;
	}

	//	
	//	cout << "percent IN " << per << endl;
	//	cout << "trainDocs IN:OUT " << DocsIn << ":" << DocsOut << endl;
	//	for(int i = 0; i < DocsIn; i++) cout << trainDocsIn[i] << ", ";
	//	cout << endl;
	//	for(int i = 0; i < DocsOut; i++) cout << trainDocsOut[i] << ", ";
	//		cout << endl;
	// select the documents
	totalTrain = DocsIn + DocsOut;
	reducedTrainDocs = new int[totalTrain];
	int * tmpInd = randomOrder(totalTrain); //This may not be needed now
	/* since we are now using libSVM the inputs do not need to be randomized
	 * I think the reading process can be optimized so the system only reads
	 * documents that are listed in the read file*/
	//	for(int i = 0; i < totalTrain; i++) cout << tmpInd[i] << ", ";
	//	cout << endl;
	for(int i = totalTrain - 1; i >= 0 ; i--)
	{
		double newP = (double)rand()/(double)RAND_MAX;
		if((newP > per && DocsOut != 0) || (DocsIn == 0 && DocsOut != 0) ){
			reducedTrainDocs[tmpInd[i]] = trainDocsOut[--DocsOut];
			//			cout << i << " out :  " << tmpInd[i] 
			//			     << "  :  " << reducedTrainDocs[tmpInd[i]]
			//			     << "  :  " << DocsOut << endl;
		}
		else if((newP <= per && DocsIn != 0) || (DocsOut == 0 && DocsIn != 0) ){
			reducedTrainDocs[tmpInd[i]] = trainDocsIn[--DocsIn];
			//			cout << i << " in :  " << tmpInd[i] 
			//			     << "  :  " << reducedTrainDocs[tmpInd[i]]
			//			     << "  :  " << DocsIn << endl;
		}
		else {
			cout << "missed \n";
			i--;
		}
	}
//	
//	cout << "training docs\n\t";
//	for(int i = 0; i < totalTrain; i++) cout << reducedTrainDocs[i] << ", ";
//	cout << endl;
}

void documentCollection::reduceTerms(int cat, int n)
{
	double *igS = new double[termCounts_];
	// if no input or if the number requested is larger than the number
	// of terms than use all terms
	cout << termCounts_ << "line 496\n";
	if (n == 0 || n > termCounts_){

		// keep all the terms;
		reducedTerms_ = new int[termCounts_];
		for(int i = 0; i < termCounts_; i++) reducedTerms_[i] = i;
		finalTermCount = termCounts_;

	}
	else
	{
		for(int i = 0; i < termCounts_ ; i++)
			igS[i] = IG_[cat - 1][i];
		// find the IG of the nth term
		sort(igS, igS + termCounts_,greater<double>());
		double thresh = igS[n - 1];
		cout << "line 489: threshhold - " << thresh << endl;
		int totalN = n - 1;
		// find the first nonzero or 
		if(thresh == 0){
			while(igS[--totalN] <= 0);
			thresh = igS[totalN];
		}
		else{
			while(igS[++totalN] == igS[n - 1] && igS[totalN] > 0);
//			cout << "lienst 476\n";
		}
		reducedTerms_ = new int[totalN];
		finalTermCount = totalN;
		for(int i = termCounts_ - 1; i >=0 ; i--)
		{
			//cout << i << " line 517 " << totalN << "\n";
			if (IG_[cat - 1][i] >= thresh) reducedTerms_[--totalN] = i;
			
		}
		cout << "coll:line digStomp\n";
		cout << "Final Term Count : " << finalTermCount << endl;
	}

	{
		
		
		vector<int> keptInd(finalTermCount);
		for(int i = 0; i < finalTermCount; i++)
			keptInd[i] = reducedTerms_[i];

		sort(keptInd.begin(), keptInd.end() ); // It is now sorted inprep
		
		{
			modelString += "NumberOfTerms:";
			std::ostringstream oss;
			oss<< finalTermCount;
			modelString += oss.str();
			modelString += "\n";
		}

		

		// produce a term string for the model
		// read the term string
		int nChars = strlen( terms_.c_str());
//		string punctuation_string_ = "{},";
//		for( int i = 0; i < nChars; i++) 
//		{
//			int j = strcspn( terms_.c_str(), punctuation_string_.c_str());
//			if( j >= nChars) break;
//			//terms_[ j] = '';
//			terms_.replace(j,1,"");
//		}
		char * tmpSt = new char[nChars];
		string::iterator begin = terms_.begin();
		int charI = 0;
		while(begin != terms_.end())
		{
			if(*begin == '{' || *begin == '}' || *begin == ',')
			{
				//do nothing
			}
			else tmpSt[charI++] = *begin;
			begin++;
		}
		

		terms_ = string(tmpSt);
		modelString += "Terms:";
		vector<string> tmpT;

		Tokenize(terms_, tmpT, " ");

		for(uint i = 0; i < tmpT.size(); i++)
		{
			string tmpString = tmpT[i];
			int j = strcspn(tmpString.c_str(), "=");
			tmpString.replace(j,1," ");

			istringstream b;
			b.str(tmpString);

			string word;
			int indx;
			b >> word;
			b >> indx;

			if (find(keptInd.begin(), keptInd.end() , indx) != keptInd.end())
			{
				modelString += word;
				modelString += "=";
				std::ostringstream oss;
				oss<< indx;
				modelString += oss.str();
				modelString += " ";
			}


		}
	}
	modelString += "\n";
	// add the thesaurus to the modelString
	modelString += "Thesaurus:";
	modelString += thesaurus_;
	modelString += "\n";
}

void documentCollection::toLibSvmFormat
( svm_problem & train,
		int cat,
		bool normalize)
{
	
	// set up the svm problem
	
	train.l = totalTrain;//sizeof(reducedTrainDocs)/sizeof(int);
//	cout << "toLibSvmFormat(480): " << train.l << endl;
	train.x = new svm_node*[train.l];
	train.y = new double[train.l];

	double* sum = new double[train.l];
	for(int k=0; k<train.l; k++) sum[k]=1;
	int nElements=0;
	int dim  = finalTermCount;//sizeof(reducedTerms_)/sizeof(reducedTerms_[0]);
//	cout << "line 495:" << dim << endl;

	// If no normalization was requested, simply increment the number of 
	// elements and set succesive elements of the SUM vector to '1'.  (Note 
	// that this was already done during the initlaization process above.)  
	// Otherwise, normalize term frequencies.
	if( normalize) {

		// Loop: Evaluate values for successive lines of the svm_problem structure
		for( int h=0; h<train.l; h++) {

			// Add an element for each line (the lines with "-1")
			nElements++;

			// Add an element for each line (the lines with "-1")
			nElements++;

			// Get the index and value for this row
			int rowSize = bow.rowSize( trainDocs_[h]);
			int* idx = bow.getRowIndex( trainDocs_[h]);
			double* val = bow.getRowValue( trainDocs_[h]);

			// cout << "index:" << h <<":" << idx[0]
			//      << ":" << val[0] <<":" << rowSize << " \n";

			// Loop: Evaluate successive elements in this line
			double tmp = 0;
			int kd = 0, kv = 0;
			while( kv < rowSize && kd <  dim ){
				// cout << "MMM:" << kv <<":" << val[kv] << ":" 
				//      << dim[kd] <<":" << idx[kv] << " \n";

				// Work through elements of the sparse vector
				if( idx[kv] == reducedTerms_[kd]) {
					tmp+=val[kv];
					// cout << "=idx>dim:" << idx[kv] << ":" << dim[kd] << endl;
					nElements++;
					kd++; kv++;
				}
				else if(idx[kv] < reducedTerms_[kd]) {
					// cout << "-idx<dim:" << idx[kv] << ":" << dim[kd] << endl;
					kv++;
				}
				else {
					//cout << "+idx>dim:"<< idx[kv] << ":" << dim[kd] << endl;
					kd++;
				}
			}
			//delete [] idx;
			//delete [] val;
			sum[h] = tmp;
			//cout <<  h << endl;
		}
	}
	else {
		nElements=bow.nElements() + train.l;
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
			//cout << "assign category:" << Cat[i] << endl;

			train.y[i] = catMatrix_[trainDocs_[i]][cat - 1];
			//cout << "toLibSvm(567): " << catMatrix_[i][cat - 1] << ":" << train.y[i] << endl;
			train.x[i] = &x_space[j];
			//cout << "category " << train.y[i]  << endl;

			// Get dimensions and sizes associated with the BOW structure 
			int* idx = bow.getRowIndex(trainDocs_[i]);
			double* val = bow.getRowValue(trainDocs_[i]);
			int rowSize = bow.rowSize(trainDocs_[i]);

			// Loop: process successive elements from the BOW structure
			int didNotWriteAnything = 1;
			int kd = 0, kv = 0;
			while( kv < rowSize && kd < (int) dim) {

				// Go through the sparse vector
			if( idx[kv] == reducedTerms_[kd]) {
				x_space[j].index = idx[kv];
				x_space[j].value = val[kv]/sum[i];
				//cout << "toLibSvm::" << j << ":" << dim<< ": " << val[kv] << " \n";
				nElements++;
				kd++; 
				kv++;
				j++;
				didNotWriteAnything=0;
			}
			else if(idx[kv] < reducedTerms_[kd]) kv++;
			else kd++;
		}


		// If nothing was written, generate a 'blank line'
		if( didNotWriteAnything==1) {
			//train.x[i] = &x_space[j];
			x_space[j].index=reducedTerms_[0];
			x_space[j].value=0.0;
			//cout << "bbbb " << x_space[j].index << ":" << x_space[i].value;
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
	//cout << "done building problem\n";
	//delete[] x_space;
}

void documentCollection::valLibSvmFormat
( svm_problem & train,
		int cat,
		bool normalize)
{
	//struct svm_node *x_space;
	// set up the svm problem
	
	train.l = totalVal;//sizeof(reducedTrainDocs)/sizeof(int);
//	cout << "toLibSvmFormat(480): " << train.l << endl;
	train.x = new svm_node*[train.l];
	train.y = new double[train.l];

	double* sum = new double[train.l];
	for(int k=0; k<train.l; k++) sum[k]=1;
	int nElements=0;
	int dim  = finalTermCount;//sizeof(reducedTerms_)/sizeof(reducedTerms_[0]);
//	cout << "line 495:" << dim << endl;

	// If no normalization was requested, simply increment the number of 
	// elements and set succesive elements of the SUM vector to '1'.  (Note 
	// that this was already done during the initlaization process above.)  
	// Otherwise, normalize term frequencies.
	if( normalize) {
		

		// Loop: Evaluate values for successive lines of the svm_problem structure
		for( int h=0; h<train.l; h++) {

			// Add an element for each line (the lines with "-1")
			nElements++;

			// Add an element for each line (the lines with "-1")
			nElements++;

			// Get the index and value for this row
			int rowSize = bow.rowSize( valDocs_[h]);
			int* idx = bow.getRowIndex( valDocs_[h]);
			double* val = bow.getRowValue( valDocs_[h]);

			// cout << "index:" << h <<":" << idx[0]
			//      << ":" << val[0] <<":" << rowSize << " \n";

			// Loop: Evaluate successive elements in this line
			double tmp = 0;
			int kd = 0, kv = 0;
			while( kv < rowSize && kd <  dim ){
				// cout << "MMM:" << kv <<":" << val[kv] << ":" 
				//      << dim[kd] <<":" << idx[kv] << " \n";

				// Work through elements of the sparse vector
				if( idx[kv] == reducedTerms_[kd]) {
					tmp+=val[kv];
					// cout << "=idx>dim:" << idx[kv] << ":" << dim[kd] << endl;
					nElements++;
					kd++; kv++;
				}
				else if(idx[kv] < reducedTerms_[kd]) {
					// cout << "-idx<dim:" << idx[kv] << ":" << dim[kd] << endl;
					kv++;
				}
				else {
					//cout << "+idx>dim:"<< idx[kv] << ":" << dim[kd] << endl;
					kd++;
				}
			}
			//delete [] idx;
			//delete [] val;
			sum[h] = tmp;
			//cout <<  h << endl;
		}
	}
	else {
		nElements=bow.nElements() + train.l;
		for(int h=0; h<train.l; h++) sum[h] = 1;
	}
	
	// Allocate storage for global buffer, x_space, that will be used to read 
	// all of the elements for this svm_problem structure.  NOTE 2) Does this 
	// have to be global?  NOTE 2) Why doesn't this crash during successive 
	// calls to this method?  Could the new (or malloc) operation be allocating 
	// new storage while leaking the old storage, which is later deallocated 
	// when the relevant svm_problem structure is destroyed?
	x_space = new svm_node[ nElements];
	
	// cout << Cat[0];
	// cout << problem.l << "\tallocated space\n";

	// Loop: Go through and map the BOW into the X_SPACE vector
	int max_index = 0;   // Biggest term index
	int j = 0;
	for( int i=0; i<train.l; i++) {
		//cout << "assign category:" << Cat[i] << endl;

		train.y[i] = catMatrix_[valDocs_[i]][cat - 1];
		//cout << "toLibSvm(567): " << catMatrix_[i][cat - 1] << ":" << train.y[i] << endl;
		train.x[i] = &x_space[j];
		//cout << "category " << train.y[i]  << endl;

		// Get dimensions and sizes associated with the BOW structure 
		int* idx = bow.getRowIndex(valDocs_[i]);
		double* val = bow.getRowValue(valDocs_[i]);
		int rowSize = bow.rowSize(valDocs_[i]);

		// Loop: process successive elements from the BOW structure
		int didNotWriteAnything = 1;
		int kd = 0, kv = 0;
		while( kv < rowSize && kd < (int) dim) {

			// Go through the sparse vector
			if( idx[kv] == reducedTerms_[kd]) {
				x_space[j].index = idx[kv];
				x_space[j].value = val[kv]/sum[i];
				//cout << "toLibSvm::" << j << ":" << dim<< ": " << val[kv] << " \n";
				nElements++;
				kd++; 
				kv++;
				j++;
				didNotWriteAnything=0;
			}
			else if(idx[kv] < reducedTerms_[kd]) kv++;
			else kd++;
		}


		// If nothing was written, generate a 'blank line'
		if( didNotWriteAnything==1) {
			//train.x[i] = &x_space[j];
			x_space[j].index=reducedTerms_[0];
			x_space[j].value=0.0;
			//cout << "bbbb " << x_space[j].index << ":" << x_space[i].value;
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
	//cout << "done building problem\n";
	//delete[] x_space;
}

void documentCollection::testLibSvmFormat
( svm_problem & train,
		int cat,
		bool normalize)
{
	//struct svm_node *x_space;
	// set up the svm problem
	
	train.l = totalTest;//sizeof(reducedTrainDocs)/sizeof(int);
	//cout << "toLibSvmFormat(919): " << train.l << endl;
	train.x = new svm_node*[train.l];
	train.y = new double[train.l];

	double* sum = new double[train.l];
	for(int k=0; k<train.l; k++) sum[k]=1;
	int nElements=0;
	int dim  = finalTermCount;//sizeof(reducedTerms_)/sizeof(reducedTerms_[0]);
//	cout << "line 495:" << dim << endl;

	// If no normalization was requested, simply increment the number of 
	// elements and set succesive elements of the SUM vector to '1'.  (Note 
	// that this was already done during the initlaization process above.)  
	// Otherwise, normalize term frequencies.
	if( normalize) {

		// Loop: Evaluate values for successive lines of the svm_problem structure
		for( int h=0; h<train.l; h++) {

			// Add an element for each line (the lines with "-1")
			nElements++;

			// Add an element for each line (the lines with "-1")
			//nElements++;

			// Get the index and value for this row
			int rowSize = bow.rowSize( testDocs_[h]);
			int* idx = bow.getRowIndex( testDocs_[h]);
			double* val = bow.getRowValue( testDocs_[h]);

			// cout << "index:" << h <<":" << idx[0]
			//      << ":" << val[0] <<":" << rowSize << " \n";

			// Loop: Evaluate successive elements in this line
			double tmp = 0;
			int kd = 0, kv = 0;
			while( kv < rowSize && kd <  dim ){
				// cout << "MMM:" << kv <<":" << val[kv] << ":" 
				//      << dim[kd] <<":" << idx[kv] << " \n";

				// Work through elements of the sparse vector
				if( idx[kv] == reducedTerms_[kd]) {
					tmp+=val[kv];
					// cout << "=idx>dim:" << idx[kv] << ":" << dim[kd] << endl;
					nElements++;
					kd++; kv++;
				}
				else if(idx[kv] < reducedTerms_[kd]) {
					// cout << "-idx<dim:" << idx[kv] << ":" << dim[kd] << endl;
					kv++;
				}
				else {
					//cout << "+idx>dim:"<< idx[kv] << ":" << dim[kd] << endl;
					kd++;
				}
			}
			//delete [] idx;
			//delete [] val;
			sum[h] = tmp;
			//cout <<  h << endl;
		}
	}
	else {
		nElements=bow.nElements() + train.l;
		for(int h=0; h<train.l; h++) sum[h] = 1;
	}
	

	// Allocate storage for global buffer, x_space, that will be used to read 
	// all of the elements for this svm_problem structure.  NOTE 2) Does this 
	// have to be global?  NOTE 2) Why doesn't this crash during successive 
	// calls to this method?  Could the new (or malloc) operation be allocating 
	// new storage while leaking the old storage, which is later deallocated 
	// when the relevant svm_problem structure is destroyed?
	x_space = new svm_node[ nElements];
	
	// cout << Cat[0];
	// cout << problem.l << "\tallocated space\n";

	// Loop: Go through and map the BOW into the X_SPACE vector
	int max_index = 0;   // Biggest term index
	int j = 0;
	for( int i=0; i<train.l; i++) {
		//cout << "assign category:" << Cat[i] << endl;

		train.y[i] = catMatrix_[testDocs_[i]][cat - 1];
		//cout << "toLibSvm(567): " << catMatrix_[i][cat - 1] << ":" << train.y[i] << endl;
		train.x[i] = &x_space[j];
		//cout << "category " << train.y[i]  << endl;

		// Get dimensions and sizes associated with the BOW structure 
		int* idx = bow.getRowIndex(testDocs_[i]);
		double* val = bow.getRowValue(testDocs_[i]);
		int rowSize = bow.rowSize(testDocs_[i]);

		// Loop: process successive elements from the BOW structure
		int didNotWriteAnything = 1;
		int kd = 0, kv = 0;
		while( kv < rowSize && kd < (int) dim) {

			// Go through the sparse vector
			if( idx[kv] == reducedTerms_[kd]) {
				x_space[j].index = idx[kv];
				x_space[j].value = val[kv]/sum[i];
				//cout << "toLibSvm::" << j << ":" << dim<< ": " << val[kv] << " \n";
				nElements++;
				kd++; 
				kv++;
				j++;
				didNotWriteAnything=0;
			}
			else if(idx[kv] < reducedTerms_[kd]) kv++;
			else kd++;
		}


		// If nothing was written, generate a 'blank line'
		if( didNotWriteAnything==1) {
			//train.x[i] = &x_space[j];
			x_space[j].index=reducedTerms_[0];
			x_space[j].value=0.0;
			//cout << "bbbb " << x_space[j].index << ":" << x_space[i].value;
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
	//cout << "done building problem\n";
	//delete[] x_space;
}

string documentCollection::getModelS()
{
	return modelString;
}
