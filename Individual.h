#include <iostream>
class Individual
{
private:
	int chromosome[2];
	int phenotype, fitness;
	bool alive = true;
public:
	bool getAlive()
	{
		return alive;
	}
	void kill()
	{
		alive = false;
	}
	int* getChromosomes()
	{
		return chromosome;
	}
	int getPhenotype()
	{
		return phenotype;
	}
	void setPhenotype(int x)
	{
		phenotype = x;
	}
	int getFitness()
	{
		return fitness;
	}
	void setFitness(int x)
	{
		fitness = x;
	}
}; 
