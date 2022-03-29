import re
from utils import *
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

location = [45.666596, -111.045824]  # Cobleigh's Hall, Montana State University

# Read data
df = pd.read_csv('data//march23_2022.csv')  # Or 'data//march22_2022.csv'
col1 = df.iloc[1:-1:5, 0].to_numpy()
col2 = df.iloc[4:len(df) + 1:5, 1].to_numpy()

# Format data: Zenith vs Voltage
data = np.zeros((len(col1), 2))
dates = []
date = None
for c in range(len(col1)):
    dates.append(col1[c][:5])
    date = re.split(':| ', col1[c])
    # Correct date to year, month, day, hour, minute
    date = [int(date[5]), int(date[4]), int(date[3]), int(date[0]), int(date[1])]
    data[c, 0] = calculate_zenith(time=date, delta=0, location=location)
data[:, 1] = col2

# Plot collected data
plt.figure()
plt.scatter(np.arange(len(data)), data[:, 1])
plt.xticks(np.arange(len(data)), dates, rotation=45)
plt.yticks(fontsize=12)
plt.xticks(fontsize=12)
plt.locator_params(axis='x', nbins=10)
plt.locator_params(axis='y', nbins=10)
plt.xlabel('UTM hour', fontsize=14)
plt.ylabel('$V_d$', fontsize=18)
plt.title('Voltage vs. Hour. March ' + str(date[2]) + '$^{th}$, 2022', fontsize=18)

# Data transformation
X = air_mass_correction(data[:, 0])  # Air mass calculation using refraction correction
Y = np.log(data[:, 1])  # Log voltage

# Remove outliers manually
remove_indices = np.where(((X < 2) & (Y < 1.53)) | ((X < 3) & (Y < 1.41)) |
                          ((4.9 < X) & (X < 5.9) & (Y < 0.88)))[0]
X = np.delete(X, remove_indices)
Y = np.delete(Y, remove_indices)

# Linear Regression
lm = LinearRegression()  # Init model
lm.fit(X.reshape(-1, 1), Y.reshape(-1, 1))  # Train

# Langley plot
plt.figure()
plt.scatter(X, Y)
plt.locator_params(axis='x', nbins=10)
plt.locator_params(axis='y', nbins=10)
plt.xlabel('Air mass', fontsize=12)
plt.ylabel('ln($V_d$)', fontsize=12)
plt.title('Langley Plot. March ' + str(date[2]) + '$^{th}$, 2022')
X = np.arange(1.5, 10)
plt.plot(X, (X * lm.coef_ + lm.intercept_).T, 'r')
plt.text(5, 1.2, "ln($V_d$) = " + str(np.round(lm.coef_[0][0], 4)) +
         " * $AirMass$ + " + str(np.round(lm.intercept_[0], 4)))
