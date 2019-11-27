#include <iostream>
#include <bitset>
#include <string>
#include <math.h>
#include <time.h>
#include "Individual.h"
// number of genes per individual
const int geneCount = 200;
//ideal phenotype preferred
std::bitset<geneCount> idealPhenotype(std::string("01010110010111001010"));
//number of individuals present
const int population = 10000;
//mutation rate 
const int mutationRate = 100;
int maxValue = int(pow(2, geneCount));
int seed = int(time(NULL));
Individual individuals[population];
std::bitset<geneCount> integerToBitset(int x)
{
	std::bitset<geneCount> toReturn;
	for (int i = geneCount-1; i >= 0; i--)
	{
		if ((x - pow(2, i) >= 0) && x > 0)
		{
			toReturn[i] = true;
			x -= int(pow(2, i));
		}
	}
	return toReturn;
}
int bitsetToInteger(std::bitset<geneCount> x)
{
	int toReturn = 0;
	for (int i = geneCount-1; i >= 0; i--)
	{
		if (x[i] == true)
		{
			toReturn += int(pow(2, i));
		}
	}
	return toReturn;
}
int generateChromosome()
{
	srand(seed++);
	return (rand() % maxValue);
}
int returnFitness(int phenotype)
{
	std::bitset<geneCount> temp = integerToBitset(phenotype);
	int count = 0;
	for (int i = 0; i < geneCount; i++)
	{
		if (temp[i] == idealPhenotype[i])
		{
			count++;
		}
	}
	return count;
}
int genotypeToPhenotype(int chromosomes[2])
{
	std::bitset<geneCount> chromosomeOne = integerToBitset(chromosomes[0]);
	std::bitset<geneCount> chromosomeTwo = integerToBitset(chromosomes[1]);
	std::bitset<geneCount> phenotypeBitset;
	for (int i = 0; i < geneCount; i++)
	{
		if (chromosomeOne[i] || chromosomeTwo[i])
		{
			phenotypeBitset[i] = true;
		}
		else
		{
			phenotypeBitset[i] = false;
		}
	}

	return bitsetToInteger(phenotypeBitset);
}
Individual generateNewIndividual()
{
	Individual temp = Individual();
	temp.getChromosomes()[0] = generateChromosome();
	temp.getChromosomes()[1] = generateChromosome();
	temp.setPhenotype(genotypeToPhenotype(temp.getChromosomes()));
	temp.setFitness(returnFitness(temp.getPhenotype()));
	return temp;
}
void printPopulation()
{
	
	int individualsPerLine = 10;
	
	for (int i = 0; i < population; i++)
	{
		int pSpaces = 0;
		int fSpaces = int(log10(geneCount));
		for (int n = 0; n < pSpaces; n++)
		{
			std::cout << " ";
		}
		std::cout << individuals[i].getPhenotype() << ",";
		for (int n = 0; n < fSpaces - (log10(individuals[i].getFitness()+1)); n++)
		{
			std::cout << " ";
		}
		std::cout << individuals[i].getFitness() << "|";
		if ((i+1) % individualsPerLine == 0)
		{
			std::cout << std::endl;
		}
	}
	std::cout << "" <<std::endl;
}
int chromosomalMutation(int chromosome)
{
	int mutation = rand() % (mutationRate + 1);
	int locus = rand() % geneCount;
	if (mutation == 0)
	{
		std::bitset<geneCount> temp = integerToBitset(chromosome);
		temp[locus] = !temp[locus];
		return bitsetToInteger(temp);
	}
	return chromosome;
}
Individual produceOffspring(Individual one, Individual two)
{
	Individual temp = Individual();
	int randomOne = rand() % 2;
	int randomTwo = rand() % 2;
	temp.getChromosomes()[0] = chromosomalMutation(one.getChromosomes()[randomOne]);
	temp.getChromosomes()[1] = chromosomalMutation(two.getChromosomes()[randomTwo]);
	temp.setPhenotype(genotypeToPhenotype(temp.getChromosomes()));
	temp.setFitness(returnFitness(temp.getPhenotype()));
	return temp;
}
int main()
{

	for (int i = 0; i < population; i++)
	{
		individuals[i] = generateNewIndividual();
	}

	double averageFitness = 0;
	int generationCount = 0;
	while (averageFitness < geneCount)
	{
		generationCount++;
		averageFitness = 0.0;
		for (int i = 0; i < population; i++)
		{
			averageFitness += double(individuals[i].getFitness());
		}
		std::cout << std::endl;
		std::cout << "Generation #"<<generationCount << std::endl;
		averageFitness /= double(population);
		std::cout << "Average Fitness: " << averageFitness << std::endl;
		printPopulation();
		int deathCount = population / 2;
		for (int i = 0; i < population; i++)
		{
			if (double(individuals[i].getFitness()) <= (averageFitness))
			{
				individuals[i].kill();
				deathCount--;
			}
			if (deathCount == 0)
			{
				break;
			}
		}

		Individual newIndividuals[population] = {};
		for (int i = 0; i < population; i++)
		{
			int randomOne = 0;
			int randomTwo = 0;
			do
			{
				randomOne = rand() % population;
			} while (!individuals[randomOne].getAlive());
			do
			{
				randomTwo = rand() % population;
			} while (!individuals[randomTwo].getAlive());

			newIndividuals[i] = produceOffspring(individuals[randomOne], individuals[randomTwo]);
		}
		for (int i = 0; i < population; i++)
		{
			if (!individuals[i].getAlive())
			{
				individuals[i] = newIndividuals[i];
				newIndividuals[i] = Individual();
			}
		}

	}

}

