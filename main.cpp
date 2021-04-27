#include <iostream>
#include <cmath>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include <random>
#include <fstream>
using namespace std;
random_device randomDevice;
mt19937 generator(randomDevice());
double pr_krzyz = 0.9;
double pr_mut = 0.1;
int const a = 4;
int const b = 7;
int const c = 2;
int const ile_wyn = 40;
int const lb_pop = 3;
int const ile_os = 10;
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
double getSum(vector <string> population){
    double sum = 0;
    for(string individual : population){
        sum+= f(toDec(individual));
    }
    return sum;
}
vector <string> selectPopulation(vector <string> population){
    vector <double> populationDist;
    vector <string> selectedIndividuals;
    int sum = getSum(population);
    uniform_real_distribution<double> populationRandom(0.0, 1.0);
    double random;
    double decValue;
    double prop;
    int length = population.size();
    for(int i = 0; i < population.size(); i++){
        decValue = f(toDec(population[i]));
        prop = decValue / sum;
        if(i==0){
        populationDist.push_back(prop);
        }else{
        populationDist.push_back((populationDist[i-1]+prop));
        }
    }
    for(int i = 0; i < population.size(); i++){
        decValue = f(toDec(population[i]))/sum;
        if(i==0){
        populationDist.push_back(decValue);
        }else{
        populationDist.push_back((populationDist[i-1]+decValue));
        }
    }
    for(int i = 0; i < population.size(); i++){
        random = populationRandom(generator);
        for(int j = 0; j < population.size(); j++){
            if (j == 0){
                if(random >= 0 && random < populationDist[j]){
                    selectedIndividuals.push_back(population[j]);
                    continue;
                }
            }else{
                if(random >= populationDist[j-1] && random < populationDist[j]){
                    selectedIndividuals.push_back(population[j]);
                }
            }
        }
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
int main(){
    ofstream file;
    file.open ("results.txt");
    for(int i = 0; i < ile_wyn; i++){
        vector <int> mutatedPopulationFx;
        vector <int> mutatedPopulationX;
        int maxIndex;
        vector<int>::iterator result;
        vector <string> population = generateRandomPop();
        for(int j = 0; j < lb_pop; j++){
            shuffle(population.begin(), population.end(), generator);
            population = crossPopulation(population);
            population = mutatePopulation(population);
            population = selectPopulation(population);
        }
        for(string individual : population){
            mutatedPopulationFx.push_back(f(toDec(individual)));
            mutatedPopulationX.push_back(toDec(individual));
        }
        result = max_element(mutatedPopulationFx.begin(), mutatedPopulationFx.end());
        maxIndex = distance(mutatedPopulationFx.begin(), result);
        cout << mutatedPopulationX[maxIndex] << "   " << mutatedPopulationFx[maxIndex] << endl;
        file << to_string(mutatedPopulationX[maxIndex]) + "  " + to_string(mutatedPopulationFx[maxIndex]) + "\n";
        mutatedPopulationFx.clear();
        mutatedPopulationX.clear();
        population.clear();
    }
        file.close();
}