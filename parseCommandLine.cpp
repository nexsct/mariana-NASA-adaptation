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
#include "parseCommandLine.h"



void readArgv(struct inParam *p, int argc, char *argv[])
{
	string mUsage = "Usage:\n\t mariana -i inputFile -m modelName -o testOuput\n";
	mUsage.append("   or\n\t mariana -t processedText -c classOfInterest -m modelName -o testOuput\n");
	mUsage.append("\n");
	mUsage.append("Regression Usage:\n\t marianaR -i inputFile -m modelName -o testOuput\n");
	mUsage.append("\n");
	mUsage.append("Other Parameters\n");
	mUsage.append("\t-n\t:max number of instances to use (default 1000)\n");
	mUsage.append("\t-d\t:target number of dimensions to use (default 1000)\n");
	mUsage.append("\t-v\t:verbose (default 1)\n");
	mUsage.append("\t-s\t:percent split of train validation test (default 0.5 0.25 0.25)\n");
	mUsage.append("\t-e\t:minimum optimization error (default 5E-3)\n");
	mUsage.append("\t-p\t:minimum in-class percentage (default 10%)\n");
	
	// set up the default parameters
	p->inputF =  "";
	p->modelF = "";
	p->testF = "";
	p->maxInst = 1000;
	p->split = new double[3];
	p->split[0] = 0.5; p->split[1] = 0.25; p->split[2] = 0.25;
	p->epsil = 5E-3;
	p->minPer = 0.1;
	p->sSteps = 50;
	p->tSteps = 13;
	p->maxC = 1000;
	p->targetTerms = 500;
	p->verbose = 1;
	
	bool declare = 0;


	int check = 0;
	for(int i = 1; i < argc; i++)
		{
			if(argv[i][0] == '-' && argv[i][1] == 'i'){
				p->inputF = argv[i+1];
				check++;
				p->readMode = uciFormat;
				if(p->modelF.length() == 0){
					p->modelF = p->inputF;
					p->modelF.append(".model");
				}
				if(p->testF.length() == 0){
					p->testF = p->inputF;
					p->testF.append(".test");
				}
			}
			else if(argv[i][0] == '-' && argv[i][1] == 'm'){
				p->modelF = argv[i+1];
				
			}
			else if(argv[i][0] == '-' && argv[i][1] == 'o'){
				p->testF = argv[i+1];
				
			}
			if(argv[i][0] == '-' && argv[i][1] == 't'){
				p->inputF = argv[i+1];
				check++;
				p->readMode = textFormat;
				if(p->modelF.length() == 0){
					p->modelF = p->inputF;
					p->modelF.append(".model");
				}
				if(p->testF.length() == 0){
					p->testF = p->inputF;
					p->testF.append(".test");
				}
			}
			if(argv[i][0] == '-' && argv[i][1] == 'n'){
				p->maxInst = atoi(argv[i+1]);
			}
			if(argv[i][0] == '-' && argv[i][1] == 's'){
				double *a = new double[3];
				double total = 0;
				a[0] = atof(argv[i+1]);  //total+=a[0];
				a[1] = atof(argv[i+2]);  //total+=a[1];
				a[2] = atof(argv[i+3]);  //total+=a[2];
				for(int i = 0; i < 3; i++){
					//cout << a[i] << " ";
					total = total + a[i];
				}
				if(total < 1.0 || total > 1.0) cout << total << "\n";
				if( a[0] == 0.0 || a[1] == 0.0 )
				{
					cerr << "the splits must equal one and"
						<<	" the first two cannot be zero\n";
					exit(-1);
				}
				else {
					p->split = a;
				}
				
			}
			if(argv[i][0] == '-' && argv[i][1] == 'v'){
				if(*argv[i+1] != '0')
					p->verbose = 1;
				else p->verbose = 0;
			}
			if(argv[i][0] == '-' && argv[i][1] == 'e'){
				p->epsil = atof(argv[i+1]);
			}
			if(argv[i][0] == '-' && argv[i][1] == 'p'){
				p->minPer = atof(argv[i+1]);
			}
			if(argv[i][0] == '-' && argv[i][1] == 'c'){
				p->cat = atoi(argv[i+1]);
				declare = 1;
			}
			if(argv[i][0] == '-' && argv[i][1] == 'd'){
				p->targetTerms = atoi(argv[i+1]);
				//cout << p->targetTerms << " terms to use\n";
				declare = 1;
			}

		}
		if(check != 1 || p->readMode == nill){
//			cout << "Usage:\n\t Mariana -i inputFile -o modelName -t testOuput\n";
//			cout << "or\n\t Mariana -b processedText -o modelName -t testOuput\n\n";
			//cout << mUsage;
			exit(-1);
		}
		if(p->readMode == textFormat && declare == 0){
			cerr << "A category of interest must be stated\n";
			cerr << "  -c xx\n";
			exit(-1);
		}
}