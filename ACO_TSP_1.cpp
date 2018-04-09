//============================================================================
// Name        : ACO.cpp
// Author      : Niranjana Deshpande
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#define MAXITER 100
#define MAXANTS 3
using namespace std;

int best_tour = 9999;

float alpha = 1;
float beta = 0.8;

float q_0 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
float q= static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

float epsilon = 0.1;
float rho = 0.01;

float t0 = 0.0000001 ;
int numants=10;

int goal = 9;

float adjacency_matrix[10][10];
float pheromone_matrix[10][10];

float local_pheromone_trail(int i,int j);

int final_dest;

class ant{


public:
	vector<int> local_mem;
	vector<int> cities_to_be_visited;
	float route_val=0;
	int current_position=0;
	int initial_position=0;
};


void initializeMatrix(float A[10][10]){
	int size=10;
	for(int h=0;h<size;h++){
		for(int i=0;i<size;i++){
			A[h][i]= t0;// + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2-0)));
			if(i==h){
				A[h][i] = 999;
			}
		}
	}

}

void printMatrix(float A[10][10]){
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			cout<<A[i][j]<<"\t";

		}
		cout<<endl;
	}
}

void copyMatrix(float A[10][10],float B[10][10]){
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			B[i][j] = A[i][j];
		}
	}
}

void initialize(ant* ants){
	initializeMatrix(adjacency_matrix);
	copyMatrix(adjacency_matrix,pheromone_matrix);

	for(int i=0;i<numants;i++){
		ants[i].route_val = 0 ;
		ants[i].current_position = rand() % numants;
		ants[i].initial_position = ants[i].current_position;
		//cout<<"ant "<<i<<" positioned at node "<<ants[i].current_position<<endl;

		for(int z=0;z<10;z++){
			if(z == ants[i].current_position){
				//do nothing
			}
			else{
				ants[i].cities_to_be_visited.push_back(z);
			}

		}
	}





}



int chooseNextCity(ant a){

	float val = 0;
	float temp_val = 0;
	int most_prob = -1 ;
	float sum = 0;
	float temp = 0;
	float prob = 0;

	q_0 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	q = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);


	if(q<q_0){

		//analyse probability of every neighbor, and then choose which one to go to
		for(std::vector<int>::iterator it = a.cities_to_be_visited.begin(); it != a.cities_to_be_visited.end(); ++it){
			int j = *it;
			val = pheromone_matrix[a.current_position][j] * ( pow((1/pheromone_matrix[a.current_position][j]), beta));
			if(val>temp_val){
				//if((find(local_mem.begin(), local_mem.end(), j) != local_mem.end()) == false)
					most_prob = j ;
			}
			temp_val=val;
		}

		return most_prob;

	}

	else{
		for(std::vector<int>::iterator it = a.cities_to_be_visited.begin(); it != a.cities_to_be_visited.end(); ++it){

			sum = sum + ((pheromone_matrix[a.current_position][*it]) * pow((1/pheromone_matrix[a.current_position][*it]),beta) );

			}

		//choose best neighbor
		for(std::vector<int>::iterator it = a.cities_to_be_visited.begin(); it != a.cities_to_be_visited.end(); ++it){
			int j = *it;
			prob = (pow(pheromone_matrix[a.current_position][j], alpha) * (pow((1/pheromone_matrix[a.current_position][j]), beta )))/sum;

			if(prob>temp){
				//if((find(local_mem.begin(), local_mem.end(), j) != local_mem.end()) == false)
					most_prob = j;
			}
			temp = prob;
		}

		return most_prob;

	}



	return 0;
}

void touring(ant* ants){
	int n = 10;
	int cityChosen = -1;
	for(int i=1;i<=10;i++){ //<MAXITER
		if(i<n){
			for(int k=0;k<numants;k++){
				cityChosen = chooseNextCity(ants[k]);
				ants[k].cities_to_be_visited.erase(std::remove(ants[k].cities_to_be_visited.begin(), ants[k].cities_to_be_visited.end(), cityChosen), ants[k].cities_to_be_visited.end());
				ants[k].local_mem.push_back(cityChosen);
				//ants[k].current_position = cityChosen;
			}

		}
		else{
			for(int k=0;k<numants;k++){
				cityChosen = ants[k].initial_position;
				ants[k].local_mem.push_back(cityChosen);
				//ants[k].current_position = cityChosen;
			}


		}
		for(int k=0;k<numants;k++){
				pheromone_matrix[ants[k].current_position][cityChosen] = ((1-rho)*pheromone_matrix[ants[k].current_position][cityChosen]) + rho*t0;
				ants[k].current_position = cityChosen;
			}
	}



}

void global_update(ant* ants){
	int best_ant = 0 ;
	float best_val = 999;

	for(int k=0;k<numants;k++){
		int sum = 0;
		int p = 0;
		for(std::vector<int>::iterator it = ants[k].local_mem.begin(); it != ants[k].local_mem.end(); ++it){
			if(p==0){
				p=ants[k].current_position;

			}
			else{
				p=*(it-1);
			}
			sum = sum + ((pheromone_matrix[p][*it]) * pow((1/pheromone_matrix[p][*it]),beta) );

		}
		if(sum < best_val){

			best_val = sum;
			best_ant = k;
		}
	}
	int sum = 0;
	int p = 0;
	for(std::vector<int>::iterator it = ants[best_ant].local_mem.begin(); it != ants[best_ant].local_mem.end(); ++it){
			if(p==0){
				p = ants[best_ant].current_position;

			}
			else{
				p = *(it-1);
			}
			pheromone_matrix[p][*it] = (1-alpha)*pheromone_matrix[p][*it] + alpha * pow(best_val,-1);// pow((1/pheromone_matrix[p][*it]),beta) );

	}
	best_tour = best_ant;

}

int main(){

	ant ants[numants];
	int iter = 1;
	vector<int> new_;
	//printMatrix(pheromone_matrix);
	initialize(ants);
	while(iter!=200){
		cout<<"Iteration: "<<iter<<endl;
		touring(ants);
		global_update(ants);
		iter = iter + 1;
		new_ = ants[best_tour].local_mem;
		for(int u=0;u<numants;u++){
			ants[u].local_mem.clear();
			ants[u].cities_to_be_visited.clear();
		}

		if(iter == 200){
			cout << "Best tour is : "<<new_.size()<<endl;
			for(std::vector<int>::iterator it = new_.begin(); it != new_.end(); ++it){

				cout << *it << "->";
			}
		}
	}




	return 0;
}
