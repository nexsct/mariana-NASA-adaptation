/*****************************************************************************
* Notices: Mariana - Copyright � 2007 United States Government as
* represented by the Administrator of the National Aeronautics and Space
* Administration.� All Rights Reserved.
* 
* Libsvm - support vector routine library - Copyright (c) 2000-2007
* Chih-Chung Chang and Chih-Jen Lin All rights reserved.
* 
* Disclaimers
* 
* Mariana No Warranty: THE SUBJECT SOFTWARE IS PROVIDED�"AS IS" WITHOUT
* ANY WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY,
* INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE
* WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM
* INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR FREE,
* OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
* SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
* ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
* RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
* RESULTING FROM USE OF THE SUBJECT SOFTWARE.� FURTHER, GOVERNMENT AGENCY
* DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
* IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT�"AS IS."
* 
* Waiver and Indemnity:� RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS
* AGAINST THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND
* SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.� IF RECIPIENT'S USE OF
* THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES,
* EXPENSES OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM
* PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S USE OF THE SUBJECT
* SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED STATES
* GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR
* RECIPIENT, TO THE EXTENT PERMITTED BY LAW.� RECIPIENT'S SOLE REMEDY FOR
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
* � THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. �IN NO EVENT SHALL THE REGENTS OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/
#ifndef UCITYPE_H_
#define UCITYPE_H_

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "Sparse.h"
#include "svm.h"
#include "helperRoutines.h"
#include <cmath>



class uciType
{
	
private:
	Sparse featureBag;
	int *Cat; // make this more than +-1 (multicat)
	int totalInstances;
	int trainInsIn;
	int valInsIn;
	int testInsIn;
	int trainCount;
	int valCount;
	int testCount;
	int *trainIns_;
	int *valIns_;
	int *testIns_;
	double *useSplit;
	int *usage;
	int features;
	int catOfInterest; // may not be used if not multicat
	struct svm_node *x_space;
	bool normalize;
	
public:
	uciType();
	virtual ~uciType();
	
	void read(string fileName);
	void setUsage(double *theSplit);
	void reduceInstance(int number, double rPer);
	void toLibSvmFormat( svm_problem & train,
		bool norm,
		int wht);
	void printBag();
	void printBag2();
};

#endif /*UCITYPE_H_*/
