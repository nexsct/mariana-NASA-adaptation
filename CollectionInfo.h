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
#ifndef COLLECTIONINFO_H_
#define COLLECTIONINFO_H_

#include <string>
#include <iostream>

using namespace std;

class CollectionInfo
{
private:
	string ID_;
	string DocumentFile_;
	string CategoryFile_;
	string ThesaurusFile_;
	int NoOfDocuments_;
	int termCounts_;
public:
	CollectionInfo();
	virtual ~CollectionInfo();
	void ID(string name);
	string ID();
	void DocumentFile(string name);
	string DocumentFile();
	void CategoryFile(string name);
	string CategoryFile();
	void ThesaurusFile(string name);
	string ThesaurusFile();
	void NoOfDocuments(int num);
	int NoOfDocuments();
	void termCounts(int num);
	int termCounts();
	void printInfo();
};

#endif /*COLLECTIONINFO_H_*/
