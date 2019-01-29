//******************************************************************************

#include <iostream>
#include <algorithm>

#include "Accept.h"
#include "FA_tools.h"

void Affiche(const sAutoNDE& at)
{
	int j = 0;
	for(trans_t::const_iterator it = at.trans.begin(); it != at.trans.end(); it++, j++)
	{
		int k = 0;
		for(int i = 0 ; i < it[0].size(); i++, k++)
		{
			std::cout << j <<" "<<k+ASCII_A<<" " <<it[0][i]<<"\n";
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

bool EstDeterministe(const sAutoNDE& at){
  //TODO définir cette fonction
	bool r = true;
	// check if exist e transitions
	int j =0;
	for(epsilon_t::const_iterator it = at.epsilon.begin(); it != at.epsilon.end() && r; it++, j++)
	{
		if(it->begin() != it->end())
		{
			std::cout<<" Not Determinist : one or more e transition/s exist.\n";
			r = false;
		}
	}
	std::cout<<r<<"\n";
	if(r ) //if theree is missing transitions or if there is not enough transitions
	{
		for(trans_t::const_iterator it = at.trans.begin(); it != at.trans.end() && r; it++)
		{
			int k = 0;
			for(int i = 0 ; i < it[0].size() && r; i++, k++)
			{
				if(it[0][i].begin()==it[0][i].end())
				{
					r = false;
					std::cout<<" Not Determinist : one or more transition/s missing.\n";
				}
			}
		}
	}	
  return r;
}

////////////////////////////////////////////////////////////////////////////////

void Fermeture(const sAutoNDE& at, etatset_t& Pos){
  //TODO définir cette fonction
	for(std::set<etat_t>::iterator j = Pos.begin(); j != Pos.end(); j++)
	{
		for(int it = 0; (etat_t)it != at.epsilon.size(); it++)
		{
			if((etat_t)it == *j)
			{
				for (std::set<etat_t>::iterator i = at.epsilon[it].begin(); i != at.epsilon[it].end(); i++) {
					Pos.insert(*i);
					//std::cout<<*j<<"  "<<it<<"  "<<*i<<"\n";
				}
			}
		}
	}

}

////////////////////////////////////////////////////////////////////////////////

etatset_t Delta(const sAutoNDE& at, const etatset_t& Pos, symb_t sign){
	etatset_t r;
	for(std::set<etat_t>::iterator j = Pos.begin(); j != Pos.end(); j++)
	{
		int k =0; // the character
		for(int i = 0 ; i < at.trans[*j].size(); i++,k++)
		{
			//std::cout << *j <<" "<<k+ASCII_A<<" " <<at.trans[0][i]<<"\n";
			if(k+ASCII_A == sign )
			{
				//std::cout <<*j <<" "<<(char)(k+ASCII_A)<<" " <<at.trans[*j][i]<<"\n\n";
				etatset_t temp = at.trans[*j][i];
				for (etatset_t::iterator it2 = temp.begin(); it2 != temp.end(); it2++) 
				{
				   r.insert(*it2);
				}

			}
		}
		Fermeture(at, r);
	}
/*	for (etatset_t::iterator it2 = r.begin(); it2 != r.end(); it2++) 
	{
	   std::cout<<*it2<<"\n";
	}*/
	std::cout<<"\n";
  return r;
}

////////////////////////////////////////////////////////////////////////////////

bool Accept(const sAutoNDE& at, std::string str){
  //TODO définir cette fonction
	bool r = false;
	etat_t  startPos = at.initial;//start 
	etatset_t steps;
	steps.insert(startPos);
	Fermeture(at, steps);
	std::cout<<"initial"<<startPos<<"\n";

	for(std::string::iterator it = str.begin(); it!=str.end(); it++)
	{
		steps = Delta(at, steps,*it);
	}
	if(steps.begin() != steps.end())
	{
		for (etatset_t::iterator it2 = steps.begin(); it2 != steps.end() && !r; it2++) 
		{
			for (etatset_t::iterator it3 = at.finaux.begin(); it3 != at.finaux.end() && !r; it3++) 
			{
				if(*it2 == *it3)
				{
					r = true;
					std::cout<<" accepted by steps : "<<*it2<<"\n";
				}
			}
		}
	}
  return r;
}

//******************************************************************************
