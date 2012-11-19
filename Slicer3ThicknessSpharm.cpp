# include <iostream>
#include <unistd.h>
#include "Slicer3ThicknessSpharmCLP.h"
#include "string"
#include <vector>
#include "errno.h"
#include <fstream>
#include "bmScriptParser.h"
void readFileCSV(std::string line, char group[100], char subjid[100], 	char labelMapInput[200], char measurementOutput[200], char labelfile[100],char CorrespondenceMesh[200],char WarpedCorrespondenceMesh[200],char ThicknessSamplingResult[200])
{
			int pos1=0, pos2=0, pos3=0,pos4=0, pos5=0, pos6=0, pos7=0, pos8=0, l=0;


				pos1 = line.find('	',0);
				l = line.copy(group,pos1,0);			
				group[l] = '\0';

				pos2 = line.find('	',pos1+1);
				l = line.copy(subjid,pos2-pos1-1,pos1+1);		
				subjid[l] = '\0';

				pos3 = line.find('	',pos2+1);
				l = line.copy(labelMapInput,pos3-pos2-1,pos2+1);		
				labelMapInput[l] = '\0';

				pos4 = line.find('	',pos3+1);
				l = line.copy(measurementOutput,pos4-pos3-1,pos3+1);		
				measurementOutput[l] = '\0';
				      
				pos5 = line.find('	',pos4+1);
				l = line.copy(labelfile,pos5-pos4-1,pos4+1);		
				labelfile[l] = '\0';

				pos6 = line.find('	',pos5+1);            
				l = line.copy(CorrespondenceMesh,pos6-pos5-1,pos5+1);	
				CorrespondenceMesh[l] = '\0';

				pos7 = line.find('	',pos6+1);            
				l = line.copy(WarpedCorrespondenceMesh,pos7-pos6-1,pos6+1);	
				WarpedCorrespondenceMesh[l] = '\0';
				
				pos8=line.find('	',pos7+1); 
				l = line.copy(ThicknessSamplingResult,pos8-pos7-1,pos7+1);
				ThicknessSamplingResult[l] = '\0';

}
int GetNumberGroups(std::string file)
{
    std::ifstream groups(file.c_str(), std::ios::in);
    int c = 0;
    
    if (groups)
    {
        std::string s;
	while ( getline(groups,s) ) c++;
	groups.close();
    }
    else std::cerr << "File doesn't exist" << std::endl;
    
    return c;
}

int GetGroups(std::string file, char** groupIds , char** subjgroups, char** Groups)
{
    std::ifstream groups(file.c_str(), std::ios::in);
    int length = 0;
 int length1 = 0;
int length2 = 0;
    int first_line = 0;
    
    if (groups)
    {
        std::string s;
       
        while ( getline(groups,s) )
	{
	  if (first_line != 0)
	  {
	    // Get the values
	    int pos = s.find('	',0);
	    int l;
	    char group_name[20];		l = s.copy(group_name,pos,0);          		 group_name[l] = '\0';
	    
	    int pos1= s.find('	',pos+1);
	    int t;
	char subj_name[100];    			t =s.copy(subj_name,pos1-pos-1,pos+1); 	subj_name[t]='\0';

	    int test = 0;
	   
	    for (int i = 0; i < length; ++i) if ( strcmp(groupIds[i], group_name) == 0 ) test = 1;
	    //New entry if test = 0
	    if (test == 0)
	    {
	      groupIds[length] = new char[pos+1];
	      strcpy(groupIds[length],group_name);
	      length++;
	    }
	  
	      subjgroups[length1]= new char[pos1-pos+1];
	      strcpy(subjgroups[length1],subj_name);
	      length1++;
	   
		Groups[length2] = new char[pos+1];
	      strcpy(Groups[length2],group_name);
	      length2++;
	
	  }
	  
	  first_line = 1;
	  s.clear();
        }
        groups.close();
    }
    else std::cerr << "File doesn't exist" << std::endl;
    
    return length;
}

int main(int argc, char * argv[]) 
{
	PARSE_ARGS;
////////////////////////////////////////////////////////////////////////////////////////////////////////Pipipeline 1//////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	std::string nameFile;
	nameFile="/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/dataset.csv";	
	std::ifstream Sub(nameFile.c_str(),std::ios::in);
	int i=0;
	if(Sub)
	{
		std :: string line;
		
		while(getline( Sub, line ))
		{  std :: cout <<"1"<<endl;

			if (i>0)
			{
				char labelfile[200];  
				char subjid[100];
				char WarpedCorrespondenceMesh[200];
				char CorrespondenceMesh[200];
						
				char labelMapInput[200];
				char measurementOutput[200];
				char group[100];
				char ThicknessSamplingResult[200];
		
				/*std :: string labelfile;  
				std :: string subjid;
				std :: string WarpedCorrespondenceMesh;
				std :: string CorrespondenceMesh;
						
				std :: string labelMapInput;
				std :: string measurementOutput;
				std :: string group;
				std :: string ThicknessSamplingResult;*/
				
				std::string BatchMakeScriptFile = "slicer3ThicknessSPHARM.bms";
				std::ofstream file( BatchMakeScriptFile.c_str());

				readFileCSV(line, group,subjid,labelMapInput, measurementOutput,labelfile, CorrespondenceMesh, WarpedCorrespondenceMesh, ThicknessSamplingResult);
			

				//std :: cout << "s is : " << group << std :: endl;
				file <<"set (labelMapInput "<< labelMapInput<<")"<<std::endl;
				file <<"set (measurementOutput "<< measurementOutput<<")"<<std::endl;
				file <<"set (spharmBinaryInput "<<labelfile<<")"<<std::endl;
				file <<"set (case1 "<< subjid <<")"<<std::endl;
				file <<"set (group "<< group <<")"<<std::endl;
				file <<"set (subjWorkDir "<< subjWorkDir<<")"<<std::endl;
				file <<"set (exprId "<< exprId<<")"<<std::endl;

				file <<"include (/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/ThicknessSpharm.bms)"<<std::endl;

				file.close();
				/*
				bm::ScriptParser m_Parser;
				
				m_Parser.LoadWrappedApplication( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" );
				m_Parser.SetBatchMakeBinaryPath( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" ); 
				m_Parser.Execute(BatchMakeScriptFile); 
				*/
			}
			i++;
		}
		Sub.close();
	} // fin du if (sub)
	else
	{
	      std::cout << "ERROR: Unable  to open file for reading." << std::endl;

	}
///////////////////////////////////////////////////////////////////////////////////////////////////////Pipeline2///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string nameFile2;
	nameFile2=dataset;	
	int number_of_groups = GetNumberGroups(nameFile2);
	number_of_groups = number_of_groups - 1; // First line isn't a group name
 	



	//Initialisation
	char** groupIds = new char*[number_of_groups];
  	for(int i = 0; i < number_of_groups; ++i) groupIds[i] = NULL;
  
	char** Groups = new char*[number_of_groups];
  	for(int i = 0; i < number_of_groups; ++i) Groups[i] = NULL;

	char** subjgroups = new char*[number_of_groups];
  	for(int i = 0; i < number_of_groups; ++i) subjgroups[i] = NULL;
  
  	int L = GetGroups(nameFile2, groupIds, subjgroups, Groups);  
  	for(int i = 0; i < L; ++i) std::cout << groupIds[i] << std::endl;  
  	for(int i = 0; i < number_of_groups; ++i) std::cout << subjgroups[i] << std::endl;
	for(int i = 0; i < number_of_groups; ++i) std::cout << Groups[i] << std::endl;
  	
std::string final_groups;
	std::string subjGroup;
  	for(int i = 0; i < L; ++i)
  	{
   		 if (i != L-1) final_groups = final_groups + groupIds[i] + ' ';
    		else final_groups = final_groups + groupIds[i];
 	 }
  for(int i = 0; i < number_of_groups; ++i)
  	{
   		 if (i != number_of_groups-1) subjGroup = subjGroup + subjgroups[i] +'/'+ Groups[i] + ' ';
    		else subjGroup = subjGroup + subjgroups[i] +'/'+ Groups[i];
 	 }
  	//std::cout << final_groups << std::endl;
std::cout << subjGroup << std::endl;


//file bms
	std::string BatchMakeScriptFile2 = "slicer3ThicknessSPHARM1.bms";
	std::ofstream file2( BatchMakeScriptFile2.c_str());
 
	file2 <<"set (groupIds "<< final_groups<<")"<<std::endl;
	file2 <<"set (subjectsWithGroups "<< subjGroup<<")"<<std::endl;
	file2 <<"set (subjWorkDir "<< subjWorkDir<<")"<<std::endl;
	file2 <<"set (exprId "<< exprId<<")"<<std::endl;

	file2 <<"include (/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/particlecorrespondance.bms)"<<std::endl;

	file2.close();  
/*
	bm::ScriptParser m_Parser1;
	m_Parser1.LoadWrappedApplication( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" );
	m_Parser1.SetBatchMakeBinaryPath( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" ); 
	m_Parser1.Execute(BatchMakeScriptFile2);
*/


////////////////////////////////////////////////////////////////////////////////////////////////////////Pipeline 3/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string nameFile3;
	nameFile3=dataset;	
	std::ifstream Sub1(nameFile.c_str(),std::ios::in);
	int i1=0;
	if(Sub1)
	{
		std :: string line;
		while(getline( Sub1, line ))
		{  
			if (i1>0)
			{
				char labelfile[200];  
				char subjid[100];
				char WarpedCorrespondenceMesh[200];
				char CorrespondenceMesh[200];
				char labelMapInput[200];
				char measurementOutput[200];
				char group[100];
				char ThicknessSamplingResult[200];

				std::string BatchMakeScriptFile = "slicer3writefile.bms";
				std::ofstream file( BatchMakeScriptFile.c_str());

				readFileCSV(line, group, subjid, labelMapInput, measurementOutput, labelfile, CorrespondenceMesh, WarpedCorrespondenceMesh, ThicknessSamplingResult);


				file <<"set (labelMapInput "<< labelMapInput<<")"<<std::endl;
				file <<"set (measurementOutput "<< measurementOutput<<")"<<std::endl;
				file <<"set (spharmBinaryInput "<<labelfile<<")"<<std::endl;
				file <<"set (case1 "<< subjid <<")"<<std::endl;
				file <<"set (CorrespondenceMesh "<<CorrespondenceMesh<<")"<<std::endl;
				file <<"set (WarpedCorrespondenceMesh  "<<WarpedCorrespondenceMesh<<")"<<std::endl;
				file <<"set (group "<< group <<")"<<std::endl;
				file <<"set (subjWorkDir "<< subjWorkDir<<")"<<std::endl;
				file <<"set (exprId "<< exprId<<")"<<std::endl;

				file <<"include (/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/writefile.bms)"<<std::endl;

				file.close();
				/*
				bm::ScriptParser m_Parser;

				m_Parser.LoadWrappedApplication( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" );
				m_Parser.SetBatchMakeBinaryPath( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" ); 
				m_Parser.Execute(BatchMakeScriptFile); 
				*/
			}
			i1++;
		}
		Sub1.close();
	} // fin du if (sub)
	else
	{
	      std::cout << "ERROR: Unable  to open file for reading." << std::endl;

	}

//////////////////////////////////////////////////////////////////////////////////////////////////Pipeline4////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string nameFile4;
	nameFile4=dataset;	

//file bms
	std::string BatchMakeScriptFile4 = "slicer3shapeworks.bms";
	std::ofstream file4( BatchMakeScriptFile4.c_str());
 
	file4 <<"set (groupIds "<< final_groups<<")"<<std::endl;
	file4 <<"set (subjWorkDir "<< subjWorkDir<<")"<<std::endl;
	file4 <<"set (exprId "<< exprId<<")"<<std::endl;

	file4 <<"include (/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/shapeworks.bms)"<<std::endl;

	file4.close();  
/*
	bm::ScriptParser m_Parser4;
	m_Parser4.LoadWrappedApplication( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" );
	m_Parser4.SetBatchMakeBinaryPath( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" ); 
	m_Parser4.Execute(BatchMakeScriptFile4);
*/

////////////////////////////////////////////////////////////////////////////////////////////////Pipeline5/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string nameFile5;
	nameFile5=dataset;	
	std::ifstream Sub2(nameFile.c_str(),std::ios::in);
	int i2=0;
	if(Sub2)
	{
		std :: string line;
		while(getline( Sub2, line ))
		{  
			if (i2>0)
			{
				char labelfile[200];  
				char subjid[100];
				char WarpedCorrespondenceMesh[200];
				char CorrespondenceMesh[200];		
				char labelMapInput[200];
				char measurementOutput[200];
				char group[100];
				char ThicknessSamplingResult[200];
				
				std::string BatchMakeScriptFile5 = "slicer3meshintensity.bms";
				std::ofstream file( BatchMakeScriptFile5.c_str());

				readFileCSV(line, group, subjid, labelMapInput, measurementOutput, labelfile, CorrespondenceMesh, WarpedCorrespondenceMesh, ThicknessSamplingResult);
		

				file <<"set (labelMapInput "<< labelMapInput<<")"<<std::endl;
				file <<"set (measurementOutput "<< measurementOutput<<")"<<std::endl;
				file <<"set (spharmBinaryInput "<<labelfile<<")"<<std::endl;
				file <<"set (case1 "<< subjid <<")"<<std::endl;
				file <<"set (CorrespondenceMesh "<<CorrespondenceMesh<<")"<<std::endl;
				file <<"set (WarpedCorrespondenceMesh  "<<WarpedCorrespondenceMesh<<")"<<std::endl;
				file <<"set (ThicknessSamplingResult  "<<ThicknessSamplingResult<<")"<<std::endl;
				file <<"set (group "<< group <<")"<<std::endl;
				file <<"set (subjWorkDir "<< subjWorkDir<<")"<<std::endl;
				file <<"set (exprId "<< exprId<<")"<<std::endl;

				file <<"include (/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/meshintensity.bms)"<<std::endl;

				file.close();
				/*
				bm::ScriptParser m_Parser;

				m_Parser.LoadWrappedApplication( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" );
				m_Parser.SetBatchMakeBinaryPath( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" ); 
				m_Parser.Execute(BatchMakeScriptFile5); 
				*/
			}
			i2++;
		}
		Sub2.close();
	} // fin du if (sub)
	else
	{
	      std::cout << "ERROR: Unable  to open file for reading." << std::endl;

	}
///////////////////////////////////////////////////////////////////////////////////////////////////////Pipeline6///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string nameFile1;
	nameFile1=dataset;	

	//file bms
	std::string BatchMakeScriptFile1 = "slicer3ThicknessSPHARM2.bms";
	std::ofstream file6( BatchMakeScriptFile1.c_str());
 
	file6 <<"set (groupIds "<< final_groups<<")"<<std::endl;
	file6 <<"set (subjWorkDir "<< subjWorkDir<<")"<<std::endl;
	file6 <<"set (exprId "<< exprId<<")"<<std::endl;
	file6 <<"set (subjectsWithGroups "<< subjGroup<<")"<<std::endl;
	file6 <<"include (/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/correspondance.bms)"<<std::endl;

	file6.close();  
/*
	bm::ScriptParser m_Parser2;
	m_Parser2.LoadWrappedApplication( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" );
	m_Parser2.SetBatchMakeBinaryPath( "/biomed-resimg/work/mjacquem/Slicer3Thickness2/SampleBatchMake/bmm2" ); 
	m_Parser2.Execute(BatchMakeScriptFile1);
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 // Memory Space liberation
  	for (int i = 0; i < L; ++i) delete [] groupIds[i];
  	delete [] groupIds;

 // Memory Space liberation
  	for (int i = 0; i < number_of_groups; ++i) delete [] subjgroups[i];
  	delete [] subjgroups;
 // Memory Space liberation
  	for (int i = 0; i < number_of_groups; ++i) delete [] Groups[i];
  	delete [] Groups;
	
return 0;
	
}

