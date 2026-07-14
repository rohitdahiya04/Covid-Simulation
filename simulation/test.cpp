#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../include/doctest.h"
#include <mutex>
#include "simulation.h"

TEST_CASE("Person starts susceptible")
{
    Person p;

    CHECK(p.getStatus() == Status::SUSCEPTIBLE);
    CHECK(p.isSusceptible());
}

TEST_CASE("Person can be infected")
{
    Person p;

    p.infect();

    CHECK(p.getStatus() == Status::INFECTIOUS);
    CHECK(p.isInfectious());
}

TEST_CASE("Person can recover")
{
    Person p;

    p.infect();
    p.recover();

    CHECK(p.getStatus() == Status::RECOVERED);
    CHECK(p.isRecovered());
}

TEST_CASE("Person can be vaccinated")
{
    Person p;

    p.vaccinate();

    CHECK(p.getStatus() == Status::VACCINATED);
    CHECK(p.isVaccinated());
}

TEST_CASE("Vaccinated person cannot be infected")
{
    Person p;

    p.vaccinate();
    p.infect();

    CHECK(p.getStatus() == Status::VACCINATED);
    CHECK(p.isVaccinated());
}

TEST_CASE("Recovered person cannot be infected again")
{
    Person p;

    p.infect();
    p.recover();
    p.infect();

    CHECK(p.getStatus() == Status::RECOVERED);
    CHECK(p.isRecovered());
}

TEST_CASE("Person recovers automatically after disease duration")
{
    Person p(7);

    p.infect();

    for (int i = 0; i < 7; i++)
    {
        p.update();
    }

    CHECK(p.getStatus() == Status::RECOVERED);
    CHECK(p.isRecovered());
}

TEST_CASE("Susceptible person is not infectious")
{
    Person p;

    CHECK_FALSE(p.isInfectious());
}

TEST_CASE("Vaccinated person is not infectious")
{
    Person p;

    p.vaccinate();

    CHECK_FALSE(p.isInfectious());
}

TEST_CASE("Recovered person is not infectious")
{
    Person p;

    p.infect();
    p.recover();

    CHECK_FALSE(p.isInfectious());
}

TEST_CASE("Disease object can be created")
{
    Disease covid(
        "Corona",
        7,
        0.3
    );

    CHECK(covid.getName() == "Corona");

    CHECK(covid.getDuration() == 7);

    CHECK(covid.getTransmissibility() == doctest::Approx(0.3));
}

TEST_CASE("Population starts with all susceptible")
{
    Population pop(100);

    CHECK(pop.countSusceptible() == 100);

    CHECK(pop.countInfectious() == 0);

    CHECK(pop.countRecovered() == 0);

    CHECK(pop.countVaccinated() == 0);
}

TEST_CASE("Patient zero becomes infected")
{
    Population pop(100);

    pop.infectPatientZero();

    CHECK(pop.countInfectious() == 1);

    CHECK(pop.countSusceptible() == 99);
}

TEST_CASE("Population vaccination works")
{
    Population pop(100);

    pop.vaccinatePopulation(0.2);

    CHECK(pop.countVaccinated() == 20);
}


TEST_CASE("Disease spreads with probability 1")
{
    Population pop(100);

    pop.infectPatientZero();

    pop.simulateDay(1.0);

    CHECK(pop.countInfectious() > 1);
}

TEST_CASE("Simulation object can be created")
{
    Simulation sim;

    CHECK(true);
}