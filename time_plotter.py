import re
import matplotlib.pyplot as plt

# Regular expression to match timings
timing_regex = re.compile(r'---TIMING---\s+(\d+\.?\d*)s\s+(\w+)|Time measured to (\w+)\s+([\w\s]*):\s+(\d+\.?\d*)s')

# Dictionary to store timings
timings = {}

# Parse the terminal output and extract timings
with open('terminal_output.txt', 'r') as f:
    for line in f:
        match = timing_regex.search(line)
        if match:
            if match.group(1) and match.group(2):
                time = float(match.group(1))
                category = match.group(2)
            else:
                time = float(match.group(5))
                category = match.group(3) + ' ' + match.group(4)
            if category not in timings:
                timings[category] = []
            timings[category].append(time)

# Create a new figure
fig = plt.figure()

# Plot the timings for each category
for category, times in timings.items():
    plt.plot(times, label=category)

# Add a legend and show the plot
plt.legend()
plt.show()
