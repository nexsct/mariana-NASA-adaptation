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


#include "documentCollection.h"
#include "uciType.h"
#include "svm.h"
#include "parseCommandLine.h"

using namespace std;

//set the verbose flag
bool verbose = true;


int main(int argc, char *argv[])
{
	struct inParam pp;
	readArgv(&pp, argc, argv);
	//******things read from the command line******//
	bool normalize = 1;
	string documents = "";
	verbose = pp.verbose;
	int maxCount = pp.maxInst;
	double percentIn = pp.minPer;
	int termMin = pp.targetTerms;
	int cat = 10;
	double *useSplt = pp.split;
	fileFormat readMode = pp.readMode;
	//*********************************************//
//	// parse the command line
//	char input_file_name[ 128];
//	char output_file_name[ 128];
//	char test_output_name[ 128];
	char * input_file_name;
	char * output_file_name;
	char * test_file_name;
		int check = 0;
//	exit(-1);

//	for(int i = 1; i < argc; i++)
//	{
//		if(argv[i][0] == '-' && argv[i][1] == 'i'){
//			input_file_name = argv[i+1];
//			check++;
//			readMode = uciFormat;
//		}
//		else if(argv[i][0] == '-' && argv[i][1] == 'm'){
//			output_file_name = argv[i+1];
//			check++;
//		}
//		else if(argv[i][0] == '-' && argv[i][1] == 't'){
//			test_file_name = argv[i+1];
//			check++;
//		}
//		if(argv[i][0] == '-' && argv[i][1] == 'b'){
//			input_file_name = argv[i+1];
//			check++;
//			readMode = textFormat;
//		}
//	}
//	if(check != 3 || readMode == nill){
//		cout << "Usage:\n\t Mariana -i inputFile -o modelName -t testOuput\n";
//		cout << "or\n\t Mariana -b processedText -o modelName -t testOuput\n\n";
//		exit(-1);
//	}
	documents = pp.inputF;
	cat = pp.cat;
	
// set up the libsvm stuff
	struct svm_problem prob;
	struct svm_problem valid;
	struct svm_problem test;
	struct svm_parameter param;
	struct svm_model *model;
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 10;
	param.gamma = 0.05;  // 1/k
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 5*1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 2;
	param.weight_label = new int[ param.nr_weight];
	param.weight = new double[ param.nr_weight];

	param.weight_label[ 0] = 1;   // the 1 class
	param.weight_label[ 1] = -1;   // the -1 class
	param.weight[ 0] = 1;   // Change this, arrays are not necessary?
	param.weight[ 1] = 1;   //

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// = new documentCollection();
	string docInfo;
	if (readMode == textFormat)
	{
		documentCollection a;
		a.load(documents.c_str());
		a.findTrainDocs(cat, percentIn, maxCount);
		a.reduceTerms(cat, termMin);

		docInfo = a.getModelS().c_str();

		a.toLibSvmFormat(prob, cat, normalize);
		a.valLibSvmFormat(valid, cat, normalize);
		a.testLibSvmFormat(test, cat, normalize);
	}
	else
	{
		uciType aa;
		aa.read(documents.c_str());
		aa.setUsage(useSplt);
		aa.reduceInstance(maxCount, percentIn);
		aa.toLibSvmFormat(prob, 1, 1);
		aa.toLibSvmFormat(valid, 1, 2);
		aa.toLibSvmFormat(test, 1, 3);
	}
	
//	exit(1);
	model = svm_train( &prob, &param);
	
	// I have put the evaluation into a function.
	double A = evalModel(&test, model, &param);
	double defaultA = A;

	//**************************************************************************
	// ******set up the annealing problem******//
	int tSteps = 15; // number of temperature steps
	double* temp = new double[ tSteps];
	getKT( temp, 1, tSteps);

	// Report some results  
	// for( int hhh=0; hhh<tSteps; hhh++) cout << temp[hhh] << endl;

	//******set up the substeps******//
	int sSteps = 50; // the number of sub-steps before decreaseing temp
	int correct, rotate = 1;
	double Aold, oldW0, oldW1; // remember the old values
	double Abest = A, bestW0 = param.weight[0], bestW1 = param.weight[1];
	struct svm_parameter  bestParam;  bestParam = param;
	double bc = 0, bg = 0, bw = 0; // steps for the different params
	double energy, jumpProb, deltaA; // the annealing variables
	double MaxG = 5, MaxW0 = 15, MaxW1 = 15;


	for( int t=0; t<tSteps; t++)
	{
		correct = 0;
		for( int s=0; s<sSteps; s++)
		{
			Aold = A;
			// Record current state
			struct svm_parameter oldParam;
			oldParam = param;
			oldW0 = param.weight[ 0];
			oldW1 = param.weight[ 1];

			double Tn = (double)1/((t+1)*(t+1));

			// Displace parameters randomly according to this rotation mode
			if( rotate==0) {
				bc=2*getRand();
				bg=Tn*getRand();
				bw=Tn*getRand();
				rotate=1;
			}
			else if( rotate==1) {
				bc=Tn*getRand();
				bg=2*getRand();
				bw=Tn*getRand();
				rotate=2;
			}
			else if( rotate==2) {
				bc=Tn*getRand();
				bg=Tn*getRand();
				bw=2*getRand();
				rotate=3;
			}
			else if( rotate==3) {
				bc=Tn*getRand();
				bg=Tn*getRand();
				bw=Tn*getRand();
				rotate=0;
			}


			// Calculate and update parameters
			param.weight[0] = fabs( oldW0 + bc*MaxW0*(2*getRand()-1));
			param.gamma = fabs( oldParam.gamma + bg*MaxG*(2*getRand()-1));
			param.weight[1] = fabs( oldW1 + bw*MaxW1*(2*getRand()-1));

			// Now that the parameters have been updated, train a new model
			model = svm_train( &prob, &param);

			double A = evalModel(&valid, model, &param);
			jumpProb = getRand();
			deltaA = fabs( A-Aold);
			energy = exp( -deltaA*temp[t]);
			;
			if( verbose) {
				double outA = A;//(1-2*(isClassificationMode==0))*A;
				cout << "AUROC:" << setiosflags( ios::fixed)
				<< " " << setw( 8) << setprecision( 5) << outA
				<< " " << setw( 8) << setprecision( 5) << param.gamma
				<< " " << setw( 8) << setprecision( 5) << param.weight[ 0]
				<< " " << setw( 8) << setprecision( 5) << param.weight[ 1]
				<< resetiosflags( ios::scientific)
				<< " " << setw( 8) << setprecision( 3) << deltaA
				<< setiosflags( ios::fixed)
				<< " " << setw( 8) << setprecision( 4) << energy
				<< " " << setw( 8) << setprecision( 4) << jumpProb
				<< resetiosflags( ios::fixed);
			}
			//cout <<  endl;
			// Perform test to keep this sample or discard it and restore 
			// parameters for the old sample.
			if( A>Aold || jumpProb<energy) {
				if( verbose) cout << " KEPT" << endl;
			}
			else {
				if( verbose) cout << " DISC" << endl;
				A=Aold;
				// param.C = oldC;
				// param.gamma = oldG;
				// param.weight[0] = oldW;
				param = oldParam;
				param.weight[0] = oldW0;
				param.weight[1] = oldW1;
			}

			// If these are the best parameters so far, record them.
			if( A>Abest) {
				bestParam = param;
				Abest = A;
				bestW0 = param.weight[0];
				bestW1 = param.weight[1];
			}
			if(A==Abest && (param.weight[0]+param.weight[1] < bestW0 + bestW1)){
				bestParam = param;
				Abest = A;
				bestW0 = param.weight[0];
				bestW1 = param.weight[1];
			}
			if( A>0.9999999) break;
		}

		// Restore the current best parameters for use in a restart
		param = bestParam;
		param.weight[0] = bestW0;
		param.weight[1] = bestW1;
		A = Abest;
		// Test values of area to see if we should restart, then write 
		// information to the console.
		if( param.svm_type==C_SVC) {
//			cout << "finishing early\n";
			if( A>0.9999999) break;
		}
		if( param.svm_type==EPSILON_SVR) {
			if( A>-0.00000001) break;
		}
		if( verbose) {
			// double outAbest = (1-2*(isClassificationMode==0))*Abest;
			cout << "BestAUROC "
			<< " " << setw( 12) << setprecision( 6) << Abest
			<< " " << setw( 8) << setprecision( 6) << param.gamma
			<< " " << setw( 8) << setprecision( 6) << param.weight[0]
			<< " " << setw( 8) << setprecision( 6) << param.weight[ 1]
			                                                        << endl;
		}

		// Use best values to update the maximum values
		MaxW0 = bestW0*0.5;
		MaxG = bestParam.gamma*0.5;
		MaxW1 = bestW1*0.5;
	}

	delete[] valid.x;
	delete[] valid.y;
	
	model = svm_train( &prob, &param);

	
	delete[] prob.x;
	delete[] prob.y;

	if(readMode == textFormat){	
		
		// I have put the evaluation into a function.
		double finalA = runTest(&test, model, &param, pp.testF.c_str());
		
		cout << "area under ROC curve w/ default parameters: "
		<< defaultA << endl;
		cout << "area under ROC curve w/ optimized parameters: "
			<< finalA << endl;
		svm_save_model_wTerms(pp.modelF.c_str(), model, docInfo.c_str());
//		svm_destroy_model(model);
//		svm_destroy_param(&param);

	}
	else if (readMode == uciFormat)
	{
		
	
		double finalA = runTest(&test, model, &param, pp.testF.c_str());
	
		cout << "area under ROC curve w/ default parameters: "
		<< defaultA << endl;
		cout << "area under ROC curve w/ optimized parameters: "
		<< finalA << endl;
		svm_save_model(pp.modelF.c_str(), model);
		//		
//		svm_destroy_model(model);
//		svm_destroy_param(&param);

	}
	svm_destroy_model(model);
	svm_destroy_param(&param);

}