//******************************************************************************

#include <iostream>

#include "Determinize.h"
#include "FA_tools.h"
#include "Accept.h"
#include <string>

int exist( std::vector<etatset_t>& vec, std::vector<etatset_t>& vec2,  etatset_t& elem)
{
	int r = 0;
	bool b = false;
	for(std::vector<etatset_t>::iterator it = vec.begin(); it != vec.end() && !b; it++)
	{
		//std::cout<<" LOL  "<<*it<<"  "<<r<<"\n";
		r++;
		if(*it == elem)
		{
			b = true;
		}
	}

	for(std::vector<etatset_t>::iterator it = vec2.begin(); it != vec2.end() && !b; it++)
	{
		//std::cout<<" LOL2  "<<*it<<"  "<<r<<"\n";
		r++;
		if(*it == elem)
		{

			b=true;
		}
	}

	if(!b)
	{
		r = 0;
	}
	return r;
}

bool checkFinal(const sAutoNDE& at, etatset_t& result)
{
	bool r = false;
	for(std::set<etat_t>::iterator it = at.finaux.begin(); it != at.finaux.end(); it++)
	{
		std::cout<<*it<<"\n";
		for(std::set<etat_t>::iterator it2 = result.begin(); it2 != result.end(); it2++)
		{
			std::cout<<" check "<<*it<<"  "<<*it2<<"\n";
			if(*it == *it2)
			{
				r = true;
			}
		}
	}
	return r;
}
////////////////////////////////////////////////////////////////////////////////

sAutoNDE Determinize(const sAutoNDE& at)
{
	std::ofstream file, tempFile;
	file.open("Determinized.txt", std::ofstream::out | std::ofstream::trunc);
	file.close();
	file.open("test.txt", std::ofstream::out | std::ofstream::trunc);
	file.close();
	
	file.open ("Determinized.txt" , std::ofstream::out | std::ofstream::app);
	tempFile.open ("temp.txt" , std::ofstream::out | std::ofstream::app);
	
	sAutoNDE r;
	//find all start pos, create node
	etat_t  startPos = at.initial;//start 
	etatset_t Pos;
	etatset_t Finals;

	Pos.insert(startPos);
	if(checkFinal(at, Pos))
	{
		Finals.insert(0);
	}

	// init start with empty links
	Fermeture(at, Pos);	
	std::vector<etatset_t>  steps;
	std::vector<etatset_t>  stepsPile;
	stepsPile.push_back(Pos);
	std::vector<etatset_t>::iterator it = stepsPile.begin();
	int i = 0;
	while (!stepsPile.empty())
	{
		for (size_t cnt = 0; cnt<at.nb_symbs; cnt++)
		{
			etatset_t result = Delta(at, *it, cnt+ASCII_A);
			//std::cout<<"character : "<<cnt+ASCII_A<<" result : " <<*it<<"\n";

			int a = exist(steps, stepsPile, result);
		    if(a == 0) 
		    {
		    	//std::cout<<"don t  exist : "<<result<<"  "<<a<<"\n";
				stepsPile.push_back(result);
				tempFile<<i<<" "<<(char)(cnt+ASCII_A)<<" "<<steps.size()+stepsPile.size()-1<</*"  res : "<<result<<*/"\n";
				if(checkFinal(at, result))
				{
					Finals.insert(steps.size()+stepsPile.size()-1);
				}
				it = stepsPile.begin();
			}
			else
			{
				//std::cout<<"already exist : "<<result<<"  "<<a<<"\n";
				tempFile<<i<<" "<<(char)(cnt+ASCII_A)<<" "<<a-1/*<<"  res : "<<result*/<<"\n";
			}
		}
		steps.push_back(*it);		
		stepsPile.erase(stepsPile.begin());
		i++;
	}
	file << steps.size()<<" "<<at.nb_symbs<<" "<<Finals.size()<<"\n";
	tempFile.close();
	file<<at.initial<<"\n";
	for(std::set<etat_t>::iterator it = Finals.begin(); it != Finals.end(); it++)
	{
		file<<*it<<"\n";
	}
	std::ifstream f("temp.txt");
	std::string str;
	while(std::getline(f, str))
	{
		file<<str<<"\n";
	}
	tempFile.close();
	file.close();
	return r;
}
//******************************************************************************
