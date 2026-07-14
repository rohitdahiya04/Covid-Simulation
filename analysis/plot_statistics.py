import os
import pandas as pd
import matplotlib.pyplot as plt

# Get path of this script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Read CSV
csv_path = os.path.join(script_dir, "..", "disease_stats.csv")
df = pd.read_csv(csv_path)

print(df)

# Create graph
plt.figure(figsize=(8, 5))

plt.plot(
    df["VaccinationRate"],
    df["AverageDays"],
    marker="o",
    linewidth=2
)

plt.title("Vaccination Rate vs Average Disease Duration")
plt.xlabel("Vaccination Rate (%)")
plt.ylabel("Average Days Until Disease Extinction")

plt.grid(True)

# Save graph
output_path = os.path.join(script_dir, "average_days.png")
plt.savefig(output_path, dpi=300)

plt.show()

# -------------------------------
# Graph 2
# Vaccination Rate vs Average Recovered
# -------------------------------

plt.figure(figsize=(8, 5))

plt.plot(
    df["VaccinationRate"],
    df["AverageRecovered"],
    marker="o",
    linewidth=2
)

plt.title("Vaccination Rate vs Average Recovered")
plt.xlabel("Vaccination Rate (%)")
plt.ylabel("Average Recovered People")

plt.grid(True)

output_path = os.path.join(script_dir, "average_recovered.png")
plt.savefig(output_path, dpi=300)

plt.show()

# -------------------------------
# Graph 3
# Vaccination Rate vs Average Vaccinated
# -------------------------------

plt.figure(figsize=(8, 5))

plt.plot(
    df["VaccinationRate"],
    df["AverageVaccinated"],
    marker="o",
    linewidth=2
)

plt.title("Vaccination Rate vs Average Vaccinated")
plt.xlabel("Vaccination Rate (%)")
plt.ylabel("Average Vaccinated People")

plt.grid(True)

output_path = os.path.join(script_dir, "average_vaccinated.png")
plt.savefig(output_path, dpi=300)

plt.show()

# -------------------------------
# Graph 4
# S-I-R-V Curves
# -------------------------------

# Read detailed simulation data
details_path = os.path.join(script_dir, "..", "disease_details.csv")
details = pd.read_csv(details_path)

# Select ONE vaccination rate and ONE run
selected = details[
    (details["VaccinationRate"] == 30) &
    (details["Run"] == 1)
]

# Create graph
plt.figure(figsize=(12,8))

plt.plot(
    selected["Day"],
    selected["Susceptible"],
    label="Susceptible"
)

plt.plot(
    selected["Day"],
    selected["Infectious"],
    label="Infectious"
)

plt.plot(
    selected["Day"],
    selected["Recovered"],
    label="Recovered"
)

plt.plot(
    selected["Day"],
    selected["Vaccinated"],
    label="Vaccinated"
)

plt.title(
    "Disease Progression\n"
    "Vaccination Rate = 30%\n"
    r"$\beta$ = 0.3, Duration = 7 days"
)
plt.xlabel("Day")
plt.ylabel("Number of People")

plt.legend()
plt.grid(True)

output_path = os.path.join(script_dir, "sirv_curve_30.png")

plt.savefig(output_path, dpi=300)

plt.show()