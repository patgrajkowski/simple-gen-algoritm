#include <iostream>
#include <cmath>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include <random>

using namespace std;
random_device randomDevice;
mt19937 generator(randomDevice());
double pr_krzyz = 0.9;
double pr_mut = 0.15;
int const a = 4;
int const b = 7;
int const c = 2;
int const ile_wyn = 40;
int const lb_pop = 10;
int const ile_os = 15;
int f(int x){
    return a*pow(x, 2)+(b*x)+c;
}
int toDec(string x){
    int bin = stoi(x);
    int remainder = 0, dec = 0, i = 0;
    while(bin != 0){
        remainder = bin%10;
        bin /= 10;
        dec += remainder*pow(2,i);
        ++i;
    }
    return dec;
}
vector <string> generateRandomPop(int populationSize = ile_os){
    vector<string> population;
    string individual = "xxxxxxxx";
    uniform_int_distribution<int> distribution(0,1);
    for(int i = 0; i < populationSize; i++){
        individual = "xxxxxxxx";
        for(int j = 0; j < 8; j++){
            individual[j] = '0'+ distribution(generator);
        }
        population.push_back(individual);
    }
    return population;
}
int getSum(vector <string> population){
    int sum = 0;
    for(string individual : population){
        sum+= f(toDec(individual));
    }
    return sum;
}
vector <string> selectPopulation(vector <string> population){
    vector <double> populationDist;
    vector <string> selectedIndividuals;
    int sum = getSum(population);
    double decValue;
    for(string individual : population){
        decValue =f(toDec(individual));
        populationDist.push_back((decValue/sum)*100);
    }
    discrete_distribution<> discreteDist(populationDist.begin(), populationDist.end());
        for(double value : populationDist) {
            selectedIndividuals.push_back(population[discreteDist(generator)]);
        }
    return selectedIndividuals;
}
vector <string> crossPopulation(vector <string> population){
    uniform_int_distribution<int> cuttingPointRandom(1,7);
    uniform_real_distribution<double> crossRandom(0.0, 1.0);
    string parent1, parent2, child1, child2;
    vector <string> crossedPopulation;
    bool cross;
    for(int i = 0; i < population.size(); i+=2){
        crossRandom(generator) < pr_krzyz ? cross = true : cross = false;
        if(i+1 == population.size()){
            crossedPopulation.push_back(population.at(i));
            break;
        }
        child1 = "";
        child2 = "";
        if(cross){
            int cuttingPoint = cuttingPointRandom(generator);
            parent1 = population.at(i);
            parent2 = population.at(i+1);
            for(int j = 0; j < population.at(i).length(); j++){
                if(j < cuttingPoint){
                    child1.push_back(parent1.at(j));
                    child2.push_back(parent2.at(j));
                }else{
                    child1.push_back(parent2.at(j));
                    child2.push_back(parent1.at(j));
                }
            }
                crossedPopulation.push_back(child1);
                crossedPopulation.push_back(child2);
        }else{
            crossedPopulation.push_back(population.at(i));
            crossedPopulation.push_back(population.at(i+1));
        };
    }
    return crossedPopulation;
}
vector <string> mutatePopulation(vector <string> population){
    uniform_real_distribution<double> mutateRandom(0.0, 1.0);
    vector <string> mutatedPopulation;
    bool mutate;
    for(string individual : population){
        for(int j = 0; j < individual.length(); j++){
            mutateRandom(generator) < pr_mut ? mutate = true : mutate = false;
            if(mutate){
                individual[j] == '0' ? individual[j] = '1' : individual[j] = '0';
            }
        }
            mutatedPopulation.push_back(individual);
    }
    return mutatedPopulation;
}
int main()
{
    for(int i = 0; i < ile_wyn; i++){
        vector <int> mutatedPopulation;
        vector <string> population = generateRandomPop();
         for(int j = 0; j < lb_pop; j++){
            shuffle(population.begin(), population.end(), generator);
            population = mutatePopulation(population);
            population = crossPopulation(population);
            population = selectPopulation(population);
        }
        for(string i : population){
            mutatedPopulation.push_back(toDec(i));
        }
        int max = *max_element(mutatedPopulation.begin(), mutatedPopulation.end());
        cout<<"x = " << max << "   " << "f(x) = " << f(max) << endl;
        mutatedPopulation.clear();
        population.clear();
    }
}