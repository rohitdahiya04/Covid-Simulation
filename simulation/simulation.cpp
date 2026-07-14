#include "simulation.h"
#include "../include/INIReader.h"
#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>

// -------------------- Person --------------------

Person::Person(int duration)
{
    status = Status::SUSCEPTIBLE;
    infectedDays = 0;
    diseaseDuration = duration;
}

Status Person::getStatus() const
{
    return status;
}

void Person::infect()
{
    // Only susceptible persons can be infected
    if (status == Status::SUSCEPTIBLE)
    {
        status = Status::INFECTIOUS;
        infectedDays = 0;
    }
}

void Person::recover()
{
    // Recovered persons become immune
    status = Status::RECOVERED;
}

void Person::vaccinate()
{
    // Only susceptible persons can be vaccinated
    if (status == Status::SUSCEPTIBLE)
    {
        status = Status::VACCINATED;
    }
}

void Person::update()
{
    // Advance infection by one day
    if (status == Status::INFECTIOUS)
    {
        infectedDays++;
        // Recover after disease duration
        if (infectedDays >= diseaseDuration)
        {
            recover();
        }
    }
}

bool Person::isSusceptible() const
{
    return status == Status::SUSCEPTIBLE;
}

bool Person::isInfectious() const
{
    return status == Status::INFECTIOUS;
}

bool Person::isRecovered() const
{
    return status == Status::RECOVERED;
}

bool Person::isVaccinated() const
{
    return status == Status::VACCINATED;
}

// -------------------- Disease --------------------

Disease::Disease(
    std::string disease_name,
    int disease_duration,
    double disease_transmissibility)
{
    name = disease_name;

    duration = disease_duration;

    transmissibility = disease_transmissibility;
}

std::string Disease::getName() const
{
    return name;
}

int Disease::getDuration() const
{
    return duration;
}

double Disease::getTransmissibility() const
{
    return transmissibility;
}

// -------------------- Population --------------------

Population::Population(
    int populationSize,
    int diseaseDuration)
{
    for (int i = 0; i < populationSize; i++)
    {
        people.push_back(
            Person(diseaseDuration)
        );
    }
}

void Population::infectPatientZero()
{
    for (auto& person : people)
    {
        if (person.isSusceptible())
        {
            person.infect();
            std::cout << "Patient Zero infected." << std::endl;
            return;
        }
    }
    return;
}

void Population::vaccinatePopulation(double vaccinationRate)
{
    int numberVaccinated =
    static_cast<int>(
        std::round(vaccinationRate * people.size()));

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(
        0,
        people.size() - 1);

    int vaccinated = 0;

    while (vaccinated < numberVaccinated)
    {
        int index = dist(gen);

        if (people[index].isSusceptible())
        {
            people[index].vaccinate();
            vaccinated++;
        }
    }
}


int Population::countSusceptible() const
{
    int count = 0;

    for (const auto& person : people)
    {
        if (person.isSusceptible())
        {
            count++;
        }
    }

    return count;
}

int Population::countInfectious() const
{
    int count = 0;

    for (const auto& person : people)
    {
        if (person.isInfectious())
        {
            count++;
        }
    }

    return count;
}

int Population::countRecovered() const
{
    int count = 0;

    for (const auto& person : people)
    {
        if (person.isRecovered())
        {
            count++;
        }
    }

    return count;
}

int Population::countVaccinated() const
{
    int count = 0;

    for (const auto& person : people)
    {
        if (person.isVaccinated())
        {
            count++;
        }
    }

    return count;
}

int Population::getPopulationSize() const
{
    return people.size();
}

void Population::updatePeople()
{
    for (auto& person : people)
    {
        person.update();
    }
}

void Population::simulateDay(double transmissibility)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> personDistribution(
        0,
        people.size() - 1);

    std::uniform_real_distribution<> probabilityDistribution(
        0.0,
        1.0);

    std::vector<int> newInfections;

    for (int i = 0; i < people.size(); i++)
    {
        if (people[i].isInfectious())
        {
            for (int contact = 0; contact < 6; contact++)
            {
                int randomPerson =
                    personDistribution(gen);

                if (people[randomPerson].isSusceptible())
                {
                    double probability =
                        probabilityDistribution(gen);

                    if (probability < transmissibility)
                    {
                        newInfections.push_back(randomPerson);
                    }
                }
            }
        }
    }

    for (int index : newInfections)
    {
        people[index].infect();
    }

    updatePeople();
}


// -------------------- Simulation --------------------

Simulation::Simulation(std::string in_file)
{
    input_file = "configExample/disease_in.ini";
    INIReader reader(input_file);

    if (reader.ParseError() != 0)
    {
        std::cout << "Error loading INI file!" << std::endl;
        return;
    }

    std::string diseaseName =
    reader.Get("disease", "name", "Corona");

    int diseaseDuration =
        reader.GetInteger("disease", "duration", 7);

    double transmissibility =
        reader.GetReal("disease", "transmissibility", 0.3);
    
    populationSize =
    reader.GetInteger(
        "population_1",
        "size",
        30000);
    
    simulationRuns =
    reader.GetInteger(
        "global",
        "simulation_runs",
        5);

    disease = Disease(
        diseaseName,
        diseaseDuration,
        transmissibility);
    
    population = Population(
    populationSize,
    disease.getDuration());
    
    std::cout << "Disease Name: "
          << disease.getName() << std::endl;

    std::cout << "Duration: "
            << disease.getDuration() << std::endl;

    std::cout << "Transmissibility: "
            << disease.getTransmissibility() << std::endl;

    std::cout << "Population Size: "
          << population.getPopulationSize()
          << std::endl;
        
    std::cout << "Simulation Runs: "
          << simulationRuns
          << std::endl;
}

void Simulation::start()
{
    std::cout << "Starting simulation...\n";

    std::ofstream detailsFile("disease_details.csv");
    detailsFile
    << "VaccinationRate,"
    << "Run,"
    << "Day,"
    << "Susceptible,"
    << "Infectious,"
    << "Recovered,"
    << "Vaccinated\n";

    std::ofstream statsFile("disease_stats.csv");
    statsFile
    << "VaccinationRate,"
    << "AverageDays,"
    << "AverageRecovered,"
    << "AverageVaccinated\n";

    int totalDays;
    int totalRecovered;
    int totalVaccinated;
    
    for (int i = 0; i <= 10; i++)
    {
        double vaccinationRate = i / 10.0;
        totalDays = 0;
        totalRecovered = 0;
        totalVaccinated = 0;
        std::cout << "\n=================================\n";
        std::cout << "Vaccination Rate = "
                << vaccinationRate * 100
                << "%\n";

        for (int run = 1; run <= simulationRuns; run++)
        {
            std::cout << "\nRun "
                    << run
                    << " of "
                    << simulationRuns
                    << std::endl;

            int days = runExperiment(
                    vaccinationRate,
                    run,
                    detailsFile,
                    statsFile);

            totalDays += days;
            totalRecovered += population.countRecovered();
            totalVaccinated += population.countVaccinated();
        }

        double averageDays =
            static_cast<double>(totalDays) / simulationRuns;

        double averageRecovered =
            static_cast<double>(totalRecovered) / simulationRuns;

        double averageVaccinated =
            static_cast<double>(totalVaccinated) / simulationRuns;
        
        saveExperimentData(
            statsFile,
            vaccinationRate,
            averageDays,
            averageRecovered,
            averageVaccinated);

        std::cout << "\nAverage Results\n";

        std::cout << "Average Days = "
                << averageDays
                << std::endl;

        std::cout << "Average Recovered = "
                << averageRecovered
                << std::endl;

        std::cout << "Average Vaccinated = "
                << averageVaccinated
                << std::endl;

    }
            detailsFile.close();
            statsFile.close();
}

void Simulation::printStatistics(int day)
{
    std::cout
        << "Disease extinct after "
        << day
        << " days."
        << std::endl;
}

void Simulation::saveDailyData(
    std::ofstream& detailsFile,
    double vaccinationRate,
    int run,
    int day)
{
    detailsFile
    << vaccinationRate * 100 << ","
    << run << ","
    << day << ","
    << population.countSusceptible() << ","
    << population.countInfectious() << ","
    << population.countRecovered() << ","
    << population.countVaccinated()
    << '\n';
}

void Simulation::saveExperimentData(std::ofstream& statsFile,
                                    double vaccinationRate,
                                    double averageDays,
                                    double averageRecovered,
                                    double averageVaccinated)
{
    statsFile
        << vaccinationRate * 100 << ","
        << averageDays << ","
        << averageRecovered << ","
        << averageVaccinated
        << '\n';
}

int Simulation::runExperiment(
    double vaccinationRate,
    int run,
    std::ofstream& detailsFile,
    std::ofstream& statsFile)
{
    population = Population(
        populationSize,
        disease.getDuration());

    population.vaccinatePopulation(vaccinationRate);

    population.infectPatientZero();

    std::cout << "Susceptible = "
              << population.countSusceptible() << std::endl;

    std::cout << "Infectious = "
              << population.countInfectious() << std::endl;

    std::cout << "Recovered = "
              << population.countRecovered() << std::endl;

    std::cout << "Vaccinated = "
              << population.countVaccinated() << std::endl;

    int day = 0;

    while (population.countInfectious() > 0)
    {
        std::cout
            << "Day "
            << day
            << "  ";

        std::cout
            << "S="
            << population.countSusceptible()
            << " ";

        std::cout
            << "I="
            << population.countInfectious()
            << " ";

        std::cout
            << "R="
            << population.countRecovered()
            << " ";

        std::cout
            << "V="
            << population.countVaccinated()
            << '\n';

        saveDailyData(detailsFile,
              vaccinationRate,
              run,
              day);

        population.simulateDay(
            disease.getTransmissibility());

        day++;
    }

    printStatistics(day);
    return day;
}


