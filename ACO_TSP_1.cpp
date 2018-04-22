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
#include <ctime>
#define MAXITER 200
#define MAXANTS 1024
#define SIZE 1024
#define GOAL 970

using namespace std;

int best_tour = 9999;
int best_ant = 999;

float alpha = 1;
float beta = 0.8;

float q_0 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
float q= static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

float epsilon = 0.1;
float rho = 0.01;

float t0 = 0.5 ;
int numants = MAXANTS;

int goal = GOAL;

float adjacency_matrix[SIZE][SIZE];
float pheromone_matrix[SIZE][SIZE];


class ant{


public:
	vector<int> local_mem;
	vector<int> cities_to_be_visited;
	float route_cost=0;
	int current_position=0;
	int initial_position=0;
};

void initializeMatrixAdjacent(float A[SIZE][SIZE]){
	;
	for(int h=0;h<SIZE;h++){
		for(int i=0;i<SIZE;i++){
			A[h][i]= 1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(56-1)));
			if(i==h){
				A[h][i] = INFINITY;
			}
		}
	}

}


void initializePheroMatrix(float A[SIZE][SIZE]){

	for(int h=0;h<SIZE;h++){
		for(int i=0;i<SIZE;i++){
			A[h][i]= t0;// + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2-0)));
			if(i==h){
				A[h][i] = 0;
			}
		}
	}

}

void printMatrix(float A[SIZE][SIZE]){
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			cout<<A[i][j]<<"\t";

		}
		cout<<endl;
	}
}


void initialize(ant* ants){
	initializeMatrixAdjacent(adjacency_matrix);
	//copyMatrix(adjacency_matrix,pheromone_matrix);
	initializePheroMatrix(pheromone_matrix);
	for(int i=0;i<numants;i++){
		ants[i].route_cost = 0 ;
		ants[i].current_position = rand() % SIZE;//numants;
		ants[i].initial_position = ants[i].current_position;
		//cout<<"ant "<<i<<" positioned at node "<<ants[i].current_position<<endl;

		for(int z=0;z<SIZE;z++){
			if(z == ants[i].current_position || adjacency_matrix[ants[i].current_position][z]==999){
				//do nothing
			}
			else{
				//cout<<"Pushing back: "<<z<<endl;
				ants[i].cities_to_be_visited.push_back(z);
			}

		}
	}
}



int chooseNextCity(ant a){

	float temp_prob = 0;
	float best_prob = 0;
	int most_prob_city = -1 ;
	float sum = 0;
	float temp = 0;
	float prob = 0;

	q_0 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	q = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);


	if(q<q_0){

		//analyse probability of every neighbor, and then choose which one to go to
		for(std::vector<int>::iterator it = a.cities_to_be_visited.begin(); it != a.cities_to_be_visited.end(); ++it){
			int j = *it;
			if(adjacency_matrix[a.current_position][j]!=INFINITY){
				temp_prob = pheromone_matrix[a.current_position][j] * ( pow((1/pheromone_matrix[a.current_position][j]), beta));
				if(temp_prob>best_prob){
					//if((find(local_mem.begin(), local_mem.end(), j) != local_mem.end()) == false)
						most_prob_city = j ;
						best_prob=temp_prob;
				}

			}
		}

		return most_prob_city;

	}

	else{
		for(std::vector<int>::iterator it = a.cities_to_be_visited.begin(); it != a.cities_to_be_visited.end(); ++it){

			sum = sum + ((pheromone_matrix[a.current_position][*it]) * pow((1/pheromone_matrix[a.current_position][*it]),beta) );

			}

		//choose best neighbor
		for(std::vector<int>::iterator it = a.cities_to_be_visited.begin(); it != a.cities_to_be_visited.end(); ++it){
			int j = *it;

			if(adjacency_matrix[a.current_position][j]!=INFINITY){

				prob = (pow(pheromone_matrix[a.current_position][j], alpha) * (pow((1/pheromone_matrix[a.current_position][j]), beta )))/sum;

				if(prob>temp){
					//if((find(local_mem.begin(), local_mem.end(), j) != local_mem.end()) == false)
						most_prob_city = j;
						temp = prob;
				}

			}
		}

		return most_prob_city;

	}



	//return 0;
}

void touring(ant* ants){
	int n = SIZE;
	int cityChosen = -1;
	//for(int i=1;i<=SIZE;i++){ //<MAXITER
		//if(i<n){
			for(int k=0;k<numants;k++){
				cityChosen = chooseNextCity(ants[k]);

				if(cityChosen!=-1 && cityChosen!=ants[k].initial_position && cityChosen!=ants[k].current_position){

					ants[k].local_mem.push_back(cityChosen);

					//cout<<"Current position "<<ants[k].current_position <<" city chosen "<< cityChosen<<" initial position "<<ants[k].initial_position<<endl;

					//cout<<adjacency_matrix[ants[k].current_position][cityChosen]<<endl;

					ants[k].route_cost += adjacency_matrix[ants[k].current_position][cityChosen];

					ants[k].cities_to_be_visited.erase(std::remove(ants[k].cities_to_be_visited.begin(), ants[k].cities_to_be_visited.end(), cityChosen), ants[k].cities_to_be_visited.end());

					pheromone_matrix[ants[k].current_position][cityChosen] = ((1-rho)*pheromone_matrix[ants[k].current_position][cityChosen]) + rho*t0;

					ants[k].current_position = cityChosen;
				}
			}

		//}

	//}



}

void global_update(ant* ants){
	int best_ant = 0 ;
	float best_val = 999;

	//finds the best ant and updates

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
	int p = 0;

	//actual global update
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

void printExecutionTime(clock_t begin, clock_t end){
	double duration = double(end - begin)/CLOCKS_PER_SEC;
	cout<<"Time taken to execute (s) "<<duration<<endl;
}

int main(){

	ant ants[numants];

	float prev_cost[numants];
	bool equal = false;
	int iter = 0;
	vector<int> new_;
	//printMatrix(pheromone_matrix);
	initialize(ants);

	clock_t begin = clock();
	clock_t end;

	while(iter!=MAXITER){


		cout<<"Iteration: "<<iter<<endl;

		touring(ants);
		global_update(ants);


		new_ = ants[best_tour].local_mem;

		//compare previous values with current route values

		if(iter!=0){

			for(int y = 0;y<numants;y++){
				if(prev_cost[y]==ants[y].route_cost){
					equal = true;
				}
			}
		}


		//store new values in prev_cost

		for(int y = 0;y<numants;y++){

			cout<<"Ant "<<y <<" route cost "<<ants[y].route_cost<<endl;
			prev_cost[y] = ants[y].route_cost;
		}

		//clear path followed

		for(int u=0;u<numants;u++){
			ants[u].local_mem.clear();
			//ants[u].cities_to_be_visited.clear();
		}
		iter = iter + 1;

		//if the costs are the same, terminate

		if(equal){
			cout<<"Convergence reached at iteration "<<(iter-1)<<endl;
			cout << "Best tour is by ant "<<best_tour<<endl;
			end = clock();
			printExecutionTime(begin, end);
			exit(0);
		}
	}

	end = clock();
	printExecutionTime(begin, end);
	cout << "Best tour is by ant "<<best_tour<<endl;

	return 0;
}
