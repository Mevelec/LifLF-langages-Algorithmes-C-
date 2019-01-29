//******************************************************************************

#include <iostream>
#include <cassert>
#include <utility>
#include <string>

#include "Equivalence.h"
#include "Accept.h"
#include "FA_tools.h"



////////////////////////////////////////////////////////////////////////////////

std::string Automate2ExpressionRationnelle(sAutoNDE at){
  std::string sr;

  	int j = 0;
  	//set automate values 
  	at.nb_etats+=2;
  	at.nb_finaux=1;
  	/////////////////////////////// INCREMENT IN TRANS ////////////////////////////////
  	//add step start
  	std::vector<etatset_t> a;
  	at.trans.insert(at.trans.begin(), a );
  	//add e trans to old start
  	etatset_t es;
  	es.insert(at.initial);
 	at.epsilon.insert(at.epsilon.begin(),es);
 	at.initial = 0;
  	std::cout<<"INTITIAL : "<<at.initial<<"\n\n";
	// add  final step
	std::vector<etatset_t> fs;
	at.trans.insert(at.trans.end(), fs );
	//
	for(trans_t::iterator it = at.trans.begin(); it != at.trans.end(); it++, j++)
	{
		std::cout<<j<<"\n";
		int k = 0;
		for(int i = 0 ; i < it[0].size(); i++, k++)
		{
			etatset_t temp;
			for (etatset_t::iterator it2 = it[0][k].begin(); it2 != it[0][k].end(); it2++) 
			{
				temp.insert(*it2+1);
			}
			it[0][k] = temp;

			for (etatset_t::iterator it2 = it[0][k].begin(); it2 != it[0][k].end(); it2++) 
			{
				temp.insert(*it2+1);
			    std::cout<<  j <<" -"<<k<<"-> "<<*it2 <<"\n";
			}
		}
	}
	std::cout<<"\n\n";

	/////////////////////////////// INCREMENT IN E-TRANS ////////////////////////////////
	//ad last step
	etatset_t efs;
	for(etatset_t::iterator it = at.finaux.begin(); it != at.finaux.end(); it++)
	{
		at.epsilon[*it+1].insert(at.epsilon.size()-1);
	}
	at.finaux.clear();
	at.finaux.insert(at.epsilon.size()-1);
	at.epsilon.insert(at.epsilon.end(), efs);
	//
	int k = 0;
	j=0;
	for(int i = 0 ; i < at.epsilon.size(); i++, k++)
	{
		std::cout<<i<<"\n";
		etatset_t temp;
		for (etatset_t::iterator it2 = at.epsilon[k].begin(); it2 != at.epsilon[k].end(); it2++) 
		{
			temp.insert(*it2+1);
		}
		at.epsilon[k] = temp;

		for (etatset_t::iterator it2 = at.epsilon[k].begin(); it2 != at.epsilon[k].end(); it2++) 
		{
			temp.insert(*it2+1);
		    std::cout<<  i <<" -"<<k+ASCII_A<<"-> "<<*it2 <<"\n";
		}
	}
	std::cout<<"\nBEGIN \n";
	std::cout<<"AA : "<< at.nb_etats;
	std::vector<std::vector<std::pair<etat_t, std::string>>> start(at.nb_etats);

	////////////////// GAT ALL LINKS FROM STEp //////////////////////////
	j = 0;
	for(trans_t::const_iterator it = at.trans.begin(); it != at.trans.end(); it++, j++)
	{
		int k = 0;
		for(int i = 0 ; i < it[0].size(); i++, k++)
		{
			for(etatset_t::iterator it2 = it[0][i].begin(); it2 != it[0][i].end(); it2++)
			{
				std::cout << j <<" "<<k<<" " <<*it2<<"\n";
				char tmpString = (char)(i+ASCII_A);
				start[j].insert(start[j].begin(), std::make_pair(*it2, std::to_string(tmpString)));
			}
		}
	}
	j = 0;
	std::cout<<"epsilon links : \n";
	for(epsilon_t::iterator it = at.epsilon.begin();it != at.epsilon.end(); it++, j++)
	{
		for(etatset_t::iterator it2 = (*it).begin();it2 != (*it).end(); it2++)
		{
			start[j].insert(start[j].begin(), std::make_pair(*it2, "e"));
			std::cout<<j<<" -> "<<*it2<<"\n"; 
		}
	}
std::cout<<"\n AFFICHE : \n";
	j = 0;
	for(std::vector<std::vector<std::pair<etat_t, std::string>>>::iterator it = start.begin(); it != start.end(); it++, j++)
	{
		for(std::vector<std::pair<etat_t, std::string>>::iterator it2 = (*it).begin(); it2 != (*it).end(); it2++)
		{
			std::cout<< j <<" -to-> "<< (*it2).first<<" with : "<<(*it2).second<<"\n";
		}
	}
	std::cout<<"\n\n";
///////////////////////////
	//iterate over list, select elem to delete // 	i,  j = node number
	j = 1;
	for(int i = 1; i!= start.size()-1; j++)
	{
		std::cout<<"DEL : "<<j<<"\n";
		std::string stars;
		//DETECT loop on itself
		for(int m = 0; m!= start[i].size(); )
		{
			if(start[i][m].first == j)
			{
				stars += "."+start[i][m].second;
				start[i].erase(start[i].begin()+m);
			}
			else
			{
				m++;
			}
		}
		if(stars.size()>0)
		{
				stars = ".("+stars+")*";
		}
		std::cout<<"stars : "<<stars<<"\n"; 

		// DELETE NODE
		//iterate over list // 	a
		for(int a =0; a !=start.size(); a++)
		{
			//do not modify deleting node
			if(a != j)
			{
				//iterate over destination // 	b
				for(int b =0; b!=start[a].size(); )
				{
					if(start[a][b].first == j )
					{
						std::cout<<"     col : "<<a<<"   "<<j<<"\n";
						std::string save = start[a][b].second;
						//erase entrys to deleting node
						start[a].erase(start[a].begin()+b);
						//insert entrys to succesor of deleting node
						for(int k = 0; k<=start[i].size(); k++, b++)
						{
							//start[a].insert(start[a].end(), std::make_pair(start[i][k].first, ));
							if(start[i][k].first != j && start[i][k].first != a && start[i][k].first != 0)
							{
								std::string tempString = save+stars+"."+start[i][k].second;
								std::cout<<"       add to : "<<a<<"  pair : "<<start[i][k].first<<", "<<tempString<<"\n";
								start[a].insert(start[a].begin(), std::make_pair(start[i][k].first, tempString));
							}
							else
							{
								b--;
							}
						}
					}
					else
					{
						b++;
					}
				}
			}
		}
		// CHECK if double dest
		for(int a =0; a !=start.size(); a++)
		{
			for(int b =0; b!=start[a].size();b++)
			{
				for(int c=b+1; c !=start[a].size();)
				{
					if(start[a][b].first == start[a][c].first)
					{
						start[a][b].second += " | "+start[a][c].second;
						start[a].erase(start[a].begin()+c);
					}
					else
					{
						c++;
					}
				}

			}
		}
		start.erase(start.begin()+1);
	}
/*	j = 1;
	for(std::vector<std::vector<std::pair<etat_t, std::string>>>::iterator it = start.begin()+1; it != start.end()-1; it++, j++)
	{
		//// seek link to himself and create star
		std::string star;
		for(std::vector<std::pair<etat_t, std::string>>::iterator it2 = (*it).begin(); it2 != (*it).end(); it2++)
		{
			if(j == (*it2).first)
			{
				star += "."+(*it2).second+'*';
				(*it).erase(it2);
				it2 = (*it).begin();
			}
		}
		//// all other links 
		for(int a =0 ; a != start.size(); a++)
		{
			for(int b =0; b != start[a].size(); b++)
			{
				if(start[a][b].first == j)
				{

					std::cout<<"Del : "<<j<<"  "<<star + start[a][b].second <<"\n";
					start[a].erase(start[a].begin()+b);
					for(int c = 0; c != (*it).size(); c++)
					{
						
					}
				} 
			}
		}
		//// couple links to the same *		for(std::vector<std::pair<etat_t, std::string>>::iterator it2 = (*it).begin(); it2 != (*it).end();)
		{
			(*it).erase(it2);
			it2 = (*it).begin();
		}
	}*/
////////////////////////////////
	std::cout<<"\n AFFICHE \n";
	for(j=0; j != start.size(); j++)
	{
		for(int k =0; k != start[j].size(); k++)
		{
			std::cout<< j <<" -to-> "<< start[j][k].first<<" with : "<<start[j][k].second<<"\n";
		}
	}


	std::cout<<"\nSegt fault after\n\n";


  return sr;
}

////////////////////////////////////////////////////////////////////////////////

bool PseudoEquivalent(const sAutoNDE& a1, const sAutoNDE& a2, unsigned int word_size_max) {
  //TODO définir cette fonction

  return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2) {
  //TODO définir cette fonction

  return true;
}

//******************************************************************************
