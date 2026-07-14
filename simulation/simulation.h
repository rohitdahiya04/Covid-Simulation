#ifndef HPC_DISEASE_SIMULATION_H_
#define HPC_DISEASE_SIMULATION_H_
#include <string>
#include <vector>

enum class Status
{
    SUSCEPTIBLE,
    INFECTIOUS,
    RECOVERED,
    VACCINATED
};
class Person
{
public:
    Person(int duration = 5);
    Status getStatus() const;
    void infect();
    void recover();
    void vaccinate();
    void update();
    bool isSusceptible() const;
    bool isInfectious() const;
    bool isRecovered() const;
    bool isVaccinated() const;
private:
    Status status;
    int infectedDays;
    int diseaseDuration;
};
class Disease
{
    public:
    Disease(
        std::string name = "Corona",
        int duration = 5,
        double transmissibility = 0.1
    );

    std::string getName() const;
    int getDuration() const;
    double getTransmissibility() const;

private:
    std::string name;
    int duration;
    double transmissibility;
};
class Population
{
    public:
    Population(
        int populationSize = 100,
        int diseaseDuration = 5
    );
    void infectPatientZero();
    void vaccinatePopulation(double vaccinationRate);
    int getPopulationSize() const;
    void simulateDay(double transmissibility);
    void updatePeople();
    int countSusceptible() const;
    int countInfectious() const;
    int countRecovered() const;
    int countVaccinated() const;
   
    private:
        std::vector<Person> people;
};

class Simulation
{
    public:
        Simulation(std::string in_file = "disease_in.ini");
        void start();

    private:
        std::string input_file;
        Disease disease;
        Population population;
        int populationSize;
        int simulationRuns;
        int runExperiment(double vaccinationRate,
                  int run,
                  std::ofstream& detailsFile,
                  std::ofstream& statsFile);
        void printStatistics(int day);
        void saveDailyData(std::ofstream& detailsFile,
                   double vaccinationRate,
                   int run,
                   int day);
        void saveExperimentData(std::ofstream& statsFile,
                        double vaccinationRate,
                        double averageDays,
                        double averageRecovered,
                        double averageVaccinated);
};

#endif // HPC_DISEASE_SIMULATION_H_