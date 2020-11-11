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
#include "Sparse.h"

using namespace std;
/******************************************************************************
 * 
 * a first attempt at a sparse matrix rep
 * Pat Castle
 *****************************************************************************/


Sparse::Sparse()
{
    nRows_=0;
    nCols_=0;
    elements_= 0;
    maxCol_ = 0;
    //col_ = new int[1];
    //value_ = new double[1];
}

Sparse::~Sparse()
{
	delete col_;
	delete value_;
	delete row_;
	delete rSize;
//	delete maxCol_;
//	delete nRows_;
//	delete nCols_;
//	delete elements_;
	//cout << "deleting data\n";
}

void Sparse::insertRow(int row, int size, int ind[], double val[])
{
    if(elements_ == 0)
    {
        
        nRows_ = 1;
        rSize = new int[1]; rSize[0] = size;
        row_ = new int[1]; row_[0] = 0;
        //col_ = new int[size];
        col_ = ind;
        //value_ = new double[size]; 
        value_ = val; 
        elements_ = size;
        maxCol_ = ind[size-1];
    }
    else
    {
        
        int * rowTempA = new int[nRows_ + 1];
        int * rowTempB = new int[nRows_ + 1];
        //cout << "Adding row " << row
        //<< " to the existing " << nRows_ << " Rows\n";
        for(int i=0; i<nRows_; i++)
        {
        	rowTempA[i] = row_[i];
        	rowTempB[i] = rSize[i];
        }
        
        rowTempA[nRows_] = row;
        rowTempB[nRows_] = size;
        delete [] row_;  delete [] rSize;
        row_ = rowTempA;
        rSize = rowTempB;
        //rowTempA = NULL ; rowTempB = NULL;
        
        
        int * rowTemp1 = new int[elements_ + size];
        double * rowTemp2 = new double[elements_ + size];
        //cout << "Expanding from " << elements_ << " to " 
        //<< elements_ + size << endl;
        for(int i=0; i<elements_; i++)
        {
            rowTemp1[i] = col_[i];
            rowTemp2[i] = value_[i];
        }
        int j=0;
        for(int i=elements_; i<elements_+size; i++, j++)
        {
            rowTemp1[i] = ind[j];
            rowTemp2[i] = val[j];
            
        }
        delete [] col_; delete [] value_;
        col_ = rowTemp1;
        value_ = rowTemp2;
        //rowTemp1 = NULL;
        //rowTemp2 = NULL;
        //delete [] rowTemp1; delete [] rowTemp2;//  *** dont do this use NULL 
        elements_ += size;
        
        if(maxCol_ < ind[size - 1]) maxCol_ = ind[size-1];
        nRows_++;
    }
    
}
double * Sparse::getRowValue(int row)
{
	int i, startPoint = 0;
	for(i = 0; i<nRows_; i++)
	{
		if(row == row_[i]){
			//cout << i << ":" << row_[i] << " endOfRows\n";
			break;
		}
		startPoint += rSize[i];
		//cout << i << "::" << row_[i] << "::"
		//<< rSize[i] << "::" << startPoint << "\t";
	}
	//cout << "sp" << startPoint << "\t";
	double * ans = new double[rSize[i]];
	int j = startPoint, k = 0;
	while(j < startPoint + rSize[i])
	{
		ans[k] = value_[j];
		j++; k++;
	}
	return ans;
}

int * Sparse::getRowIndex(int row)
{
	
	int i, startPoint = 0;
	for(i = 0; i<nRows_; i++)
	{
		if(row == row_[i]) break;
		startPoint += rSize[i];
	//	cout << i << ":" << row_[i] << ":"
	//	<< rSize[i] << ":" << startPoint << "\t";
	}
	//cout << "spI" << startPoint << "\t";
	int * ans = new int[rSize[i]];
	int j = startPoint, k = 0;
	while(j < startPoint + rSize[i])
	{
		ans[k] = col_[j];
		j++; k++;
	}
	return ans;
}

double Sparse::get(int row, int col)
{
	
	double ans = 0.0;
	if(row >= nRows_)
	{
		return ans;
	}
	int i, startPoint = 0;
	for(i = 0; i<nRows_; i++)
	{
		if(row == row_[i]) break;
		startPoint += rSize[i];
	//	cout << i << ":" << row_[i] << ":"
	//	<< rSize[i] << ":" << startPoint << "\t";
	}
	//cout << startPoint << "\t";
	int j = startPoint;
	while(j < startPoint + rSize[i])
	{
		if(col_[j] == col)
		{
			ans = value_[j];
			break;
		}
		j++;
	}
	return ans;
}

void Sparse::info()
{
	cout << "row/col/elements : " << nRows_ << " "
		<< nCols_ << " "
		<< elements_ << endl;
}

int Sparse::rowSize(int row)
{
	// find the row startpoint
	for(int i = 0; i<nRows_; i++)
	{
		if(row == row_[i]){
			return rSize[i];
			break;
		}
	}
	return 0;
}
		
void Sparse::rowInfo(int row)
{
	cout << "row/col/size/elements : " << nRows_ << " "
		<< nCols_ << " " << rSize[row] << " "
		<< elements_ << endl;
}

void Sparse::printRow(int row)
{
	cout << row << "\t";
	if(row >= nRows_)
	{
		cout << "0:0";
		return;
	}
	int i, startPoint = 0;
	for(i = 0; i<nRows_; i++)
	{
		if(row == row_[i]) break;
		startPoint += rSize[i];
//		cout << i << ":" << row_[i] << ":"
//		<< rSize[i] << ":" << startPoint << "\t";
	}
	//cout << startPoint << "\t";
	int j = startPoint;
	while(j < startPoint + rSize[i])
	{
		cout << col_[j] << ":" << value_[j];
		if(++j == (startPoint + rSize[i])) cout << endl;
		else cout << " ";
	}
	
}
